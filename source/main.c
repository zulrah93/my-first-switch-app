#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <switch.h>

//Showing some of my arm-fu -- unfortunately this homebrew app runs at exception level 0 so I cannot do too much magic :(

static inline volatile uint64_t get_arm64_system_tick(void) {
    uint64_t ticks;
    __asm__ __volatile__ ("MRS %x[data], CNTPCT_EL0" : [data] "=r" (ticks));
    return ticks;
}

//Source: https://developer.arm.com/documentation/ddi0601/2026-03/AArch64-Registers/RNDRRS--Random-Number-Full-Entropy
//Undefined on the switch for some reason -- not sure cannot check since this app is not on EL0
static inline volatile uint64_t get_rndrrs_special_arm_register_value(void) {
    uint64_t seed;
    __asm__ __volatile__ ("MRS %x[data], RNDRRS" : [data] "=r" (seed));
    return seed;
}

int main(int argc, char **argv)
{
    consoleInit(NULL);
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    PadState pad;
    padInitializeDefault(&pad);


    printf("\x1b[0;0H  CNTPCT_EL0=%zu", get_arm64_system_tick());

    while(appletMainLoop()) {
        padUpdate(&pad);
        uint64_t kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Plus) {
            break;
        }
        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}
