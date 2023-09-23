# Install script for directory: /Users/spencer/Projects/pksav-sv/include/pksav/gen2

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pksav/gen2" TYPE FILE FILES
    "/Users/spencer/Projects/pksav-sv/include/pksav/gen2/badges.h"
    "/Users/spencer/Projects/pksav-sv/include/pksav/gen2/common.h"
    "/Users/spencer/Projects/pksav-sv/include/pksav/gen2/daycare_data.h"
    "/Users/spencer/Projects/pksav-sv/include/pksav/gen2/items.h"
    "/Users/spencer/Projects/pksav-sv/include/pksav/gen2/mom_money_policy.h"
    "/Users/spencer/Projects/pksav-sv/include/pksav/gen2/options.h"
    "/Users/spencer/Projects/pksav-sv/include/pksav/gen2/palette.h"
    "/Users/spencer/Projects/pksav-sv/include/pksav/gen2/pokemon.h"
    "/Users/spencer/Projects/pksav-sv/include/pksav/gen2/save.h"
    "/Users/spencer/Projects/pksav-sv/include/pksav/gen2/text.h"
    "/Users/spencer/Projects/pksav-sv/include/pksav/gen2/time.h"
    )
endif()

