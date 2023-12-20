/*
 * TacheADC.h
 *
 *  Created on: 30 nov. 2023
 *      Author: GEII Robot
 */

#ifndef TACHEADC_TACHEADC_H_
#define TACHEADC_TACHEADC_H_

void TacheADC_taskFxn(UArg a0, UArg a1);
void TacheADC_CreateTask(void);
void myClockSwiFxn(uintptr_t arg0);
uint32_t Sampling(uint_least8_t Board_ADC_Number);


#define FFT_WINDOW_SIZE 256



#endif /* TACHEADC_TACHEADC_H_ */
