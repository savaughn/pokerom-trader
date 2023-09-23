# Install script for directory: /Users/spencer/Projects/pokerom-trader/libpksav/include/pksav/gba

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pksav/gba" TYPE FILE FILES
    "/Users/spencer/Projects/pokerom-trader/libpksav/include/pksav/gba/box_wallpaper.h"
    "/Users/spencer/Projects/pokerom-trader/libpksav/include/pksav/gba/common.h"
    "/Users/spencer/Projects/pokerom-trader/libpksav/include/pksav/gba/daycare.h"
    "/Users/spencer/Projects/pokerom-trader/libpksav/include/pksav/gba/items.h"
    "/Users/spencer/Projects/pokerom-trader/libpksav/include/pksav/gba/language.h"
    "/Users/spencer/Projects/pokerom-trader/libpksav/include/pksav/gba/mail.h"
    "/Users/spencer/Projects/pokerom-trader/libpksav/include/pksav/gba/map.h"
    "/Users/spencer/Projects/pokerom-trader/libpksav/include/pksav/gba/options.h"
    "/Users/spencer/Projects/pokerom-trader/libpksav/include/pksav/gba/pokedex.h"
    "/Users/spencer/Projects/pokerom-trader/libpksav/include/pksav/gba/pokemon.h"
    "/Users/spencer/Projects/pokerom-trader/libpksav/include/pksav/gba/roamer.h"
    "/Users/spencer/Projects/pokerom-trader/libpksav/include/pksav/gba/save.h"
    "/Users/spencer/Projects/pokerom-trader/libpksav/include/pksav/gba/text.h"
    "/Users/spencer/Projects/pokerom-trader/libpksav/include/pksav/gba/time.h"
    )
endif()

