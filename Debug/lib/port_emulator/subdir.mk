################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/port_emulator/port_emulator.c 

C_DEPS += \
./lib/port_emulator/port_emulator.d 

OBJS += \
./lib/port_emulator/port_emulator.o 


# Each subdirectory must supply rules for building sources it contributes
lib/port_emulator/%.o: ../lib/port_emulator/%.c lib/port_emulator/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-port_emulator

clean-lib-2f-port_emulator:
	-$(RM) ./lib/port_emulator/port_emulator.d ./lib/port_emulator/port_emulator.o

.PHONY: clean-lib-2f-port_emulator

