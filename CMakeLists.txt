cmake_minimum_required(VERSION 3.10)
project(Builder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# fmt の設定
find_package(fmt REQUIRED)
include_directories(${fmt_INCLUDE_DIRS})

# Google Test の設定
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# include ディレクトリを追加
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/main)

# src ディレクトリの cpp ファイルを取得（サブフォルダも含む）
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# src ディレクトリのソースファイルをオブジェクトライブラリとしてビルド
add_library(Builder OBJECT ${SRC_FILES})

# main ディレクトリの cpp ファイルを取得
file(GLOB MAIN_SOURCES "main/*.cpp")

# 各 main ソースファイルごとに実行可能ファイルを生成
foreach(MAIN_SOURCE ${MAIN_SOURCES})
  get_filename_component(MAIN_NAME ${MAIN_SOURCE} NAME_WE)
  add_executable(${MAIN_NAME} ${MAIN_SOURCE} $<TARGET_OBJECTS:Builder>)
  target_include_directories(${MAIN_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src ${fmt_INCLUDE_DIRS})
  target_link_libraries(${MAIN_NAME} fmt::fmt)
  set_target_properties(${MAIN_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
endforeach()

# テストディレクトリの cpp ファイルを取得
file(GLOB TEST_SOURCES "test/*.cpp")

# 各テストソースファイルごとにテストターゲットを生成
foreach(TEST_SOURCE ${TEST_SOURCES})
  get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
  add_executable(${TEST_NAME} ${TEST_SOURCE} $<TARGET_OBJECTS:Builder>)
  target_include_directories(${TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src ${fmt_INCLUDE_DIRS})
  target_link_libraries(${TEST_NAME} fmt::fmt GTest::GTest GTest::Main)
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
  set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/test)
endforeach()

# CTest の有効化
enable_testing()
