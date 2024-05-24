include(FetchContent)

function (FetchGuiLite)
	FetchContent_Declare(
        guilite
        GIT_REPOSITORY git@github.com:idea4good/GuiLite.git
        GIT_TAG v2.1
        GIT_SHALLOW ON # 浅克隆，git 参数加 --depth=1
        GIT_PROGRESS ON # 打印下载进度
    )
    FetchContent_MakeAvailable(guilite)
    set(GUILITE_SRC_DIR "${guilite_SOURCE_DIR}")
    set(GUILITE_SRC_DIR "${guilite_SOURCE_DIR}" PARENT_SCOPE)
    message(STATUS "[guilite] ${GUILITE_SRC_DIR}")
endfunction()
