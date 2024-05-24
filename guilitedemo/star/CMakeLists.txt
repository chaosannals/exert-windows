add_executable(star WIN32)

if (MSVC)
    target_compile_definitions(
        star
        PRIVATE
        "-DUNICODE"
        "-D_UNICODE"
    )
endif()

message(STATUS "[star] ${GUILITE_SRC_DIR}")

#target_include_directories(
#    star
#    PUBLIC
#    "${GUILITE_SRC_DIR}"
#)

target_link_libraries(
    star
    starui
)

target_sources(
	star
	PRIVATE
	"main.cpp"
    "uiblock.cpp"
)