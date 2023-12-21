/*
 * TacheADC.c
 *
 *  Created on: 22 nov. 2023
 *      Author: TABLE 6
 */


/*
 * TacheADC.s
 *
 *  Created on: 22 nov. 2023
 *      Author: TABLE 6
 */
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
#include <TacheADC/TacheADC.h>

#include <ti/drivers/GPIO.h>
#include <ti_drivers_config.h>
#include <ti/drivers/ADC.h>
#include <TacheLCD/TacheLCD.h>

#include <Profiles/Accelerometre.h>
#include <Application/simple_peripheral.h>

#define TACHEADC_TASK_PRIORITY 1
#define TACHEADC_TASK_STACK_SIZE 1024
#define ADC_SAMPLE_COUNT (10)


Task_Struct TacheADC;
uint8_t TacheADCStack[TACHEADC_TASK_STACK_SIZE];
Semaphore_Struct semTacheADCStruct;
Semaphore_Handle semTacheADCHandle;

void Turn_on_LEDS(void)
{
    GPIO_write(LED_RED,1);
    //GPIO_write(LED_GREEN,1);
}

void Turn_off_LEDS(void)
{
    GPIO_write(LED_RED,0);
    //GPIO_write(LED_GREEN,0);
}

float Voltaccx;
float Voltaccy;
float Voltaccz;
float Joystick_V;
float Joystick_H;
int c = 0;
static void TacheADC_taskFxn(UArg a0, UArg a1)
{


    TacheADC_init();

    for(;;)
    {

    // Manipulation LEDS

    //Turn_on_LEDS();
    //Task_sleep(100*(1000/Clock_tickPeriod)); // On attend 500 ms
    //Turn_off_LEDS();
    //Task_sleep(100*(1000/Clock_tickPeriod)); // On attend 500 ms


    // Le semaphore est poste par le timer myClock
    Semaphore_pend(semTacheADCHandle, BIOS_WAIT_FOREVER);
/*
    uint32_t DatadampledX = Sampling(Board_ADC5);
    uint32_t DatadampledY = Sampling(Board_ADC3);
    uint32_t DatadampledZ = Sampling(Board_ADC4);

    float AccX = uVTToG_float(DatadampledX);
    float AccY = uVTToG_float(DatadampledY);
    float AccZ = uVTToG_float(DatadampledZ);

    */

    Joystick_V = Sampling(Vertical);
    Joystick_H = Sampling(Horizontal);


    if (c>=5){

        Voltaccx = Sampling(CONFIG_ADC_0_AX);
        Voltaccy = Sampling(CONFIG_ADC_1_AY);
        Voltaccz = Sampling(CONFIG_ADC_2_AZ);

     c =0;
    }
        c++;
    SaveDataToSend(Voltaccx,Voltaccy,Voltaccz,Joystick_V,Joystick_H);
    Carte_enqueueMsg(PZ_MSG_ACCELEROMETRE);

    afficherDonnees(Voltaccx, Voltaccy, Voltaccz,Joystick_V,Joystick_H);



    }

}

uint16_t adcValue0;
uint32_t adcValue0MicroVolt;
uint16_t adcValue1[ADC_SAMPLE_COUNT];
uint32_t adcValue1MicroVolt[ADC_SAMPLE_COUNT];

static Clock_Struct myClock;


void myClockSwiFxn(uintptr_t arg0)
{
Semaphore_post(semTacheADCHandle);
}

uint16_t i;
ADC_Handle adc;
ADC_Params params;
int_fast16_t res;

extern void TacheADC_init(void){
    GPIO_init();
    ADC_init();
    ADC_Params_init(&params);
    Clock_Params clockParams;
    Clock_Params_init(&clockParams);
    clockParams.period = 20 * (1000/Clock_tickPeriod),//100ms
    Clock_construct(&myClock, myClockSwiFxn,0 /*Initial delay before first timeout*/, &clockParams);
    Clock_start(Clock_handle(&myClock));//Timer start
}

float Voltacc;

float Sampling(uint_least8_t Board_ADC_Number){
    adc = ADC_open(Board_ADC_Number, &params);

    if (adc == NULL){
        while (1);
    }
    for (i = 0; i < ADC_SAMPLE_COUNT; i++){
        res = ADC_convert(adc, &adcValue1[i]);
        if (res == ADC_STATUS_SUCCESS){
            adcValue1MicroVolt[i] = ADC_convertRawToMicroVolts(adc,adcValue1[i]);
        }
    }
    ADC_close(adc);


    Voltacc = (float)(adcValue1MicroVolt[0]/1000000.0f);
   // Voltacc = ((float)(adcValue1MicroVolt[0]/1000000.0f) - 1.65.0f)/0.66.0f;

    return Voltacc;
}



void TacheADC_CreateTask(void){

    Semaphore_Params semParams;
    Task_Params taskParams;

    /* Configuration de la tache*/
    Task_Params_init(&taskParams);
    taskParams.stack = TacheADCStack;
    taskParams.stackSize = TACHEADC_TASK_STACK_SIZE;
    taskParams.priority = TACHEADC_TASK_PRIORITY;
    /* Creation de la tache*/
    Task_construct(&TacheADC,TacheADC_taskFxn,&taskParams, NULL);
    /* Construire un objet semaphore
    pour etre utilise comme outil de
    verrouillage, comptage initial 0 */
    Semaphore_Params_init(&semParams);
    Semaphore_construct(&semTacheADCStruct,0, &semParams);
    /* Obtenir la gestion de l'instance */
    semTacheADCHandle = Semaphore_handle(&semTacheADCStruct);

}






