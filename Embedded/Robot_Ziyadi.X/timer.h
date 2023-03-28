/* 
 * File:   timer.h
 * Author: GEII Robot
 *
 * Created on 7 décembre 2022, 13:49
 */

#ifndef TIMER_H
#define TIMER_H

extern unsigned long timestamp;


void InitTimer23(void);
void InitTimer1(void);
void InitTimer4(void);
void SetFreqTimer1(float freq);
void SetFreqTimer4(float freq);

#endif 

