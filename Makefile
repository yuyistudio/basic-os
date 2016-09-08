
# Makefile for Basic OS

# this value must be same with that in load.inc   -----------------------------

BASEADDR	= 0x30400

# it is just a part of BASEADDR    ---------------------------------------------

OFFSET2BASE	=   0x400


# path and files ---------------------------------------------------------------

BOSIMG		= /mnt/shared/os/bos.img
FLOPPYPATH	= /mnt/osfloppy
LIBPATH		= lib/
KERNELPATH	= kernel/
INCPATH		= include/
USER		= user/
BOOTBIN		= boot/boot.bin boot/loader.bin
KERNELBIN	= $(KERNELPATH)kernel.bin
OBJS		= $(KERNELPATH)kernel.o $(KERNELPATH)start.o $(KERNELPATH)main.o $(KERNELPATH)clock.o $(KERNELPATH)i8259.o \
			 $(KERNELPATH)protect.o $(KERNELPATH)proc.o \
			$(LIBPATH)lib.o $(LIBPATH)libc.o $(KERNELPATH)keyboard.o $(KERNELPATH)video.o\
			$(KERNELPATH)videoc.o $(KERNELPATH)syscall.o $(KERNELPATH)syscallc.o \
			$(USER)lib.o $(USER)test.o $(USER)syscall.o $(USER)console.o 
DASMOUTPUT	= kernel.asm


# build tools and options    --------------------------------------------------

ASM		= nasm
DASM		= ndisasm
CC		= gcc -m32	# fored to be built to 32 bit architecture  coz my pc is 64-bit
LD		= ld -m elf_i386 -s	# fored to be built to 32 bit architecture coz my pc is 64-bit
ASMBFLAGS	= -I boot/$(INCPATH)
ASMKFLAGS	= -I $(INCPATH) -f elf
ASMUFLAGS	= $(ASMKFLAGS)
CFLAGS		= -I $(INCPATH) -c -fno-builtin -fno-stack-protector
LDFLAGS		= -s -Ttext $(BASEADDR)
DASMFLAGS	= -u -o $(BASEADDR) -e $(OFFSET2BASE)



# all functions ----------------------------------------------------------------

.PHONY : everything clean disasm

# make .o mid-file only
everything : $(BOOTBIN) $(KERNELBIN)

# make .o mid-file, and create floppy image
all: $(BOOTBIN) $(KERNELBIN) buildimg

# clean all mid-file
clean :
	rm -f $(OBJS) $(BOOTBIN) $(KERNELBIN)

# disasm the kernel.bin to asm file
disasm :
	$(DASM) $(DASMFLAGS) $(KERNELBIN) > $(DASMOUTPUT)

# build .o mid-file, and create floppy image
buildimg :
	dd if=boot/boot.bin of=$(BOSIMG) bs=512 count=1 conv=notrunc
	sudo mount -o loop $(BOSIMG) $(FLOPPYPATH)/
	sudo cp -fv boot/loader.bin $(FLOPPYPATH)/
	sudo cp -fv $(KERNELBIN) $(FLOPPYPATH)
	sudo umount $(FLOPPYPATH)

# boot  -----------------------------------------------------------------------

boot/boot.bin : boot/boot.asm boot/$(INCPATH)load.inc boot/$(INCPATH)fat12hdr.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

boot/loader.bin : boot/loader.asm boot/$(INCPATH)load.inc boot/$(INCPATH)fat12hdr.inc boot/$(INCPATH)pm.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

# kernel ----------------------------------------------------------------------

$(KERNELBIN) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(KERNELBIN) $(OBJS)

$(KERNELPATH)kernel.o : $(KERNELPATH)kernel.asm $(INCPATH)const.inc
	$(ASM) $(ASMKFLAGS) -o $@ $<
	
$(KERNELPATH)video.o : $(KERNELPATH)video.asm $(INCPATH)const.inc
	$(ASM) $(ASMKFLAGS) -o $@ $<

$(KERNELPATH)start.o: $(KERNELPATH)start.c $(INCPATH)type.h $(INCPATH)const.h    $(INCPATH)proto.h \
			$(INCPATH)global.h
	$(CC) $(CFLAGS) -o $@ $<

$(KERNELPATH)main.o: $(KERNELPATH)main.c $(INCPATH)type.h $(INCPATH)const.h    $(INCPATH)proto.h \
			$(INCPATH)global.h
	$(CC) $(CFLAGS) -o $@ $<

$(KERNELPATH)clock.o: $(KERNELPATH)clock.c
	$(CC) $(CFLAGS) -o $@ $<

$(KERNELPATH)i8259.o: $(KERNELPATH)i8259.c $(INCPATH)type.h $(INCPATH)const.h  $(INCPATH)proto.h
	$(CC) $(CFLAGS) -o $@ $<


$(KERNELPATH)protect.o: $(KERNELPATH)protect.c $(INCPATH)type.h $(INCPATH)const.h   $(INCPATH)proto.h \
			$(INCPATH)global.h
	$(CC) $(CFLAGS) -o $@ $<
	
$(KERNELPATH)keyboard.o: $(KERNELPATH)keyboard.c
	$(CC) $(CFLAGS) -o $@ $<
	
$(USER)console.o: $(USER)console.c
	$(CC) $(CFLAGS) -o $@ $<
	
$(KERNELPATH)videoc.o: $(KERNELPATH)video.c
	$(CC) $(CFLAGS) -o $@ $<

$(KERNELPATH)proc.o: $(KERNELPATH)proc.c
	$(CC) $(CFLAGS) -o $@ $<

$(KERNELPATH)syscallc.o: $(KERNELPATH)syscall.c
	$(CC) $(CFLAGS) -o $@ $<

$(KERNELPATH)syscall.o : $(KERNELPATH)syscall.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

$(LIBPATH)libc.o: $(LIBPATH)libc.c $(INCPATH)type.h $(INCPATH)const.h    $(INCPATH)proto.h \
			$(INCPATH)global.h
	$(CC) $(CFLAGS) -o $@ $<

	
$(LIBPATH)lib.o : $(LIBPATH)lib.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<



$(USER)lib.o : $(USER)lib.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<
	
$(USER)test.o : $(USER)test.c
	$(CC) $(CFLAGS) -o $@ $<


$(USER)syscall.o: $(USER)syscall.asm $(INCPATH)const.inc
	$(ASM) $(ASMUFLAGS) -o $@ $<
























