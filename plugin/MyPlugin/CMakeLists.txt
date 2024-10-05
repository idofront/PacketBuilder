cmake_minimum_required(VERSION 3.10)
project(MyPlugin)

include_directories(${PROJECT_SOURCE_DIR}/src)

# plugin ライブラリを追加
include_directories(${PROJECT_SOURCE_DIR}/../../proj/plugincontract/src)

# utility ライブラリを追加
include_directories(${PROJECT_SOURCE_DIR}/../../proj/utility/src)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# ライブラリを生成
add_library(MyPlugin SHARED ${SRC_FILES})

# 出力ディレクトリを設定
set_target_properties(MyPlugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)
