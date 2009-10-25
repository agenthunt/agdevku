################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bplustree/BPlusTree.cpp \
../src/bplustree/IndexHeaderPage.cpp \
../src/bplustree/IndexNode.cpp \
../src/bplustree/LeafNode.cpp 

OBJS += \
./src/bplustree/BPlusTree.o \
./src/bplustree/IndexHeaderPage.o \
./src/bplustree/IndexNode.o \
./src/bplustree/LeafNode.o 

CPP_DEPS += \
./src/bplustree/BPlusTree.d \
./src/bplustree/IndexHeaderPage.d \
./src/bplustree/IndexNode.d \
./src/bplustree/LeafNode.d 


# Each subdirectory must supply rules for building sources it contributes
src/bplustree/%.o: ../src/bplustree/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


