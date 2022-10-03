add_executable(stringdemo)

# 定义 UNICODE 宏，使用 UNICODE
target_compile_definitions(
    stringdemo
    PRIVATE
    "-DUNICODE"
    "-D_UNICODE"
)

target_sources(
    stringdemo
    PRIVATE
    main.c
)
