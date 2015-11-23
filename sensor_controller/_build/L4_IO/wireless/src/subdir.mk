################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../L4_IO/wireless/src/mesh.c \
../L4_IO/wireless/src/nrf24L01Plus.c \
../L4_IO/wireless/src/wireless.c 

OBJS += \
./L4_IO/wireless/src/mesh.o \
./L4_IO/wireless/src/nrf24L01Plus.o \
./L4_IO/wireless/src/wireless.o 

C_DEPS += \
./L4_IO/wireless/src/mesh.d \
./L4_IO/wireless/src/nrf24L01Plus.d \
./L4_IO/wireless/src/wireless.d 


# Each subdirectory must supply rules for building sources it contributes
L4_IO/wireless/src/%.o: ../L4_IO/wireless/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wshadow -Wlogical-op -Wfloat-equal -DBUILD_CFG_MPU=0 -I"C:\cmpe244\projects\sensor_controller" -I"C:\cmpe244\projects\sensor_controller\newlib" -I"C:\cmpe244\projects\sensor_controller\L0_LowLevel" -I"C:\cmpe244\projects\sensor_controller\L1_FreeRTOS" -I"C:\cmpe244\projects\sensor_controller\L1_FreeRTOS\include" -I"C:\cmpe244\projects\sensor_controller\L1_FreeRTOS\portable" -I"C:\cmpe244\projects\sensor_controller\L2_Drivers" -I"C:\cmpe244\projects\sensor_controller\L2_Drivers\base" -I"C:\cmpe244\projects\sensor_controller\L3_Utils" -I"C:\cmpe244\projects\sensor_controller\L3_Utils\tlm" -I"C:\cmpe244\projects\sensor_controller\L4_IO" -I"C:\cmpe244\projects\sensor_controller\L4_IO\fat" -I"C:\cmpe244\projects\sensor_controller\L4_IO\wireless" -I"C:\cmpe244\projects\sensor_controller\L5_Application" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


