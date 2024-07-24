cmake_minimum_required(VERSION 3.14)

project(raknet-sdk LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES ./*.cpp ./*.h)
add_library(raknet-sdk STATIC ${SHARED_SOURCE_FILES} ${SOURCE_FILES})

# disable warning C4996
target_compile_definitions(raknet-sdk PRIVATE _CRT_SECURE_NO_WARNINGS)

target_include_directories(raknet-sdk PUBLIC ./)
