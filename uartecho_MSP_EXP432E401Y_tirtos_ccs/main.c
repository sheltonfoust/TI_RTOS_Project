#include "library.h"

int main(void)
{
    Glo.uartInQ.msgReading = 0;
    Glo.uartInQ.msgWriting = 0;
    Glo.payloadQ.payloadReading = 0;
    Glo.payloadQ.payloadWriting = 0;
    Glo.timer0Period = 1000000;
    Glo.bios.messageTask = task0;
    Glo.bios.uartReadTask = task1;
    Glo.bios.uartReadSemaphore = semaphore0;
    Glo.bios.payloadSemaphore = semaphore1;
    Glo.bios.timer0Swi = swi0;
    Glo.bios.callbackGate = gateSwi0;

    int i;
    for (i = 0; i < NUMERROR; i++)
    {
        Glo.errorCounts[i] = 0;
    }

    for (i = 0; i < NUMMSGS; i++)
    {
        Glo.uartInQ.messages[i].index = i;
        Glo.uartInQ.messages[i].msgBuffer[0] = 0;
        Glo.uartInQ.messages[i].msgCount = 0;
    }
    for (i = 0; i < NUMPAYLOADS; i++)
    {
        Glo.payloadQ.payloads[i][0] = 0;
    }

    for (i = 0; i < NUMCALLBACKS; i++)
    {
        Glo.callbacks[i].index = i;
        Glo.callbacks[i].count = 0;
        Glo.callbacks[i].hwiFlag = false;
        Glo.callbacks[i].payload[0] = 0;
    }

    GPIO_init();
    UART_init();

    GPIO_write(0, 1); // CONFIG_GPIO_0, CONFIG_GPIO_LED_ON
    GPIO_write(1, 1);
    GPIO_write(2, 1);
    GPIO_write(3, 1);
    GPIO_write(4, 1);
    GPIO_write(5, 1);

    UART_Params uartParams;
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 115200;

    Glo.uart = UART_open(0, &uartParams); // CONFIG_UART_0, &uartParams

    if (Glo.uart == NULL)
    {
        // UART_open() failed
        while (1)
            ;
    }




    ti_ndk_config_Global_startupFxn();

    BIOS_start();

    return (0);
}
