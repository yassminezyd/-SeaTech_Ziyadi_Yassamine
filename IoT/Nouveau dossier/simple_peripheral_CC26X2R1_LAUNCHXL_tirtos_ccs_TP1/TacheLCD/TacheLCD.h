/*
 * TacheLCD.h
 *
 *  Created on: 22 nov. 2023
 *      Author: TABLE 6
 */

#ifndef TACHELCD_TACHELCD_H_
#define TACHELCD_TACHELCD_H_

extern void TacheLCD_init(void);
void LCD_Init(void);
void floatToString(char* ax, float AX);
void floatToString1d(char* ax, float AX);
void intToString(char* ax, float AX);
void TacheLCD_CreateTask(void);
void afficherDonnees(float Voltaccx, float Voltaccy, float Voltaccz, float Joystick_V, float Joystick_H);




#endif /* TACHELCD_TACHELCD_H_ */
