add_executable(rxdemo WIN32)

target_sources(
    rxdemo
    PRIVATE
    "main.cpp"
    "wnd.cpp"
)


target_link_libraries(
    rxdemo
    PRIVATE
    rxcpp
)
