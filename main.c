#include "APP/Bootloader/BOOTLOADER_interface.h"

int main()
{
    /* Initialize the bootloader module */
    BOOTLOADER_Init();

    /* Start the bootloader operation */
    BOOTLOADER_Start();

    /* Return 0 to indicate successful execution (optional) */
    return 0;
}
