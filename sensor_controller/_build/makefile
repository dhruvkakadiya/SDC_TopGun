################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include newlib/subdir.mk
-include L5_Application/source/cmd_handlers/subdir.mk
-include L5_Application/source/subdir.mk
-include L5_Application/periodic_scheduler/subdir.mk
-include L5_Application/examples/subdir.mk
-include L5_Application/subdir.mk
-include L4_IO/wireless/src/subdir.mk
-include L4_IO/src/subdir.mk
-include L4_IO/fat/option/subdir.mk
-include L4_IO/fat/disk/subdir.mk
-include L4_IO/fat/subdir.mk
-include L3_Utils/tlm/src/subdir.mk
-include L3_Utils/src/subdir.mk
-include L2_Drivers/src/subdir.mk
-include L2_Drivers/base/subdir.mk
-include L1_FreeRTOS/src/subdir.mk
-include L1_FreeRTOS/portable/no_mpu/subdir.mk
-include L1_FreeRTOS/portable/subdir.mk
-include L1_FreeRTOS/hooks/subdir.mk
-include L0_LowLevel/source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
sensor_controller.hex \

SECONDARY_LIST += \
sensor_controller.lst \

SECONDARY_SIZE += \
sensor_controller.siz \


# All Target
all: sensor_controller.elf secondary-outputs

# Tool invocations
sensor_controller.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wshadow -Wlogical-op -Wfloat-equal -T ../loader.ld -nostartfiles -Xlinker --gc-sections -Wl,-Map,"sensor_controller.map" -specs=nano.specs -o "sensor_controller.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

sensor_controller.hex: sensor_controller.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "sensor_controller.elf"  "sensor_controller.hex"
	@echo 'Finished building: $@'
	@echo ' '

sensor_controller.lst: sensor_controller.elf
	@echo 'Invoking: Cross ARM GNU Create Listing'
	arm-none-eabi-objdump --source --all-headers --demangle --line-numbers --wide "sensor_controller.elf" > "sensor_controller.lst"
	@echo 'Finished building: $@'
	@echo ' '

sensor_controller.siz: sensor_controller.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "sensor_controller.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(C++_DEPS)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(CC_DEPS)$(SECONDARY_FLASH)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS)$(SECONDARY_LIST)$(S_UPPER_DEPS) sensor_controller.elf
	-@echo ' '

post-build:
	-@echo 'Create binary file'
	-arm-none-eabi-objcopy -O binary sensor_controller.elf sensor_controller.bin
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
