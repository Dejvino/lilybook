set(COMPONENT_PRIV_INCLUDEDIRS ".")
set(COMPONENT_SRCS "spi_master_lobo.c")

set(COMPONENT_REQUIRES spi_flash)
set(COMPONENT_PRIV_REQUIRES bootloader_support)

register_component()


