#include "QMC5883L.h"
#include "gd32f4xx_i2c.h"
#include "gd32f4xx_gpio.h"
#include "math.h"
#include "stdlib.h"

//uint8_t F_TASK_QMC5883L=0;
//float_xyz Mag_angle;
//int16_t Mag_temperature;

void IIC_init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(unsigned char txd);
uint8_t IIC_Read_Byte(unsigned char ack);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);

#define SDA_GPIO         GPIOC
#define SDA_PINx         GPIO_PIN_10

#define SCL_GPIO         GPIOC
#define SCL_PINx         GPIO_PIN_9

#define SET_SDA()         gpio_bit_set(SDA_GPIO,SDA_PINx)
#define CLR_SDA()         gpio_bit_reset(SDA_GPIO,SDA_PINx)

#define SET_SCL()         gpio_bit_set(SCL_GPIO,SCL_PINx)
#define CLR_SCL()         gpio_bit_reset(SCL_GPIO,SCL_PINx)


#define SDA_IN()  {GPIO_CTL(SDA_GPIO) &= (~(GPIO_CTL_CTL10));}   
#define SDA_OUT()  {SDA_IN();GPIO_CTL(SDA_GPIO) |= (BIT(20));} 


static void delay_us(unsigned long n)
{
    volatile unsigned long i;
    for(i = 0; i < n * 16; i++)
    {
        ;
    }
    return;
}

static void log_write_data(unsigned char *in, unsigned short len)
{
	
    while(len --) {
        usart1_sendbyte(*in);
        in ++;
    }
}

/***************************************************************************
  Function:        IIC_Write_Byte()
  Description:     写一个字节
  Input:           reg:寄存器地址
				           data:数据
  Output:
  Return:          0-成功 其他-失败
  revision history:
***************************************************************************/
uint8_t IIC_Write_Byte(uint8_t DrvAddr,uint16_t WriteAddr,uint8_t data)
{
	uint8_t ret=0;
  IIC_Start();

	IIC_Send_Byte(DrvAddr);
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr);
	IIC_Wait_Ack();		
	IIC_Send_Byte(data); 						   
	IIC_Wait_Ack(); 

	IIC_Stop();
	delay_us(30);
	return ret;
}

uint8_t IIC_WriteMulByte(uint8_t DrvAddr,uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	uint8_t ret=0;
	IIC_Start();

	IIC_Send_Byte(DrvAddr);	
	ret |= IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr);  
	ret |= IIC_Wait_Ack();		

	while(NumToWrite--)
	{ 										  		   
		IIC_Send_Byte(*pBuffer); 					   
		ret |= IIC_Wait_Ack(); 
		pBuffer++;
	}
	IIC_Stop();
	delay_us(10);
	return ret;
}

/***************************************************************************
  Function:        QMC_Read_Buf()
  Description:     读多个字节
  Input:           *magnx,*magny,*magnz XYZ三轴磁场
  Output:
  Return:          读取数据
  revision history:

 1T = 10000 G  1mT = 10G  1G = 100uT  地图磁场约 0.04mT = 40uT = 0.4G
***************************************************************************/
uint8_t IIC_ReadMulByte(uint8_t DrvAddr,uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)//???
{  	    																 
	uint8_t ret=0;
	
	IIC_Start();  
	IIC_Send_Byte(DrvAddr);	
	ret |= IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr);    
	ret |= IIC_Wait_Ack();	    
	
	IIC_Start();
	IIC_Send_Byte(DrvAddr+1);	   
	ret |= IIC_Wait_Ack();
	while(NumToRead)
	{
		if(NumToRead==1)
		{
			*pBuffer=IIC_Read_Byte(0);	
		}
		else
		{
			*pBuffer=IIC_Read_Byte(1);
		}
		pBuffer++;
		NumToRead--;
	}
	IIC_Stop();
	return ret;	
}

/***************************************************************************
  Function:        QMC_Init()
  Description:     初始化
  Input:           
  Output:
  Return:          
  revision history:
***************************************************************************/
void QMC_Init(void)
{
	uint8_t temp,Config=0;
	uint8_t data_buff[100] = {0};
	IIC_init();
	
	IIC_Write_Byte(QMC5883L_ADDRESS,0x0B,	0x01);
	IIC_Write_Byte(QMC5883L_ADDRESS,0x0B,	0x01);
	IIC_Write_Byte(QMC5883L_ADDRESS,0x20,	0x40);
	IIC_Write_Byte(QMC5883L_ADDRESS,0x21,	0x01);
	Config =	OVER_SAMPLE_RATIO_512\
						|FULL_SCALE_2G\
						|OUTPUT_DATA_RATE_100HZ\
						|MODE_CONTROL_CONTINUOUS;
	/****OSR=512,RNG=+/-2G,ODR=200Hz,MODE= continuous*******/
	IIC_Write_Byte(QMC5883L_ADDRESS,0x09,	Config);
	
	delay_us(2000);
	
	IIC_ReadMulByte(QMC5883L_ADDRESS,0x09,&temp,1);
        
	if(temp==Config)
	{
				sprintf((char*)data_buff,"QMC5883L_OK\r\n");
			  log_write_data(data_buff,sizeof(data_buff));
	}
	else
	{
			  sprintf((char*)data_buff,"QMC5883L_ERROR\r\n");
			  log_write_data(data_buff,sizeof(data_buff));
	}
}

void QMC5883L_GetAngle(float_xyz* data)
{
		uint8_t data_buff[100] = {0};
		uint8_t Mag[6],temp;
		float_t current_angle = 0; 
		i16_xyz Mag_data;
		IIC_ReadMulByte(QMC5883L_ADDRESS,0x06,&temp,1);


		IIC_ReadMulByte(QMC5883L_ADDRESS,0x00,Mag,6);


		Mag_data.x= (Mag[1] << 8 | Mag[0]);// First byte is LSB, second byte is MSB
		Mag_data.y= (Mag[3] << 8 | Mag[2]);
		Mag_data.z= (Mag[5] << 8 | Mag[4]);
  
		current_angle = atan2((double)Mag_data.y,(double)Mag_data.x)*573+1800;
    if(current_angle > 3500 || current_angle < 100) {
		    data->x = current_angle;
		}else {
				data->x = data->x*0.8+ 0.2*current_angle;
		}
}

void IIC_init(void)
{
    gpio_mode_set(SDA_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, SDA_PINx);
    gpio_output_options_set(SDA_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, SDA_PINx);
	
	  gpio_mode_set(SCL_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, SCL_PINx);
    gpio_output_options_set(SCL_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_200MHZ, SCL_PINx);
	
	  delay_us(2000);
	
}

//IIC start signal
void IIC_Start(void)
{
    SDA_OUT();
    SET_SDA();
    SET_SCL();
    delay_us(5);
    CLR_SDA();//START:when CLK is high,DATA change form high to low
    delay_us(5);
    CLR_SCL();
}

//IIC stop signal
void IIC_Stop(void)
{
    SDA_OUT();    
	  CLR_SCL();
		CLR_SDA();
//STOP:when CLK is high DATA change form low to high
    delay_us(5);
    SET_SCL();
    delay_us(5);
    SET_SDA();
    delay_us(5);
}

unsigned char IIC_Wait_Ack(void)
{
    unsigned char ucErrTime = 0;
    SET_SDA();
    SDA_IN();

    delay_us(1);
    SET_SCL();
    delay_us(5);
    while(gpio_input_bit_get(SDA_GPIO,SDA_PINx))
    {
        ucErrTime++;
        delay_us(2);
        if(ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    CLR_SCL();
    delay_us(2);
    return 0;
}

void IIC_Ack(void)
{
    CLR_SCL();
    SDA_OUT();
    CLR_SCL();
    CLR_SDA();
    delay_us(5);
    SET_SCL();
    delay_us(5);
    CLR_SCL();
}

void IIC_NAck(void)
{
    CLR_SCL();
    SDA_OUT();
    CLR_SCL();
    SET_SDA();
    delay_us(5);
    SET_SCL();
    delay_us(5);
    CLR_SCL();
}


void IIC_Send_Byte(unsigned char txd)
{
    unsigned char t;
    SDA_OUT();
    delay_us(2);
    CLR_SCL();
    for(t = 0; t < 8; t++)
    {
        if((txd & 0x80) >> 7 == 1)
            SET_SDA();
        else
            CLR_SDA();

        txd <<= 1;
        delay_us(5);  
        SET_SCL();
        delay_us(5);
        CLR_SCL();
        delay_us(5);
    }
}

unsigned char IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    
    SDA_IN();

    for(i = 0; i < 8; i++ )
    {
        CLR_SCL();
        delay_us(5);
        SET_SCL();
        receive <<= 1;
        if(gpio_input_bit_get(SDA_GPIO,SDA_PINx))
            receive++;
        delay_us(5);
    }
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack(); 
    return receive;
}
 