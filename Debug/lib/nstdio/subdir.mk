################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/nstdio/nstdio.c 

C_DEPS += \
./lib/nstdio/nstdio.d 

OBJS += \
./lib/nstdio/nstdio.o 


# Each subdirectory must supply rules for building sources it contributes
lib/nstdio/%.o: ../lib/nstdio/%.c lib/nstdio/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lib-2f-nstdio

clean-lib-2f-nstdio:
	-$(RM) ./lib/nstdio/nstdio.d ./lib/nstdio/nstdio.o

.PHONY: clean-lib-2f-nstdio

