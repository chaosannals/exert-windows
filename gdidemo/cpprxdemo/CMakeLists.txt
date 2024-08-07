add_executable(cpprxdemo)

target_sources(
    cpprxdemo
    PRIVATE
    "main.cpp"
)


target_link_libraries(
    cpprxdemo
    PRIVATE
    rxcpp
)
