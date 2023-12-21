/*
 * TacheSPI.c
 *
 *  Created on: 14 nov. 2022
 *      Author: GEII Robot
 */

#include<stdbool.h>
#include<stdint.h>
#include<stddef.h>
#include<string.h>
#include<math.h>

#include<ti/sysbios/knl/Task.h>
#include<ti/sysbios/knl/Clock.h>
#include<ti/sysbios/knl/Event.h>
#include<ti/sysbios/knl/Queue.h>
#include<ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/BIOS.h>
#include<ti_drivers_config.h>
#include<ti/drivers/GPIO.h>
#include<ti/drivers/SPI.h>
#include <TacheLCD/LCD_LAUNCHPAD/OLED_Display.h>
#include <TacheLCD/SPI_UTLN/SPI_UTLN.h>
#include <TacheLCD/LCD_LAUNCHPAD/LCD_LAUNCHPAD.h>
#include <TacheLCD/TacheLCD.h>




#define TACHELCD_TASK_PRIORITY 1
#define TACHELCD_TASK_STACK_SIZE 4096

Task_Struct TacheLCD;
uint8_t TacheLCDStack[TACHELCD_TASK_STACK_SIZE];

Semaphore_Struct semTacheLCDStruct;
Semaphore_Handle semTacheLCDHandle;

/*#define SPI_SAMPLE_COUNT  (10)

uint16_t adcValue0 ;
uint32_t adcValue0MicroVolt ;
uint16_t adcValue1 [SPI_SAMPLE_COUNT] ;
uint32_t adcValue1MicroVolt [SPI_SAMPLE_COUNT] ;

static Clock_Struct myClock ;*/

extern void TacheLCD_init(void) ;
void LCD_Init ( void ) ;
void floatToString(char*ax, float AX);
void floatToString1d(char*ax, float AX);
void intToString(char*ax, float AX);
float Ax, Ay, Az, J1,J2 ;

void afficherDonnees(float accx, float accy, float accz, float joy1, float joy2){
    Ax = accx ;
    Ay = accy ;
    Az = accz ;
    J1= joy1;
    J2=joy2;

    Semaphore_post(semTacheLCDHandle);

}


void LCD_Init ( void ){
    // I n i t i a l i z e the LCD c o n t r o l l e r
    Initialize_LCD();
    // F i l l d i s p l a y with a gi v e n RGB val u e
    Fill_LCD(0xFF,0x00, 0x00); //RGB
    Task_sleep (500000/Clock_tickPeriod );// Delay 100ms
    char DataLCD[] = "UTLN";
    OLEDText22(40, 17 , DataLCD , SIZE_TWO, 0x00 , 0xFF , 0xFF ) ;
    Task_sleep (1000000/ Clock_tickPeriod ) ; // Delay 1 s
}

void TacheLCD_init(void ){
    SPI_UTLN_Init( ) ;
    LCD_Init( ) ;
}

void floatToString1d(char*ax, float AX){
    char convert[6] ;
    // char* ax = " " ;
    if(AX<0){
        char moins[6]= "-";
        strcat(ax, moins) ;
        AX = AX*-1.0f;
    }
    char point[6] = "." ;
    long Axlong = (long)AX;
    long Axdeclong =(long)((AX-(float)Axlong)*10 ) ;
    ltoa(Axlong ,convert,10) ;
    strcat(ax, convert);
    strcat(ax, point);
    ltoa(Axdeclong ,convert,10) ;
    strcat(ax, convert);

}

void intToString(char*ax, float AX){
    char convert[6] ;
    // char* ax = " " ;
    if(AX<0){
        char moins[6]= "-";
        strcat(ax, moins) ;
        AX = AX*-1.0f;
    }
    long Axlong=(long)AX;
    ltoa(Axlong ,convert,10) ;
    strcat(ax, convert);

}
extern void TacheLCD_init (void);

static void TacheLCD_taskFxn(UArg a0,UArg a1)
{
    TacheLCD_init();

    char DataLCD[]="AX:";
    char DataLCD2[]="AY:";
    char DataLCD3[]="AZ:";
    char DataLCD4[]="J1:";
    char DataLCD5[]="J2:";

    // Fill display with a given RGB Value
    Fill_LCD(0xFF,0x00, 0x00);
    OLEDText22(8,8,DataLCD,SIZE_TWO, 0xFF,0xFF, 0x00) ;
    OLEDText22(8,33,DataLCD2,SIZE_TWO, 0xFF,0xFF, 0x00) ;
    OLEDText22(8,58,DataLCD3,SIZE_TWO, 0xFF,0xFF, 0x00) ;
    OLEDText22(8,83,DataLCD4,SIZE_TWO, 0xFF,0xFF, 0x00) ;
    OLEDText22(8,108,DataLCD5,SIZE_TWO, 0xFF,0xFF, 0x00) ;

    for( ; ; )
    {
        Semaphore_pend(semTacheLCDHandle, BIOS_WAIT_FOREVER);
        char axDataLCD[10]="";
        char ayDataLCD[10]="";
        char azDataLCD[10]="";
        char joy1DataLCD[10]="";
        char joy2DataLCD[10]="";
        floatToString1d(axDataLCD, Ax);
        floatToString1d(ayDataLCD, Ay);
        floatToString1d(azDataLCD, Az);
        floatToString1d(joy1DataLCD, J1);
        floatToString1d(joy2DataLCD, J2);
        OLEDText22(40,8,axDataLCD,SIZE_TWO, 0xFF,0xFF, 0x00) ;
        OLEDText22(40,33,ayDataLCD,SIZE_TWO, 0xFF,0xFF, 0x00) ;
        OLEDText22(40,58,azDataLCD,SIZE_TWO, 0xFF,0xFF, 0x00) ;
        OLEDText22(40,83,joy1DataLCD,SIZE_TWO, 0xFF,0xFF, 0x00) ;
        OLEDText22(40,108,joy2DataLCD,SIZE_TWO, 0xFF,0xFF, 0x00) ;



    }

}

void TacheLCD_CreateTask ( void ){
    Semaphore_Params semParams;
    Task_Params taskParams ;

    /*Configuration de la tache*/
    Task_Params_init(&taskParams ) ;
    taskParams.stack = TacheLCDStack ;
    taskParams.stackSize = TACHELCD_TASK_STACK_SIZE;
    taskParams.priority = TACHELCD_TASK_PRIORITY;

    /*Creation de la tache*/

    Task_construct (&TacheLCD, TacheLCD_taskFxn, &taskParams, NULL ) ;

    /*Construire un obljectemaphore
    pour et r e u t i l i s e comme o u t i l de
    v e r r o u i l l a g e , comptage i n i t i a l 0 */
    Semaphore_Params_init(&semParams ) ;
    Semaphore_construct (&semTacheLCDStruct,0, &semParams ) ;

    /* Ob teni r l a g e s t i o n de l ’ i n s t a n c e */
    semTacheLCDHandle =Semaphore_handle(&semTacheLCDStruct);

}

