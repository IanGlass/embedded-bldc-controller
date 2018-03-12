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

#ifndef PWM_PORT
#define PWM_PORT	gpioPortD
#endif
#ifndef PWM_PIN
#define PWM_PIN		6
#endif

//defines number of startup pulses, drive for 2 secs
#define StartupStop	200
//Systick all frequency
#define SYS_FREQ 1000
//PWM ISR call frequency. Need 0.1MHz to achieve 1% resolution in PWM output for Hyperion BLDC
#define PWM_FREQ 1600000
//Time offset to start motor spinning, in 10us increments
#define Offset 100

