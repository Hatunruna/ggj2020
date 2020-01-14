cmake_minimum_required(VERSION 3.10)

project(TMPGAME
  VERSION 0.1.0
)

include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")

find_package(gf REQUIRED)
find_package(gfnet REQUIRED)
# find_package(Box2D REQUIRED)

set(TMPGAME_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/tmpgame")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

if(NOT MSVC)
  add_definitions(-Wall -Wextra -g -O2 -std=c++14 -pedantic)
endif()

# client

add_executable(tmpgame
  code/client.cc
  code/bits/IntroScene.cc
  code/bits/MenuEntity.cc
  code/bits/Scenes.cc
  code/bits/WaitingScene.cc
)

target_include_directories(tmpgame
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(tmpgame
  gf::gf0
  gf::gfnet0
)

install(
  TARGETS tmpgame
  RUNTIME DESTINATION bin
)

# server

add_executable(tmpgame-server
  code/server.cc
)

target_include_directories(tmpgame-server
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(tmpgame-server
  gf::gfcore0
  gf::gfnet0
)

install(
  TARGETS tmpgame-server
  RUNTIME DESTINATION bin
)

# data

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/tmpgame"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)
