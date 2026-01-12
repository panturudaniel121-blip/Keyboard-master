cmake_minimum_required(VERSION 3.26)

set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")


project(${MAIN_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

set(SFML_WARNINGS_AS_ERRORS OFF)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        # renovate: datasource=github-tags depName=SFML/SFML versioning=loose
        GIT_TAG        3.0.2
        # renovate: depName=SFML/SFML
        # GIT_TAG        0fa201c969e48ecc253581c5841ce73f44d42f49 # 3.0.2
        GIT_SHALLOW    1  # works only with branches or tags, not with arbitrary commit hashes
)

FetchContent_MakeAvailable(SFML)

find_package(Threads REQUIRED)
if(APPLE)
elseif(UNIX)
    find_package(X11)
endif()

add_executable(${MAIN_EXECUTABLE_NAME}
        main.cpp
        header/Static.cpp
        header/Static.hpp
        header/Exceptii.hpp
        header/Buton.cpp
        header/Buton.hpp
        header/Cuvant.cpp
        header/Cuvant.hpp
        header/Scor.cpp
        header/Scor.hpp
        header/Joc.hpp
        header/Joc.cpp
        header/CuvantSpecial.cpp
        header/CuvantSpecial.hpp
        header/Efecte.cpp
        header/Efecte.hpp
        header/Achievements.cpp
        header/Achievements.hpp
        header/Shop.cpp
        header/Shop.hpp


)

target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ${SFML_SOURCE_DIR}/include)
target_link_directories(${MAIN_EXECUTABLE_NAME} PRIVATE ${SFML_BINARY_DIR}/lib)
target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio Threads::Threads )

if(APPLE)
elseif(UNIX)
    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
endif()

set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ${SFML_SOURCE_DIR}/include)
target_link_directories(${MAIN_EXECUTABLE_NAME} PRIVATE ${SFML_BINARY_DIR}/lib)
target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio Threads::Threads)

if(APPLE)
elseif(UNIX)
    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
endif()

install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()


copy_files(DIRECTORY fonts COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
copy_files(DIRECTORY Date COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
copy_files(DIRECTORY Sunet COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
copy_files(DIRECTORY Imagini COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
set_target_properties(${MAIN_EXECUTABLE_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)