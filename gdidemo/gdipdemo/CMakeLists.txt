add_executable(gdipdemo WIN32)

target_sources(
    gdipdemo
    PRIVATE
    "main.cpp"
    "wnd.cpp"
)

target_link_libraries(
    gdipdemo
    PRIVATE
    Gdiplus.lib
)
