# Binaries will be generated with this name (.elf, .bin, .hex, etc)
PROJ_NAME=main

# Put your stlink folder here so make burn will work.
STM32CUBEPROG:= ../../stmcubeprog/bin/STM32_Programmer.sh -vb 1 -q -c port=SWD 
SDK = ../../sdk

# Put your source files here (or *.c, etc)
SRCS = main.c 
SRCS += usart.c
SRCS += misc.c
SRCS += delay.c


# Normally you shouldn't need to change anything below this line!
#######################################################################################
GNUGCC = ../../gcc-arm-none-eabi/bin
CC = $(GNUGCC)/arm-none-eabi-gcc
OBJCOPY = $(GNUGCC)/arm-none-eabi-objcopy
SIZE =  $(GNUGCC)/arm-none-eabi-size

CFLAGS  = -g -O2 -Wall -T$(SDK)/stm32_flash.ld 
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
# important flag is -fsingle-precision-constant which prevents the double precision emulation
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant
CFLAGS += -I.

# Include files from STM libraries
CFLAGS += -I $(SDK)

# add startup file to build
SRCS += $(SDK)/startup_stm32f4xx.s 
SRCS += $(SDK)/system_stm32f4xx.c 
OBJS = $(SRCS:.c=.o)


.PHONY: proj

all: $(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -lm -lc -lnosys -o $@ 
	$(CC) $(CFLAGS) -S $< $^
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	$(SIZE) -B  $(PROJ_NAME).elf
	ls -l $(PROJ_NAME).bin
#	@./build


clean:
	rm -rf *.o $(PROJ_NAME).elf $(PROJ_NAME).hex $(PROJ_NAME).bin *.s
	ls

# Flash the STM32F4

example:
	@$(STM32CUBEPROG) -w students.bin  0x08000000 
	@sleep 1
	@$(STM32CUBEPROG) -hardRst 

upload: proj
	@$(STM32CUBEPROG) -w $(PROJ_NAME).bin  0x08000000 
	@sleep 1
	@$(STM32CUBEPROG) -hardRst 

