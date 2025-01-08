cmake_minimum_required(VERSION 3.10)

# Setup the project and settings
project(pokeromtrader C)
set(PROJECT_VERSION 1.0.1)

# Define the platform
if (WIN32)
    set(PLATFORM WINDOWS)
elseif (UNIX AND NOT APPLE)
    set(PLATFORM LINUX)
elseif (APPLE)
    set(PLATFORM OSX)
endif()

# Fetch and build raylib if not found
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/4.5.0.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    endif()
endif()

# Include directories
include_directories(
    ${raylib_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/deps/pksav/build/include
    ${CMAKE_SOURCE_DIR}/deps/pksav/include
    ${CMAKE_SOURCE_DIR}/include
)

# Link directories
link_directories(
    ${CMAKE_SOURCE_DIR}/deps/pksav/build/lib/Debug
)

# Source files
file(GLOB_RECURSE SOURCES "src/*.c")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} raylib pksav)

# Compiler flags
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
        /DPROJECT_VERSION=\"${PROJECT_VERSION}\" /DPROJECT_VERSION_TYPE=\"prerelease\" /DCI_BUILD=FALSE
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -std=c11 -Wall -Wextra -Wpedantic -Wno-missing-braces -Wunused-result
        -Wformat=2 -Wno-unused-parameter -Wshadow
        -Wwrite-strings -Wstrict-prototypes -Wold-style-definition
        -Wredundant-decls -Wnested-externs -Wmissing-include-dirs -D_DEFAULT_SOURCE
        -DPROJECT_VERSION=\"${PROJECT_VERSION}\" -DPROJECT_VERSION_TYPE=\"prerelease\" -DCI_BUILD=FALSE
    )
endif()

# Linker flags
if (WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:-Wl,--subsystem,windows>
    )
    target_link_libraries(${PROJECT_NAME} opengl32 gdi32 winmm)
elseif (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} GL m pthread dl rt X11)
elseif (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreAudio" "-framework CoreVideo")
endif()

# Install target
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
