add_library(winmm SHARED)

set_property(TARGET winmm PROPERTY CXX_STANDARD 20)

target_sources(
    winmm
    PRIVATE
    src/main.cpp
)
