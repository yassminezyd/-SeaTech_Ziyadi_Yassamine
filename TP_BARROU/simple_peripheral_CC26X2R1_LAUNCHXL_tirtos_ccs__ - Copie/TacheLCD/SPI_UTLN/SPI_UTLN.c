/*
 * SPISMIOT.c
 *
 *  Created on: 28 janv. 2020
 *      Author: TP-EO-6
 */

#include <stdbool.h>
#include <string.h>

#include <TacheLCD/SPI_UTLN/SPI_UTLN.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <driverlib/ssi.h>

/* Driver configuration */
#include "ti_drivers_config.h"

SPI_Handle      masterSpi;
SPI_Params      spiParams;
SPI_Transaction transaction;
bool            transferOK;

void SPI_UTLN_Init(void){

      SPI_init();
      /* Open SPI as master (default) */
      SPI_Params_init(&spiParams);
      spiParams.frameFormat = SPI_POL0_PHA0;
      spiParams.bitRate = 20000000;//4 MHz
//      spiParams.dataSize = 8;//16;

      masterSpi = SPI_open(CONFIG_SPI_0, &spiParams);
      if (masterSpi == NULL) {
          //if it doesnt init it waits here
          while (1);
      }

      GPIO_write(SPI_LCD_CS,1);
      GPIO_write(SPI_LCD_RS,1);


}


void SPI_Communication(unsigned char command[],int nbWrite, unsigned char results[], int nbRead, unsigned char csID){
    uint8_t masterTxBuffer3[nbWrite];
    uint8_t *masterRxBuffer3;
    uint8_t masterRxBuffer4[nbRead + nbWrite];//Because it also reads MISO during the transmissions
    masterRxBuffer3 = masterRxBuffer4;
    for(int i=0;i<nbWrite;i++)
        masterTxBuffer3[i] = command[i];

    memset((void *) masterRxBuffer3, 0, nbWrite);

    GPIO_write(SPI_LCD_CS,0);

    transaction.count = nbWrite + nbRead;
    transaction.txBuf = (void *) masterTxBuffer3;
    transaction.rxBuf = (void *) masterRxBuffer3;

    /* Perform SPI transfer */
    transferOK = SPI_transfer(masterSpi, &transaction);

    for(int i=nbWrite;i<nbWrite + nbRead;i++)
        results[i-nbWrite] = masterRxBuffer3[i];

    GPIO_write(SPI_LCD_CS,1);
}

