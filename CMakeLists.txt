cmake_minimum_required(VERSION 4.3)
project(HubFlow)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(HubFlow
        Envio.cpp
        Envio.hpp
        Historial.hpp
        HubFlow.cpp
        HubFlow.hpp
        ListaPendientes.cpp
        ListaPendientes.hpp
        main.cpp
        NivelServicio.hpp)
