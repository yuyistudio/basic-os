#include "type.h"
#include "const.h"
#include "proto.h"
#include "global.h"

// 8259A interrupt controller ports
#define	INT_M_CTL	0x20
#define	INT_M_CTLMASK	0x21
#define	INT_S_CTL	0xA0
#define	INT_S_CTLMASK	0xA1


void Init8259A()
{
	out_byte( INT_M_CTL,	0x11);				// Master 8259, ICW1.
	out_byte( INT_S_CTL,	0x11);				// Slave  8259, ICW1.
	out_byte( INT_M_CTLMASK,	INT_VECTOR_IRQ0);	// set irq0-irq7 with int vector 0x20-0x27
	out_byte( INT_S_CTLMASK,	INT_VECTOR_IRQ8);	// set irp8-irq15 with int vector 0x28-0x35
	out_byte( INT_M_CTLMASK,	0x4);
	out_byte( INT_S_CTLMASK,	0x2);
	out_byte( INT_M_CTLMASK,	0x1);
	out_byte( INT_S_CTLMASK,	0x1);

	out_byte( INT_M_CTLMASK,	0xFF);			// set to prevent all int
	out_byte( INT_S_CTLMASK,	0xFF);
}

