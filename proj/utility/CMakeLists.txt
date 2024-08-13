cmake_minimum_required(VERSION 3.10)
project(utility)

include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# ライブラリを生成
add_library(utility STATIC ${SRC_FILES})

# 出力ディレクトリを設定
set_target_properties(utility PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
