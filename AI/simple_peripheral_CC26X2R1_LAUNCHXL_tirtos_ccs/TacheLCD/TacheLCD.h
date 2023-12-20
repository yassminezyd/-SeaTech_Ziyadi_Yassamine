/*
 * TacheLCD.h
 *
 *  Created on: 10 f�vr. 2020
 *      Author: sebas
 */

#ifndef TACHELCD_TACHELCD_H_
#define TACHELCD_TACHELCD_H_
//float xG, yG, zG;

void TacheLCD_CreateTask(void);
void TacheLCD_init(void);
static void TacheLCD_taskFxn(UArg a0, UArg a1);
void afficherDonn�es(float accx, float accy, float accz);

void LCD_PrintState(int State, float numClusters, int clusterDetected, float numAnomalies, float features[], int sizeFeatures);
float uVToG_float(uint32_t dataSampled);

#endif /* TACHELCD_TACHELCD_H_ */
