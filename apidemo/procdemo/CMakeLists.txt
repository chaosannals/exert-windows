add_executable(procdemo)

# 定义 UNICODE 宏，使用 UNICODE
target_compile_definitions(
    procdemo
    PRIVATE
    "-DUNICODE"
    "-D_UNICODE"
)

target_sources(
    procdemo
    PRIVATE
    main.c
)
