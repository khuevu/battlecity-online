cmake_minimum_required (VERSION 3.0)
project (TANKSERVER)

include_directories(src/)
file(GLOB sources src/*.cpp) 

#add_subdirectory(src) // only needed in the subdirectory contain CMAkeLists.txt
add_executable(tank-server ${sources})
target_link_libraries (tank-server ${EXTRA_LIBS})

install (TARGETS tank-server DESTINATION bin)
install (FILES "src/*.h" DESTINATION include)
