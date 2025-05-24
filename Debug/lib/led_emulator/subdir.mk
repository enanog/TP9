################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/led_emulator/led_emulator.c 

C_DEPS += \
./lib/led_emulator/led_emulator.d 

OBJS += \
./lib/led_emulator/led_emulator.o 


# Each subdirectory must supply rules for building sources it contributes
lib/led_emulator/%.o: ../lib/led_emulator/%.c lib/led_emulator/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-led_emulator

clean-lib-2f-led_emulator:
	-$(RM) ./lib/led_emulator/led_emulator.d ./lib/led_emulator/led_emulator.o

.PHONY: clean-lib-2f-led_emulator

