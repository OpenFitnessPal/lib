cmake_minimum_required(VERSION 3.14)

project(OFPExample LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(FORCE_QT6 OFF "Forcefully link to Qt 6")
option(FORCE_QT5 OFF "Forcefully link to Qt 5")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)

if (FORCE_QT6)
    set(QT_VERSION_MAJOR 6)
elseif (FORCE_QT5)
    set(QT_VERSION_MAJOR 5)
endif()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)
find_package(OFPLib REQUIRED)

message(STATUS "OFPLib found at ${OFPLib_CONFIG}")

add_executable(OFPExample
    main.cpp
)

target_link_libraries(OFPExample Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network OFPLib::OFPLib)
