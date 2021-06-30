#ifndef __QMC5883L_H_
#define __QMC5883L_H_

#include "gd32f4xx.h"

#define QMC5883L_ADDRESS  		0x1A

#define QMC5883L_DATA_READ_X_LSB	0x00
#define QMC5883L_DATA_READ_X_MSB	0x01
#define QMC5883L_DATA_READ_Y_LSB	0x02
#define QMC5883L_DATA_READ_Y_MSB	0x03
#define QMC5883L_DATA_READ_Z_LSB	0x04
#define QMC5883L_DATA_READ_Z_MSB	0x05
#define QMC5883L_STATUS		        0x06 // DOR | OVL | DRDY
#define QMC5883L_TEMP_READ_LSB		0x07
#define QMC5883L_TEMP_READ_MSB		0x08 
#define QMC5883L_CONFIG_1					0x09 // OSR | RNG | ODR | MODE
#define QMC5883L_CONFIG_2					0x0A // SOFT_RST | ROL_PNT | INT_ENB
#define QMC5883L_CONFIG_3					0x0B // SET/RESET Period FBR [7:0]
#define QMC5883L_ID								0x0D

#ifndef M_PI 
#define M_PI 3.14159265358979323846264338327950288f 
#endif 

#define QMC5883L_SCALE_FACTOR 		0.732421875f
#define QMC5883L_CONVERT_GAUSS_2G 	12000.0f
#define QMC5883L_CONVERT_GAUSS_8G 	3000.0f
#define QMC5883L_CONVERT_MICROTESLA 	100
#define QMC5883L_DECLINATION_ANGLE	93.67/1000  // radian, Tekirdag/Turkey


typedef struct 
{
	int16_t x;
	int16_t y;
	int16_t z;
}i16_xyz;

typedef struct 
{
	float x;
	float y;
	float z;
}float_xyz;

typedef enum STATUS_VARIABLES
{
	NORMAL,
	NO_NEW_DATA,
	NEW_DATA_IS_READY,
	DATA_OVERFLOW,
	DATA_SKIPPED_FOR_READING
}_qmc5883l_status;

typedef enum MODE_VARIABLES
{
	MODE_CONTROL_STANDBY=0x00,
	MODE_CONTROL_CONTINUOUS=0x01
}_qmc5883l_MODE;

typedef enum ODR_VARIABLES
{
	OUTPUT_DATA_RATE_10HZ=0x00,
	OUTPUT_DATA_RATE_50HZ=0x04,
	OUTPUT_DATA_RATE_100HZ=0x08,
	OUTPUT_DATA_RATE_200HZ=0x0C
}_qmc5883l_ODR;

typedef enum RNG_VARIABLES
{
	FULL_SCALE_2G=0x00,
	FULL_SCALE_8G=0x10
}_qmc5883l_RNG;


typedef enum OSR_VARIABLES
{
	OVER_SAMPLE_RATIO_512=0x00,
	OVER_SAMPLE_RATIO_256=0x40,
	OVER_SAMPLE_RATIO_128=0x80,
	OVER_SAMPLE_RATIO_64=0xC0
}_qmc5883l_OSR;


typedef enum INTTERRUPT_VARIABLES
{
	INTERRUPT_DISABLE,
	INTERRUPT_ENABLE
}_qmc5883l_INT;

void QMC5883L_GetAngle(float_xyz* data);
void QMC_Init(void);



#endif
