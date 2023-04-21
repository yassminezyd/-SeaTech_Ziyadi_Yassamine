#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <libpic30.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "Robot.h"
#include "ToolBox.h"
#include "PWM.h"
#include "ADC.h"
#include "main.h"
#include "UART.h"
#include "UART_Protocol.h"


unsigned char stateRobot;
unsigned char nextStateRobot = 0;

int main(void) {
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();

    /****************************************************************************************************/
    // Configuration des entr�es sorties
    /****************************************************************************************************/
    InitIO();

    InitTimer23();
    InitTimer1();
    InitPWM();
    InitADC1();
    InitTimer4();
    InitUART();


    /*
    LED_BLANCHE = 1;
    LED_BLEUE = 1;
    LED_ORANGE = 1;
     */



    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    while (1) {
        //PWMSetSpeedConsigne(-20,MOTEUR_GAUCHE);
        //PWMSetSpeedConsigne(-20,MOTEUR_DROIT);


        ADC1StartConversionSequence();
        //unsigned int * result = ADCGetResult();
        //unsigned int ADCValue0 = result[0];
        //unsigned int ADCValue1 = result[1];
        //unsigned int ADCValue2 = result[2];

        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [0])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroitEx = 34 / volts - 5;
            volts = ((float) result [1])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [3])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGaucheEx = 34 / volts - 5;
            volts = ((float) result [4])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;



            if (robotState.distanceTelemetreDroitEx < 30) {
                LED_ORANGE = 1;
            } else {
                LED_ORANGE = 0;
            }
            if (robotState.distanceTelemetreDroit < 30) {
                LED_ORANGE = 1;
            } else {
                LED_ORANGE = 0;
            }
            if (robotState.distanceTelemetreCentre < 30) {
                LED_BLEUE = 1;
            } else {
                LED_BLEUE = 0;
            }

            if (robotState.distanceTelemetreGauche < 30) {
                LED_BLANCHE = 1;
            } else {
                LED_BLANCHE = 0;
            }

            if (robotState.distanceTelemetreGaucheEx < 30) {
                LED_BLANCHE = 1;
            } else {
                LED_BLANCHE = 0;
            }


            unsigned char payload[] = {'B', 'o', 'n', 'j', 'o', 'u', 'r'};
            UartEncodeAndSendMessage(0x0080, sizeof (payload), payload);
            
            unsigned char payload2[] =  {(unsigned char)robotState.distanceTelemetreDroit, (unsigned char)robotState.distanceTelemetreCentre, (unsigned char)robotState.distanceTelemetreGauche};
            UartEncodeAndSendMessage(0x0030, sizeof (payload2), payload2);
            
            unsigned char payload3[] =  {(signed char)robotState.vitesseDroiteCommandeCourante, (signed char)robotState.vitesseGaucheCommandeCourante};
            UartEncodeAndSendMessage(0x0040, sizeof (payload3), payload3);
            
            
            unsigned char payload4[] =  {(unsigned char)(LED_ORANGE),(unsigned char)(LED_BLANCHE), (unsigned char)(LED_BLEUE)};
            UartEncodeAndSendMessage(0x0020, sizeof (payload4), payload4);
            //__delay32(40000000);

        }

        OperatingSystemLoop();

        //unsigned char x = ADCIsConversionFinished();
        //if (x == 1)
        //ADCClearConversionFinishedFlag();
        //PWMSetSpeedConsigne(-20,MOTEUR_GAUCHE);

        //        SendMessageDirect((unsigned char*) "Bonjour", 7);
        //        __delay32(40000000);
        //        int i;
        //        for (i = 0; i < CB_RX1_GetDataSize(); i++) {
        //            unsigned char c = CB_RX1_Get();
        //            SendMessage(&c, 1);
        //        }
        //__delay32(10000);

    } // fin main
}

void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;
        case STATE_AVANCE:
            PWMSetSpeedConsigne(24, MOTEUR_DROIT);
            PWMSetSpeedConsigne(24, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(24, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(24, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;
    //�Dtermination de la position des obstacles en fonction des ���tlmtres
    if (robotState.distanceTelemetreDroitEx < 30 &&
            robotState.distanceTelemetreDroit < 25 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 25 &&
            robotState.distanceTelemetreGaucheEx > 30
            ) //Obstacle �droite
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreDroitEx > 30 &&
            robotState.distanceTelemetreDroit > 25 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche < 25 &&
            robotState.distanceTelemetreGaucheEx < 30
            ) //Obstacle �gauche
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreCentre < 20) //Obstacle en face
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreDroitEx > 30 &&
            robotState.distanceTelemetreDroit > 25 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 25 &&
            robotState.distanceTelemetreGaucheEx > 30) //pas d?obstacle
        positionObstacle = PAS_D_OBSTACLE;
    //�Dtermination de l�?tat �venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    //Si l?on n?est pas dans la transition de l�?tape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}
