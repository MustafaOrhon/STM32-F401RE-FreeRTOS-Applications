################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third_Party/FreeRTOS/org/Source/croutine.c \
../Third_Party/FreeRTOS/org/Source/event_groups.c \
../Third_Party/FreeRTOS/org/Source/list.c \
../Third_Party/FreeRTOS/org/Source/queue.c \
../Third_Party/FreeRTOS/org/Source/stream_buffer.c \
../Third_Party/FreeRTOS/org/Source/tasks.c \
../Third_Party/FreeRTOS/org/Source/timers.c 

OBJS += \
./Third_Party/FreeRTOS/org/Source/croutine.o \
./Third_Party/FreeRTOS/org/Source/event_groups.o \
./Third_Party/FreeRTOS/org/Source/list.o \
./Third_Party/FreeRTOS/org/Source/queue.o \
./Third_Party/FreeRTOS/org/Source/stream_buffer.o \
./Third_Party/FreeRTOS/org/Source/tasks.o \
./Third_Party/FreeRTOS/org/Source/timers.o 

C_DEPS += \
./Third_Party/FreeRTOS/org/Source/croutine.d \
./Third_Party/FreeRTOS/org/Source/event_groups.d \
./Third_Party/FreeRTOS/org/Source/list.d \
./Third_Party/FreeRTOS/org/Source/queue.d \
./Third_Party/FreeRTOS/org/Source/stream_buffer.d \
./Third_Party/FreeRTOS/org/Source/tasks.d \
./Third_Party/FreeRTOS/org/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
Third_Party/FreeRTOS/org/Source/%.o Third_Party/FreeRTOS/org/Source/%.su Third_Party/FreeRTOS/org/Source/%.cyclo: ../Third_Party/FreeRTOS/org/Source/%.c Third_Party/FreeRTOS/org/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I"D:/CubeIde_FreeRTOS/4_LED_BUTTON_ISR_Notify_FreeRTOS/Third_Party/SEGGER/Config" -I"D:/CubeIde_FreeRTOS/4_LED_BUTTON_ISR_Notify_FreeRTOS/Third_Party/SEGGER/SEGGER" -I"D:/CubeIde_FreeRTOS/4_LED_BUTTON_ISR_Notify_FreeRTOS/Third_Party/SEGGER/OS" -I"D:/CubeIde_FreeRTOS/4_LED_BUTTON_ISR_Notify_FreeRTOS/Third_Party/FreeRTOS/org/Source/include" -I"D:/CubeIde_FreeRTOS/4_LED_BUTTON_ISR_Notify_FreeRTOS/Third_Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"D:/CubeIde_FreeRTOS/4_LED_BUTTON_ISR_Notify_FreeRTOS/Third_Party/FreeRTOS/org/Source/portable/MemMang" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Third_Party-2f-FreeRTOS-2f-org-2f-Source

clean-Third_Party-2f-FreeRTOS-2f-org-2f-Source:
	-$(RM) ./Third_Party/FreeRTOS/org/Source/croutine.cyclo ./Third_Party/FreeRTOS/org/Source/croutine.d ./Third_Party/FreeRTOS/org/Source/croutine.o ./Third_Party/FreeRTOS/org/Source/croutine.su ./Third_Party/FreeRTOS/org/Source/event_groups.cyclo ./Third_Party/FreeRTOS/org/Source/event_groups.d ./Third_Party/FreeRTOS/org/Source/event_groups.o ./Third_Party/FreeRTOS/org/Source/event_groups.su ./Third_Party/FreeRTOS/org/Source/list.cyclo ./Third_Party/FreeRTOS/org/Source/list.d ./Third_Party/FreeRTOS/org/Source/list.o ./Third_Party/FreeRTOS/org/Source/list.su ./Third_Party/FreeRTOS/org/Source/queue.cyclo ./Third_Party/FreeRTOS/org/Source/queue.d ./Third_Party/FreeRTOS/org/Source/queue.o ./Third_Party/FreeRTOS/org/Source/queue.su ./Third_Party/FreeRTOS/org/Source/stream_buffer.cyclo ./Third_Party/FreeRTOS/org/Source/stream_buffer.d ./Third_Party/FreeRTOS/org/Source/stream_buffer.o ./Third_Party/FreeRTOS/org/Source/stream_buffer.su ./Third_Party/FreeRTOS/org/Source/tasks.cyclo ./Third_Party/FreeRTOS/org/Source/tasks.d ./Third_Party/FreeRTOS/org/Source/tasks.o ./Third_Party/FreeRTOS/org/Source/tasks.su ./Third_Party/FreeRTOS/org/Source/timers.cyclo ./Third_Party/FreeRTOS/org/Source/timers.d ./Third_Party/FreeRTOS/org/Source/timers.o ./Third_Party/FreeRTOS/org/Source/timers.su

.PHONY: clean-Third_Party-2f-FreeRTOS-2f-org-2f-Source

