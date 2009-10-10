################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bplustree/BPlusTreeIndex.cpp \
../src/bplustree/BPlusTreeLeaf.cpp \
../src/bplustree/IndexFile.cpp 

OBJS += \
./src/bplustree/BPlusTreeIndex.o \
./src/bplustree/BPlusTreeLeaf.o \
./src/bplustree/IndexFile.o 

CPP_DEPS += \
./src/bplustree/BPlusTreeIndex.d \
./src/bplustree/BPlusTreeLeaf.d \
./src/bplustree/IndexFile.d 


# Each subdirectory must supply rules for building sources it contributes
src/bplustree/%.o: ../src/bplustree/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


