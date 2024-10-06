cmake_minimum_required(VERSION 3.10)
project(MyPlugin)

include_directories(${PROJECT_SOURCE_DIR}/src)

# plugin ライブラリを追加
include_directories(${PROJECT_SOURCE_DIR}/../../plugin/plugincontract/src)

# utility ライブラリを追加
include_directories(${PROJECT_SOURCE_DIR}/../../proj/utility/src)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# ライブラリを生成
add_library(MyPlugin SHARED ${SRC_FILES})

# -fPIC を有効にする
set_target_properties(MyPlugin PROPERTIES POSITION_INDEPENDENT_CODE ON)

# plugincontract ライブラリをリンク
# find_library(PLUGINCONTRACT_LIB NAMES plugincontract PATHS ${PROJECT_SOURCE_DIR}/../../lib)
# if(PLUGINCONTRACT_LIB)
#     target_link_libraries(MyPlugin PUBLIC ${PLUGINCONTRACT_LIB})
# endif()
target_link_libraries(MyPlugin PUBLIC plugincontract)
# target_link_libraries(MyPlugin PRIVATE plugincontract)
# target_link_libraries(MyPlugin INTERFACE plugincontract)

# Utility ライブラリをリンク
find_library(UTILITY_LIB NAMES utility PATHS ${PROJECT_SOURCE_DIR}/../../lib)
if(UTILITY_LIB)
    target_link_libraries(MyPlugin PUBLIC ${UTILITY_LIB})
endif()

# 出力ディレクトリを設定
set_target_properties(MyPlugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)
