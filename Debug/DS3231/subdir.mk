################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DS3231/ds3231_for_stm32_hal.c 

OBJS += \
./DS3231/ds3231_for_stm32_hal.o 

C_DEPS += \
./DS3231/ds3231_for_stm32_hal.d 


# Each subdirectory must supply rules for building sources it contributes
DS3231/%.o DS3231/%.su DS3231/%.cyclo: ../DS3231/%.c DS3231/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L031xx '-DFIRMWARE_VERSION="v0.0.5"' -DFW1=0 -DFW2=0 -DFW3=5 -DID1=0 -DID2=0 -c -I../Core/Inc -I../libSettings -I../DS3231 -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-DS3231

clean-DS3231:
	-$(RM) ./DS3231/ds3231_for_stm32_hal.cyclo ./DS3231/ds3231_for_stm32_hal.d ./DS3231/ds3231_for_stm32_hal.o ./DS3231/ds3231_for_stm32_hal.su

.PHONY: clean-DS3231

