cmake_minimum_required(VERSION 3.16)
project(GraphVisualizer VERSION 1.0.0 LANGUAGES CXX)

# Устанавливаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ВКЛЮЧАЕМ Qt MOC (ЭТО КРИТИЧЕСКИ ВАЖНО!)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Указываем директории
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# ЯВНО УКАЗЫВАЕМ ВСЕ ЗАГОЛОВОЧНЫЕ ФАЙЛЫ (для MOC)
set(HEADERS
        ${INCLUDE_DIR}/Graph.h
        ${INCLUDE_DIR}/GraphWidget.h
        ${INCLUDE_DIR}/GraphScene.h
        ${INCLUDE_DIR}/GraphLayout.h
        ${INCLUDE_DIR}/AlgorithmRunner.h
        ${INCLUDE_DIR}/MainWindow.h
)

# Исходные файлы
set(SOURCES
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/Graph.cpp
        ${SRC_DIR}/GraphWidget.cpp
        ${SRC_DIR}/GraphScene.cpp
        ${SRC_DIR}/GraphLayout.cpp
        ${SRC_DIR}/AlgorithmRunner.cpp
        ${SRC_DIR}/MainWindow.cpp
)

# Поиск Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# Создаём исполняемый файл (включаем и заголовки)
add_executable(GraphVisualizer WIN32 ${SOURCES} ${HEADERS})

# Указываем директории для поиска заголовков
target_include_directories(GraphVisualizer PRIVATE ${INCLUDE_DIR})

# Подключаем Qt библиотеки
target_link_libraries(GraphVisualizer PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
)

# Для Windows
if(WIN32)
    set_target_properties(GraphVisualizer PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
endif()