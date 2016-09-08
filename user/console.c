#include "type.h"
#include "const.h"
#include "./syscall.h"
#include "global.h"
PRIVATE void PushChar( char _char);
PRIVATE void ParserCommand();
PRIVATE void PopChar();
// public---------------------------------------------------------------------
#define CONSOLE_BUFFER_SIZE 1024
char input_buffer[ CONSOLE_BUFFER_SIZE];
int input_counter = 0;

u8 window_bg = BG_BLUE + BG_LIGHT;
u8 window_fg = FG_BLUE + FG_LIGHT + FG_RED + FG_GREEN;
u8 menu_bg = BG_RED + BG_LIGHT;
u8 menu_fg = FG_BLUE + FG_LIGHT;
u8 menu_focus_bg = BG_GREEN + BG_LIGHT;
void Shell();

char * main_menu_items[] = 
{
	"SHELL",
	"FILE VIEWER",
	"FILE EDITOR",
	"PROCESS MANAGER",
	"SHUT DOWN"
};
int main_menu_focus = 0;
int main_menu_item_n = 5;

void iPrintMenu( int _row, int _col, char ** _menu_items, int _n, int _focus, int _width)	
{	
	int row, col, i;
	SetBGColor( menu_bg);

	SetFGColor( menu_fg);
	for( row = _row; row <= _row + _n + 1; ++ row)
	{
		for( col = _col; col <= _col + _width; ++ col)
		{
			iPutCharAt( row, col, ' ');
		}
	}
	for( row = _row + 1, i = 0, col = _col + 1; row <= _row + _n; ++ row, ++ i)
	{
		if( i == _focus)
		{
			SetBGColor( menu_focus_bg);
			iPrintAt( row, col, _menu_items[ i]);
			SetBGColor( menu_bg);
		}
		else{
			iPrintAt( row, col, _menu_items[ i]);
		}
	}
	
	SetBGColor( window_bg);
	SetFGColor( window_fg);
}
void InitConsole()
{
	int width = 20;
	int height = 10;
	SetBGColor( window_bg);
	SetFGColor( window_fg);
	iCls();
	iPrintMenu( 3, 4, main_menu_items, main_menu_item_n, main_menu_focus, 16);
}
void Console()
{	
	int key;
	InitConsole();
	while( 1)
	{
		key = ViewKey( 0);
		if( key == NULL)
		{
			continue;
		}
		if( key == DOUBLE_KEY_FLAG)
		{
			key = ViewKey( 1);
			if( key == NULL)
			{
				continue;
			}
			AbandonKey( 2);
			switch( key)
			{
			case MAKE_UP:
				main_menu_focus --;
				if( main_menu_focus < 0)
				{
					main_menu_focus = main_menu_item_n - 1;
				}
				iPrintMenu( 3, 4, main_menu_items, main_menu_item_n, main_menu_focus, 16);
				break;
			case MAKE_DOWN:
				main_menu_focus ++;
				if( main_menu_focus >= main_menu_item_n)
				{
					main_menu_focus = 0;
				}
				iPrintMenu( 3, 4, main_menu_items, main_menu_item_n, main_menu_focus, 16);
				break;
			default:
				break;
			}	
		}
		else{
			AbandonKey( 1);
			switch( key){
			case MAKE_ENTER:
				switch( main_menu_focus){
				case 0:
					CreateProcess( "shell", Shell, 0);
					// 现在console自杀
					EndProcess();
					break;
				case 4:
					Shutdown();
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		
		}
	}
}

void Shell()
{
	u8 key;
	iCls();
	iSetCursorAt( 0, 0);
	iPrint( "\nBasic OS Shell   v0.1\n\n");
	iPrint( "root> ");
	while( 1)
	{
		key = ViewKey( 0);
		if( key == NULL)
		{
			continue;
		}
		if( key == DOUBLE_KEY_FLAG)
		{
			key = ViewKey( 1);
			if( key == NULL)
			{
				continue;
			}
			AbandonKey( 2);
			switch( key)
			{
			default:
				break;
			}	
		}
		else{
			AbandonKey( 1);
			switch( key){
			case MAKE_A:
				iPutChar( 'a');
				PushChar( 'A');
				break;
			case MAKE_B:
				iPutChar( 'b');
				PushChar( 'B');
				break;
			case MAKE_C:
				iPutChar( 'c');
				PushChar( 'C');
				break;
			case MAKE_D:
				iPutChar( 'd');
				PushChar( 'D');
				break;
			case MAKE_E:
				iPutChar( 'e');
				PushChar( 'E');
				break;
			case MAKE_F:
				iPutChar( 'f');
				PushChar( 'F');
				break;
			case MAKE_G:
				iPutChar( 'g');
				PushChar( 'G');
				break;
			case MAKE_I:
				iPutChar( 'i');
				PushChar( 'I');
				break;
			case MAKE_J:
				iPutChar( 'j');
				PushChar( 'J');
				break;
			case MAKE_K:
				iPutChar( 'k');
				PushChar( 'K');
				break;
			case MAKE_M:
				iPutChar( 'm');
				PushChar( 'M');
				break;
			case MAKE_N:
				iPutChar( 'n');
				PushChar( 'N');
				break;
			case MAKE_O:
				iPutChar( 'o');
				PushChar( 'O');
				break;
			case MAKE_Q:
				iPutChar( 'q');
				PushChar( 'Q');
				break;
			case MAKE_R:
				iPutChar( 'r');
				PushChar( 'R');
				break;
			case MAKE_T:
				iPutChar( 't');
				PushChar( 'T');
				break;
			case MAKE_U:
				iPutChar( 'u');
				PushChar( 'U');
				break;
			case MAKE_V:
				iPutChar( 'v');
				PushChar( 'V');
				break;	
			case MAKE_W:
				iPutChar( 'w');
				PushChar( 'W');
				break;	
			case MAKE_X:
				iPutChar( 'x');
				PushChar( 'X');
				break;	
			case MAKE_Y:
				iPutChar( 'y');
				PushChar( 'Y');
				break;	
			case MAKE_Z:
				iPutChar( 'z');
				PushChar( 'Z');
				break;	
			case MAKE_H:
				iPutChar( 'h');
				PushChar( 'H');
				break;
			case MAKE_P:
				iPutChar( 'p');
				PushChar( 'P');
				break;
			case MAKE_L:
				iPutChar( 'l');
				PushChar( 'L');
				break;
			case MAKE_S:
				iPutChar( 's');
				PushChar( 'S');
				break;
			case MAKE_ENTER:
				PushChar( '\0');// end of the string to parse
				ParserCommand();
				break;
			case MAKE_SPACE:
				iPutChar( ' ');
				PushChar( ' ');
				break;
			case MAKE_1:
				iPutChar( '1');
				PushChar( '1');
				break;
			case MAKE_2:
				iPutChar( '2');
				PushChar( '2');
				break;
			case MAKE_3:
				iPutChar( '3');
				PushChar( '3');
				break;
			case MAKE_4:
				iPutChar( '4');
				PushChar( '4');
				break;
			case MAKE_5:
				iPutChar( '5');
				PushChar( '5');
				break;
			case MAKE_6:
				iPutChar( '6');
				PushChar( '6');
				break;
			case MAKE_7:
				iPutChar( '7');
				PushChar( '7');
				break;
			case MAKE_8:
				iPutChar( '8');
				PushChar( '8');
				break;
			case MAKE_9:
				iPutChar( '9');
				PushChar( '9');
				break;
			case MAKE_0:
				iPutChar( '0');
				PushChar( '0');
				break;
			case MAKE_BKSP:
				if( input_counter > 0)
				{	
					iDelChar();
					PopChar();
				}
				break;
			default:
				break;
			}
		
		}
	}

}
// private---------------------------------------------------------------------

// push char into console buffer
PRIVATE void PushChar( char _char)
{
	if( input_counter == CONSOLE_BUFFER_SIZE)
	{
		iPrint( "\nconsole buffer full !\n");
		input_counter = 0;
	}
	else{
		input_buffer[ input_counter++] = _char;
	}
}

// pop char from console buffer
PRIVATE void PopChar()
{
	if( input_counter > 0)
	{
		input_counter --;
	}
}

// handler command
PRIVATE void ParserCommand()
{
	input_counter = 0;
	iPrint( "\n ");
	if( strequ( "CLS", input_buffer))
	{
		iCls();
		iSetCursorAt( 0, 0);
		iPrint( "\nBasic OS Shell   v0.1\n\n");
	}
	else if( strequ( "HELP", input_buffer)){
		iPrint( "available command : help cls console proc\n");
	}
	else if( strequ( "CONSOLE", input_buffer)){
		CreateProcess( "console", Console, 0);
		EndProcess();
	}
	else if( strequ( "PROC", input_buffer)){
		iPrint( "there are many procs there...\n");
	}
	else{
		iPrint( "command not found\n");
	}
	iPrint( "root> ");
}

// basic---------------------------------------------------------------------
