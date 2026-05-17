cmake_minimum_required(VERSION 3.16)
project(GraphVisualizer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

set(HEADERS
        ${INCLUDE_DIR}/Graph.h
        ${INCLUDE_DIR}/GraphWidget.h
        ${INCLUDE_DIR}/GraphScene.h
        ${INCLUDE_DIR}/GraphLayout.h
        ${INCLUDE_DIR}/AlgorithmRunner.h
        ${INCLUDE_DIR}/MainWindow.h
)

set(SOURCES
        ${SRC_DIR}/Graph.cpp
        ${SRC_DIR}/GraphWidget.cpp
        ${SRC_DIR}/GraphScene.cpp
        ${SRC_DIR}/GraphLayout.cpp
        ${SRC_DIR}/AlgorithmRunner.cpp
        ${SRC_DIR}/MainWindow.cpp
)

# Поиск Qt6 компонентов
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# Включаем директории Qt для компиляции
include_directories(${Qt6Core_INCLUDE_DIRS})
include_directories(${Qt6Widgets_INCLUDE_DIRS})
include_directories(${Qt6Gui_INCLUDE_DIRS})

# =============================================
# БИБЛИОТЕКА (общая логика)
# =============================================
add_library(GraphVisualizer_lib STATIC ${SOURCES} ${HEADERS})
target_include_directories(GraphVisualizer_lib
        PUBLIC
        ${INCLUDE_DIR}
        ${Qt6Core_INCLUDE_DIRS}
        ${Qt6Widgets_INCLUDE_DIRS}
        ${Qt6Gui_INCLUDE_DIRS}
)
target_link_libraries(GraphVisualizer_lib PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
)

# =============================================
# ИСПОЛНЯЕМОЕ ПРИЛОЖЕНИЕ
# =============================================
add_executable(GraphVisualizer WIN32 ${SRC_DIR}/main.cpp)
target_link_libraries(GraphVisualizer PRIVATE GraphVisualizer_lib)
target_include_directories(GraphVisualizer PRIVATE
        ${INCLUDE_DIR}
        ${Qt6Core_INCLUDE_DIRS}
        ${Qt6Widgets_INCLUDE_DIRS}
        ${Qt6Gui_INCLUDE_DIRS}
)

# =============================================
# ТЕСТИРОВАНИЕ
# =============================================
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()