#include "uart.h"


void main() {
    puts("Hypervisor BOOTED");
    unsigned long el = 0;
    asm volatile("mrs %0, CurrentEL": "=r" (el));
    if( (el >> 2) == 2)
        puts("Hypervisor BOOTED in EL2");
    else if ( (el >> 2) == 1)
        puts("Hypervisor BOOTED in EL1");
    else if ( (el >> 2) == 0)
        puts("Hypervisor BOOTED in EL0");
    else if ( (el >> 2) == 3)
        puts("Hypervisor BOOTED in EL3");

}
