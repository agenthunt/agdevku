################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bufmgr/BufferManager.cpp \
../src/bufmgr/Frame.cpp \
../src/bufmgr/LRUReplacementPolicy.cpp 

OBJS += \
./src/bufmgr/BufferManager.o \
./src/bufmgr/Frame.o \
./src/bufmgr/LRUReplacementPolicy.o 

CPP_DEPS += \
./src/bufmgr/BufferManager.d \
./src/bufmgr/Frame.d \
./src/bufmgr/LRUReplacementPolicy.d 


# Each subdirectory must supply rules for building sources it contributes
src/bufmgr/%.o: ../src/bufmgr/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


