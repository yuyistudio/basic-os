#include "type.h"
#include "const.h"
#include "proto.h"
#include "global.h"

void userproc();

// system is finally established here
PUBLIC int CMain()
{
	// create process "Console"
	CreateProcess( "shell", Console, 0);
	//CreateProcess( "test", Test, 5);
	//CreateProcess( "user", userproc, 2);

	// the flag of ReEnter of the interrupt
	k_reenter = 0;

        // enable keyboard
        enable_irq(KEYBOARD_IRQ);
        
        // enable clock
        enable_irq(CLOCK_IRQ);
        
	// start the first process
	restart();
	
	// we will never reach here
	while( 1){}
}


void Test()
{
	PrintAt( 5, 5, "- test process -");
	while(1);
}


// exception handler
// do nothing useful
PUBLIC void ExceptionHandler( int _vec_no, int _err_code, int _eip, int _cs, int _eflags)
{
	char error_no[10];
	itoa( error_no, _vec_no);
	Print( "exception makes system halted : ");
	Print( error_no);
	disable_int();
	halt();
}
