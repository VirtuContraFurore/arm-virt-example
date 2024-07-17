/* Bare metal entry point */

.global _start_bare_metal
_start_bare_metal:
    /* Setup stack */
    ldr x30, =_stack_start
    mov sp, x30
    bl main
    b .
