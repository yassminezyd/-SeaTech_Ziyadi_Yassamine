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
#include <ti/drivers/I2C.h>
#include <TacheLCD/TacheLCD.h>
#include <ti/display/Display.h>


#define TACHEI2C_TASK_PRIORITY 1
#define TACHEI2C_TASK_STACK_SIZE 640


Task_Struct TacheI2C;
uint8_t TacheI2CStack[TACHEI2C_TASK_STACK_SIZE];
Semaphore_Struct semTacheI2CStruct;
Semaphore_Handle semTacheI2CHandle;




uint8_t         txBuffer[10];
uint8_t         rxBuffer[10];
I2C_Handle      i2c;
I2C_Params      i2cParams;
I2C_Transaction i2cTransaction;

//static uint8_t slaveAddress;
//static Display_Handle display;

//static const struct {
//    uint8_t address;
//    uint8_t resultReg;
//    char *id;
//} sensors[TMP_COUNT] = {
//    { TMP11X_BASSENSORS_ADDR, TMP11X_RESULT_REG, "11X" },
//    { TMP116_LAUNCHPAD_ADDR,  TMP11X_RESULT_REG, "116" },
//    { TMP006_LAUNCHPAD_ADDR,  TMP006_RESULT_REG, "006" }
//};


void TacheI2C_init(void){

    GPIO_init();
    I2C_init();

    /* Create I2C for usage */
        I2C_Params_init(&i2cParams);
        i2cParams.bitRate = I2C_400kHz;
        i2c = I2C_open(CONFIG_I2C_LM, &i2cParams);
        if (i2c == NULL) {

            while (1);
        }
        else {

        }

//    Clock_Params clockParams;
//    Clock_Params_init(&clockParams);
//    clockParams.period = 100 * (1000/Clock_tickPeriod),//100ms
//    Clock_construct(&myClock, myClockSwiFxn,0 /*Initial delay before first timeout*/, &clockParams);
//    Clock_start(Clock_handle(&myClock));//Timer start
}


static void TacheI2C_taskFxn(UArg a0, UArg a1)
{


    TacheI2C_init();
    i2cParams.bitRate = I2C_400kHz;


    /* Common I2C transaction setup */
    i2cTransaction.writeBuf   = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf    = rxBuffer;
    i2cTransaction.readCount  = 2;
    i2cTransaction.slaveAddress = 0x44;
    txBuffer[0] = 0x7F;
    if (I2C_transfer(i2c, &i2cTransaction)) {
        int dataRx = rxBuffer[1]<<8 | rxBuffer[0];
    }
    else {
//        i2cErrorHandler(&i2cTransaction, display);
    }

    /* Common I2C transaction setup */
    i2cTransaction.writeBuf   = txBuffer;
    i2cTransaction.writeCount = 3;
    i2cTransaction.readBuf    = rxBuffer;
    i2cTransaction.readCount  = 0;
    i2cTransaction.slaveAddress = 0x44;
    txBuffer[0] = 0x01;
    txBuffer[1] = 0b11000110;
    txBuffer[2] = 0b00010000;
    if (I2C_transfer(i2c, &i2cTransaction)) {
        int dataRx = rxBuffer[1]<<8 | rxBuffer[0];
    }
    else {
//        i2cErrorHandler(&i2cTransaction, display);
    }


    while(1)
    {

//
//    // Le semaphore est poste par le timer myClock
//        Semaphore_pend(semTacheI2CHandle, BIOS_WAIT_FOREVER);
//
//        display = Display_open(Display_Type_UART, NULL);
//        if (display == NULL)
//        {
//            while (1)
//                ;
//        }
//
//        i2cParams.bitRate = I2C_400kHz;
//        i2c = I2C_open(CONFIG_I2C_LM, &i2cParams);
//
//
//        /* Common I2C transaction setup */
//        i2cTransaction.writeBuf = txBuffer;
//        i2cTransaction.writeCount = 1;
//        i2cTransaction.readBuf = rxBuffer;
//        i2cTransaction.readCount = 0;
//
//        for (i = TMP_COUNT - 1; i >= 0; i--)
//        {
//            i2cTransaction.slaveAddress = sensors[i].address;
//            txBuffer[0] = sensors[i].resultReg;
//
//            if (I2C_transfer(i2c, &i2cTransaction))
//            {
//                slaveAddress = sensors[i].address;
//                Display_printf(display, 0, 0, "Detected TMP%s sensor with slave"
//                               " address 0x%x",
//                               sensors[i].id, sensors[i].address);
//            }
//            else
//            {
//                i2cErrorHandler(&i2cTransaction, display);
//            }
//        }
//
//        I2C_close(i2c);


    }

}

//static Clock_Struct myClock;
//
//void myClockSwiI2CFxn(uintptr_t arg0)
//{
//    Semaphore_post(semTacheI2CHandle);
//}





void TacheI2C_CreateTask(void){

    Semaphore_Params semParams;
    Task_Params taskParams;

    /* Configuration de la tache*/
    Task_Params_init(&taskParams);
    taskParams.stack = TacheI2CStack;
    taskParams.stackSize = TACHEI2C_TASK_STACK_SIZE;
    taskParams.priority = TACHEI2C_TASK_PRIORITY;
    /* Creation de la tache*/
    Task_construct(&TacheI2C,TacheI2C_taskFxn,&taskParams, NULL);
    /* Construire un objet semaphore
    pour etre utilise comme outil de
    verrouillage, comptage initial 0 */
    Semaphore_Params_init(&semParams);
    Semaphore_construct(&semTacheI2CStruct,0, &semParams);
    /* Obtenir la gestion de l'instance */
    semTacheI2CHandle = Semaphore_handle(&semTacheI2CStruct);


}


