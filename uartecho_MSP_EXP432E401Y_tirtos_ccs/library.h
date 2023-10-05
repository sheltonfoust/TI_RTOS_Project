#ifndef LIBRARY_H_
#define LIBRARY_H_

#define VERSION "4.2"
#define ASSIGNMENT 4
#define AUTHOR "Shelton Foust"
#define MAXLEN 365
#define NUMGPIOS 8
#define NUMCALLBACKS 4
#define NUMMSGS 10
#define NUMPAYLOADS 32

#define MSG_OVERFLOW 0
#define INVALID_OPERATION 1
#define WRONG_TYPE 2
#define BAD_NUMBER 3
#define PAYLOAD_Q_OVERFLOW 4
#define NUMERROR 5


#include <ti/drivers/UART.h>
#include <ti/drivers/GPIO.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/gates/GateSwi.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/Timer.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <stdbool.h>


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structs


typedef struct _BiosStruct
{
    Task_Handle messageTask;
    Task_Handle uartReadTask;
    Semaphore_Handle uartReadSemaphore;
    Semaphore_Handle payloadSemaphore;
    Swi_Handle timer0Swi;
    GateSwi_Handle callbackGate;
} BiosStruct;


typedef struct _callback
{
    int index;
    int count;
    bool hwiFlag; // used for audio
    char payload[MAXLEN];
} Callback;

typedef struct _message
{
    int index; // the index in the messages array, for debugging only
    int msgCount; // bytes going in, used to detect overflow
    char msgBuffer[MAXLEN];
} Message;

// Queues multiple messages if input types really fast. Piles up messages from the console
// Only really useful if using an external app.
typedef struct _MessageQ
{
    int msgReading;
    int msgWriting;
    Message messages[NUMMSGS];
} MessageQ;


typedef struct _PayloadQ // Parse and Process queue
{
    int payloadReading; // the index of which message is being read
    int payloadWriting; // the index of which message is being written
    char payloads[NUMPAYLOADS][MAXLEN];
} PayloadQ;


typedef struct _GlobalStruct
{
    int timer0Period;
    int errorCounts[NUMERROR];
    Callback callbacks[NUMCALLBACKS];
    BiosStruct bios;
    MessageQ uartInQ;
    PayloadQ payloadQ;
    UART_Handle uart;
    Timer_Handle timer0;
} GlobalStruct;


// Structs
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


GlobalStruct Glo;



void aboutParse();
void errorParse(char* input);
void errorBase();
void gpioBase();
void helpBase();
void memoryReadParse(char* input);
void printParse(char* input);
void readGPIOprint(int gpio);
void writeGPIOprint(int gpioNum, char* remainder);
void removeSpaces(char* noSpaces, const char* input);
void gpioParse(char* input);
void helpParse(char* input);
int startsWith(char *input, char *commandName);



#endif
