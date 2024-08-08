add_executable(asyncdemo)

target_sources(
    asyncdemo
    PRIVATE
    "main.cpp"
)


target_link_libraries(
    asyncdemo
    PRIVATE
    rxcpp
)
