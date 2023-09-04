project(client)
cmake_minimum_required(VERSION 3.19)

add_compile_options(-Wall -Wextra -Wpedantic -mconsole)

add_subdirectory(Networking)

add_subdirectory(Client)
add_subdirectory(Server)

