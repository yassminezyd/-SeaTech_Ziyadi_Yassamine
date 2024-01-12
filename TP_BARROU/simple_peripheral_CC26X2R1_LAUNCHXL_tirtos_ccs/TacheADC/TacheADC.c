/*
 * TacheADC.c
 *
 *  Created on: 22 nov. 2023
 *      Author: TP_EO_6
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
#include <ti_drivers_config.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/ADC.h>


#define TACHEADC_TASK_PRIORITY 1
#define TACHEADC_TASK_STACK_SIZE 1024
#define ADC_SAMPLE_COUNT (10)

Task_Struct TacheADC;

uint8_t TacheADCStack[TACHEADC_TASK_STACK_SIZE];

Semaphore_Struct semTacheADCStruct;
Semaphore_Handle semTacheADCHandle;
uint16_t adcValue0;
uint32_t adcValue0MicroVolt;
uint16_t adcValue1[ADC_SAMPLE_COUNT];
uint32_t adcValue1MicroVolt[ADC_SAMPLE_COUNT];
static Clock_Struct myClock;
extern void TacheADC_init(void);



float Sampling(uint_least8_t Board_ADC_Number);
void myClockSwiFxn(uintptr_t arg0)
	{
		Semaphore_post(semTacheADCHandle);
	}
uint16_t i;
ADC_Handle adc;
ADC_Params params;
int_fast16_t res;
void Turn_on_LEDS(void)
{
	GPIO_write(LED_1, 1);
	//GPIO_write(LED_2, 1);
}
void Turn_off_LEDS(void)
{
	GPIO_write(LED_1, 0);
	//GPIO_write(LED_2, 0);
}
extern void TacheADC_init(void){
	GPIO_init();
	ADC_init();
	ADC_Params_init(&params);
	Clock_Params clockParams;
	Clock_Params_init(&clockParams);
	clockParams.period = 100 * (1000/Clock_tickPeriod),Clock_construct(&myClock, myClockSwiFxn,0,&clockParams);
	Clock_start(Clock_handle(&myClock));//Timer start

}
static void TacheADC_taskFxn(UArg a0, UArg a1)
{
	TacheADC_init();
         for(;;)
         {
//        	 Turn_on_LEDS();
//		     Task_sleep(100 * (1000 / Clock_tickPeriod));
//		     Turn_off_LEDS();
//        	 Task_sleep(100 * (1000 / Clock_tickPeriod));
        	 //Le semaphore est poste par le timer myClock

        	 Semaphore_pend(semTacheADCHandle, BIOS_WAIT_FOREVER);
        	 float Accx = Sampling(CONFIG_ADC_0);
        	 float Accy = Sampling(CONFIG_ADC_1);
        	 float Accz = Sampling(CONFIG_ADC_2);
        	 afficherDonnees(Accx,  Accy,  Accz);
         }
}

float Sampling(uint_least8_t Board_ADC_Number){
	adc = ADC_open(Board_ADC_Number, &params);
	if (adc == NULL){
	while (1);
	}
	for (i = 0; i < ADC_SAMPLE_COUNT; i++){
		res = ADC_convert(adc, &adcValue1[i]);
			if (res == ADC_STATUS_SUCCESS){
			return adcValue1MicroVolt[i] =ADC_convertRawToMicroVolts(adc,adcValue1[i]);
	}
	}
	ADC_close(adc);
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
		Task_construct(&TacheADC, TacheADC_taskFxn,
		&taskParams, NULL);
		/* Construire un objet semaphore pour etre utilise comme outil de verrouillage, comptage initial 0 */
		Semaphore_Params_init(&semParams);
		Semaphore_construct(&semTacheADCStruct,
		0, &semParams);
		/* Obtenir la gestion de l'instance */
		semTacheADCHandle =
		Semaphore_handle(&semTacheADCStruct);

}








