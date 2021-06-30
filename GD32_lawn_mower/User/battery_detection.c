#include "battery_detection.h"

void adc_config(void)
{
	  gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_1);
	
	    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV4);
		adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
	
	  /* ADC channel length config */
    adc_channel_length_config(ADC0,ADC_REGULAR_CHANNEL,1);
	  adc_regular_channel_config(ADC0,0,ADC_CHANNEL_1,ADC_SAMPLETIME_15);
    adc_interrupt_enable(ADC0, ADC_INT_EOC);
    nvic_irq_enable(ADC_IRQn, 2U, 0U);
    
	  adc_resolution_config(ADC0,ADC_RESOLUTION_12B);
    
    /* enable ADC interface */
    adc_enable(ADC0);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
}
