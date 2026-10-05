cmake_minimum_required(VERSION 3.16)

project(cpp-user-crud
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Sql
)

add_executable(cpp-user-crud
    src/main.cpp
    src/Database.cpp
    src/UserRepository.cpp
    src/MainWindow.cpp
    include/User.hpp
    include/Database.hpp
    include/UserRepository.hpp
    include/MainWindow.hpp
)

target_include_directories(cpp-user-crud
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(cpp-user-crud
    PRIVATE
        Qt6::Widgets
        Qt6::Sql
)
