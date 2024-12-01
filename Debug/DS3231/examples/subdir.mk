################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DS3231/examples/main.c \
../DS3231/examples/stm32f4xx_it.c 

OBJS += \
./DS3231/examples/main.o \
./DS3231/examples/stm32f4xx_it.o 

C_DEPS += \
./DS3231/examples/main.d \
./DS3231/examples/stm32f4xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
DS3231/examples/%.o DS3231/examples/%.su DS3231/examples/%.cyclo: ../DS3231/examples/%.c DS3231/examples/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L031xx -c -I../Core/Inc -I../DS3231 -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-DS3231-2f-examples

clean-DS3231-2f-examples:
	-$(RM) ./DS3231/examples/main.cyclo ./DS3231/examples/main.d ./DS3231/examples/main.o ./DS3231/examples/main.su ./DS3231/examples/stm32f4xx_it.cyclo ./DS3231/examples/stm32f4xx_it.d ./DS3231/examples/stm32f4xx_it.o ./DS3231/examples/stm32f4xx_it.su

.PHONY: clean-DS3231-2f-examples

