#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME "BuildingExplosion3")
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Build output path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# Define sources and executable
set(EXECUTABLE_NAME ${PROJECT_NAME})

set(SOURCE_FILES
        src/main.cpp
        src/Cube.h
        src/Cube.cpp
        src/CubeRenderer.h
        src/CubeRenderer.cpp
        src/shader.h
        src/shader.cpp
        dependencies/jsoncpp.cpp
        dependencies/json/json.h
        dependencies/json/json-forwards.h
        src/FloorRenderer.h
        src/SkyboxRenderer.h
        src/SkyboxRenderer.cpp
        src/WindowSize.h
        src/Maths/Vector3f.cpp
        src/Maths/Vector3f.h
        src/Maths/VectorMaths.h
        src/Utils/CachedArray.h
        src/Physics/AosParticleSystem.h
        src/Physics/AosParticleSystem.inl
        src/Physics/ParticleInfo.h
        src/Physics/ParticleSystem.h
        src/Physics/ParticleSystem.inl
        src/GameObjects/Block.cpp
        src/GameObjects/Block.h
        src/GameObjects/GameSystem.cpp
        src/GameObjects/GameSystem.h
        src/GameObjects/Bomb.cpp
        src/GameObjects/Bomb.h
        src/GameObjects/Explosion.cpp
        src/GameObjects/Explosion.h
        src/GameObjects/Camera.h
        src/GameObjects/Camera.cpp
        src/GameParameters.cpp
        src/GameParameters.h
        src/GameObjects/Line.h
        src/GameObjects/Line.cpp
        )

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

# Enable debug symbols by default
if (CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif ()

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif ()

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VERSION_MAJOR "${myproject_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${myproject_VERSION_MINOR}")
include(CPack)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
if (OPENGL_FOUND)
    target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
    # or: target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})
    target_link_libraries(${EXECUTABLE_NAME} m)  # if you use maths.h
endif ()

# GLEW
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(${EXECUTABLE_NAME} ${GLEW_LIBRARIES})
endif ()
