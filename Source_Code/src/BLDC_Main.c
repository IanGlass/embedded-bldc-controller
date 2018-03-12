/**************************************************************************//**
 * @file
 * @brief BLDC Motor Driver Main
 * @author Ian Glass
 * @version 1.00.0
 ******************************************************************************
 *******************************************************************************
 *
 * This file is licensed under the Silicon Labs Software License Agreement. See 
 * "http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt"  
 * for details. Before using this software for any purpose, you must agree to the 
 * terms of that agreement.
 *
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_timer.h"
#include "em_int.h"
#include "em_adc.h"

//Declare global variables
int PWMTime;
uint32_t Speed = 0;
int Timer = 0;

//Systick ISR
void SysTick_Handler(void) {
	Timer++;
}

//PWM output ISR
void TIMER0_IRQHandler(void) {
	//Clear overflow interrupt flag
	TIMER_IntClear(TIMER0, TIMER_IF_OF);
	//Drive range is 1-2ms so first 1ms is mandatory. Full wave is 20ms
	if (PWMTime < (Offset + Speed)) {
		GPIO_PinOutSet(PWM_PORT, PWM_PIN);
	}
	else {
		GPIO_PinOutClear(PWM_PORT, PWM_PIN);
	}
	PWMTime++;
	if (PWMTime > 2000) {
		PWMTime = 0;
	}
}

void Delay(int milliseconds) {
	//Reset global time increment
	Timer = 0;
	//Hard lock wait
	while (Timer < milliseconds);
}

//Function for starting Hyperion motor. Start with 2ms high PWM on 20ms wave period for 2 secs
void InitialiseHyperion(void) {
	int StartupCounter = 0;
	//Drive BLDC at full speed
	while (StartupCounter < StartupStop) {
		//If PWM pin low
		if (GPIO_PinInGet(PWM_PORT, PWM_PIN)) {
			GPIO_PinOutClear(PWM_PORT, PWM_PIN);
			//Drive low for 18 ms
			Delay(18);
		}
		//Drive PWM pin high
		else {
			GPIO_PinOutSet(PWM_PORT, PWM_PIN);
			//Drive high for 2 ms
			Delay(2);
		}
		StartupCounter++;
	}
	//Enable TIMER0 Interrupt vector in NVIC, begins normal PWM operation
	NVIC_EnableIRQ(TIMER0_IRQn);
}

//ADC setup for PD7, the analogue input controls the BLDC drive speed
void InitADC(void) {
	//Enable clock for ADC0 peripheral
	CMU_ClockEnable(cmuClock_ADC0, true);
	//Initialiser structure for ADC0 peripheral using channel 7
	ADC_InitSingle_TypeDef ADCInit0 =
	{
			.prsSel = adcPRSSELCh0,
			.acqTime = adcAcqTime1,
			.reference = _ADC_SINGLECTRL_REF_VDD,
			.resolution = adcRes12Bit,
			.input = adcSingleInpCh7,
			.diff = false,
			.prsEnable = false,
			.leftAdjust = false,
			.rep = false,
	};
	//Enable ADC module
	ADC_InitSingle(ADC0, &ADCInit0);
}

//Initialise PD6 GPIO and Timer for use as PWM driver
void InitPWM(void) {
	//Enables GPIO peripheral clocking
	CMU_ClockEnable(cmuClock_GPIO, true);
	//Set PWM pin to output with low state
	GPIO_PinModeSet(PWM_PORT, PWM_PIN, gpioModePushPull, 0);
	//Enable clock for TIMER0 peripheral
	CMU_ClockEnable(cmuClock_TIMER0, true);
	//Set TIMER0 overflow value, defines PWM output frequency
	TIMER_TopSet(TIMER0, CMU_ClockFreqGet(cmuClock_HFPER)/PWM_FREQ);
	//Construct TIMER0 initialiser structure
	TIMER_Init_TypeDef timerInit0 =
	{
			.enable = true,
			.debugRun = true,
			.prescale = timerPrescale16,
			.clkSel = timerClkSelHFPerClk,
			.fallAction = timerInputActionNone,
	  		.riseAction = timerInputActionNone,
	  		.mode = timerModeUp,
	  		.dmaClrAct = false,
	  		.quadModeX4 = false,
	  		.oneShot = false,
	  		.sync = false,
	};
	//Enable overflow interrupt flag
	TIMER_IntEnable(TIMER0, TIMER_IF_OF);
	//Configure timer, start normal operation
	TIMER_Init(TIMER0, &timerInit0);
}

uint32_t SampleADC(int wait) {
	uint32_t ADCVal;
	//Request ADC sample
	ADC_Start(ADC0, adcStartSingle);
	//Wait for conversion, needs to be replaced with wait function to check for ADC conversion complete
	Delay(wait);
	//Grab register value
	ADCVal = ADC_DataSingleGet(ADC0);
	//Scale input voltage as a percentage of 3.6V max expected at voltage divided input
	return ADCVal*100/4095;
}

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  /* Chip errata */
  CHIP_Init();
  //Setup Systick timer for 1mS interrupts
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE)/SYS_FREQ)) while (1);
  InitADC();
  InitPWM();
  //Run function to initialise Hyperion BLDC controller
  InitialiseHyperion();

  //Globally enable interrupts, begins normal operation
  INT_Enable();

  /* Background loop */
  while (1) {
	  //ADC sample, wait in milliseconds as argument
	  Speed = SampleADC(10);
  }
}
