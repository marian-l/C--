cmake_minimum_required(VERSION 3.26)
project(others)

add_compile_options(-Wall -Wextra -Wpedantic -mconsole)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(executable ${SOURCES} main.cpp)

find_package(Boost 1.77.0 REQUIRED)

target_include_directories(executable
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${Boost_INCLUDE_DIRS}
        PRIVATE
)

target_link_libraries(executable
        PRIVATE
        ${Boost_LIBRARIES}
)

if(MINGW)
    target_link_libraries(executable
            PRIVATE
            ws2_32
            wsock32
    )
endif()