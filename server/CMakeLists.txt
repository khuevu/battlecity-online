cmake_minimum_required (VERSION 3.0)
project (battle_tank_multiplayer_server)

include_directories(src/)
file(GLOB SOURCES src/*.cpp)

add_executable(tank-server ${SOURCES})
target_link_libraries (tank-server ${EXTRA_LIBS})

install (TARGETS tank-server DESTINATION bin)
install (FILES "src/*.h" DESTINATION include)
