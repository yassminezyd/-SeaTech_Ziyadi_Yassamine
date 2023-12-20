#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/BIOS.h>
/* Driver configuration */
#include "ti_drivers_config.h"
#include "TacheFFTClassification.h"
#define TacheFFTClassification_TASK_PRIORITY 2
#define TacheFFTClassification_TASK_STACK_SIZE 1024
Task_Struct TacheFFTClassification;
uint8_t TacheFFTClassificationStack[TacheFFTClassification_TASK_STACK_SIZE];
Semaphore_Struct semTacheFFTClassificationStruct;
Semaphore_Handle semTacheFFTClassificationHandle;

#define FFT_WINDOW_SIZE 256
float FFTDataX[FFT_WINDOW_SIZE];
float FFTDataY[FFT_WINDOW_SIZE];


void TacheFFTClassification_CreateTask(void)
{
    Semaphore_Params semParams;
    Task_Params taskParams;
// Configure task
    Task_Params_init(&taskParams);
    taskParams.stack = TacheFFTClassificationStack;
    taskParams.stackSize = TacheFFTClassification_TASK_STACK_SIZE;
    taskParams.priority = TacheFFTClassification_TASK_PRIORITY;
    Task_construct(&TacheFFTClassification, TacheFFTClassification_taskFxn,
                   &taskParams, NULL);
    /* Construct a Semaphore object
     to be used as a resource lock, initial count 0 */
    Semaphore_Params_init(&semParams);
    Semaphore_construct(&semTacheFFTClassificationStruct, 0, &semParams);
    /* Obtain instance handle */
    semTacheFFTClassificationHandle = Semaphore_handle(
            &semTacheFFTClassificationStruct);
}
static void TacheFFTClassification_taskFxn(UArg a0, UArg a1)
{
//Initialisations
//Boucle infinie
    for (;;)
    {
        Semaphore_pend(semTacheFFTClassificationHandle, BIOS_WAIT_FOREVER);
    }
}

void FFTClassificationTrigger(float FFTX[], float FFTY[])
{
//Copie locale des buffer de data pour la FFT
    for (int i = 0; i < FFT_WINDOW_SIZE; i++)
    {
        FFTDataX[i] = FFTX[i];
        FFTDataY[i] = FFTY[i];
    }
//Lancement de la �tche FFT Classification
    Semaphore_post(semTacheFFTClassificationHandle);
}




