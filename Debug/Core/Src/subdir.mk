################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/buttons.c \
../Core/Src/callbacks.c \
../Core/Src/main.c \
../Core/Src/menu.c \
../Core/Src/nixie_tubes.c \
../Core/Src/opt3001.c \
../Core/Src/stm32l0xx_hal_msp.c \
../Core/Src/stm32l0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l0xx.c \
../Core/Src/temperature_sensor.c 

OBJS += \
./Core/Src/buttons.o \
./Core/Src/callbacks.o \
./Core/Src/main.o \
./Core/Src/menu.o \
./Core/Src/nixie_tubes.o \
./Core/Src/opt3001.o \
./Core/Src/stm32l0xx_hal_msp.o \
./Core/Src/stm32l0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l0xx.o \
./Core/Src/temperature_sensor.o 

C_DEPS += \
./Core/Src/buttons.d \
./Core/Src/callbacks.d \
./Core/Src/main.d \
./Core/Src/menu.d \
./Core/Src/nixie_tubes.d \
./Core/Src/opt3001.d \
./Core/Src/stm32l0xx_hal_msp.d \
./Core/Src/stm32l0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l0xx.d \
./Core/Src/temperature_sensor.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L031xx '-DFIRMWARE_VERSION="v0.0.5"' -DFW1=0 -DFW2=0 -DFW3=5 -DID1=0 -DID2=0 -c -I../Core/Inc -I../libSettings -I../DS3231 -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/buttons.cyclo ./Core/Src/buttons.d ./Core/Src/buttons.o ./Core/Src/buttons.su ./Core/Src/callbacks.cyclo ./Core/Src/callbacks.d ./Core/Src/callbacks.o ./Core/Src/callbacks.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/menu.cyclo ./Core/Src/menu.d ./Core/Src/menu.o ./Core/Src/menu.su ./Core/Src/nixie_tubes.cyclo ./Core/Src/nixie_tubes.d ./Core/Src/nixie_tubes.o ./Core/Src/nixie_tubes.su ./Core/Src/opt3001.cyclo ./Core/Src/opt3001.d ./Core/Src/opt3001.o ./Core/Src/opt3001.su ./Core/Src/stm32l0xx_hal_msp.cyclo ./Core/Src/stm32l0xx_hal_msp.d ./Core/Src/stm32l0xx_hal_msp.o ./Core/Src/stm32l0xx_hal_msp.su ./Core/Src/stm32l0xx_it.cyclo ./Core/Src/stm32l0xx_it.d ./Core/Src/stm32l0xx_it.o ./Core/Src/stm32l0xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l0xx.cyclo ./Core/Src/system_stm32l0xx.d ./Core/Src/system_stm32l0xx.o ./Core/Src/system_stm32l0xx.su ./Core/Src/temperature_sensor.cyclo ./Core/Src/temperature_sensor.d ./Core/Src/temperature_sensor.o ./Core/Src/temperature_sensor.su

.PHONY: clean-Core-2f-Src

