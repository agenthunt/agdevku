################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/heap/DBMainHeaderPage.cpp \
../src/heap/DataPage.cpp \
../src/heap/DirectoryEntry.cpp \
../src/heap/DirectoryHeaderPage.cpp \
../src/heap/DirectoryPage.cpp \
../src/heap/HeapFile.cpp 

OBJS += \
./src/heap/DBMainHeaderPage.o \
./src/heap/DataPage.o \
./src/heap/DirectoryEntry.o \
./src/heap/DirectoryHeaderPage.o \
./src/heap/DirectoryPage.o \
./src/heap/HeapFile.o 

CPP_DEPS += \
./src/heap/DBMainHeaderPage.d \
./src/heap/DataPage.d \
./src/heap/DirectoryEntry.d \
./src/heap/DirectoryHeaderPage.d \
./src/heap/DirectoryPage.d \
./src/heap/HeapFile.d 


# Each subdirectory must supply rules for building sources it contributes
src/heap/%.o: ../src/heap/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


