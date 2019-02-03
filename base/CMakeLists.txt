include_directories(${PROJECT_SOURCE_DIR})

# add the binary tree to the search path for include files
# so that we will find Config.h
include_directories(${PROJECT_BINARY_DIR})

add_library(base-objs OBJECT
	global.cpp
	multithread_io.cpp
	)

# Visible to parent directory
set(base-objs $<TARGET_OBJECTS:base-objs> PARENT_SCOPE)

add_library(base-lib STATIC $<TARGET_OBJECTS:base-objs>)

