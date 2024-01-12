/*
 * Accelerometre.h
 *
 *  Created on: 21 déc. 2023
 *      Author: TP_EO_6
 */
#ifndef _ACCELEROMETRE_H_
#define _ACCELEROMETRE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
* CONSTANTS
*/
// Service UUID
#define ACCELEROMETRE_SERV_UUID 0xBA55

//  Characteristic defines
#define ACCELEROMETRE_ACCELEROMETREMESURES      0
#define ACCELEROMETRE_ACCELEROMETREMESURES_UUID 0x2BAD
#define ACCELEROMETRE_ACCELEROMETREMESURES_LEN  20

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */

// Callback when a characteristic value has changed
typedef void (*AccelerometreChange_t)( uint8 paramID );

typedef struct
{
  AccelerometreChange_t        pfnChangeCb;  // Called when characteristic value changes
} AccelerometreCBs_t;



/*********************************************************************
 * API FUNCTIONS
 */


/*
 * Accelerometre_AddService- Initializes the Accelerometre service by registering
 *          GATT attributes with the GATT server.
 *
 */
extern bStatus_t Accelerometre_AddService( void );

/*
 * Accelerometre_RegisterAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
extern bStatus_t Accelerometre_RegisterAppCBs( AccelerometreCBs_t *appCallbacks );

/*
 * Accelerometre_SetParameter - Set a Accelerometre parameter.
 *
 *    param - Profile parameter ID
 *    len - length of data to right
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
extern bStatus_t Accelerometre_SetParameter( uint8 param, uint8 len, void *value );

/*
 * Accelerometre_GetParameter - Get a Accelerometre parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
extern bStatus_t Accelerometre_GetParameter( uint8 param, void *value );

void SendAccelerometreMesure(void);

void SaveDataToSend(float AxADC, float AyADC, float AzADC);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _ACCELEROMETRE_H_ */
