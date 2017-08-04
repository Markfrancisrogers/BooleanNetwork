set(lab_SOURCES
	lab/AttractorExpression.cpp
	lab/AttractorPattern.cpp
	#lab/ExpandAttractor.cpp
	lab/expression_based_distance.cpp
	lab/factory.cpp
	lab/hamming.cpp
	#lab/PrintAttractor.cpp
	lab/derrida_parameter.cpp
	lab/boolean_analysis.cpp
)
set_source_files_properties(${lab_SOURCES} PROPERTIES
	COMPILE_FLAGS "-fno-rtti"
)

set(rbn_SOURCES
	core/BooleanFunction.cpp
	core/Attractor.cpp
	core/ImmutableBooleanNetwork.cpp
	core/MutableBooleanNetwork.cpp
	core/ControllableBooleanNetwork.cpp
	core/simplification.cpp
	core/bn_factory.cpp
)
set_source_files_properties(${rbn_SOURCES} PROPERTIES
	COMPILE_FLAGS "-fno-rtti"
)

set(runner_SOURCES
	#experiment/BasinRunner.cpp
	#experiment/DerridaRunner.cpp
	#experiment/GardenerRunner.cpp
	#experiment/PerturbativeRunner.cpp
	#experiment/StateSpaceRunner.cpp
	#experiment/TrajectoryRunner.cpp
	experiment/NetworkDynamics.cpp
	experiment/NetworkAttractor.cpp
	experiment/cycle_finder/brent.cpp
	experiment/cycle_finder/naive.cpp
	#experiment/DamianiPlotter.cpp
)
set_source_files_properties(${runner_SOURCES} PROPERTIES
	COMPILE_FLAGS "-fno-rtti"
)

set(gen_SOURCES
	gen/RandomStateGen.cpp
	gen/StateEnumerator.cpp
)
set_source_files_properties(${gen_SOURCES} PROPERTIES
	COMPILE_FLAGS "-fno-rtti"
)

set(util_SOURCES
	util/state_util.cpp
)
set_source_files_properties(${util_SOURCES} PROPERTIES
	COMPILE_FLAGS "-fno-rtti"
)

add_library(bn-toolkit SHARED
	${lab_SOURCES} ${rbn_SOURCES} ${runner_SOURCES} ${gen_SOURCES} ${util_SOURCES}
)
set_target_properties(bn-toolkit PROPERTIES
	LINKER_LANGUAGE CXX
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib
)
