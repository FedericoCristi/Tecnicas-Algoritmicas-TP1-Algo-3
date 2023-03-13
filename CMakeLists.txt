cmake_minimum_required(VERSION 3.18)
project(tp1algo3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(redSocial ejercicios/ej12.cpp ejercicios/ej12.h)
set_target_properties(redSocial PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/")


add_executable(redSocial_poda ejercicios/ej12.cpp ejercicios/ej12.h)
set_target_properties(redSocial_poda PROPERTIES COMPILE_DEFINITIONS "PODAR")
set_target_properties(redSocial_poda PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/")


add_executable(actividades_topDown ejercicios/actividades_dinamico.cpp)
set_target_properties(actividades_topDown PROPERTIES COMPILE_DEFINITIONS "TOPDOWN")
set_target_properties(actividades_topDown PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/")


add_executable(actividades_goloso ejercicios/actividades_dinamico.cpp)
set_target_properties(actividades_goloso PROPERTIES COMPILE_DEFINITIONS "GOLOSO")
set_target_properties(actividades_goloso PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/")


add_executable(actividades_bottomUp ejercicios/actividades_dinamico.cpp)
set_target_properties(actividades_bottomUp PROPERTIES COMPILE_DEFINITIONS "BOTTOMUP")
set_target_properties(actividades_bottomUp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/")
