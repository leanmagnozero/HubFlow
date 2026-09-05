cmake_minimum_required(VERSION 3.14)
project(HubFlow CXX)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Incluir GoogleTest para CI/CD
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(include)

set(SOURCE_FILES
        src/Envio.cpp
        src/HubFlow.cpp
        src/ListaPendientes.cpp
)

# 1. Ejecutable Principal
add_executable(HubFlowApp src/main.cpp ${SOURCE_FILES})

# 2. Ejecutable de Pruebas Unitarias
add_executable(unit_tests tests/test_hubflow.cpp ${SOURCE_FILES})
target_link_libraries(unit_tests gtest_main)

# Registrar tests en CTest
include(GoogleTest)
gtest_discover_tests(unit_tests)