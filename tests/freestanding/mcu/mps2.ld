ENTRY(Reset_Handler)
MEMORY {
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 4M
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 4M
}
SECTIONS {
  .text : { KEEP(*(.isr_vector)) *(.text*) *(.rodata*) } > FLASH
  _data_load = LOADADDR(.data);
  .data : { _data_start = .; *(.data*) _data_end = .; } > RAM AT > FLASH
  .bss  : { __bss_start__ = .; *(.bss* COMMON) __bss_end__ = .; } > RAM
  . = ALIGN(8);
  . = . + 0x8000;        /* 32K stack */
  _stack_top = ORIGIN(RAM) + LENGTH(RAM);
}
