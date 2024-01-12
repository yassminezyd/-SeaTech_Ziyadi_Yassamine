/*
 * TacheADC.h
 *
 *  Created on: 22 nov. 2023
 *      Author: TP_EO_6
 */

#ifndef TACHEADC_TACHEADC_H_
#define TACHEADC_TACHEADC_H_

#define TACHEADC_TASK_PRIORITY 1
#define TACHEADC_TASK_STACK_SIZE 1024
void afficherDonnees(float accx, float accy, float accz);
void TacheADC_CreateTask(void);
static void TacheADC_taskFxn(UArg a0, UArg a1);
float Sampling(uint_least8_t Board_ADC_Number);
void Turn_on_LEDS(void);


#endif /* TACHEADC_TACHEADC_H_ */
