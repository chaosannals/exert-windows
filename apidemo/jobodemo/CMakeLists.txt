add_executable(jobodemo)

# 定义 UNICODE 宏，使用 UNICODE
target_compile_definitions(
    jobodemo
    PRIVATE
    "-DUNICODE"
    "-D_UNICODE"
)

target_sources(
	jobodemo
	PRIVATE
	"src/main.c")


 add_executable(job1demo)

 # 定义 UNICODE 宏，使用 UNICODE
target_compile_definitions(
    job1demo
    PRIVATE
    "-DUNICODE"
    "-D_UNICODE"
)

target_sources(
	job1demo
	PRIVATE
	"src/job1.c")