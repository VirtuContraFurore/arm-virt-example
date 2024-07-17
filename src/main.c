#include "uart.h"

#define EL2 2
#define EL1 1

#define HCR_RW  31
#define HCR_TWE 14
#define HCR_TWI 13

extern long _guest_stack_start;
extern void start_guest();
extern void el2_vec_table();

int check_execution_level();
void dummy_vector();
void hypervisor_exception_vector();
void install_el2_vbar();
void start_guest_VM();

void guest_main(){
    puts("GUEST: hello main!");
    for(int i=0; i<5;i++){
        asm volatile("wfi");
        puts("GUEST: alive after WFI");
        asm volatile("wfe");
        puts("GUEST: alive after WFE");
    }
    puts("GUEST: VM done...");
}

void main() {
    if(check_execution_level() != EL2)
        return;
    install_el2_vbar();
    start_guest_VM();
    puts("Unreachable code");
}

int check_execution_level(){
    /* Read CurrentEL register to check Execution Level */
    unsigned long el;
    asm volatile("mrs %0, CurrentEL": "=r" (el));
    if( (el >> 2) == 2) {
        puts("HYPERV: Hypervisor BOOTED in EL2");
        return EL2;
    } else {
        puts("HYPERV: Hypervisor is not in EL2, is virtualization=on ?");
        puts("HYPERV: Exiting...");
        return EL1;
    }
}

void install_el2_vbar(){
    asm volatile("msr vbar_el2, %0": : "r" (el2_vec_table));
    puts("HYPERV: VBAR_EL2 installed.");
}

void start_guest_VM(){
    unsigned long hcr_el2;
    asm volatile("mrs %0, hcr_el2" : "=r" (hcr_el2));
    hcr_el2 |= (1<<HCR_RW) | (1<<HCR_TWI) | (1<<HCR_TWE); // set RW=1 (lower levels are 64bit) + trap wait for interrupt
    asm volatile("msr hcr_el2, %0" : : "r" (hcr_el2));

    asm volatile("msr elr_el2, %0" : : "r" (start_guest)); // set guest entry point
    asm volatile("msr spsr_el2, %0" : : "r" (0)); // clear guest saved program state register
    //asm volatile("msr sctlr_el1, %0" : : "r" (0)); // clear
    asm volatile("msr sp_el1, %0" : : "r" (_guest_stack_start)); // setup guest stack
    puts("HYPERV: Starting VM...");
    asm volatile("eret"); // start guest VM
}

void dummy_vector(){
    while(1);
}

void hypervisor_exception_vector(){
    puts("HYPERV: Exception caught from EL1/0");
    /* Now read excpetion syndrome */
    unsigned long esr_el2, elr_el2;
    asm volatile("mrs %0, esr_el2" : "=r" (esr_el2));
    asm volatile("mrs %0, elr_el2" : "=r" (elr_el2));
    unsigned long ec = esr_el2 >> 26;
    if(ec == 0x01) {
        if(esr_el2 & 1)
            puts("HYPERV: trapped WFE");
        else
            puts("HYPERV: trapped WFI");
    } else
        puts("HYPERV: unrecognized exception syndrome");
    elr_el2 += 4; // move to next instr
    asm volatile("msr elr_el2, %0" : : "r" (elr_el2)); // updated PC
    asm volatile("eret"); // return from exception
}
