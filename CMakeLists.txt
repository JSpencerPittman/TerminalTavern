if(CMAKE_SYSTEM_NAME STREQUAL "Darwin") # MacOS
    set(CMAKE_MINIMUM_REQUIRED_VERSION 3.24)
    set(PLATFORM_INCLUDE_PATH include/terminaltavern/platform/macos)
    set(PLATFORM_SRC_PATH platform/macos)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux") # Ubuntu
    set(CMAKE_MINIMUM_REQUIRED_VERSION 3.22)
    set(PLATFORM_INCLUDE_PATH include/terminaltavern/platform/ubuntu-jammy)
    set(PLATFORM_SRC_PATH platform/ubuntu-jammy)
else ()
    set(CMAKE_MINIMUM_REQUIRED_VERSION 3.24)
    set(PLATFORM_INCLUDE_PATH include/terminaltavern/platform/macos)
    set(PLATFORM_SRC_PATH platform/macos)
endif()

cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})
project(TerminalTavern)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-lncurses")

find_package(Boost REQUIRED COMPONENTS system)
find_package(nlohmann_json REQUIRED)
find_package(Curses REQUIRED)

include_directories(include/terminaltavern/shared)
include_directories(${PLATFORM_INCLUDE_PATH})

set(GENERAL_SOURCE_FILES shared/Player.cpp shared/Action.cpp)
set(CLIENT_SOURCE_FILES shared/client.cpp ${PLATFORM_SRC_PATH}/TTClient.cpp shared/Room.cpp)
set(SERVER_SOURCE_FILES shared/server.cpp ${PLATFORM_SRC_PATH}/TTServer.cpp shared/TCPConnection.cpp)

add_executable(Client ${CLIENT_SOURCE_FILES} ${GENERAL_SOURCE_FILES})
add_executable(Server ${SERVER_SOURCE_FILES} ${GENERAL_SOURCE_FILES})

target_link_libraries(Client Boost::system nlohmann_json::nlohmann_json  ${CURSES_LIBRARIES})
target_link_libraries(Server Boost::system nlohmann_json::nlohmann_json)
