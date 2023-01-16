/* 
 * File:   PWM.h
 * Author: GEII Robot
 *
 * Created on 7 décembre 2022, 16:08
 */

#ifndef PWM_H
#define	PWM_H
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1

#ifdef	__cplusplus
extern "C" {
#endif

void InitPWM(void);
//void PWMSetSpeed(float vitesseEnPourcents, int m);
void PWMSetSpeedConsigne(float vitesseEnPourcents, char moteur) ;
void PWMUpdateSpeed();


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

