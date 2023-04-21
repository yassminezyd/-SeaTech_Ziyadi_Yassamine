
#include <xc.h>
#include "UART_Protocol.h"
#include "CB_TX1.h"
#include "IO.h"

unsigned char UartCalculateChecksum(int msgFunction,
        int msgPayloadLength, unsigned char* msgPayload) {
    //Fonction prenant entree la trame et sa longueur pour calculer le checksum
    unsigned char checksum = 0;
    checksum ^= 0xFE;
    checksum ^= (unsigned char) (msgFunction >> 8);
    checksum ^= (unsigned char) (msgFunction >> 0);
    checksum ^= (unsigned char) (msgPayloadLength >> 8);
    checksum ^= (unsigned char) (msgPayloadLength >> 0);

    for (int i = 0; i < msgPayloadLength; i++) {
        checksum ^= msgPayload[i];
    }

    return checksum;
}

void UartEncodeAndSendMessage(int msgFunction,
        int msgPayloadLength, unsigned char* msgPayload) {
    //Fonction d?encodage et d?envoi d?un message
    unsigned char message[msgPayloadLength + 6];
    int pos = 0;
    message[pos++] = 0xFE;
    message[pos++] = (unsigned char) (msgFunction >> 8);
    message[pos++] = (unsigned char) (msgFunction >> 0);
    message[pos++] = (unsigned char) (msgPayloadLength >> 8);
    message[pos++] = (unsigned char) (msgPayloadLength >> 0);

    for (int i = 0; i < msgPayloadLength; i++) {
        message[pos++] = msgPayload[i];
    }

    message[pos++] = UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload);

    SendMessage(message, pos);

}

enum StateReception {
    Waiting,
    FunctionMSB,
    FunctionLSB,
    PayloadLengthMSB,
    PayloadLengthLSB,
    Payload,
    CheckSum
};

int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;

unsigned char rcvState;

//void UartDecodeMessage(unsigned char c) {
//    //Fonction prenant en entree un octet et servant a reconstituer les trames
//    switch (rcvState) {
//        case Waiting:
//            if (c == 0xFE) {
//                rcvState = FunctionMSB;
//            }
//            break;
//        case FunctionMSB:
//            msgDecodedFunction = c << 8;
//            rcvState = FunctionLSB;
//            break;
//        case FunctionLSB:
//            msgDecodedFunction += c << 0;
//            rcvState = PayloadLengthMSB;
//            break;
//        case PayloadLengthMSB:
//            msgDecodedFunction = c << 8;
//            rcvState = PayloadLengthLSB;
//            break;
//        case PayloadLengthLSB:
//            msgDecodedFunction += c << 0;
//
//            if (msgDecodedPayloadLength > 0) {
//                rcvState = Payload;
//            } else {
//                rcvState = CheckSum;
//                msgDecodedPayloadIndex = 0;
//                msgDecodedPayload[msgDecodedPayloadLength];
//            }
//            break;
//        case Payload:
//            msgDecodedPayload[msgDecodedPayloadIndex] = c;
//            msgDecodedPayloadIndex++;
//            if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
//                rcvState = CheckSum;
//            break;
//        case CheckSum:
//            unsigned char receivedChecksum = c;
//            unsigned char calculatedChecksum = UartCalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
//            
//            if (calculatedChecksum == receivedChecksum) {
//                //Success, on a un message valide
//                //UartEncodeAndSendMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
//            }
//            rcvState = Waiting;
//            break;
//        default:
//            rcvState = Waiting;
//            break;
//    }
//}
//
//void UartProcessDecodedMessage(int function,
//        int payloadLength, unsigned char* payload) {
//    //Fonction appelee apres le decodage pour executer l?action
//    //correspondant au message recu
//    switch (function) {
//        case (0x0080):
//            //UartEncodeAndSendMessage(function, payload, payloadLength);
//            break;
//        case (0x0020):          
//
//            break;
//        case (0x0030):
//
//            break;
//        case (0x0040):
//
//            break;
//    }
//}
//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/