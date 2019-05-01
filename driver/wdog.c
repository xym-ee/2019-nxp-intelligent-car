#include "system.h"

#define WDOG_WCT_INSTRUCITON_COUNT (256U)

static WDOG_Type *wdog_base = WDOG1;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void WaitWctRefresh()
{
    for (uint32_t i = 0; i < WDOG_WCT_INSTRUCITON_COUNT; i++)
    {
    }
}

/* 看门狗测试 */
void Test_WDOG(void)
{
    wdog_config_t config;

    printf("--- Start Wdog test ---\r\n");
    /*
    * wdogConfig->enableWdog = true;
    * wdogConfig->workMode.enableWait = true;
    * wdogConfig->workMode.enableStop = false;
    * wdogConfig->workMode.enableDebug = false;
    * wdogConfig->enableInterrupt = false;
    * wdogConfig->enablePowerdown = false;
    * wdogConfig->resetExtension = flase;
    * wdogConfig->timeoutValue = 0xFFU;
    * wdogConfig->interruptTimeValue = 0x04u;
    */
    WDOG_GetDefaultConfig(&config);
    config.timeoutValue = 0xffu;
    config.interruptTimeValue = 0x08u;
    WDOG_Init(wdog_base, &config);
    printf("--- wdog Init done---\r\n");

    for (uint32_t i = 0; i < 10; i++)
    {
        WDOG_Refresh(wdog_base);
        printf("--- Refresh wdog %d time ---\r\n", i + 1);
        WaitWctRefresh();
    }

    printf("\r\nEnd of Wdog example!\r\n");
    while (1)
    {
    }
}
