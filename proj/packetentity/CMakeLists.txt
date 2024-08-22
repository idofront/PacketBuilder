cmake_minimum_required(VERSION 3.10)
project(packetentity)

include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# utility ライブラリを追加
include_directories(${PROJECT_SOURCE_DIR}/../utility/src)

# utility ライブラリを追加
include_directories(${PROJECT_SOURCE_DIR}/../../submodules)

# ライブラリを生成
add_library(packetentity STATIC ${SRC_FILES})

# 出力ディレクトリを設定
set_target_properties(packetentity PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
