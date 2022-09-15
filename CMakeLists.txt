cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

# set(CMAKE_CXX_STANDARD 17)

project(server)				# Название проекта

set (${CMAKE_PROJECT_NAME}_VERSION_MAJOR 1)
set (${CMAKE_PROJECT_NAME}_VERSION_MINOR 0)

set (${CMAKE_PROJECT_NAME}_VERSION "${${CMAKE_PROJECT_NAME}_VERSION_MAJOR}.${${CMAKE_PROJECT_NAME}_VERSION_MINOR}")

# set(SOURCES 
#     uhtml.cpp 
#     uhttp.cpp
#     usrv.cpp
#     ./ctrlPage/serverCtrlPage.cpp
#     ucli.cpp
#     uservice.cpp)			# Установка переменной со списком исходников

LIST(APPEND SOURCES src/uhtml.cpp)
LIST(APPEND SOURCES src/uhttp.cpp)
LIST(APPEND SOURCES src/usrv.cpp)
LIST(APPEND SOURCES src/serverCtrlPage.cpp)
LIST(APPEND SOURCES src/ucli.cpp)
LIST(APPEND SOURCES src/uservice.cpp)



include_directories(inc)			# Расположение заголовочных файлов

set(source_d)
configure_file(
    "${PROJECT_SOURCE_DIR}/configure/version.h.in"
    "${PROJECT_SOURCE_DIR}/inc/configure/version.h")

configure_file(
    "${PROJECT_SOURCE_DIR}/configure/path.h.in"
    "${PROJECT_SOURCE_DIR}/inc/configure/version.h")
    # "${PROJECT_BINARY_DIR}/path.h")


add_executable(test_srv ${SOURCES} test/test_srv.cpp)	# Создает исполняемый файл с именем main
add_executable(test_cli ${SOURCES} test/test_cli.cpp)	# Создает исполняемый файл с именем main

# add_subdirectory(foo)				# Добавление подпроекта, указывается имя дирректории

target_link_libraries(test_srv winmm ws2_32)		# Линковка программы с библиотекой
target_link_libraries(test_cli winmm ws2_32)		# Линковка программы с библиотекой