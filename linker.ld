SECTIONS {
  .init 0x8000 : {
    *(.init)
  }
  .text : { *(.text) }

  . = ALIGN(4);
  .data : { *(.data) }

  . = ALIGN(4);
  inicio_bss = .;
  .bss : { *(.bss*) }
  fim_bss = .;

  . = ALIGN(8);
  inicio_heap = .;
  . = . + 32K;
  inicio_stack = .;
}