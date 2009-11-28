################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/heap/DataPage.cpp \
../src/heap/DirectoryEntry.cpp \
../src/heap/DirectoryHeaderPage.cpp \
../src/heap/DirectoryPage.cpp \
../src/heap/HeapFile.cpp \
../src/heap/TableScan.cpp \
../src/heap/Tuple.cpp 

OBJS += \
./src/heap/DataPage.o \
./src/heap/DirectoryEntry.o \
./src/heap/DirectoryHeaderPage.o \
./src/heap/DirectoryPage.o \
./src/heap/HeapFile.o \
./src/heap/TableScan.o \
./src/heap/Tuple.o 

CPP_DEPS += \
./src/heap/DataPage.d \
./src/heap/DirectoryEntry.d \
./src/heap/DirectoryHeaderPage.d \
./src/heap/DirectoryPage.d \
./src/heap/HeapFile.d \
./src/heap/TableScan.d \
./src/heap/Tuple.d 


# Each subdirectory must supply rules for building sources it contributes
src/heap/%.o: ../src/heap/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


