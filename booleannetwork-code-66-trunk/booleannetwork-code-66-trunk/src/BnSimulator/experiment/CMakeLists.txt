set(runner_SOURCES
	BasinRunner.cpp
	DerridaRunner.cpp
	GardenerRunner.cpp
	PerturbativeRunner.cpp
	StateSpaceRunner.cpp
	TrajectoryRunner.cpp
	NetworkDynamics.cpp
	DamianiPlotter.cpp
)

add_library(runner_lib STATIC ${runner_SOURCES}) 
set_target_properties(runner_lib PROPERTIES
	VERSION 1.0
	COMPILE_FLAGS "-fno-rtti"
)
