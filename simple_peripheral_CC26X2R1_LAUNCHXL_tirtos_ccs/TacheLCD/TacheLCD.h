/*
 * TachLDC.h
 *
 *  Created on: 22 nov. 2023
 *      Author: GEII Robot
 */

#ifndef TACHELCD_TACHLDC_H_
#define TACHELCD_TACHLDC_H_
void LCD_Init(void);
void floatToString(char *ax, float AX);
void floatToString1d(char *ax, float AX);
//void intToString(char *ax, float AX);

void afficherDonnees(float vccx, float vccy, float vccz, float joyver, float joyhor);
void TacheLCD_CreateTask(void);


#endif /* TACHELCD_TACHLDC_H_ */
