################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/diskmgr/DiskManager.cpp 

OBJS += \
./src/diskmgr/DiskManager.o 

CPP_DEPS += \
./src/diskmgr/DiskManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/diskmgr/%.o: ../src/diskmgr/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


