cmake_minimum_required(VERSION 3.10.0)
project(HyperAcceleratedDragon VERSION 0.1.0 LANGUAGES CXX)
set(${CXX_STANDARD_REQUIRED} 20)
message("C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
find_package(Boost REQUIRED COMPONENTS program_options)

add_executable(HyperAcceleratedDragon 
${PROJECT_SOURCE_DIR}/src/main.cpp
${PROJECT_SOURCE_DIR}/src/compiler_driver.cpp
${PROJECT_SOURCE_DIR}/src/had_compiler.cpp
${PROJECT_SOURCE_DIR}/src/maximal_munch_lexer.cpp
${PROJECT_SOURCE_DIR}/src/ast.cpp
)

target_include_directories(HyperAcceleratedDragon PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(HyperAcceleratedDragon PRIVATE Boost::program_options)

include(CTest)
enable_testing()