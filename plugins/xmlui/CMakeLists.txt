cmake_minimum_required(VERSION 3.5)
project(xmlui)

set(SOURCES xmlui.cpp)

add_library(xmlui SHARED ${SOURCES})
target_link_libraries(xmlui sampgdk)
target_include_directories(xmlui PRIVATE ${SAMPGDK_INCLUDE_DIRS})
set_target_properties(xmlui PROPERTIES PREFIX "" SUFFIX ".so")