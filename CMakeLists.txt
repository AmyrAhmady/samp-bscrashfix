cmake_minimum_required(VERSION 2.8...3.15)
project(bscrashfix LANGUAGES C CXX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if (MSVC)
  add_definitions("/MP")
else()
  set(CMAKE_C_FLAGS "-m32 ${CMAKE_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "-m32 ${CMAKE_CXX_FLAGS}")
endif()

add_subdirectory(deps)

add_library(${PROJECT_NAME} SHARED src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE urmem raknet-sdk subhook)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "bscrashfix" PREFIX "")
