# Install script for directory: /Users/spencer/Projects/pokerom-trader/deps/pksav/include/pksav/common

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pksav/common" TYPE FILE FILES
    "/Users/spencer/Projects/pokerom-trader/deps/pksav/include/pksav/common/condition.h"
    "/Users/spencer/Projects/pokerom-trader/deps/pksav/include/pksav/common/constants.h"
    "/Users/spencer/Projects/pokerom-trader/deps/pksav/include/pksav/common/contest_stats.h"
    "/Users/spencer/Projects/pokerom-trader/deps/pksav/include/pksav/common/datetime.h"
    "/Users/spencer/Projects/pokerom-trader/deps/pksav/include/pksav/common/gen4_encounter_type.h"
    "/Users/spencer/Projects/pokerom-trader/deps/pksav/include/pksav/common/item.h"
    "/Users/spencer/Projects/pokerom-trader/deps/pksav/include/pksav/common/markings.h"
    "/Users/spencer/Projects/pokerom-trader/deps/pksav/include/pksav/common/nature.h"
    "/Users/spencer/Projects/pokerom-trader/deps/pksav/include/pksav/common/nds_pokemon.h"
    "/Users/spencer/Projects/pokerom-trader/deps/pksav/include/pksav/common/pokedex.h"
    "/Users/spencer/Projects/pokerom-trader/deps/pksav/include/pksav/common/pokerus.h"
    "/Users/spencer/Projects/pokerom-trader/deps/pksav/include/pksav/common/stats.h"
    "/Users/spencer/Projects/pokerom-trader/deps/pksav/include/pksav/common/trainer_id.h"
    )
endif()

