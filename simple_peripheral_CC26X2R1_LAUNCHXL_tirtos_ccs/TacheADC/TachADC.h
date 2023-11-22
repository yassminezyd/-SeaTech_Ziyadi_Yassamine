/*
 * TachLDC.h
 *
 *  Created on: 22 nov. 2023
 *      Author: GEII Robot
 */

#ifndef TACHEADC_TACHADC_H_
#define TACHEADC_TACHADC_H_


extern void TacheADC_init(void);
void myClockSwiFxn(uintptr_t arg0);
void Sampling(uint_least8_t Board_ADC_Number);
void TacheADC_CreateTask(void);


#endif /* TACHEADC_TACHADC_H_ */
