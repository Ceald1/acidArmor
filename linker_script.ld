OUTPUT_FORMAT("pe-x86-64")
ENTRY(DriverEntry)
SECTIONS
{
    . = 0x0000000140000000;

    .text   : { *(.text*)  }
    .rdata  : { *(.rdata*) }
    .data   : { *(.data*)  }
    .bss    : { *(.bss*)   }
    .pdata  : { *(.pdata*) }
    .xdata  : { *(.xdata*) }
}
