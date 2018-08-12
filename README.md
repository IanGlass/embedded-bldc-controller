# Embedded-BLDC-Controller

This project contains a embedded microcontroller project to convert an input analogue voltage into an out PWM to drive a Hyperion BLDC controller.

## Circuit Schematic
The schematic contains three main modules:
* A TPS78001 voltage regulator to supply, configured with a 1M and 536 k resistor combination to provide a steady 3.5 V power source.
* A program header to interface to a J-Link LITE CortexM via SWD for flashing and debugging using Simplicity Studio.
* An EFM32TG microcontroller with an external oscillator.

The *Drive* pin is the analgoue input for the device, where the resistor (*R3* & *R4*) pair is currently configured to scale a 12 V input range to a 3.5 V input range. These can be altered to accommodate different inputs.

<p align="center">
<img src="https://github.com/IanGlass/Embedded-BLDC-Controller/blob/master/Circuit-Schematics/BLDC-Controller-Schematic.jpg" width="900">
</p>

## Code

Before normal operation commences, the Hyperion controller requires a 10% PWM with a 20 ms period to be driven for 2 seconds. This is a built in safety feature with Hyperion. After successful initialisation, the Hyperion controller will make a few short beeps to indicate success. Normal PWM operation is commenced by enabling the Timer0 IRQ via *NVIC_EnableIRQ(TIMER0_IRQn)*.
```c
/*-----------------------------------------------------------*/
/**
  * @brief Function used to initialise the Hyperion BLDC, which expects
  * a predefined sequence before being driveable. Start with 
  * 2ms high PWM on 20ms wave period for 2 secs
  * @param  None
  * @retval None
  */
void InitialiseHyperion(void) {
	int StartupCounter = 0;
	/* Drive BLDC at full speed */
	while (StartupCounter < StartupStop) {
		/* If PWM pin low */
		if (GPIO_PinInGet(PWM_PORT, PWM_PIN)) {
			GPIO_PinOutClear(PWM_PORT, PWM_PIN);
			/* Drive low for 18 ms */
			Delay(18);
		}
		/* Drive PWM pin high */
		else {
			GPIO_PinOutSet(PWM_PORT, PWM_PIN);
			/* Drive high for 2 ms */
			Delay(2);
		}
		StartupCounter++;
	}
	/* Enable TIMER0 Interrupt vector in NVIC, begins normal PWM operation */
	NVIC_EnableIRQ(TIMER0_IRQn);
}
```

The ADC peripheral is constantly polled in background to retrieve ADC values via the *Drive* input pin. The *SampleADC()* function takes a wait parameter which specifies the time to wait for a conversion to complete using a blocking wait function. The function then grabs the ADC value and scales it against 4,095 which is the maximum ADC register value (or 3.6 V). The function returns the scaled value to a global *Speed* value used for PWM.
```c
/*-----------------------------------------------------------*/
/**
  * @brief Performs a single acquisition of the ADC pin. This is
  * the input to specify the drive output.
  * @param  Length to wait for the ADC conversion (ms)
  * @retval The scaled PWM drive time
  */
uint32_t SampleADC(int wait) {
	uint32_t ADCVal;
	/* Request ADC sample */
	ADC_Start(ADC0, adcStartSingle);
	/* Wait for conversion, needs to be replaced with wait function to 
	* check for ADC conversion complete */
	Delay(wait);
	/* Grab register value */
	ADCVal = ADC_DataSingleGet(ADC0);
	/* Scale input voltage as a percentage of 3.6V max expected at voltage 
	* divided input */
	return ADCVal*100/4095;
}
```

The Timer0 IRQ performs PWM of the Hyperion controller using the previously calculated *Speed* variable to set the PWM duty cycle. PWM has a period of 20 ms, which is what the Hyperion controller expects.
```c
/*-----------------------------------------------------------*/
/**
  * @brief Timer0 ISR to drive PWM pin based on Speed set by
  * SampleADC() function.
  * @param  None
  * @retval None
  */
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
```

## Setup

The drive input and output pins are designated with "Drive" and "PWM" respectively. The input and output wires can be soldered to these through-hole pads. The setup with a hyperion controller is shown.

<p align="center">
<img src="https://github.com/IanGlass/Embedded-BLDC-Controller/blob/master/Circuit-Schematics/BLDC-Controller-PCB.JPG" width="420">
<img src="https://github.com/IanGlass/Embedded-BLDC-Controller/blob/master/Circuit-Schematics/BLDC-Image.jpg" width="420">
</p>