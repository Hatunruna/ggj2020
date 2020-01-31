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
  code/bits/client/Chat.cc
  code/bits/client/ClientNetwork.cc
  code/bits/client/ConnectionScene.cc
  code/bits/client/CreditsScene.cc
  code/bits/client/HelpScene.cc
  code/bits/client/IntroScene.cc
  code/bits/client/LobbyScene.cc
  code/bits/client/MenuEntity.cc
  code/bits/client/RoomScene.cc
  code/bits/client/Scenes.cc
  code/bits/client/WaitingScene.cc

  vendor/imgui/imgui.cpp
  vendor/imgui/imgui_demo.cpp
  vendor/imgui/imgui_draw.cpp
  vendor/imgui/imgui_widgets.cpp

  vendor/gf-imgui/imgui_impl_gf.cc
)

target_include_directories(tmpgame
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/gf-imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_SOURCE_DIR}/code/bits
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
  code/bits/server/GameFactory.cc
  code/bits/server/GameInstance.cc
  code/bits/server/ServerGroup.cc
  code/bits/server/ServerLobby.cc
  code/bits/server/ServerNetwork.cc
  code/bits/server/ServerPlayer.cc
  code/bits/server/ServerRoom.cc
)

target_include_directories(tmpgame-server
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_SOURCE_DIR}/code/bits
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
