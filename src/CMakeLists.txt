# Specify target sources and parameters
target_sources(${GRAPIC_LIB} PRIVATE Grapic.cpp)

# Add a DEBUG definition for use with '#ifdef DEBUG' in the source code
target_compile_definitions(${GRAPIC_LIB} PUBLIC DEBUG)

# Add compiler flags
target_compile_options(${GRAPIC_LIB} PUBLIC
        -W -Wall -Wextra
        -Wno-sign-compare -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable
        -Wno-comment -Wno-unused-but-set-variable -Wno-narrowing
        )

target_link_libraries(${GRAPIC_LIB} PUBLIC      # 'PUBLIC' to allow targets linking against Grapic to use linked libraries
        SDL2
        SDL2_image
        SDL2_ttf
        $<$<PLATFORM_ID:Windows>:SDL2main>      # Link against SDL2main only on windows
        )

target_include_directories(${GRAPIC_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})