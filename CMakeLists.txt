cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(HunterGate)
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.24.3.tar.gz"
    SHA1 "10738b59e539818a01090e64c2d09896247530c7"
)

project(Tiny)

add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE
    main.cpp
)

# https://glad.dav1d.de/
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glad-debug/src/glad.c)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glad-debug/include)
else()
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glad/src/glad.c)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glad/include)
endif()

# https://hunter.readthedocs.io/en/latest/packages/pkg/glm.html
hunter_add_package(glm)
find_package(glm REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)

# https://hunter.readthedocs.io/en/latest/packages/pkg/imgui.html
hunter_add_package(imgui)
find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

# https://hunter.readthedocs.io/en/latest/packages/pkg/SDL2.html
hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)

# https://hunter.readthedocs.io/en/latest/packages/pkg/stb.html
hunter_add_package(stb)
find_package(stb CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC stb::stb)