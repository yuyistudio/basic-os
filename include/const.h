// this head file save all #define value


#ifndef	_CONST_H_
#define	_CONST_H_

// -----------------------------------------------------------------------------	
//用于video提供的函数的 u8 _color 参数			
#define BG_LIGHT	0b10000000
#define	BG_RED		0b01000000
#define BG_GREEN	0b00100000
#define	BG_BLUE		0b00010000
#define FG_LIGHT	0b00001000
#define FG_RED		0b00000100
#define FG_GREEN	0b00000010
#define FG_BLUE		0b00000001

#define SCREEN_COLS	80
#define SCREEN_ROWS	25

// 键盘扫描码
#define DOUBLE_KEY_FLAG	0xE0
#define MAKE_A		0x1E
#define MAKE_B		0x30
#define MAKE_C		0x2E
#define MAKE_D		0x20
#define MAKE_E		0x12
#define MAKE_F		0x21
#define MAKE_G		0x22
#define MAKE_H		0x23
#define MAKE_I		0x17
#define MAKE_J		0x24
#define MAKE_K		0x25
#define	MAKE_L		0x26
#define MAKE_M		0x32
#define MAKE_N		0x31
#define MAKE_O		0x18
#define MAKE_P		0x19
#define MAKE_Q		0x10
#define MAKE_R		0x13
#define MAKE_S		0x1F
#define MAKE_T		0x14
#define MAKE_U		0x16
#define MAKE_V		0x2F
#define MAKE_W		0x11
#define MAKE_X		0x2D
#define MAKE_Y		0x15
#define MAKE_Z		0x2C
#define MAKE_UP		0x48
#define MAKE_DOWN	0x50
#define MAKE_LEFT	0x4B
#define MAKE_RIGHT	0x4D
#define MAKE_ENTER	0x1C
#define MAKE_BKSP	0x0E
#define MAKE_TAB	0x0F
#define MAKE_CLOCK	0x3A
#define MAKE_LSHIFT	0x2A
#define MAKE_RSHIFT	0x36
#define MAKE_LCTRL	0x1D
#define MAKE_0		0x0B
#define MAKE_1		0x2
#define MAKE_2		0x3
#define MAKE_3		0x4
#define MAKE_4		0x5
#define MAKE_5		0x6
#define MAKE_6		0x7
#define MAKE_7		0x8
#define MAKE_8		0x9
#define MAKE_9		0x0A
#define MAKE_SPACE	0x39

// return value
#define	NULL		0
#define TRUE		1
#define FALSE		0

// function type
#define	PUBLIC
#define	PRIVATE	static

// -----------------------------------------------------------------------------

// GDT item num
#define	GDT_SIZE	128
// IDT item num
#define	IDT_SIZE	256
// LDT item num
#define LDT_SIZE	8

// system privilege for descriptor
#define	PRIVILEGE_KRNL	0
#define	PRIVILEGE_TASK	1
#define	PRIVILEGE_USER	3
// system priviledge for selector
#define	RPL_KRNL	PRIVILEGE_KRNL
#define	RPL_TASK	PRIVILEGE_TASK
#define	RPL_USER	PRIVILEGE_USER

// GDT
#define	INDEX_DUMMY		0	
#define	INDEX_FLAT_C		1	
#define	INDEX_FLAT_RW		2	
#define	INDEX_VIDEO		3	
#define	INDEX_TSS		4
#define	INDEX_LDT_FIRST		5
// Selectors( select is the offset measured by byte)
#define	SELECTOR_DUMMY		   0		
#define	SELECTOR_FLAT_C		0x08		
#define	SELECTOR_FLAT_RW	0x10		
//#define	SELECTOR_VIDEO		(0x18+3) // why plus 3?	
#define	SELECTOR_VIDEO		0x18
#define	SELECTOR_TSS		0x20		
#define SELECTOR_LDT_FIRST	0x28
// alias for selectors
#define	SELECTOR_KERNEL_CS	SELECTOR_FLAT_C
#define	SELECTOR_KERNEL_DS	SELECTOR_FLAT_RW
#define	SELECTOR_KERNEL_GS	SELECTOR_VIDEO

// common segment attr
#define	DA_DRW			0x92
#define	DA_C			0x98

// system segment attr
#define	DA_LDT			0x82
#define	DA_386TSS		0x89
#define	DA_386IGate		0x8E

// selector type
#define	SA_RPL_MASK	0xFFFC
#define	SA_RPL0		0
#define	SA_RPL1		1
#define	SA_RPL2		2
#define	SA_RPL3		3
#define	SA_TI_MASK	0xFFFB
#define	SA_TIG		0
#define	SA_TIL		4
// -----------------------------------------------------------------------------

// 8259A hard int vector
#define	INT_VECTOR_IRQ0			0x20
#define	INT_VECTOR_IRQ8			0x28

// system call int vector
#define	INT_VECTOR_SYS_CALL		0x80

// hardware interrupt
#define	CLOCK_IRQ	0
#define	KEYBOARD_IRQ	1	



#endif
