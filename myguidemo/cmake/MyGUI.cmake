include(FetchContent)

# MYGUI_RENDERSYSTEM
#  1 - Dummy
#  2 - Export (not used)
#  3 - Ogre
#  4 - OpenGL
#  5 - Direct3D 9
#  6 - Direct3D 11
#  7 - OpenGL 3.x
#  8 - OpenGL ES 2.0 (Emscripten)
function(FetchMyGUI)
    FetchContent_Declare(
        MyGUI
        GIT_REPOSITORY git@github.com:MyGUI/mygui.git
        GIT_TAG MyGUI3.4.3
        GIT_SHALLOW ON # 浅克隆，git 参数加 --depth=1
        GIT_PROGRESS ON # 打印下载进度
    )
    # 里面有多个 submodule 用 https 要翻墙
    FetchContent_MakeAvailable(MyGUI)
    set(MYGUI_SRC_DIR "${MyGUI_SOURCE_DIR}")
    set(MYGUI_SRC_DIR "${MyGUI_SOURCE_DIR}" PARENT_SCOPE)
    message(STATUS "[MyGUI] ${MYGUI_SRC_DIR}")
endfunction()
