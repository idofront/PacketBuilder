cmake_minimum_required(VERSION 3.10)
project(plugincontainer)

include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# plugin contaract を追加
include_directories(${PROJECT_SOURCE_DIR}/../plugincontract/src)
include_directories(${PROJECT_SOURCE_DIR}/../plugincontract/src/*)

# utility ライブラリを追加
include_directories(${PROJECT_SOURCE_DIR}/../utility/src)

# 外部ライブラリを追加
include_directories(${PROJECT_SOURCE_DIR}/../../submodules)

# ライブラリを生成
add_library(plugincontainer STATIC ${SRC_FILES})

# 出力ディレクトリを設定
set_target_properties(plugincontainer PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
