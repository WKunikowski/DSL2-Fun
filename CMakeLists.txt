cmake_minimum_required(VERSION 3.7)
project(testProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(main
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)