cmake_minimum_required(VERSION 3.19)
project(Client)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME}
        PUBLIC
            Networking
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            Networking
)

# https://stackoverflow.com/questions/38767653/how-to-fix-undefined-reference-to-imp-wsacleanup-boost-asio-in-clion/38768623#38768623
if(MINGW)
    target_link_libraries(${PROJECT_NAME}
            PUBLIC
            ws2_32
    )
endif()
