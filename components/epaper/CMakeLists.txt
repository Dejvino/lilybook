set(COMPONENT_PRIV_INCLUDEDIRS "." "../")
set(COMPONENT_SRCS
"comic24.c"
"DefaultFont.c"
"DejaVuSans18.c"
"DejaVuSans24.c"
"dejavuX.c"
"EPD.c"
"EPDspi.c"
"minya24.c"
"SmallFont.c"
"tooney32.c"
"Ubuntu16.c"
)

set(COMPONENT_REQUIRES spi_flash)
set(COMPONENT_PRIV_REQUIRES bootloader_support)

register_component()


