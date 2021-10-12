cmake_minimum_required(VERSION 3.2.2)

PROJECT(Grapic)

############################## OPTIONS
SET( CMAKE_BUILD_TYPE Debug)			# used only by make
#SET( CMAKE_BUILD_TYPE RelWithDebInfo)
#SET( CMAKE_BUILD_TYPE Release)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-comment -Wno-unused-but-set-variable -Wno-narrowing")

SET(GRAPIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
SET(INCLUDE_DIR "${GRAPIC_DIR}/extern/mingw-cb20/include;${GRAPIC_DIR}/extern/mingw-cb20/include/SDL2;${GRAPIC_DIR}/src")
SET(LIBRARIES_DIR "${GRAPIC_DIR}/extern/mingw-cb20/lib")


if(UNIX)
	SET(LIBS "SDL2main;SDL2;SDL2_image;SDL2_ttf")
ELSE(UNIX)
	SET(LIBS "mingw32;SDL2main;SDL2;SDL2_image;SDL2_ttf")
	link_directories(${LIBRARIES_DIR})
endif(UNIX)


include_directories(${INCLUDE_DIR})
MESSAGE("CURRENT_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../bin")

set(GRAPIC_FILES ${GRAPIC_DIR}/src/Grapic.cpp;${GRAPIC_DIR}/src/Grapic.h )


FUNCTION(GRAPIC_ADD_EXEC MY_NAME MY_FILES)
		#MESSAGE( "FILES=${MY_FILES}" )
        ADD_EXECUTABLE(  ${MY_NAME} "${MY_FILES};${GRAPIC_FILES}" )
        TARGET_LINK_LIBRARIES( ${MY_NAME}  	${LIBS} )
        #SET_TARGET_PROPERTIES(${MY_NAME} PROPERTIES DEBUG_POSTFIX _d)
        SET_TARGET_PROPERTIES(${MY_NAME} PROPERTIES COMPILE_FLAGS -D_CRT_SECURE_NO_WARNINGS)
        SET_TARGET_PROPERTIES(${MY_NAME} PROPERTIES PROJECT_LABEL _app_${MY_NAME})
        TARGET_COMPILE_FEATURES(${MY_NAME} PRIVATE cxx_std_17)
        SET(TARGET ${TARGET}  ${MY_NAME})
        set_property(TARGET ${TARGET} PROPERTY IMPORTED_IMPLIB_RELEASE "${GRAPIC_DIR}/extern/mingw-cb20/lib/SDL2.lib")
        set_property(TARGET ${TARGET} PROPERTY IMPORTED_LOCATION_RELEASE "${GRAPIC_DIR}/extern/mingw-cb20/lib/SDL2.dll")
        MESSAGE(STATUS "Building app ${MY_NAME}")
ENDFUNCTION(GRAPIC_ADD_EXEC)

grapic_add_exec( start 					"${GRAPIC_DIR}/apps/start/main_start.cpp" )
grapic_add_exec( LIFAP1 				"${GRAPIC_DIR}/apps/LIFAP1/LIFAP1_TP3.cpp" )
grapic_add_exec( "tuto1_Simple" 		"${GRAPIC_DIR}/apps/tutorials/tuto1_Simple.cpp" )
grapic_add_exec( "tuto2_FirstLoop" 		"${GRAPIC_DIR}/apps/tutorials/tuto2_FirstLoop.cpp" )
grapic_add_exec( "tuto3_Drawing" 		"${GRAPIC_DIR}/apps/tutorials/tuto3_Drawing.cpp" )
grapic_add_exec( "tuto4_Keyboard"	 	"${GRAPIC_DIR}/apps/tutorials/tuto4_Keyboard.cpp" )
grapic_add_exec( "tuto5_Mouse" 			"${GRAPIC_DIR}/apps/tutorials/tuto5_Mouse.cpp" )
grapic_add_exec( "tuto6_Menu" 			"${GRAPIC_DIR}/apps/tutorials/tuto6_Menu.cpp" )
grapic_add_exec( "tuto7_Animation" 		"${GRAPIC_DIR}/apps/tutorials/tuto7_Animation.cpp" )
grapic_add_exec( "tuto8_Plot"			"${GRAPIC_DIR}/apps/tutorials/tuto8_Plot.cpp" )
grapic_add_exec( "tuto9_Image"			"${GRAPIC_DIR}/apps/tutorials/tuto9_Image.cpp" )
grapic_add_exec( "tuto10_demo"			"${GRAPIC_DIR}/apps/tutorials/tuto10_Demo.cpp" )
