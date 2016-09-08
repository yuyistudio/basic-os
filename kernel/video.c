#include "type.h"
#include "const.h"
#include "proto.h"
#include "global.h"

//vide.asm只提供三个最基本的底层汇编函数
// _info为四个字节,格式(高-->低)为:
// 颜色 字符 偏移h 偏移l
void 	put_char( int _info);			//同时改变字符和颜色
int 	get_char( int _char_offset);		//同时得到字符和颜色
int	put_char_only( int _info);		//只改变字符,不改变颜色
// 本文件提供两个直接和端口打交道的底层函数
static void SetShowOffset( u32 _offset);
static void SetCursorOffset( u32 _offset);
// 其他函数是对上面四个函数的封装

#define	CRTC_ADDR_REG	0x3D4		// register addr for current operation
#define	CRTC_DATA_REG	0x3D5		// store data temporarily, then trans to the register CRTC_ADDR_REG points to
#define	START_ADDR_H	0xC		// video start offset high 8 bits
#define	START_ADDR_L	0xD		// video start offset low 8 bits
#define	CURSOR_H	0xE		// cursor offset high 8 bits
#define	CURSOR_L	0xF		// cursor offset low 8 bits
#define	MEM_BASE	0xB8000		// base of video mem
#define	MEM_SIZE	0x8000		// limit of video mem( 32 KB totally)
#define MEM_LINES	( MEM_SIZE / ( SCREEN_COLS * 2))	// video ram lines ( 160 bytes per line, 160 = 80 cols * 2 bytes/col)
#define SCREEN_COLS	80		//
#define SCREEN_ROWS	25		//



int current_line 	= 0;
int show_offset 	= 0;
int cursor_offset 	= 0;

u8	color		= FG_RED + FG_BLUE + FG_GREEN + FG_LIGHT;	//记录当前的前景色和背景色, 默认为白底黑字

static void PutCharOffsetNC( int _offset, char _ch);
static void PutCharOffset( int _offset, char _ch);

// public---------------------------------------------------------------------


//光标函数
void MoveCursor( int _offset_to_current_position)
{
	cursor_offset += _offset_to_current_position;
	SetCursorOffset( cursor_offset);
}
void SetCursorAt( int _row, int _col)
{
	cursor_offset = _row * SCREEN_COLS + _col + show_offset;
	SetCursorOffset( cursor_offset);
}
void SetCursorNextLine()
{
	MoveCursor( SCREEN_COLS - cursor_offset % SCREEN_COLS);
}

//颜色
void SetBGColor( u8 _color)
{
	color &= 0x0F;
	color += _color;
}
void SetFGColor( u8 _color)
{
	color &= 0xF0;
	color += _color;
}
//滚屏函数
/*
BOOL ScrollScreen( int _offset_to_current_line)
{
	return SetLine( current_line + _offset_to_current_line);
}

BOOL SetLine( int _line)
{
	if( _line < 0 || _line >= MEM_LINES){
		return FALSE;
	}
	current_line = _line;
	SetShowOffset( _line * SCREEN_COLS);
	return TRUE;
}*/

//打印字符串函数
void Print( char * _str)
{
	int i;
	int len = strlen( _str);
	for( i = 0; _str[ i] != '\0'; ++ i)
	{
		if( _str[i] == '\n')
		{
			SetCursorNextLine();
		}
		else{
			PutChar( _str[ i]);
		}
	}
}
void PrintAt( int _row, int _col, char * _str)
{
	int i;
	int len = strlen( _str);
	for( i = 0; _str[ i] != '\0'; ++ i)
	{
		if( _str[i] == '\n')
		{
			SetCursorNextLine();
		}
		else{
			PutCharAt( _row, _col, _str[ i]);
			_col ++;
		}
	}
}

//清屏函数
void Cls()
{
	int row, col;
	for( row = 0; row < SCREEN_ROWS; row ++)
	{
		for( col = 0; col < SCREEN_COLS; col ++)
		{
			PutCharOffset( show_offset + row * SCREEN_COLS + col, ' ');	
		}
	}
	SetCursorOffset( show_offset);
}
//字符函数
void PutChar( char _ch)
{
	PutCharOffset( cursor_offset, _ch);
	MoveCursor( 1);
}
void DelChar()
{
	MoveCursor( -1);
	PutCharOffset( cursor_offset, ' ');
}

void PutCharAt( int _row, int _col, char _char)
{
	PutCharOffset( _row * SCREEN_COLS + _col, _char);
}
// private---------------------------------------------------------------------

//对put_char的封装
//offset是在屏幕上的偏移量,而不是相对于0xb8000的.

//不改变原来的字符颜色属性,只改变字符
static void PutCharOffsetNC( int _offset, char _ch)
{
	u32 par;
	if( _offset > SCREEN_ROWS * SCREEN_COLS){
		return;
	}
	par = _offset + show_offset;
	par += ( (u32)_ch) << 16;
	put_char_only( par);
}
//同时改变颜色和
static void PutCharOffset( int _offset, char _ch)
{
	u32 par;
	if( _offset > SCREEN_ROWS * SCREEN_COLS){
		return;
	}
	par = _offset + show_offset;
	par += ( (u32)_ch) << 16;
	par += ( (u32)color) << 24;
	put_char( par);
}


// basic------------------------------------------------------------------------
//这两个函数里面的offset都是相对于0xb8000的.

// set start offset to show on the screen
static void SetShowOffset( u32 _offset)
{
	show_offset = _offset;
	disable_int();
	out_byte( CRTC_ADDR_REG, START_ADDR_H);
	out_byte( CRTC_DATA_REG, ( _offset >> 8) & 0xFF);
	out_byte( CRTC_ADDR_REG, START_ADDR_L);
	out_byte( CRTC_DATA_REG, _offset & 0xFF);
	enable_int();
}

// set cursor position to the _offset from MEM_BASE( defined above)
static void SetCursorOffset(unsigned int _offset)
{
	cursor_offset = _offset;
	disable_int();
	out_byte( CRTC_ADDR_REG, CURSOR_H);
	out_byte( CRTC_DATA_REG, ( _offset >> 8) & 0xFF);
	out_byte( CRTC_ADDR_REG, CURSOR_L);
	out_byte( CRTC_DATA_REG, _offset & 0xFF);
	enable_int();
}
