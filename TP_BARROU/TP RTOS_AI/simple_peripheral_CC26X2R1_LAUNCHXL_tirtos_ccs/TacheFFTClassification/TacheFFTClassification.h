/*
 * TacheFFTClassification.h
 *
 *  Created on: 20 déc. 2023
 *      Author: TP_EO_6
 */

#ifndef TACHEFFTCLASSIFICATION_TACHEFFTCLASSIFICATION_H_
#define TACHEFFTCLASSIFICATION_TACHEFFTCLASSIFICATION_H_

void FFTClassificationTrigger(float serie[]);
void TacheFFTClassification_CreateTask(void);
static void TacheFFTClassification_taskFxn(UArg a0, UArg a1);



#endif /* TACHEFFTCLASSIFICATION_TACHEFFTCLASSIFICATION_H_ */
