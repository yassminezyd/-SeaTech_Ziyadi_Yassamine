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
#include <TacheADC/TachADC.h>
#include "ti_drivers_config.h"
#include <ti/drivers/GPIO.h>
#include <ti/drivers/ADC.h>
#include <TacheLCD/TacheLCD.h>
#include "Profiles/Accelerometre.h"
#include "Application/simple_peripheral.h"


#define TACHEADC_TASK_PRIORITY 1
#define TACHEADC_TASK_STACK_SIZE 1024
#define ADC_SAMPLE_COUNT (10)

uint16_t adcValue0;
uint32_t adcValue0MicroVolt;
uint16_t adcValue1[ADC_SAMPLE_COUNT];
uint32_t adcValue1MicroVolt[ADC_SAMPLE_COUNT];

Task_Struct TacheADC;
uint8_t TacheADCStack[TACHEADC_TASK_STACK_SIZE];
Semaphore_Struct semTacheADCStruct;
Semaphore_Handle semTacheADCHandle;

uint16_t i;
ADC_Handle adc;
ADC_Params params;
int_fast16_t res;

static Clock_Struct myClock;

float vccx, vccy, vccz, joy_ver, joy_hor;
int counter;

void myClockSwiFxn(uintptr_t arg0)
{
    Semaphore_post(semTacheADCHandle);
}




#if 1
//void Turn_on_LEDS(void){
//
//    GPIO_write(LED_Red, 1);
//    GPIO_write(LED_Green, 1);
//
//}
//
//void Turn_off_LED(void){
//
//    GPIO_write(LED_Red, 0);
//    GPIO_write(LED_Green, 0);
//}

#endif

static void TacheADC_taskFxn(UArg a0, UArg a1)
{
    TacheADC_init();

    for (;;)
    {
        //Turn_on_LEDS();
//      Task_sleep(500*(1000/Clock_tickPeriod));//on attends 5 ms
//      Turn_off_LED();
//      Task_sleep(500*(1000/Clock_tickPeriod));

//Le semaphore est poste par le timer myClock
        Semaphore_pend(semTacheADCHandle, BIOS_WAIT_FOREVER);

        if(counter>= 5)
        {
            Sampling(CONFIG_ADC_0);
            Sampling(CONFIG_ADC_1);
            Sampling(CONFIG_ADC_2);
            counter=0;
        }
        counter++;

        Sampling(CONFIG_JOY_vert);
        Sampling(CONFIG_JOY_horiz);

        SaveDataToSend(vccx, vccy, vccz, joy_ver, joy_hor);
        Carte_enqueueMsg(PZ_MSG_ACCELEROMETRE);
        afficherDonnees(vccx, vccy, vccz,joy_ver, joy_hor);
    }

}

//Chaque fois que le timer arrive à 100 millisecondes, il va poster le semaphore et
//on fera l’échantillonnage des 3 axes de l’accéléromètre.
extern void TacheADC_init(void)
{
    GPIO_init();
    ADC_init();
    ADC_Params_init(&params);
    Clock_Params clockParams;
    Clock_Params_init(&clockParams);
    clockParams.period = 20 * (1000 / Clock_tickPeriod), //20m car freq = 50HZ
    Clock_construct(&myClock, myClockSwiFxn, 0, // Initial delay before first timeout
                    &clockParams);
    Clock_start(Clock_handle(&myClock)); //Timer start
}

//l’échantillonnage de chaque axe de l’accéléromètre.
//à toute conversion il faudra ouvrir et fermer le canal avec ADC_open et ADC_close

void Sampling(uint_least8_t Board_ADC_Number)
{
    adc = ADC_open(Board_ADC_Number, &params);
    if (adc == NULL)
    {
        while (1)
            ;
    }
    for (i = 0; i < ADC_SAMPLE_COUNT; i++)
    {
        res = ADC_convert(adc, &adcValue1[i]);
        if (res == ADC_STATUS_SUCCESS)
        {
            if (Board_ADC_Number == CONFIG_ADC_0)
            {
                adcValue1MicroVolt[i] =
                ADC_convertRawToMicroVolts(adc, adcValue1[i]);
                vccx = adcValue1MicroVolt[i]/1000000.0;
            }
            if (Board_ADC_Number == CONFIG_ADC_1)
            {
                adcValue1MicroVolt[i] =
                ADC_convertRawToMicroVolts(adc, adcValue1[i]);
                vccy = adcValue1MicroVolt[i]/1000000.0;
            }
            if (Board_ADC_Number == CONFIG_ADC_2)
            {
                adcValue1MicroVolt[i] =
                ADC_convertRawToMicroVolts(adc, adcValue1[i]);
                vccz = adcValue1MicroVolt[i]/1000000.0;
            }
            if (Board_ADC_Number == CONFIG_JOY_vert)
            {
                adcValue1MicroVolt[i] =
                ADC_convertRawToMicroVolts(adc, adcValue1[i]);
                joy_ver = ((adcValue1MicroVolt[i]/1000000.0)/5)*100;
            }
            if (Board_ADC_Number == CONFIG_JOY_horiz)
            {
                adcValue1MicroVolt[i] =
                ADC_convertRawToMicroVolts(adc, adcValue1[i]);
                joy_hor = ((adcValue1MicroVolt[i]/1000000.0)/5)*100;
            }


        }
    }
    ADC_close(adc);



}

void TacheADC_CreateTask(void)
{
    Semaphore_Params semParams;
    Task_Params taskParams;
    /* Configuration de la tache*/
    Task_Params_init(&taskParams);
    taskParams.stack = TacheADCStack;
    taskParams.stackSize = TACHEADC_TASK_STACK_SIZE;
    taskParams.priority = TACHEADC_TASK_PRIORITY;
    /* Creation de la tache*/
    Task_construct(&TacheADC, TacheADC_taskFxn, &taskParams, NULL);
    /* Construire un objet semaphore
     pour etre utilise comme outil de
     verrouillage, comptage initial 0 */
    Semaphore_Params_init(&semParams);
    Semaphore_construct(&semTacheADCStruct, 0, &semParams);
    /* Obtenir la gestion de l'instance */
    semTacheADCHandle =
    Semaphore_handle(&semTacheADCStruct);
}

