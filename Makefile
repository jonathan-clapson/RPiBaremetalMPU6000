ARCH = arm-none-eabi
CC = ${ARCH}-gcc
CPP = ${ARCH}-g++
AS = ${ARCH}-as
LD = ${ARCH}-ld
AR = ${ARCH}-ar
OBJCOPY = ${ARCH}-objcopy

LIB = -L /mnt/scratch/gnuarm/arm-none-eabi/lib/ -L/mnt/scratch/gnuarm/lib/gcc/arm-none-eabi/4.8.1

PLATFORM = raspi

# Release Version -> Optimize 
#CFLAGS = -O3 -std=gnu99 -Werror -D__$(PLATFORM)__ -DRASPBERRY_PI
#ASFLAGS =

CFLAGS = -O2 -nostartfiles -ffreestanding -g -std=gnu99 -Wall -D__$(PLATFORM)__ -DRASPBERRY_PI -fno-builtin
#CFLAGS = -O0 -nostartfiles -ffreestanding -g -std=gnu99 -Wall -D__$(PLATFORM)__ -DRASPBERRY_PI -fno-builtin
ASFLAGS = -g

CFLAGS_FOR_TARGET = -mcpu=arm1176jzf-s
ASFLAGS_FOR_TARGET = -mcpu=arm1176jzf-s
LDFLAGS = -nostdlib -static --error-unresolved-symbols 

MODULES := raspi
SRC_DIR := $(addprefix src/,$(MODULES))
INC_DIR := $(addsuffix /include,$(SRC_DIR)):${GCC_PREFIX}/arm-none-eabi/include
BUILD_DIR := $(addsuffix /build,$(SRC_DIR))

ASRC     := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.s))
AOBJ     := $(ASRC:.s=.o)
CSRC     := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
COBJ     := $(CSRC:.c=.o)

INCLUDES := -Isrc $(addprefix -I,$(SRC_DIR) $(INC_DIR))
vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)
vpath %.s $(SRC_DIR)

%.o: %.c
	$(CC) $(CFLAGS_FOR_TARGET) $(INCLUDES) $(CFLAGS) -c -o $*.o $<
	
%.o: %.s
	$(AS) $(ASFLAGS_FOR_TARGET) $(INCLUDES) $(ASFLAGS) -o $*.o $<

OBJ = $(AOBJ) $(COBJ)

bin/kernel.img: bin/kernel.elf
	${OBJCOPY} -O binary $< $@
	${OBJCOPY} bin/kernel.elf -O ihex bin/kernel.hex
	${OBJCOPY} bin/kernel.elf -O binary bin/kernel.bin

bin/kernel.elf: raspi.ld $(OBJ) 
	${LD} ${LDFLAGS} $(OBJ) -Map bin/kernel.map -o $@ -T raspi.ld $(LIB) -lc -lgcc -lm

clean:
	rm -f bin/*.elf bin/*.img bin/*.map bin/*.hex bin/*.bin $(OBJ)
	
install: bin/kernel.elf raspi.ld $(OBJ)
	cp -f bin/kernel.img /media/jcla223/boot/kernel.img
	umount /media/jcla223/boot
	umount /media/jcla223/41cd5baa-7a62-4706-b8e8-02c43ccee8d9
