include(FetchContent)

function(FetchRxCpp)
    FetchContent_Declare(
        RxCpp
        GIT_REPOSITORY git@github.com:ReactiveX/RxCpp.git
        GIT_TAG v4.1.1
        GIT_SHALLOW ON # 浅克隆，git 参数加 --depth=1
        GIT_PROGRESS ON # 打印下载进度
    )
    # 里面有多个 submodule 用 https 要翻墙
    set(RXCPP_DISABLE_TESTS_AND_EXAMPLES ON CACHE INTERNAL "") # 关闭 测试和示例项目
    FetchContent_MakeAvailable(RxCpp)
    set(RXCPP_SRC_DIR "${RxCpp_SOURCE_DIR}")
    set(RXCPP_SRC_DIR "${RxCpp_SOURCE_DIR}" PARENT_SCOPE)
    message(STATUS "[RxCpp] ${RXCPP_SRC_DIR}")
endfunction()
