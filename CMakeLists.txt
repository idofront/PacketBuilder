cmake_minimum_required(VERSION 3.10)
project(Builder)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(subproject_names)
set(plugin_names)

# proj ディレクトリ内のサブプロジェクトを追加
file(GLOB subproject_dirs LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/proj/*)
foreach(subproject_dir ${subproject_dirs})
  if (EXISTS "${subproject_dir}/CMakeLists.txt")
    get_filename_component(subproject_name ${subproject_dir} NAME)
    include_directories(${subproject_dir}/src)
    list(APPEND subproject_names ${subproject_name})  # サブプロジェクト名をリストに追加
    add_subdirectory(${subproject_dir} ${CMAKE_BINARY_DIR}/proj/${subproject_name})
    source_group("Proj\\${subproject_name}" FILES ${subproject_dir}/*)
  endif()
endforeach()

# plugin ディレクトリ内のサブプロジェクトを追加
file(GLOB plugin_dirs LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/plugin/*)
foreach(plugin_dir ${plugin_dirs})
  if (EXISTS "${plugin_dir}/CMakeLists.txt")
    get_filename_component(plugin_name ${plugin_dir} NAME)
    list(APPEND plugin_names ${plugin_name})  # プラグイン名をリストに追加
    add_subdirectory(${plugin_dir} ${CMAKE_BINARY_DIR}/plugin/${plugin_name})
    source_group("Plugin\\${plugin_name}" FILES ${plugin_dir}/*)
  endif()
endforeach()

# fmt の設定
find_package(fmt REQUIRED)
include_directories(${fmt_INCLUDE_DIRS})

# Google Test の設定
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# libpcap の設定
find_path(PCAP_INCLUDE_DIR pcap.h)
find_library(PCAP_LIBRARY pcap)

if(NOT PCAP_INCLUDE_DIR OR NOT PCAP_LIBRARY)
  message(FATAL_ERROR "libpcap not found. Please install libpcap.")
endif()

# libpoco の設定
find_package(Poco REQUIRED COMPONENTS Foundation Util Net)
include_directories(${Poco_INCLUDE_DIRS})

include_directories(${PCAP_INCLUDE_DIR})

# include ディレクトリを追加
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/submodules)
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
  target_link_libraries(${MAIN_NAME} ${subproject_names} fmt::fmt ${PCAP_LIBRARY} ${Poco_LIBRARIES})
endforeach()

# テストディレクトリの cpp ファイルを取得
file(GLOB TEST_SOURCES "test/**/*.cpp")

# 各テストソースファイルごとにテストターゲットを生成
foreach(TEST_SOURCE ${TEST_SOURCES})
  get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
  add_executable(${TEST_NAME} ${TEST_SOURCE} $<TARGET_OBJECTS:Builder>)
  target_include_directories(${TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src ${fmt_INCLUDE_DIRS})
  target_link_libraries(${TEST_NAME} ${subproject_names} fmt::fmt GTest::GTest GTest::Main ${PCAP_LIBRARY} ${Poco_LIBRARIES})
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
  set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
endforeach()

# CTest の有効化
enable_testing()
