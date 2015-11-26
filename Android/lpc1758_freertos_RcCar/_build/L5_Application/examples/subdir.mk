################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../L5_Application/examples/examples.cpp \
../L5_Application/examples/rn_xv_task.cpp 

OBJS += \
./L5_Application/examples/examples.o \
./L5_Application/examples/rn_xv_task.o 

CPP_DEPS += \
./L5_Application/examples/examples.d \
./L5_Application/examples/rn_xv_task.d 


# Each subdirectory must supply rules for building sources it contributes
L5_Application/examples/%.o: ../L5_Application/examples/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wshadow -Wlogical-op -Wfloat-equal -DBUILD_CFG_MPU=0 -I"C:\SJSU\Cmpe240_autonomous_car_Fall15\lpc1758_freertos_RcCar" -I"C:\SJSU\Cmpe240_autonomous_car_Fall15\lpc1758_freertos_RcCar\newlib" -I"C:\SJSU\Cmpe240_autonomous_car_Fall15\lpc1758_freertos_RcCar\L0_LowLevel" -I"C:\SJSU\Cmpe240_autonomous_car_Fall15\lpc1758_freertos_RcCar\L1_FreeRTOS" -I"C:\SJSU\Cmpe240_autonomous_car_Fall15\lpc1758_freertos_RcCar\L1_FreeRTOS\include" -I"C:\SJSU\Cmpe240_autonomous_car_Fall15\lpc1758_freertos_RcCar\L1_FreeRTOS\portable" -I"C:\SJSU\Cmpe240_autonomous_car_Fall15\lpc1758_freertos_RcCar\L2_Drivers" -I"C:\SJSU\Cmpe240_autonomous_car_Fall15\lpc1758_freertos_RcCar\L2_Drivers\base" -I"C:\SJSU\Cmpe240_autonomous_car_Fall15\lpc1758_freertos_RcCar\L3_Utils" -I"C:\SJSU\Cmpe240_autonomous_car_Fall15\lpc1758_freertos_RcCar\L3_Utils\tlm" -I"C:\SJSU\Cmpe240_autonomous_car_Fall15\lpc1758_freertos_RcCar\L4_IO" -I"C:\SJSU\Cmpe240_autonomous_car_Fall15\lpc1758_freertos_RcCar\L4_IO\fat" -I"C:\SJSU\Cmpe240_autonomous_car_Fall15\lpc1758_freertos_RcCar\L4_IO\wireless" -I"C:\SJSU\Cmpe240_autonomous_car_Fall15\lpc1758_freertos_RcCar\L5_Application" -std=gnu++11 -fabi-version=0 -fno-exceptions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


