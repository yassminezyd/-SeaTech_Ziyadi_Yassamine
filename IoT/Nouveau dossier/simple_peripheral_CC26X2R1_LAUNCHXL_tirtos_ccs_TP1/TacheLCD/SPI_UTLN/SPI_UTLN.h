/*
 * SPISMIOT.h
 *
 *  Created on: 28 janv. 2020
 *      Author: TP-EO-6
 */

#ifndef TASKSPISENSORS_SPISMIOT_SPISMIOT_H_
#define TASKSPISENSORS_SPISMIOT_SPISMIOT_H_

void SPI_UTLN_Init(void);
void SPI_Communication(unsigned char command[],int nbWrite, unsigned char results[], int nbRead, unsigned char csID);


#endif /* TASKSPISENSORS_SPISMIOT_SPISMIOT_H_ */
