#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)
project(myproject)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Enable debug symbols by default
if (CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif ()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

# Set version information in a config.h file
set(myproject_VERSION_MAJOR 1)
set(myproject_VERSION_MINOR 0)
configure_file(
        "${PROJECT_SOURCE_DIR}/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
)
include_directories("${PROJECT_BINARY_DIR}")

# Define sources and executable
set(EXECUTABLE_NAME "Game")
add_executable(${EXECUTABLE_NAME} src/main.cpp src/Entity.cpp src/Enemies.cpp src/Player.cpp src/Bullet.cpp src/Gui.cpp src/shield.cpp src/Thrust.cpp src/aim.cpp TinyXML/tinyxml.cpp TinyXML/tinystr.cpp TinyXML/tinyxmlerror.cpp TinyXML/tinyxmlparser.cpp src/Asteroid.cpp src/Asteroid.h src/Application.cpp src/Application.h src/Menu/Menu.cpp src/Menu/Menu.h src/Menu/Button.cpp src/Menu/Button.h src/Menu/MyFont.cpp src/Menu/MyFont.h src/GameState.h src/LevelInfo.h src/LevelInfo.cpp)


# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif ()


# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)


# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${myproject_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${myproject_VERSION_MINOR}")
include(CPack)