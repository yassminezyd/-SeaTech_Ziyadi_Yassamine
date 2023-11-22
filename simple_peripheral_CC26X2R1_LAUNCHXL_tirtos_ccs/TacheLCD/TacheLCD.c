#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/BIOS.h>
#include <ti_drivers_config.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/ADC.h>


#include "TacheLCD/LCD_LAUNCHPAD/OLED_Display.h"
#include <TacheLCD/SPI_UTLN/SPI_UTLN.h>
#include <TacheLCD/LCD_LAUNCHPAD/LCD_LAUNCHPAD.h>
#include <TacheLCD/TacheLCD.h>


#define TACHEADC_TASK_PRIORITY 1
#define TACHEADC_TASK_STACK_SIZE 1024
Task_Struct TacheLCD;
uint8_t TacheLCDStack[TACHEADC_TASK_STACK_SIZE];
Semaphore_Struct semTacheLCDStruct;
Semaphore_Handle semTacheLCDHandle;

float Ax, Ay, Az;

void afficherDonnees(float vccx, float vccy, float vccz)
{
    Ax = (9.81/0.66)*(vccx-1.65);
    Ay = (9.81/0.66)*(vccy-1.65);
    Az = (9.81/0.66)*(vccz-1.65);
    Semaphore_post(semTacheLCDHandle);
}

void LCD_Init(void)
{
//Initialize the LCD controller
    Initialize_LCD();
//Fill display with a given RGB value
    Fill_LCD(0xFF, 0x00, 0x00); //RGB
    Task_sleep(500000 / Clock_tickPeriod); // Delay 100ms
    char DataLCD[] = "UTLN";
    OLEDText22(40, 17, DataLCD, SIZE_TWO, 0x00, 0xFF, 0xFF);
    Task_sleep(1000000 / Clock_tickPeriod); // Delay 1s
}
void TacheLCD_init(void)
{
    SPI_UTLN_Init();
    LCD_Init();
}
void floatToString1d(char *ax, float AX)
{
    char convert[6];
//char* ax = " ";
    if (AX < 0)
    {
        char moins[6] = "-";
        strcat(ax, moins);
        AX = AX * -1.0f;
    }
    char point[6] = ".";
    long Axlong = (long) AX;
    long Axdeclong = (long) ((AX - (float) Axlong) * 10);
    ltoa(Axlong, convert, 10);
    strcat(ax, convert);
    strcat(ax, point);
    ltoa(Axdeclong, convert, 10);

    strcat(ax, convert);
}

extern void TacheLCD_init(void);

static void TacheLCD_taskFxn(UArg a0, UArg a1)
{
    TacheLCD_init();

    char DataLCD[] = "AX:";
    char DataLCD1[] = "AY:";
    char DataLCD2[] = "AZ:";

    //Fill display with given RGB value
    Fill_LCD(0XFF, 0X00, 0X00);
    OLEDText22(8, 8, DataLCD, SIZE_TWO, 0XFF, 0XFF, 0X00);
    OLEDText22(8, 33, DataLCD1, SIZE_TWO, 0XFF,0XFF, 0X00);
    OLEDText22(8, 58, DataLCD2, SIZE_TWO, 0XFF,0XFF, 0X00);


    for (;;)
    {
        Semaphore_pend(semTacheLCDHandle, BIOS_WAIT_FOREVER);

        char axDataLCD[10]="";
        char ayDataLCD[10]="";
        char azDataLCD[10]="";

        //Afficher les données recues de la tache ADC

        floatToString1d(axDataLCD, Ax);
        floatToString1d(ayDataLCD, Ay);
        floatToString1d(azDataLCD, Az);

        OLEDText22(40, 8, axDataLCD, SIZE_TWO, 0XFF, 0XFF, 0X00);
        OLEDText22(40, 33, ayDataLCD, SIZE_TWO, 0XFF, 0XFF, 0X00);
        OLEDText22(40, 58, azDataLCD, SIZE_TWO, 0XFF, 0XFF, 0X00);



    }

}


void TacheLCD_CreateTask(void)
{
    Semaphore_Params semParams;
    Task_Params taskParams;
    /* Configuration de la tache*/
    Task_Params_init(&taskParams);
    taskParams.stack = TacheLCDStack;
    taskParams.stackSize = TACHEADC_TASK_STACK_SIZE;
    taskParams.priority = TACHEADC_TASK_PRIORITY;
    /* Creation de la tache*/
    Task_construct(&TacheLCD, TacheLCD_taskFxn, &taskParams, NULL);
    /* Construire un objet semaphore
     pour etre utilise comme outil de
     verrouillage, comptage initial 0 */
    Semaphore_Params_init(&semParams);
    Semaphore_construct(&semTacheLCDStruct, 0, &semParams);
    /* Obtenir la gestion de l'instance */
    semTacheLCDHandle =Semaphore_handle(&semTacheLCDStruct);
}

//void intToString(char *ax, float AX)
//{
//    char convert[6];
////char* ax = " ";
//    if (AX < 0)
//    {
//        char moins[6] = "-";
//        strcat(ax, moins);
//        AX = AX * -1.0f;
//    }
//    long Axlong = (long) AX;
//    ltoa(Axlong, convert, 10);
//    strcat(ax, convert);
//}
