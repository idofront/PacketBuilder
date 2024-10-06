cmake_minimum_required(VERSION 3.10)
project(plugincontract)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/Parser)
include_directories(${PROJECT_SOURCE_DIR}/src/Dumper)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# utility ライブラリを追加
include_directories(${PROJECT_SOURCE_DIR}/../../proj/utility/src)

# 外部ライブラリを追加
include_directories(${PROJECT_SOURCE_DIR}/../../submodules)

# ライブラリを生成
add_library(plugincontract SHARED ${SRC_FILES})

# -fPIC を有効にする
set_target_properties(plugincontract PROPERTIES POSITION_INDEPENDENT_CODE ON)

# 出力ディレクトリを設定
set_target_properties(plugincontract PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
