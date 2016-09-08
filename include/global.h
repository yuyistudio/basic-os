// this file contains all global variables


// system gdt
u8			gdt_ptr[6];	// word limit, dword base. gdt_ptr is used in kernel.asm	
DESCRIPTOR		gdt[GDT_SIZE];
u8			idt_ptr[6];	// word limit, dword base. idt_ptr is used in kernel.asm
GATE			idt[IDT_SIZE];

// judge if reenter
u32			k_reenter;

// tss
TSS			tss;

