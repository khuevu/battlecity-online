cmake_minimum_required (VERSION 3.0)
project (battle_tank_multiplayer_server)

set(CMAKE_CXX_STANDARD 11)

include_directories(src/)
file(GLOB SOURCES src/bt_gamecontainer.cpp
        src/bt_player.cpp
        src/bt_map.cpp
        src/bt_tank.cpp
        src/bt_model.cpp
        src/bt_server.cpp
        src/bt_socket.cpp)

add_executable(tank-server ${SOURCES} src/main.cpp)

# copy the level maps folder 
add_custom_command(TARGET tank-server POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                  ${CMAKE_SOURCE_DIR}/src/levels $<TARGET_FILE_DIR:tank-server>/levels)

target_link_libraries (tank-server ${EXTRA_LIBS})

#ENable GTEST
set(GTEST_ROOT "/Users/khuevu/tools/googletest")
enable_testing()
add_subdirectory(${GTEST_ROOT} gtest/)
add_executable(tank-test ${SOURCES} test/bt_tank.t.cpp test/bt_map.t.cpp)
target_link_libraries(tank-test gtest gtest_main)
add_test(tank-test tank-test)

install (TARGETS tank-server DESTINATION bin)
install (FILES "src/*.h" DESTINATION include)
