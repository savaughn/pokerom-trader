# Install script for directory: /Users/spencer/Projects/pksav/include/pksav

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pksav" TYPE FILE FILES
    "/Users/spencer/Projects/pksav/include/pksav/error.h"
    "/Users/spencer/Projects/pksav/include/pksav/config.h"
    "/Users/spencer/Projects/pksav/include/pksav/version.h"
    "/Users/spencer/Projects/pksav/include/pksav/gen1.h"
    "/Users/spencer/Projects/pksav/include/pksav/gen2.h"
    "/Users/spencer/Projects/pksav/include/pksav/gba.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/spencer/Projects/pksav/include/pksav/common/cmake_install.cmake")
  include("/Users/spencer/Projects/pksav/include/pksav/math/cmake_install.cmake")
  include("/Users/spencer/Projects/pksav/include/pksav/gen1/cmake_install.cmake")
  include("/Users/spencer/Projects/pksav/include/pksav/gen2/cmake_install.cmake")
  include("/Users/spencer/Projects/pksav/include/pksav/gba/cmake_install.cmake")
  include("/Users/spencer/Projects/pksav/include/pksav/gen4/cmake_install.cmake")
  include("/Users/spencer/Projects/pksav/include/pksav/gen5/cmake_install.cmake")

endif()

