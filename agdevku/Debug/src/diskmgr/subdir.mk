################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/diskmgr/DiskManager.cpp \
../src/diskmgr/FreePageManager.cpp \
../src/diskmgr/SimpleDiskFileAccessor.cpp 

OBJS += \
./src/diskmgr/DiskManager.o \
./src/diskmgr/FreePageManager.o \
./src/diskmgr/SimpleDiskFileAccessor.o 

CPP_DEPS += \
./src/diskmgr/DiskManager.d \
./src/diskmgr/FreePageManager.d \
./src/diskmgr/SimpleDiskFileAccessor.d 


# Each subdirectory must supply rules for building sources it contributes
src/diskmgr/%.o: ../src/diskmgr/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


