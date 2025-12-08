cmake_minimum_required(VERSION 3.26)

set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")

project(${MAIN_PROJECT_NAME})

# Recomand C++20 pentru SFML 3.0 (mai stabil decat 23 momentan)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include optiunile helper (daca exista)
include(cmake/Options.cmake OPTIONAL)
include(cmake/CompilerFlags.cmake OPTIONAL)
include(cmake/CopyHelper.cmake OPTIONAL)

###############################################################################
# 1. Configurare SFML
###############################################################################

include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(SFML_WARNINGS_AS_ERRORS OFF)
set(SFML_BUILD_EXAMPLES OFF)
set(SFML_BUILD_DOC OFF)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.0.2
        GIT_SHALLOW    1
)

FetchContent_MakeAvailable(SFML)

###############################################################################
# 2. Dependinte Sistem
###############################################################################

find_package(Threads REQUIRED)
if(UNIX AND NOT APPLE)
    find_package(X11)
endif()

###############################################################################
# 3. Executabilul si Sursele
###############################################################################

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
)

# Configurare Include Directories
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ${SFML_SOURCE_DIR}/include)
target_include_directories(${MAIN_EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Configurare Link Libraries (O SINGURA DATA, completa)
target_link_directories(${MAIN_EXECUTABLE_NAME} PRIVATE ${SFML_BINARY_DIR}/lib)

target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
        Threads::Threads
)

if(UNIX AND NOT APPLE)
    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
endif()

# Sanitizers (Optional)
if(COMMAND set_compiler_flags)
    set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})
endif()

###############################################################################
# 4. Copiere Resurse (Copy Files)
###############################################################################

install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

# Copiem folderele necesare langa executabil
if(COMMAND copy_files)
    copy_files(FILES tastatura.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
    copy_files(DIRECTORY fonts COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})

    # Atentie: In codul C++ folosim "Date/..." deci folderul trebuie sa se numeasca "Date" (sau "date")
    copy_files(DIRECTORY date COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})

    # Daca ai redenumit folderul in "Sunet", asigura-te ca in C++ ai schimbat caile
    # Daca fisierele audio sunt in "date", linia de mai jos e inutila, dar nu strica
    # copy_files(DIRECTORY Sunet COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
else()
    # Fallback standard CMake
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fonts DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/date DESTINATION ${CMAKE_BINARY_DIR})
endif()

set_target_properties(${MAIN_EXECUTABLE_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)