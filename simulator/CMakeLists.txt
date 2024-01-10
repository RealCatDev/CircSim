cmake_minimum_required(VERSION 3.12)

project(CircSim)

set(SOURCE_FILES
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/simulator.cpp
)

add_executable(CircSim ${SOURCE_FILES})

target_include_directories(CircSim
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(CircSim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_features(CircSim PRIVATE cxx_std_17)