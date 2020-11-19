TARGET = firmware

#MC VERSION
CPU_VER = 407

# debug build?
DEBUG = 1
# optimization
OPT = -O0 -g -Wall -fstack-usage -fdump-rtl-dfinish
# Build path
BUILD_DIR = build

# C sources
C_SOURCES =  \
Src/main.c \
Src/stm32f4xx_hal_msp.c \
Src/system_stm32f4xx.c \
Lib/MyLib/rcc.c \
Lib/USB/USB_HOST/usbh_conf.c \
Lib/USB/USB_HOST/usb_host.c \
Lib/USB/STM32_USB_Host_Library/Core/Src/usbh_pipes.c \
Lib/USB/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c \
Lib/USB/STM32_USB_Host_Library/Core/Src/usbh_core.c \
Lib/USB/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c \
Lib/USB/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.c \
Lib/USB/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.c \
Lib/USB/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
Lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c \
Lib/FATFs/ffsystem.c \
Lib/FATFs/ffunicode.c \
Lib/FATFs/ff.c \
Lib/fatfs_conf/usbh_diskio.c \
Lib/fatfs_conf/diskio.c


# ASM sources
ifeq ($(CPU_VER), 429)
ASM_SOURCES =  \
Startup/startup_stm32f429xx.s
C_DEFS =  \
-DSTM32F429xx

# link script
LDSCRIPT = Startup/STM32F429VG_FLASH.ld
endif

ifeq ($(CPU_VER), 407)
ASM_SOURCES =  \
Startup/startup_stm32f407xx.s
C_DEFS =  \
-DSTM32F407xx
# link script
LDSCRIPT = Startup/STM32F407VG_FLASH.ld
endif

GCC_PATH = /home/sergey/soft/gcc-arm-none-eabi/bin

PREFIX = arm-none-eabi-
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)
AS_DEFS = 

AS_INCLUDES = 

C_INCLUDES =  \
-ILib/CMSIS/inc \
-ILib/FATFs \
-ILib/MyLib \
-ILib/fonts \
-ILib/fatfs_conf \
-ILib/STM32F4xx_HAL_Driver/Inc \
-ILib/USB/STM32_USB_Host_Library/Class/MSC/Inc \
-ILib/USB/STM32_USB_Host_Library/Core/Inc \
-ILib/USB/USB_HOST \
-ISrc \
-ILib/fatfs_conf \
-IStartup


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -fdump-rtl-dfinish

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -Wl,--print-memory-usage

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***