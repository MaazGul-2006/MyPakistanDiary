cmake_minimum_required(VERSION 3.16)
project(MyPakistanDiary VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)

set(SOURCES
    src/TravelEntry.cpp
    src/Database.cpp
    src/City.cpp
    src/DiaryApp.cpp
    src/PreTripCheck.cpp
    src/HTMLExporter.cpp
    src/main.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${SQLITE3_INCLUDE_DIRS}
)

add_executable(MyPakistanDiary ${SOURCES})

target_link_libraries(MyPakistanDiary
    sfml-graphics
    sfml-window
    sfml-system
    ${SQLITE3_LIBRARIES}
)

target_compile_options(MyPakistanDiary PRIVATE -Wall -Wextra -Wpedantic)