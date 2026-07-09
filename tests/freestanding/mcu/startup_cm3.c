/* Minimal Cortex-M3 startup for QEMU mps2-an385 (ARMv7-M, thumb).
 * The vector table's first word is the initial SP, the second the reset entry.
 * Reset zeroes .bss then calls the Tauraro program's main(). No libc, no CRT. */
#include <stdint.h>
extern uint32_t _stack_top, __bss_start__, __bss_end__, _data_load, _data_start, _data_end;
extern int main(int, char**);
void Reset_Handler(void) {
    for (uint32_t* d=&_data_start, *s=&_data_load; d<&_data_end; ) *d++ = *s++;   /* copy .data */
    for (uint32_t* p=&__bss_start__; p<&__bss_end__; p++) *p = 0;                 /* zero .bss  */
    main(0, 0);
    for (;;) { }
}
void Default_Handler(void) { for (;;) { } }
/* Vector table at 0x0: [0]=initial SP, [1]=Reset. A handful of fault vectors
 * point at a spin so a fault is a clean hang rather than undefined behaviour. */
__attribute__((used, section(".isr_vector")))
void (* const _vectors[])(void) = {
    (void(*)(void))&_stack_top, Reset_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, 0, 0, 0, Default_Handler, Default_Handler, 0,
    Default_Handler, Default_Handler,
};
