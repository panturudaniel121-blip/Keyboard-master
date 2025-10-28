cmake_minimum_required(VERSION 3.16)

# Project setup ---------------------------------------------------------------
set(MAIN_PROJECT_NAME "oop")
project(${MAIN_PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Handle compiler quirks
# ---------------------------------------------------------------------------
# SFML requires Clang on macOS — GCC is not supported
if(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message(WARNING "GCC detected on macOS — switching to AppleClang for SFML compatibility")
    set(CMAKE_C_COMPILER "/usr/bin/clang" CACHE STRING "C compiler" FORCE)
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE STRING "C++ compiler" FORCE)
endif()

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

# Use a stable tag of SFML 3.x — works on macOS and Linux
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.0.2
        GIT_SHALLOW    1
)

# Allow build to succeed even if X11 libs are missing (headless CI)
set(SFML_BUILD_WINDOW TRUE)
set(SFML_BUILD_GRAPHICS TRUE)
set(SFML_BUILD_AUDIO TRUE)
set(SFML_BUILD_NETWORK TRUE)
set(SFML_BUILD_SYSTEM TRUE)

# Detect Linux without X11
if(UNIX AND NOT APPLE)
    include(CheckIncludeFile)
    check_include_file("X11/Xlib.h" HAVE_X11)
    if(NOT HAVE_X11)
        message(WARNING "X11 headers not found — building SFML without window/graphics")
        set(SFML_BUILD_WINDOW OFF)
        set(SFML_BUILD_GRAPHICS OFF)
    endif()
endif()

FetchContent_MakeAvailable(SFML)

find_package(Threads REQUIRED)

# ---------------------------------------------------------------------------
# Executable
# ---------------------------------------------------------------------------
add_executable(${MAIN_PROJECT_NAME}
        main.cpp
        header/Cuvant.cpp
        header/Cuvant.hpp
        header/Scor.cpp
        header/Scor.hpp
)

target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE
        SFML::System
        Threads::Threads
)

if(SFML_BUILD_WINDOW)
    target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE SFML::Window)
endif()
if(SFML_BUILD_GRAPHICS)
    target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE SFML::Graphics)
endif()
if(SFML_BUILD_AUDIO)
    target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE SFML::Audio)
endif()
if(SFML_BUILD_NETWORK)
    target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE SFML::Network)
endif()

target_include_directories(${MAIN_PROJECT_NAME} SYSTEM PRIVATE ${SFML_SOURCE_DIR}/include)

# ---------------------------------------------------------------------------
# Install rules
# ---------------------------------------------------------------------------
install(TARGETS ${MAIN_PROJECT_NAME} DESTINATION bin)

# Install launcher only if present (prevents CI errors)
if(APPLE AND EXISTS "${CMAKE_SOURCE_DIR}/launcher.command")
    install(FILES launcher.command DESTINATION bin)
endif()

# Copy assets if they exist
if(EXISTS "${CMAKE_SOURCE_DIR}/fonts")
    install(DIRECTORY fonts DESTINATION bin)
endif()
if(EXISTS "${CMAKE_SOURCE_DIR}/date")
    install(DIRECTORY date DESTINATION bin)
endif()

# ---------------------------------------------------------------------------
# Output summary
# ---------------------------------------------------------------------------
message(STATUS "------------------------------------------------------------")
message(STATUS "  Project Name     : ${PROJECT_NAME}")
message(STATUS "  C++ Standard     : ${CMAKE_CXX_STANDARD}")
message(STATUS "  Using SFML       : ${SFML_SOURCE_DIR}")
message(STATUS "  SFML Window/Graphics Enabled : ${SFML_BUILD_WINDOW}")
message(STATUS "  Compiler          : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "------------------------------------------------------------")
