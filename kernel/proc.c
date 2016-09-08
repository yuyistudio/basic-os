#include "type.h"
#include "const.h"
#include "proto.h"
#include "global.h"


// variables for this file------------------------------------------------------

// system stack for process running
// 1M - 2M RAM is for process stack
char * sys_stack_top 	= ( char *) 0x6FFFFF;
char * sys_stack_base	= ( char *) 0x600000;
// 512 stacks can be used, each is 2KB
#define	STACK_TOTAL_SIZE 	0x100000
#define	STACK_SIZE		0x1000
#define STACK_NUM		( STACK_TOTAL_SIZE / STACK_SIZE)
// reserve 2KB to record the usage of stack
BOOL * stack_usage_base = ( BOOL *) 0x801000;
BOOL * stack_usage_top  = ( BOOL *) 0x801FFF;

// system memory for saving PCB array
// 2M - 3M RAM is for process pcb array
// but as the sheduler is very bad, we allow less process
#define PCB_NUM	5
PROCESS * sys_pcb_base 	= ( PROCESS *) 0x700000;
PROCESS * sys_pcb_top;
	
// system memory for recording the usage of GDT's "LDT descriptor space"
// 3M - 3M + 2KB is for
// record GDT usage
// can record 512 GDT ldt descriptors usage
// BUT GDT_SIZE is 128, so ...
BOOL * gdt_usage_base	= ( BOOL *) 0x800000;
BOOL * gdt_usage_top	= ( BOOL *) 0x800FFF;

// for process sheduler
// sheduler always run the process that p_proc_ready points to after it's end
PROCESS * p_proc_ready;	
// this var records the number of processes
// if there is no process, our sheduler will know it by this variable, and then do nothing
int process_num = 0;


// private functions declaration ----------------------------------------------

PRIVATE int GetLDTDesc();
PRIVATE void AddProcess( int _id, int _ldt_no, int _ldt_n, char * _name, p_code _p_code, 
		p_stack _p_stack, int _stack_size, PROCESS * p_proc, int _privilege);
PRIVATE p_stack GetStackTop();

// public ---------------------------------------------------------------------
PUBLIC int sys_get_ticks()
{
	return 1;
}
// called by one process
// and kill the caller process
// 这里有一个风险:这个函数调用前后不能发生程序切换,否则可能杀死的不是调用程序,而是悲剧的后继程序.
PUBLIC void EndProcess()
{
	p_proc_ready->empty = TRUE;
}
// init process manager
// it is called before process manager and processes run
PUBLIC void InitProcessManager()
{
	int i;
	PROCESS * p_pcb;
	// pointer of the first process to run
	p_proc_ready	= sys_pcb_base;
	// init
	p_pcb  = sys_pcb_base;
	// figure out how many pcbs in 1M RAM first
	// then decide the value of sys_pcb_top
	sys_pcb_top = sys_pcb_base + PCB_NUM;
	// init all pcb node
	while( p_pcb < sys_pcb_top)
	{
		p_pcb -> empty = TRUE;
		p_pcb ++;
	}
	// init GDT'ldt desc usage to 'no use' state
	for( i = 0; i < GDT_SIZE - INDEX_LDT_FIRST; ++ i)
	{
		gdt_usage_base[i] = FALSE;
	}
	// init stack usage array to "no use" state
	for( i = 0; i < STACK_NUM; ++ i)
	{
		stack_usage_base[ i] = FALSE;
	}
}

// it is called by clock handler
// it means it will shedule processes when timer event happens
// we must make sure that when this function ends, the "p_proc_ready" points to a valid process entry
PUBLIC BOOL ProcessSheduler()
{
	if( process_num > 0)
	{
		do
		{
			p_proc_ready ++;
			if( p_proc_ready - sys_pcb_top >= 0)
			{
				p_proc_ready = sys_pcb_base;
			}
			if( p_proc_ready -> empty == FALSE)
			{
				if( p_proc_ready->counter > 0)
				{
					p_proc_ready->counter --;
					continue;
				}
				else{
					p_proc_ready->counter = p_proc_ready->privilege;
					return TRUE;
				}
			}
		} while( 1);
	}
	else{
		return FALSE;
	}
	return TRUE;
}

// create a process
PUBLIC BOOL CreateProcess( char * _name, p_code _func, int _privilege)
{
	PROCESS * p_pcb;
	int index_gdt_item;
	char buffer[50];
	p_stack p_new_stack;
	p_pcb = sys_pcb_base;
	// find one place for new process
	while( p_pcb < sys_pcb_top)
	{
		// if found one place
		if( p_pcb -> empty == TRUE)
		{
			p_pcb -> empty = FALSE;
			index_gdt_item = GetLDTDesc();
			if( index_gdt_item == NULL)
			{
				Print( "\nget gdt fail\n");
				return FALSE;
			}
			p_new_stack = GetStackTop();
			if( p_new_stack == NULL)
			{
				Print( "\nget stack fail\n");
				return FALSE;
			}
			AddProcess( index_gdt_item, index_gdt_item, LDT_SIZE, _name, _func, p_new_stack, STACK_SIZE, p_pcb, _privilege);
			return TRUE;
		}
		p_pcb ++;
	}
	// there is no pcb room for new process
	return FALSE;
}

// private ---------------------------------------------------------------------

// get a empty place for ldt descriptor from gdt
// return the index of the free gdt item found
PRIVATE int GetLDTDesc()
{
	int i;
	for( i = 0; i < GDT_SIZE - INDEX_LDT_FIRST; ++ i)
	{
		// if the corresponding gdt item is empty
		if( gdt_usage_base[i] == FALSE)
		{
			gdt_usage_base[ i] = TRUE;
			return INDEX_LDT_FIRST + i;
		}
	}
	return NULL;
}

// find a free stack place for new process
// return the top addr of stack
PRIVATE p_stack GetStackTop()
{
	int i;
	for( i = 0; i < STACK_NUM; ++ i)
	{
		if( stack_usage_base[ i] == FALSE)
		{
			stack_usage_base[ i] = TRUE;
			return ( char *) ( sys_stack_top - STACK_SIZE * i);
		}
	}
	return NULL;
}

// add process to pcb_queue
// it is called by CreateProcess API
// it will apply for memory process needs automatically
// caller needs to provide the process name, and the process function address
PRIVATE void AddProcess( int _id, int _ldt_no, int _ldt_n, char * _name, p_code _p_code, 
		p_stack _p_stack, int _stack_size, PROCESS * p_proc, int _privilege)
{
	process_num ++;
	// init process ldt descriptor in gdt
	InitDescriptor(&gdt[_ldt_no], p_proc->ldts,
			 _ldt_n * sizeof(DESCRIPTOR) - 1, DA_LDT);
	// privilege
	p_proc->privilege = _privilege;
	p_proc->counter = _privilege;
	// name of the process
	strcpy(p_proc->p_name, _name);			
	// GDT 'S LDT
	p_proc->ldt_sel = _ldt_no << 3;
	// LDT[0] CODE
	memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
	p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_USER << 5;
	// LDT[1] STACK
	memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
	p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_USER << 5;
	// CS POINT TO LDT[0]
	p_proc->regs.cs	= 0 | SA_TIL | RPL_USER;
	// DS SS GS POINT TO LDT[1]
	p_proc->regs.ds	= 1<<3 | SA_TIL | RPL_USER;
	p_proc->regs.es	= 1<<3 | SA_TIL | RPL_USER;
	p_proc->regs.fs	= 1<<3 | SA_TIL | RPL_USER;
	p_proc->regs.ss	= 1<<3 | SA_TIL | RPL_USER;
	p_proc->regs.gs	= (SELECTOR_KERNEL_GS & SA_RPL_MASK)
		| RPL_USER;
	// EIP POINT TO PROCESS CODE
	p_proc->regs.eip = (u32)_p_code;
	// ESP POINT TO PROCESS STACK
	p_proc->regs.esp = (u32) _p_stack;
	// SPECIAL EFLAGS, IGNORE I/O PROTECTION
	p_proc->regs.eflags = 0x1202; /* IF=1, IOPL=1 */
}

