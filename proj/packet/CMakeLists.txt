cmake_minimum_required(VERSION 3.10)
project(packet)

include_directories(${PROJECT_SOURCE_DIR}/src)

# packetentity ライブラリを追加
include_directories(${PROJECT_SOURCE_DIR}/../packetentity/src)

# utility ライブラリを追加
include_directories(${PROJECT_SOURCE_DIR}/../utility/src)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# ライブラリを生成
add_library(packet STATIC ${SRC_FILES})

# 出力ディレクトリを設定
set_target_properties(packet PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
