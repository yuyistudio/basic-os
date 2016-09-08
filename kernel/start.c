// this file is C init

#include "type.h"
#include "const.h"
#include "proto.h"
#include "global.h"

// do init work
PUBLIC void CInit()
{
	// copy old gdt in loader.bin to the new one in kernel.bin
	// memcpy( dest_ptr, src_ptr, n_bytes_to_copy);
	memcpy(	&gdt,(void*)(*((u32*)(&gdt_ptr[2]))),*((u16*)(&gdt_ptr[0]))+1); 
	// why the last parameter need plus 1? coz the limit is acturally the maximum offset, not the total size.
	// the minimum offset is 0, and the maximum is total_size-1
	// so the total size is limit+1

	// init gdt ptr
	u16* p_gdt_limit = (u16*)(&gdt_ptr[0]);
	u32* p_gdt_base  = (u32*)(&gdt_ptr[2]);
	*p_gdt_limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;	// the reason for minus 1 is explained above
	*p_gdt_base  = (u32)&gdt;

	// init idt ptr
	u16* p_idt_limit = (u16*)(&idt_ptr[0]);
	u32* p_idt_base  = (u32*)(&idt_ptr[2]);
	*p_idt_limit = IDT_SIZE * sizeof(GATE) - 1;
	*p_idt_base  = (u32)&idt;
	
	// init idt
	InitIDT();
	
	// init tss:
	// ss0 is set. iobase is set to sizeof( TSS)
	// init the item of tss in gdt
	InitTSS();
	
	// init process manager
	InitProcessManager();	
}
