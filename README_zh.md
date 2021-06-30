# Tuya IoTOS Embeded MCU Demo WiFi & BLE smart lawn mower

[English](./README.md) | [中文](./README_zh.md)

## 简介 


本 Demo 基于涂鸦智能云平台、涂鸦智能APP、IoTOS Embeded MCU SDK，使用涂鸦WiFi/WiFi+BLE系列模组以及 GD32F4 系列 MCU 组建一个智能割草机 demo，具有以下功能：
+ 可随意设顶割草范围面积
+ 支持手机APP控制割草机移动
+ 支持手机APP设定刀片的高低位置以及刀片转速
+ 支持弓字自动模式


## 快速上手

### 编译与烧录
+ clone demo 代码

+ 执行Project.uvprojx文件,用keil打开

+ 点击软件中的编译，并完成下载


### 文件介绍
```
├── User	
|    ├── main.c                   //main文件
|    ├── gd32f4xx_it.c              //中断服务函数实现文件
|    ├── gd32f4xx_it.h           
|    ├── movement.c            //运动控制逻辑文件
|    ├── movement.h            
|    ├── servo_motor.c            //电机、舵机、电调驱动接口文件
|    ├── servo_motor.h 
|    ├── QMC5883L.c            //地磁传感器驱动接口文件
|    ├── QMC5883L.h
|    ├── map.c            //GPS相关应用接口文件
|    ├── map.h
|    ├── systick.c            //systick 配置文件
|    ├── systick.h
|    ├── uart.c            //串口相关文件
|    └── uart.h
|
├── mcu_sdk				//tuya mcu sdk相关文件
|
├── FreeRTOS	
|    .
|    .
|    .

```

<br>

### Demo入口

入口文件：main.c

重要函数：main()

### DP点相关

+ 上报dp点处理: mcu_dp_value_update()

| 函数名 | unsigned char mcu_dp_value_update(unsigned char dpid,unsigned long value) |
| ------ | ------------------------------------------------------------ |
| dpid   | DP的ID号                                                     |
| value  | DP数据                                                       |
| Return | SUCCESS: 成功  ERROR: 失败                                   |

### I/O 列表

|right front motor|left front motor|right rear motor|left rear motor|servo_1|servo_2|blade motor|wifi module|GPS module|
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
|`pwm` PA15|`pwm` PB9|`pwm` PB10|`pwm` PB11|`pwm` PC6|`pwm` PC7|`pwm` PC8|`MCU_TX0` PA9|`MCU_TX1` PD5|
|`+` PD0|`+` PD1|`+` PD3|`+` PD8||||`MCU_RX0` PA10|`MCU_RX1` PD6|
|`-`PD7|`-` PD2|`-` PD4|`-` PD9||||||
<br>



## 相关文档

涂鸦Demo中心：https://developer.tuya.com/demo


<br>


## 技术支持

您可以通过以下方法获得涂鸦的支持:

- 开发者中心：https://developer.tuya.com
- 帮助中心: https://support.tuya.com/help
- 技术支持工单中心: [https://service.console.tuya.com](https://service.console.tuya.com/) 


<br>


