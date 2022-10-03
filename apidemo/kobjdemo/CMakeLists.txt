add_executable(kobjdemo)

# 定义 UNICODE 宏，使用 UNICODE
target_compile_definitions(
    kobjdemo
    PRIVATE
    "-DUNICODE"
    "-D_UNICODE"
)

target_sources(
    kobjdemo
    PRIVATE
    main.c
)
