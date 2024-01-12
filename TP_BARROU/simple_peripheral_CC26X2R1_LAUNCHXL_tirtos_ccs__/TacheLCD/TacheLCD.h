/*
 * TacheLCD.h
 *
 *  Created on: 22 nov. 2023
 *      Author: TP_EO_6
 */

#ifndef TACHELCD_TACHELCD_H_
#define TACHELCD_TACHELCD_H_


void afficherDonnees(float accx, float accy, float accz);
static void TacheLCD_taskFxn(UArg a0, UArg a1 );
void TacheLCD_CreateTask(void);
#endif /* TACHELCD_TACHELCD_H_ */

