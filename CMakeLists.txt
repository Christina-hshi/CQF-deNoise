cmake_minimum_required(VERSION 3.0.2)

set(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_COMPILER "g++")

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

option(SSE4_2 "Whether it is SSE4.2 architecture." OFF)

if(SSE4_2)
	set(ARCH "-msse4.2 -D__SSE4_2_")
else()
	set(ARCH "")
endif()

SET(CMAKE_FLAGS "-std=gnu++11")
#SET(CMAKE_EXE_LINKER_FLAGS "-Ofast")
#set(COMPILER_FLAGS "-Wall -Ofast -m64 -Wno-unused-result -Wno-strict-aliasing -Wno-unused-function -Wno-sign-compare")
set(COMPILER_FLAGS "-Wall -m64 -Wno-unused-result -Wno-strict-aliasing -Wno-unused-function -Wno-sign-compare ${ARCH}")

#set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS} -pg -pthread -g")
set(CMAKE_CXX_FLAGS_DEBUG "${COMPILER_FLAGS} -g -pg")
set(CMAKE_CXX_FLAGS_RELEASE "${COMPILER_FLAGS} -Ofast")

#set(CMAKE_C_FLAGS "-std=gnu11 ${COMPILER_FLAGS} -g -pg")
set(CMAKE_C_FLAGS_DEBUG "${COMPILER_FLAGS} -g -pg")
set(CMAKE_C_FLAGS_RELEASE "${COMPILER_FLAGS} -Ofast")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_subdirectory(base)
add_subdirectory(core)
add_subdirectory(src)

