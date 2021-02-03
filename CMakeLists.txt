cmake_minimum_required(VERSION 3.16)
project(AddonForAIMP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lws2_32 -D__USE_W32_SOCKETS -D_WIN32_WINNT=0x601")
set(CMAKE_CXX_STANDARD 20)

add_executable(AddonForAIMP main.cpp network.hpp yaml.hpp GUI.hpp)
