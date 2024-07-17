/* Bare metal entry point */

.global _start_bare_metal
_start_bare_metal:
    /* Setup stack */
    ldr x30, =_stack_start
    mov sp, x30
    bl main
    b . /* When leaving main code hangs here */

.global start_guest
start_guest:
    ldr x30, =_guest_stack_start
    mov sp, x30
    bl guest_main
    b .

.global el2_vec_table
.balign 0x80
el2_vec_table:
    b dummy_vector
.balign 0x80
    b dummy_vector
.balign 0x80
    b dummy_vector
.balign 0x80
    b dummy_vector

.balign 0x80
    b dummy_vector
.balign 0x80
    b dummy_vector
.balign 0x80
    b dummy_vector
.balign 0x80
    b dummy_vector

.balign 0x80
    b hypervisor_exception_vector
.balign 0x80
    b dummy_vector
.balign 0x80
    b dummy_vector
.balign 0x80
    b dummy_vector

.balign 0x80
    b dummy_vector
.balign 0x80
    b dummy_vector
.balign 0x80
    b dummy_vector
.balign 0x80
    b dummy_vector
