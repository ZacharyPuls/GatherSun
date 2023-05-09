cmake_minimum_required(VERSION 3.25)

set(APPLICATION_NAME "Gather Sun")
set(APPLICATION_VERSION "0.0.1")
set(APPLICATION_AUTHOR "Zachary Puls")

string(TOLOWER ${APPLICATION_NAME} APPLICATION_SLUG)
string(REPLACE " " "-" APPLICATION_SLUG ${APPLICATION_SLUG})

project(${APPLICATION_SLUG} VERSION ${APPLICATION_VERSION})

set(APPLICATION_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(APPLICATION_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(APPLICATION_VERSION_PATCH ${PROJECT_VERSION_PATCH})

configure_file(${PROJECT_SOURCE_DIR}/src/config.h.in ${PROJECT_BINARY_DIR}/config.h)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h)
add_executable(${APPLICATION_SLUG} ${SOURCE_FILES})
target_include_directories(${APPLICATION_SLUG} PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/src)

# GLAD CMake project requires python3 + jinja2
find_package(Python3 COMPONENTS Interpreter jinja2)
add_subdirectory(lib/glad/cmake)
glad_add_library(glad_glcore_46 API gl:core=4.6)

set(GLFW_INSTALL OFF)
add_subdirectory(lib/glfw)

add_subdirectory(lib/glm)

add_subdirectory(lib/entt)

target_link_libraries(${APPLICATION_SLUG} PRIVATE glad_glcore_46 glfw glm EnTT::EnTT)

target_include_directories(${APPLICATION_SLUG} PRIVATE lib/stb)

target_include_directories(${APPLICATION_SLUG} PRIVATE lib/json/include)

target_precompile_headers(${APPLICATION_SLUG} PRIVATE src/GatherSun.h)

add_custom_command(TARGET ${APPLICATION_SLUG} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${APPLICATION_SLUG}>/res)
