cmake_minimum_required(VERSION 3.22)

project(SUBSET_MAKES_SIMPLE)

set(DATA_DIR ${CMAKE_SOURCE_DIR}/data)


add_executable(dfs src/dfs.cpp)
add_executable(dp src/dp.cpp)
add_executable(paper src/paper.cpp)
add_executable(fft src/fft.cpp)


add_custom_target(copy_data ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${DATA_DIR} ${CMAKE_BINARY_DIR}/data
)

add_dependencies(dfs copy_data)
add_dependencies(dp copy_data)
add_dependencies(paper copy_data)
