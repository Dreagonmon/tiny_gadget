WCH_RV_TOOLCHAIN_DIR ?= /home/dreagonmon/scripts/MRS_Toolchain_Linux_x64_V1.60

# 编译工具链前缀
PREFIX = $(WCH_RV_TOOLCHAIN_DIR)/RISC-V\ Embedded\ GCC/bin/riscv-none-embed-
# OpenOCD 工具链设置
WCH_OPENOCD = $(WCH_RV_TOOLCHAIN_DIR)/OpenOCD/bin/openocd
WCH_OPENOCD_LD = $(WCH_RV_TOOLCHAIN_DIR)/beforeinstall
WCH_OPENOCD_CFG = $(WCH_RV_TOOLCHAIN_DIR)/OpenOCD/bin/wch-riscv.cfg
# 编译目标
BUILD_TARGET = ch32v003-template
# 编译的目标目录
BUILD_DIR = build
# 设置调试标志
DEBUG = 1
# 架构相关选项
ARCH = -march=rv32ec -mabi=ilp32e
# 系统优化选项配置
OPT = -Os
# C语言标准
STD = -std=gnu17

# C源文件目录
C_SOURCES = \
Core/core_riscv.c \
Core/ch32v00x_it.c \
Core/system_ch32v00x.c \
Core/Debug/debug.c \
Core/Peripheral/src/ch32v00x_adc.c \
Core/Peripheral/src/ch32v00x_dbgmcu.c \
Core/Peripheral/src/ch32v00x_dma.c \
Core/Peripheral/src/ch32v00x_exti.c \
Core/Peripheral/src/ch32v00x_flash.c \
Core/Peripheral/src/ch32v00x_gpio.c \
Core/Peripheral/src/ch32v00x_i2c.c \
Core/Peripheral/src/ch32v00x_iwdg.c \
Core/Peripheral/src/ch32v00x_misc.c \
Core/Peripheral/src/ch32v00x_opa.c \
Core/Peripheral/src/ch32v00x_pwr.c \
Core/Peripheral/src/ch32v00x_rcc.c \
Core/Peripheral/src/ch32v00x_spi.c \
Core/Peripheral/src/ch32v00x_tim.c \
Core/Peripheral/src/ch32v00x_usart.c \
Core/Peripheral/src/ch32v00x_wwdg.c \
SRC/main.c

ASM_SOURCES = \
Core/Startup/startup_ch32v00x.S

C_INCLUDES = \
-ICore \
-ICore/Debug \
-ICore/Peripheral/inc \
-ISRC

# 编译器定义
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

# 编译器选项
CFLAGS = $(ARCH) \
$(OPT) \
$(STD) \
-msmall-data-limit=0 \
-msave-restore \
-fmessage-length=0 \
-fsigned-char \
-ffunction-sections \
-fdata-sections \
-fno-common \
-fstack-usage \
-Wunused -Wuninitialized \
$(C_INCLUDES) \
-MMD -MP -MF"$(@:%.o=%.d)"

ASFLAGS = $(ARCH) $(OPT) -x assembler

# 链接文件定义
LDSCRIPT = Core/Ld/Link.ld
# 链接库定义
LIBS = -lnosys

# 链接选项
LDFLAGS = $(ARCH) \
$(OPT) \
$(STD) \
-msmall-data-limit=0 \
-msave-restore \
-fmessage-length=0 \
-fsigned-char \
-ffunction-sections \
-fdata-sections \
-fno-common \
-fstack-usage \
-Wunused -Wuninitialized \
-T$(LDSCRIPT) \
-nostartfiles \
-Xlinker \
--gc-sections \
-Wl,-Map,$(BUILD_DIR)/${BUILD_TARGET}.map \
--specs=nano.specs \
--specs=nosys.specs \
$(LIBS)

# 调试选项
ifeq ($(DEBUG), 1)
CFLAGS += -g
LDFLAGS += -g
endif


# 指定目标文件列表(C文件所构建)
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
# 指定C文件搜索路径
vpath %.c $(sort $(dir $(C_SOURCES)))
# 指定目标文件列表(汇编文件所构建)
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))
# 指定汇编文件搜索路径
vpath %.S $(sort $(dir $(ASM_SOURCES)))

# 完整构建需要构建elf可执行程序，hex文件和bin文件
all:$(BUILD_DIR)/$(BUILD_TARGET).elf $(BUILD_DIR)/$(BUILD_TARGET).hex $(BUILD_DIR)/$(BUILD_TARGET).bin

# 对所有的C文件，全部编译为目标文件
$(BUILD_DIR)/%.o:%.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

# 对所有的汇编文件，全部编译为目标文件
$(BUILD_DIR)/%.o:%.S Makefile | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

# 链接目标文件为可执行程序
$(BUILD_DIR)/$(BUILD_TARGET).elf:$(OBJECTS) Makefile
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	$(SZ) --format=berkeley $@

# 生成hex文件
$(BUILD_DIR)/%.hex:$(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

# 生成bin文件
$(BUILD_DIR)/%.bin:$(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

# 生成构建目录
$(BUILD_DIR):
	mkdir $@

# 清理构建
clean:
	rm -rf $(BUILD_DIR)

erase:
	LD_LIBRARY_PATH="$(WCH_OPENOCD_LD):$(LD_LIBRARY_PATH)" "$(WCH_OPENOCD)" -f $(WCH_OPENOCD_CFG) -c init -c halt -c "flash erase_sector wch_riscv 0 last" -c exit

flash: all
	LD_LIBRARY_PATH="$(WCH_OPENOCD_LD):$(LD_LIBRARY_PATH)" "$(WCH_OPENOCD)" -f $(WCH_OPENOCD_CFG) -c init -c halt -c "flash erase_sector wch_riscv 0 last " -c "program $(BUILD_DIR)/$(BUILD_TARGET).elf" -c "verify_image $(BUILD_DIR)/$(BUILD_TARGET).elf" -c reset -c resume -c exit

reset:
	LD_LIBRARY_PATH="$(WCH_OPENOCD_LD):$(LD_LIBRARY_PATH)" "$(WCH_OPENOCD)" -f $(WCH_OPENOCD_CFG) -c init -c reset -c resume -c exit

start_openocd:
	LD_LIBRARY_PATH="$(WCH_OPENOCD_LD):$(LD_LIBRARY_PATH)" "$(WCH_OPENOCD)" -f $(WCH_OPENOCD_CFG)

stop_openocd:
	pkill -f "$(WCH_OPENOCD)"

gdb:
	$(PREFIX)gdb "$(BUILD_DIR)/$(BUILD_TARGET).elf"
