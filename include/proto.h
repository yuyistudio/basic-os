// this file list all functions in this system

// proc.c
BOOL 	ProcessSheduler();
BOOL 	CreateProcess( char * _name, p_code _func, int _privilege);
void 	InitProcessManager();
void 	EndProcess();

// lib/lib.asm

void	memset(void* p_dst, char ch, int size);
void*	memcpy(void* p_dst, void* p_src, int size);
char* 	strcpy(char* p_dst, char* p_src);
void	out_byte(u16 port, u8 value);
u8	in_byte(u16 port);
void	halt();

// protect.c
void 	InitDescriptor( DESCRIPTOR * p_desc, u32 base, u32 limit, u16 attribute);

// libc.c
void	delay(int time);
int 	strlen( char * _str);
BOOL 	strequ( char * _1, char * _2);

// kernel.asm
void restart();
void sys_call();

// main.c
void Test();
void FatalError();
void SystemCallHandler();
void ExceptionHandler( int _vec_no, int _err_code, int _eip, int _cs, int _eflags);

// i8259.c
void Init8259A();

// video.c
void MoveCursor( int _offset_to_current_position);
void SetCursorAt( int _row, int _col);
void SetCursorNextLine();
void Print( char * _str);
void PrintAt( int _row, int _col, char * _str);
void Cls();
void DelChar();
void PutChar( char _ch);
void PutCharAt( int _row, int _col, char _char);

// keyboard.c
void 	KeyboardHandler();
u8 	GetKey();
u8 	ViewKey( int _offset);
void 	AbandonKey( int _num);
void 	InitKeyboardHandler();

// console.c
void Console();

// syscall.asm
void SystemCallHandler( int _function_no);
