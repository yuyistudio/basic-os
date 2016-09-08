#include "type.h"
#include "const.h"
#include "proto.h"
#include "global.h"



// asm functions to handle exceptions
void	exception_handler();
void	clock_handler();
void	keyboard_handler();


// init idt and tss
PUBLIC void InitIDT()
{
	int i;
	
	// init 8259A, and disable all interruption
	Init8259A();
	
	// init all idt items
	// real exceptions from 0x00 to 0x10
	for( i = 0; i < IDT_SIZE; ++ i)
	{
		InitIDTDescriptor( i,	DA_386IGate, exception_handler,	PRIVILEGE_KRNL);
	}

	// init 8259A interrupt handler
	InitIDTDescriptor( INT_VECTOR_IRQ0 + 0,	DA_386IGate, clock_handler,	PRIVILEGE_KRNL);
	InitIDTDescriptor( INT_VECTOR_IRQ0 + 1,	DA_386IGate, keyboard_handler,	PRIVILEGE_KRNL);
	
	// init system call handler
	// 注意这里的权限是user权限，否则会发生保护违规。
	InitIDTDescriptor( INT_VECTOR_SYS_CALL,	DA_386IGate, sys_call,	PRIVILEGE_USER);

	
}
PUBLIC void InitTSS()
{
	// reset all items in TSS first
	// actually we hava only ONE TSS in our OS -.-
	memset( & tss, 0, sizeof( tss));
	
	// init TSS.ss for ring0( kernel)
	tss.ss0	= SELECTOR_KERNEL_DS;
	
	// init the descriptor of TSS in GDT
	InitDescriptor( & gdt[ INDEX_TSS], ( u32)& tss, sizeof( tss) - 1, DA_386TSS);
			
	// no I/O protection map
	tss.iobase = sizeof( tss);
}

// init an IDT item in IDT
PUBLIC void InitIDTDescriptor( unsigned char vector, u8 desc_type, int_handler handler, unsigned char privilege)
{
	GATE *	p_gate	= &idt[vector];
	u32	base	= (u32)handler;
	p_gate->offset_low	= base & 0xFFFF;
	p_gate->selector	= SELECTOR_KERNEL_CS;
	p_gate->dcount		= 0;
	p_gate->attr		= desc_type | (privilege << 5);
	p_gate->offset_high	= (base >> 16) & 0xFFFF;
}

// init an Descriptor item in GDT
PUBLIC void InitDescriptor( DESCRIPTOR * p_desc, u32 base, u32 limit, u16 attribute)
{
	p_desc->limit_low		= limit & 0x0FFFF;		
	p_desc->base_low		= base & 0x0FFFF;		
	p_desc->base_mid		= (base >> 16) & 0x0FF;	
	p_desc->attr1			= attribute & 0xFF;	
	p_desc->limit_high_attr2	= ((limit >> 16) & 0x0F) | (attribute >> 8) & 0xF0;
	p_desc->base_high		= (base >> 24) & 0x0FF;		
}




