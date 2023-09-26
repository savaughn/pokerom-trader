# Install script for directory: /Users/spencer/Projects/pokerom-trader/deps/pksav/lib

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

if(CMAKE_INSTALL_COMPONENT STREQUAL "Libraries" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/spencer/Projects/pokerom-trader/deps/pksav/lib/libpksav.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpksav.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpksav.dylib")
    execute_process(COMMAND "/usr/bin/install_name_tool"
      -id "libpksav.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpksav.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpksav.dylib")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Libraries" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/Users/spencer/Projects/pokerom-trader/deps/pksav/lib/pksav.pc")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/spencer/Projects/pokerom-trader/deps/pksav/lib/gen1/cmake_install.cmake")
  include("/Users/spencer/Projects/pokerom-trader/deps/pksav/lib/gen2/cmake_install.cmake")
  include("/Users/spencer/Projects/pokerom-trader/deps/pksav/lib/gba/cmake_install.cmake")
  include("/Users/spencer/Projects/pokerom-trader/deps/pksav/lib/gen4/cmake_install.cmake")
  include("/Users/spencer/Projects/pokerom-trader/deps/pksav/lib/gen5/cmake_install.cmake")
  include("/Users/spencer/Projects/pokerom-trader/deps/pksav/lib/common/cmake_install.cmake")
  include("/Users/spencer/Projects/pokerom-trader/deps/pksav/lib/crypto/cmake_install.cmake")
  include("/Users/spencer/Projects/pokerom-trader/deps/pksav/lib/math/cmake_install.cmake")
  include("/Users/spencer/Projects/pokerom-trader/deps/pksav/lib/util/cmake_install.cmake")

endif()

