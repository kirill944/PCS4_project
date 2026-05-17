# Автоматическое скачивание GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# === UNIT ТЕСТЫ ===
add_executable(unit_tests
        unit_tests/test_graph.cpp
        unit_tests/test_algorithm_runner.cpp
        unit_tests/test_graph_layout.cpp
)

target_link_libraries(unit_tests PRIVATE
        GraphVisualizer_lib
        gtest_main
)

target_include_directories(unit_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

add_test(NAME UnitTests COMMAND unit_tests)

# === СЦЕНАРИИ ===
add_executable(scenario_bfs scenarios/scenario_bfs.cpp)
target_link_libraries(scenario_bfs PRIVATE GraphVisualizer_lib)

add_executable(scenario_dfs scenarios/scenario_dfs.cpp)
target_link_libraries(scenario_dfs PRIVATE GraphVisualizer_lib)

add_executable(scenario_graph_io scenarios/scenario_graph_io.cpp)
target_link_libraries(scenario_graph_io PRIVATE GraphVisualizer_lib Qt6::Core)

add_executable(scenario_layout scenarios/scenario_layout.cpp)
target_link_libraries(scenario_layout PRIVATE GraphVisualizer_lib)

add_executable(scenario_complete_workflow scenarios/scenario_complete_workflow.cpp)
target_link_libraries(scenario_complete_workflow PRIVATE GraphVisualizer_lib)

# Добавляем тесты для сценариев (опционально)
add_test(NAME BFSScenario COMMAND scenario_bfs)
add_test(NAME DFSScenario COMMAND scenario_dfs)
add_test(NAME GraphIOScenario COMMAND scenario_graph_io)
add_test(NAME LayoutScenario COMMAND scenario_layout)
add_test(NAME CompleteWorkflow COMMAND scenario_complete_workflow)