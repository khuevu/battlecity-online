cmake_minimum_required (VERSION 3.0)
project (battle_tank_multiplayer_server)

set(CMAKE_CXX_STANDARD 11)

include_directories(src/)
file(GLOB SOURCES src/*.cpp)

add_executable(tank-server ${SOURCES})

# copy the level maps folder 
add_custom_command(TARGET tank-server POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                  ${CMAKE_SOURCE_DIR}/src/levels $<TARGET_FILE_DIR:tank-server>/levels)

target_link_libraries (tank-server ${EXTRA_LIBS})

install (TARGETS tank-server DESTINATION bin)
install (FILES "src/*.h" DESTINATION include)
