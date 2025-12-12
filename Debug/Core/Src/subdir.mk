################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/GPIOinputs.c \
../Core/Src/SPIhandler.c \
../Core/Src/TimeAndDelays.c \
../Core/Src/main.c \
../Core/Src/statemachine.c \
../Core/Src/statemachine2.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c \
../Core/Src/test.c 

OBJS += \
./Core/Src/GPIOinputs.o \
./Core/Src/SPIhandler.o \
./Core/Src/TimeAndDelays.o \
./Core/Src/main.o \
./Core/Src/statemachine.o \
./Core/Src/statemachine2.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o \
./Core/Src/test.o 

C_DEPS += \
./Core/Src/GPIOinputs.d \
./Core/Src/SPIhandler.d \
./Core/Src/TimeAndDelays.d \
./Core/Src/main.d \
./Core/Src/statemachine.d \
./Core/Src/statemachine2.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d \
./Core/Src/test.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/GPIOinputs.cyclo ./Core/Src/GPIOinputs.d ./Core/Src/GPIOinputs.o ./Core/Src/GPIOinputs.su ./Core/Src/SPIhandler.cyclo ./Core/Src/SPIhandler.d ./Core/Src/SPIhandler.o ./Core/Src/SPIhandler.su ./Core/Src/TimeAndDelays.cyclo ./Core/Src/TimeAndDelays.d ./Core/Src/TimeAndDelays.o ./Core/Src/TimeAndDelays.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/statemachine.cyclo ./Core/Src/statemachine.d ./Core/Src/statemachine.o ./Core/Src/statemachine.su ./Core/Src/statemachine2.cyclo ./Core/Src/statemachine2.d ./Core/Src/statemachine2.o ./Core/Src/statemachine2.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su ./Core/Src/test.cyclo ./Core/Src/test.d ./Core/Src/test.o ./Core/Src/test.su

.PHONY: clean-Core-2f-Src

