set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/msvc2022_64")

cmake_minimum_required(VERSION 3.16)
project(JeonjuFoodMap)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(JeonjuFoodMap
    main.cpp
    MainWindow.h
    MainWindow.cpp
    Graph.h
    Graph.cpp
    MapWidget.h
    MapWidget.cpp
)

target_link_libraries(JeonjuFoodMap Qt6::Widgets)