cmake_minimum_required(VERSION 3.10)
project(EthernetPlugin)

include_directories(${PROJECT_SOURCE_DIR}/src)

# plugin ライブラリを追加
include_directories(${PROJECT_SOURCE_DIR}/../../plugin/plugincontract/src)

# utility ライブラリを追加
include_directories(${PROJECT_SOURCE_DIR}/../../proj/utility/src)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE SRC_FILES "src/**/*.cpp")

# ライブラリを生成
add_library(EthernetPlugin SHARED ${SRC_FILES})

# -fPIC を有効にする
set_target_properties(EthernetPlugin PROPERTIES POSITION_INDEPENDENT_CODE ON)

# plugincontract ライブラリをリンク
target_link_libraries(EthernetPlugin PUBLIC plugincontract)

# Utility ライブラリをリンク
find_library(UTILITY_LIB NAMES utility PATHS ${PROJECT_SOURCE_DIR}/../../lib)
if(UTILITY_LIB)
    target_link_libraries(EthernetPlugin PUBLIC ${UTILITY_LIB})
endif()

# 出力ディレクトリを設定
set_target_properties(EthernetPlugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)
