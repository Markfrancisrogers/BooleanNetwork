set(example_SOURCES
	attractor_basics.cpp
	sample_attractors.cpp
	sample_boa_sizes.cpp
)

foreach(example_file ${example_SOURCES})
	string(REGEX REPLACE ".cpp$" "" example_name ${example_file})
	add_executable(${example_name} ${example_file})
	target_link_libraries(${example_name} bn-toolkit)
endforeach(example_file ${example_SOURCES})
