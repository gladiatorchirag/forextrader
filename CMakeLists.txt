cmake_minimum_required(VERSION 2.8)

project(ForexTrader)

set( SOURCES
   main.cpp
   order.cpp
   buyorder.cpp
   sellorder.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
