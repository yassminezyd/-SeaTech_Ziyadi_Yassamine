/* 
 * File:   ADC.h
 * Author: Table2
 *
 * Created on 14 décembre 2022, 15:32
 */

#ifndef ADC_H
#define	ADC_H



void InitADC1(void);
void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void);
void ADC1StartConversionSequence();
unsigned int * ADCGetResult(void);
unsigned char ADCIsConversionFinished(void);
void ADCClearConversionFinishedFlag(void);





#endif	/* ADC_H */

