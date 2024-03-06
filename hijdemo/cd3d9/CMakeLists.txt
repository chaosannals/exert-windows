add_library(cd3d9 STATIC)

set_property(TARGET cd3d9 PROPERTY C_STANDARD 17)

target_sources(
    cd3d9
    PRIVATE
    "device.c"
)
