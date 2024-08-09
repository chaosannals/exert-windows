add_executable(composedemo WIN32)

target_sources(
    composedemo
    PRIVATE
    "main.cpp"
    "wnd.cpp"
)

target_link_libraries(
    composedemo
    PRIVATE
    rxcpp
)
