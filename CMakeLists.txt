cmake_minimum_required(VERSION 3.5)

project(ftp-viewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/includes
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/ui
)

file(GLOB
    SRC_FILES
    *.cpp
    *.h
    src/*.cpp
    src/*.h
    src/ui/*.cpp
    src/ui/*.h
)

set(PROJECT_SOURCES
        ${SRC_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ftp-viewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(ftp-viewer SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ftp-viewer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

add_library(Qt5Ftp STATIC IMPORTED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes)
set_property(TARGET Qt5Ftp PROPERTY
             IMPORTED_LOCATION
             ${CMAKE_CURRENT_SOURCE_DIR}/lib/libQt5Ftp.a
)

target_link_libraries(
    ftp-viewer
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
        Qt5Ftp
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ftp-viewer)
endif()
