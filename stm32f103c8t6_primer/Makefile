####################################
# stm32 timer led example Makefile
####################################

PROGRAM = timer_led


ALL_OFILES = main.o startup.o stm32_gpio.o system_stm32f10x.o

TARGET_BIN = $(PROGRAM).bin
TARGET_ELF = $(PROGRAM).elf

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
CP = arm-none-eabi-objcopy

LKR_SCRIPT = ./stm32_libs/memory_layout.ld

FAMILY = STM32F10X_MD

DEFINES = -D$(FAMILY)
INCLUDES = -I.stm32_libs/ \
            -I.stm32_libs/SPL/ \


CFLAGS  = -c -march=armv7-m -mcpu=cortex-m3 -mthumb \
          -fno-common -nostdlib -fno-builtin -ffreestanding \
          -Wall -O0 -g -ffunction-sections -fdata-sections \
          $(DEFINES)

LFLAGS  = -nostartfiles -T$(LKR_SCRIPT) -I.stm32_libs/ -I.stm32_libs/SPL/  --gc-sections
CPFLAGS = -Obinary

.PHONY: all clean write

all: $(TARGET_BIN)

$(TARGET_BIN): $(TARGET_ELF)
	$(CP) $(CPFLAGS) $< $@

$(TARGET_ELF): $(ALL_OFILES)
	$(LD) $(LFLAGS) $(ALL_OFILES) -o $@


startup.o: stm32_libs/startup.c
	$(CC) $(INCLUDES) $(CFLAGS) stm32_libs/startup.c -o $@

system_stm32f10x.o: stm32_libs/system_stm32f10x.c
	$(CC) $(INCLUDES) $(CFLAGS) stm32_libs/system_stm32f10x.c -o $@

stm32_gpio.o: stm32_libs/boctok/stm32_gpio.c
	$(CC) $(INCLUDES) $(CFLAGS) stm32_libs/boctok/stm32_gpio.c -o $@

main.o: main.c
	$(CC) $(INCLUDES) $(CFLAGS) main.c -o $@

clean:
	rm -rf *.o *.elf *.bin

write:
	/opt/stlink/st-upload.sh $(TARGET_BIN)

