cmake_minimum_required(VERSION 3.16)

project(Ants)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML COMPONENTS graphics window system REQUIRED)
add_compile_options(-Wall -Wextra)

add_executable(
        Ants
        main.cpp
        src/app.cpp
        src/ant.cpp
)

target_include_directories(Ants PUBLIC include)
target_include_directories(Ants PRIVATE src)
target_link_libraries(Ants sfml-graphics)

# ---- TEST ----
enable_testing()
add_subdirectory(test)