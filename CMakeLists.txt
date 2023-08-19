cmake_minimum_required(VERSION 3.22)
project(TerminalTavern)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-lncurses")

find_package(Boost REQUIRED COMPONENTS system)
find_package(nlohmann_json REQUIRED)
find_package(Curses REQUIRED)

include_directories(include/terminaltavern)

set(GENERAL_SOURCE_FILES src/Player.cpp src/Action.cpp src/Room.cpp)
set(CLIENT_SOURCE_FILES src/client.cpp src/TTClient.cpp)
set(SERVER_SOURCE_FILES src/server.cpp src/TTServer.cpp)

add_executable(Client ${CLIENT_SOURCE_FILES} ${GENERAL_SOURCE_FILES})
add_executable(Server ${SERVER_SOURCE_FILES} ${GENERAL_SOURCE_FILES})

target_link_libraries(Client Boost::system nlohmann_json::nlohmann_json  ${CURSES_LIBRARIERS})
target_link_libraries(Server Boost::system nlohmann_json::nlohmann_json)
