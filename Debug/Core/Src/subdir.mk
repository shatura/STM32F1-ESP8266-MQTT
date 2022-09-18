################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/MQTTConnectClient.c \
../Core/Src/MQTTConnectServer.c \
../Core/Src/MQTTDeserializePublish.c \
../Core/Src/MQTTFormat.c \
../Core/Src/MQTTPacket.c \
../Core/Src/MQTTSerializePublish.c \
../Core/Src/MQTTSubscribeClient.c \
../Core/Src/MQTTSubscribeServer.c \
../Core/Src/MQTTUnsubscribeClient.c \
../Core/Src/MQTTUnsubscribeServer.c \
../Core/Src/dma.c \
../Core/Src/esp8266.c \
../Core/Src/gpio.c \
../Core/Src/led.c \
../Core/Src/main.c \
../Core/Src/mqttclient.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/MQTTConnectClient.o \
./Core/Src/MQTTConnectServer.o \
./Core/Src/MQTTDeserializePublish.o \
./Core/Src/MQTTFormat.o \
./Core/Src/MQTTPacket.o \
./Core/Src/MQTTSerializePublish.o \
./Core/Src/MQTTSubscribeClient.o \
./Core/Src/MQTTSubscribeServer.o \
./Core/Src/MQTTUnsubscribeClient.o \
./Core/Src/MQTTUnsubscribeServer.o \
./Core/Src/dma.o \
./Core/Src/esp8266.o \
./Core/Src/gpio.o \
./Core/Src/led.o \
./Core/Src/main.o \
./Core/Src/mqttclient.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/MQTTConnectClient.d \
./Core/Src/MQTTConnectServer.d \
./Core/Src/MQTTDeserializePublish.d \
./Core/Src/MQTTFormat.d \
./Core/Src/MQTTPacket.d \
./Core/Src/MQTTSerializePublish.d \
./Core/Src/MQTTSubscribeClient.d \
./Core/Src/MQTTSubscribeServer.d \
./Core/Src/MQTTUnsubscribeClient.d \
./Core/Src/MQTTUnsubscribeServer.d \
./Core/Src/dma.d \
./Core/Src/esp8266.d \
./Core/Src/gpio.d \
./Core/Src/led.d \
./Core/Src/main.d \
./Core/Src/mqttclient.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/MQTTConnectClient.d ./Core/Src/MQTTConnectClient.o ./Core/Src/MQTTConnectClient.su ./Core/Src/MQTTConnectServer.d ./Core/Src/MQTTConnectServer.o ./Core/Src/MQTTConnectServer.su ./Core/Src/MQTTDeserializePublish.d ./Core/Src/MQTTDeserializePublish.o ./Core/Src/MQTTDeserializePublish.su ./Core/Src/MQTTFormat.d ./Core/Src/MQTTFormat.o ./Core/Src/MQTTFormat.su ./Core/Src/MQTTPacket.d ./Core/Src/MQTTPacket.o ./Core/Src/MQTTPacket.su ./Core/Src/MQTTSerializePublish.d ./Core/Src/MQTTSerializePublish.o ./Core/Src/MQTTSerializePublish.su ./Core/Src/MQTTSubscribeClient.d ./Core/Src/MQTTSubscribeClient.o ./Core/Src/MQTTSubscribeClient.su ./Core/Src/MQTTSubscribeServer.d ./Core/Src/MQTTSubscribeServer.o ./Core/Src/MQTTSubscribeServer.su ./Core/Src/MQTTUnsubscribeClient.d ./Core/Src/MQTTUnsubscribeClient.o ./Core/Src/MQTTUnsubscribeClient.su ./Core/Src/MQTTUnsubscribeServer.d ./Core/Src/MQTTUnsubscribeServer.o ./Core/Src/MQTTUnsubscribeServer.su ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/esp8266.d ./Core/Src/esp8266.o ./Core/Src/esp8266.su ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/led.d ./Core/Src/led.o ./Core/Src/led.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mqttclient.d ./Core/Src/mqttclient.o ./Core/Src/mqttclient.su ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

