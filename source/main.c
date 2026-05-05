#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <switch.h>

//Showing some of my arm-fu -- unfortunately this homebrew app runs at exception level 0 so I cannot do too much magic :(
//Souce: https://developer.arm.com/documentation/ddi0601/2021-06/AArch64-Registers/CNTPCT-EL0--Counter-timer-Physical-Count-register
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

    while(appletMainLoop()) {
        padUpdate(&pad);
        uint64_t kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Plus) {
            break;
        }
        uint64_t ticks = get_arm64_system_tick();
        printf("\x1b[0;0H  Register CNTPCT_EL0 has the value %zu in tick(s) or approx. %zu second(s) since last CPU reset", ticks, (ticks / 10000000));
        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}
