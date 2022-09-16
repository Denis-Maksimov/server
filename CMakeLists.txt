cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")



set (MINGW true)
# set (UNIX false)

project(server)				# Название проекта

set (${CMAKE_PROJECT_NAME}_VERSION_MAJOR 0)
set (${CMAKE_PROJECT_NAME}_VERSION_MINOR 1)

set (${CMAKE_PROJECT_NAME}_VERSION "${${CMAKE_PROJECT_NAME}_VERSION_MAJOR}.${${CMAKE_PROJECT_NAME}_VERSION_MINOR}")


LIST(APPEND SOURCES src/uhtml.cpp)
LIST(APPEND SOURCES src/uhttp.cpp)
LIST(APPEND SOURCES src/usrv.cpp)
LIST(APPEND SOURCES src/ctrlPage/serverCtrlPage.cpp)
LIST(APPEND SOURCES src/ucli.cpp)
LIST(APPEND SOURCES src/uservice.cpp)

# set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O2)
add_link_options(-static)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
include_directories(inc)			# Расположение заголовочных файлов

# set(source_d)
configure_file(
    "${PROJECT_SOURCE_DIR}/configure/version.h.in"
    "${PROJECT_SOURCE_DIR}/inc/configure/version.h")

configure_file(
    "${PROJECT_SOURCE_DIR}/configure/path.h.in"
    "${PROJECT_SOURCE_DIR}/inc/configure/path.h")
    # "${PROJECT_BINARY_DIR}/path.h")

# MESSAGE("CMAKE_HOST_SYSTEM_NAME ${CMAKE_HOST_SYSTEM_NAME}")



if(MINGW)
    MESSAGE("Сборка под вындю")
    set(TEST_SRV    test_srv_v${${CMAKE_PROJECT_NAME}_VERSION}.exe)
    set(TEST_CLI    test_cli_v${${CMAKE_PROJECT_NAME}_VERSION}.exe)
    add_executable(${TEST_SRV} ${SOURCES} test/test_srv.cpp)	# Создает исполняемый файл с именем main
    add_executable(${TEST_CLI} ${SOURCES} test/test_cli.cpp)	# Создает исполняемый файл с именем main
    target_link_libraries(${TEST_SRV} winmm ws2_32)		# Линковка программы с библиотекой
    target_link_libraries(${TEST_CLI} winmm ws2_32)		# Линковка программы с библиотекой
endif(MINGW)


if (UNIX)
    MESSAGE("Сборка под линю")
    set(TEST_SRV test_srv_v${${CMAKE_PROJECT_NAME}_VERSION})
    set(TEST_CLI test_cli_v${${CMAKE_PROJECT_NAME}_VERSION})
    add_executable(${TEST_SRV} ${SOURCES} test/test_srv.cpp)	# Создает исполняемый файл с именем main
    add_executable(${TEST_CLI} ${SOURCES} test/test_cli.cpp)	# Создает исполняемый файл с именем main
endif(UNIX)


