cmake_minimum_required(VERSION 3.3)
project(Labyrinth)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    bot/bot.cpp
    player_client/player_client.cpp
    server/server.cpp
    viewer_client/viewer_client.cpp)

set(SERVER_SOURCE
        server/server.cpp)

add_executable(server ${SERVER_SOURCE})