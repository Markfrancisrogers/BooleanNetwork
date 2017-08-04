cmake_minimum_required (VERSION 2.6 FATAL_ERROR)
project(BooleanNetworkToolkit CXX)

add_definitions("-Wall -Wcast-qual")

set(CMAKE_C_FLAGS_PROFILE
	"${CMAKE_C_FLAGS_RELEASE} -pg"
	CACHE STRING "C compiler flags for Profile build"
)
set(CMAKE_CXX_FLAGS_PROFILE
	"${CMAKE_CXX_FLAGS_RELEASE} -pg"
	CACHE STRING "C++ compiler flags for Profile build"
)
set(CMAKE_EXE_LINKER_FLAGS_PROFILE
	"-pg"
	CACHE STRING "Linker flags for Profile build (executeble)"
)
set(CMAKE_SHARED_LINKER_FLAGS_PROFILE
	"-pg"
	CACHE STRING "Linker flags for Profile build (shared lib)"
)
mark_as_advanced(
	CMAKE_C_FLAGS_PROFILE
	CMAKE_CXX_FLAGS_PROFILE
	CMAKE_EXE_LINKER_FLAGS_PROFILE
	CMAKE_SHARED_LINKER_FLAGS_PROFILE
)

set(CMAKE_DEBUG_POSTFIX "-dbg")
set(CMAKE_PROFILE_POSTFIX "-prof")

find_package(Boost 1.40)

include_directories(${Boost_INCLUDE_DIR} include)
add_subdirectory(src)
add_subdirectory(example)
