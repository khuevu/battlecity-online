cmake_minimum_required(VERSION 3.0)
project(battle_tank_multiplayer)

set(CMAKE_CXX_STANDARD 11)

#set(SOURCE_FILES
#    server/src/bt_server.cpp
#    server/src/bt_server.h
#    server/src/bt_socket.cpp
#    server/src/bt_socket.h
#    server/src/main.cpp
#    server/CMakeLists.txt)

#add_executable(battle_tank_multiplayer ${SOURCE_FILES})

add_subdirectory(server)