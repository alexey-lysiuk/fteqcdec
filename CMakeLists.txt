cmake_minimum_required(VERSION 3.0)

project(fteqcdec)

set(COMMON_SOURCES
	cmdlib.h
	comprout.c
	hash.c
	hash.h
	pr_comp.h
	progsint.h
	progslib.h
	progtype.h
	qcc.h
	qcc_cmdlib.c
	qcc_pr_comp.c
	qcc_pr_lex.c
	qccmain.c
	qcd.h
	qcd_main.c
)

add_executable(fteqcc
	${COMMON_SOURCES}
	packager.c
	qcctui.c
)

add_executable(fteqcdec
	${COMMON_SOURCES}
	decomp.c
	gui.h
	qccguistuff.c
	qcdec.c
)

set_target_properties(fteqcc fteqcdec PROPERTIES
	COMPILE_DEFINITIONS "NO_ZLIB"
)

find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
	target_link_libraries(fteqcc PUBLIC ${MATH_LIBRARY})
	target_link_libraries(fteqcdec PUBLIC ${MATH_LIBRARY})
endif()

if(${CMAKE_GENERATOR} STREQUAL Xcode)
	# Disable annoying `warning: implicit conversion loses integer precision: '...' to 'unsigned ...'`
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-shorten-64-to-32")
endif()
