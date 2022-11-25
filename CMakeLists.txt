cmake_minimum_required(VERSION 3.20)

cmake_path(GET CMAKE_SOURCE_DIR STEM FOLDER_STEM)
string(REGEX REPLACE "[^0-9a-zA-Z_+\-\.]" "_" FOLDER_STEM_TARGET ${FOLDER_STEM})

project("${FOLDER_STEM_TARGET}" VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(
    SRC_FILES
    main.cpp
    glad.c
    CalculatorApp.cpp
    CalculatorApp.hpp
    Display.cpp
    Display.hpp
    RPNCalculator.cpp
    RPNCalculator.hpp
)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)

add_subdirectory(glfw-3.3.8 glfw)
include_directories(glfw-3.3.8/include include)

find_package(OpenGL REQUIRED)

add_subdirectory(Utilities)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} Utilities)
target_link_libraries(${PROJECT_NAME} opengl32)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${FOLDER_STEM}")