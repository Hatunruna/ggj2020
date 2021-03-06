cmake_minimum_required(VERSION 3.10)

project(PAX_ET_MORS
  VERSION 0.1.0
)

include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")

find_package(gf REQUIRED COMPONENTS graphics net)

if (UNIX)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SFML2 REQUIRED sfml-audio>=2.1)
elseif (WIN32)
  find_package(SFML 2.1 REQUIRED COMPONENTS audio)
  set(SFML2_LIBRARIES "sfml-audio")
endif ()

set(PEM_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/pax_et_mors" CACHE PATH "data path")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(NOT MSVC)
  add_definitions(-Wall -Wextra -g -pedantic)
else ()
  add_definitions(/DNOMINMAX /W4 /utf-8 /permissive-)
endif()

# client

add_executable(pax_et_mors
  code/client.cc
  code/bits/client/BackgroundEntity.cc
  code/bits/client/LobbyChat.cc
  code/bits/client/GameChat.cc
  code/bits/client/ClientNetwork.cc
  code/bits/client/ConnectionScene.cc
  code/bits/client/CreditsScene.cc
  code/bits/client/GameScene.cc
  code/bits/client/HelpScene.cc
  code/bits/client/IntroScene.cc
  code/bits/client/LobbyScene.cc
  code/bits/client/MenuEntity.cc
  code/bits/client/ResourceManager.cc
  code/bits/client/PlayerInfo.cc
  code/bits/client/RoomScene.cc
  code/bits/client/Scenes.cc
  code/bits/client/ShipEntity.cc
  code/bits/client/SplashScreenEntity.cc
  code/bits/client/SplashScreenScene.cc
  code/bits/client/StarsEntity.cc
  code/bits/client/Singletons.cc
  code/bits/client/ViewAdaptator.cc
  code/bits/client/VoteEntity.cc

  vendor/imgui/imgui.cpp
  vendor/imgui/imgui_demo.cpp
  vendor/imgui/imgui_draw.cpp
  vendor/imgui/imgui_widgets.cpp

  vendor/gf-imgui/imgui_impl_gf.cc
)

target_include_directories(pax_et_mors
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/gf-imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_SOURCE_DIR}/code/bits
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(pax_et_mors
  ${GF_LIBRARIES}
  ${SFML2_LIBRARIES}
)

install(
  TARGETS pax_et_mors
  RUNTIME DESTINATION bin
)

# server

add_executable(pax_et_mors_server
  code/server.cc
  code/bits/server/Crew.cc
  code/bits/server/Deck.cc
  code/bits/server/GameFactory.cc
  code/bits/server/GameInstance.cc
  code/bits/server/PemFactory.cc
  code/bits/server/PemInstance.cc
  code/bits/server/ServerGroup.cc
  code/bits/server/ServerLobby.cc
  code/bits/server/ServerNetwork.cc
  code/bits/server/ServerPlayer.cc
  code/bits/server/ServerRoom.cc
  code/bits/server/Ship.cc
)

target_include_directories(pax_et_mors_server
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_SOURCE_DIR}/code/bits
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(pax_et_mors_server
  ${GF_LIBRARIES}
)

install(
  TARGETS pax_et_mors_server
  RUNTIME DESTINATION bin
)

# data

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/pax_et_mors"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)
