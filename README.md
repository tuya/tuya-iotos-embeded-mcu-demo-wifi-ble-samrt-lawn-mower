# Tuya IoTOS Embeded MCU Demo WiFi & BLE smart lawn mower

[English](./README.md) | [中文](./README_zh.md)

## Introduction 


This demo is based on Tuya Smart Cloud Platform, Tuya Smart App, IoTOS Embeded MCU SDK, using Tuya WiFi/WiFi+BLE series modules and GD32F4 series MCUs to build a smart lawn mower demo with the following functions.
+ Can set the top mowing area at will
+ Support cell phone APP to control the movement of the mower
+ Support cell phone APP to set the blade height position and blade speed
+ Support bow automatic mode


## Quick start

### Compile and burn
+ clone demo code

+ Execute Project.uvprojx file, open it with keil

+ click compile in software and finish download


### File introduction

```
├── User	
|    ├── main.c                   //main file
|    ├── gd32f4xx_it.c              //Interrupt service function implementation file
|    ├── gd32f4xx_it.h           
|    ├── movement.c            //Motion control logic files
|    ├── movement.h            
|    ├── servo_motor.c            //Motor, servo, ESC drive interface files
|    ├── servo_motor.h 
|    ├── QMC5883L.c            //Geomagnetic sensor driver interface file
|    ├── QMC5883L.h
|    ├── map.c            //GPS-related application interface files
|    ├── map.h
|    ├── systick.c            //systick Configuration file
|    ├── systick.h
|    ├── uart.c            //Serial port related files
|    └── uart.h
|
├── mcu_sdk				//tuya mcu sdk related files
|
├── FreeRTOS	
|    .
|    .
|    .

```

<br>

### Demo entry

Entry file: main.c

Important functions: main()

### DP point related

+ Uploaded dp point processing: mcu_dp_value_update()

| function name | unsigned char mcu_dp_value_update(unsigned char dpid,unsigned long value) |
| ------ | ------------------------------------------------------------ |
| dpid | DP's ID number |
| value | DP data |
| Return | SUCCESS: Success ERROR: Failure |

### I/O list


|right front motor|left front motor|right rear motor|left rear motor|servo_1|servo_2|blade motor|wifi module|GPS module|
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
|`pwm` PA15|`pwm` PB9|`pwm` PB10|`pwm` PB11|`pwm` PC6|`pwm` PC7|`pwm` PC8|`MCU_TX0` PA9|`MCU_TX1` PD5|
|`+` PD0|`+` PD1|`+` PD3|`+` PD8||||`MCU_RX0` PA10|`MCU_RX1` PD6|
|`-`PD7|`-` PD2|`-` PD4|`-` PD9||||||
<br>



## Related Documents

Tuya Demo Center: https://developer.tuya.com/demo


<br>


## Technical Support

You can get support for Tuya by using the following methods:

- Developer Center: https://developer.tuya.com
- Help Center: https://support.tuya.com/help
- Technical Support Work Order Center: [https://service.console.tuya.com](https://service.console.tuya.com/) 


<br>


