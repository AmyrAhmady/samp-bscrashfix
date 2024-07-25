add_library(urmem INTERFACE)
target_include_directories(urmem INTERFACE "./urmem")
# file(GLOB_RECURSE urmem_source_list "*.hpp")
# set_property(TARGET urmem PROPERTY SOURCES ${urmem_source_list})

add_subdirectory(raknet)

add_definitions(-DSUBHOOK_STATIC -DSUBHOOK_FORCE_32BIT)
add_subdirectory(subhook)
