cmake_minimum_required(VERSION 3.16)
project(MySFMLApp VERSION 0.1 LANGUAGES CXX)

# Options ---------------------------------------------------------------------
option(BUILD_STATIC "Link SFML statically" ON)
set(SFML_MINIMUM_VERSION 2.5)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_NAME oop)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Force static libs" FORCE)
set(SFML_BUILD_SHARED_LIBS OFF CACHE BOOL "Force static SFML" FORCE)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

include(FetchContent)

# --- Try to find SFML installed on system ------------------------------------
message(STATUS "Checking for system SFML installation...")
find_package(SFML ${SFML_MINIMUM_VERSION} COMPONENTS system window graphics audio network QUIET)

# --- If not found, download and build SFML automatically ---------------------
if(NOT SFML_FOUND)
    message(WARNING "System SFML not found. Fetching and building SFML from source...")

    set(FETCHCONTENT_QUIET OFF)

    FetchContent_Declare(
            SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG        master)
    FetchContent_MakeAvailable(SFML)
endif()

# --- Create your executable ---------------------------------------------------
add_executable(${PROJECT_NAME}
        main.cpp
        header/Cuvant.cpp
        header/Cuvant.hpp
        header/Scor.cpp
        header/Scor.hpp
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# --- Link SFML ---------------------------------------------------------------
if(SFML_FOUND)
    message(STATUS "Using system SFML ${SFML_VERSION}")
    target_link_libraries(${PROJECT_NAME} PRIVATE
            sfml-graphics
            sfml-window
            sfml-system
            sfml-audio
            sfml-network
    )
else()
    message(STATUS "Using locally built SFML from FetchContent")
    target_link_libraries(${PROJECT_NAME} PRIVATE
            sfml-graphics
            sfml-window
            sfml-system
            sfml-audio
            sfml-network
    )
endif()

# --- Optional: static linking configuration ----------------------------------
if(BUILD_STATIC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE SFML_STATIC)
endif()

# --- Include directories -----------------------------------------------------
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- Platform-specific setup -------------------------------------------------
if(WIN32)
    if(MSVC AND BUILD_STATIC)
        foreach(flag_var
                CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_DEBUG
                CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_DEBUG)
            if(${flag_var} MATCHES "/MD")
                string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
            endif()
        endforeach()
    endif()
elseif(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_RPATH ON
    )
elseif(UNIX)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "$ORIGIN"
    )
endif()

# --- Force static runtime linking ---------------------------------
if(MSVC)
    foreach(flag_var
            CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_DEBUG)
        if(${flag_var} MATCHES "/MD")
            string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif()
    endforeach()
elseif(MINGW)
    # Only force static linking of SFML and dependencies, not C++ runtime twice
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -Wl,--allow-multiple-definition")
endif()

# --- Install rules -----------------------------------------------------------
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
if(EXISTS "${CMAKE_SOURCE_DIR}/date")
    install(DIRECTORY Date DESTINATION ${PROJECT_NAME})
endif()
if(EXISTS "${CMAKE_SOURCE_DIR}/fonts")
    install(DIRECTORY fonts DESTINATION ${PROJECT_NAME})
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/date  $<TARGET_FILE_DIR:${PROJECT_NAME}>/date
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/fonts $<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts
)

message(STATUS "------------------------------------------------------------")
message(STATUS "  Project Name     : ${PROJECT_NAME}")
message(STATUS "  Build Type       : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Using SFML       : ${SFML_VERSION}")
message(STATUS "  Static Linking   : ${BUILD_STATIC}")
message(STATUS "------------------------------------------------------------")
