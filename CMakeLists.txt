cmake_minimum_required(VERSION 3.24)
project(TerminalTavern)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-lncurses")

find_package(Boost REQUIRED COMPONENTS system)
find_package(nlohmann_json REQUIRED)
find_package(Curses REQUIRED)

add_executable(Client client.cpp Player.cpp Action.cpp Room.cpp TTClient.cpp)
add_executable(Server server.cpp Player.cpp Action.cpp Room.cpp TTServer.cpp)

target_link_libraries(Client Boost::system nlohmann_json::nlohmann_json  ${CURSES_LIBRARIERS})
target_link_libraries(Server Boost::system nlohmann_json::nlohmann_json)
#target_link_libraries(AsyncServer Boost::system nlohmann_json::nlohmann_json)
#target_link_libraries(Other ${CURSES_LIBRARIERS} nlohmann_json::nlohmann_json)
