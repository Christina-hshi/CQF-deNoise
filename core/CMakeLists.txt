include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

add_library(core-objs OBJECT
	gqf.c
	true2falseKmer_DP.cpp
)	

set(core-objs $<TARGET_OBJECTS:core-objs> PARENT_SCOPE)

add_library(core-lib STATIC $<TARGET_OBJECTS:core-objs>)
