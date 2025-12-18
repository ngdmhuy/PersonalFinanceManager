cmake_minimum_required(VERSION 3.10)
project(PersonalFinanceManager)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(PFM ${SOURCES} ${HEADERS})

set(DATA_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(DATA_DEST "${CMAKE_CURRENT_BINARY_DIR}/data")

add_custom_command(TARGET PFM POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${DATA_SOURCE}
    ${DATA_DEST}
    COMMENT "Copying data folder to build directory..."
)