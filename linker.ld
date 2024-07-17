_stack_size = 0x4000; /* Statically allocate a 16kB stack for hypervisor */

ENTRY(_start_bare_metal) /* if omitted, qemu starts fecthing from the first .text section it founds in memory */

SECTIONS {
    . = 0x40000000; /* mem start, tried to put code at 0x0000 but didnt worked */
    .text.startup : {
        build/start.o(.text) /* force entry point at the beginning of text */
    }
    .text : {
        *(.text*)
    }
    .data : {
        *(.data*)
    }
    .bss : {
        *(.bss*)
    }
    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    }
    .stack : {
        . = ALIGN(16);
        _stack_end = .;
        . = . + _stack_size;
        _stack_start = .;  /* since stack grows towards smaller address, higher address is stack start! */
        . = ALIGN(16);
        _guest_stack_end = .;
        . = . + _stack_size;
        _guest_stack_start = .;  /* since stack grows towards smaller address, higher address is stack start! */
    }    

}
