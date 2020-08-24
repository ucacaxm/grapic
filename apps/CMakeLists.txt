function(grapic_add_executable _NAME)
    add_executable(${_NAME})

    foreach (SOURCE_FILE IN LISTS ARGN)
        target_sources(${_NAME} PRIVATE ${SOURCE_FILE})
    endforeach ()

    target_link_libraries(${_NAME} PRIVATE Grapic)

    message("[GRAPIC] Configured target ${_NAME}")
endfunction()

# start
grapic_add_executable(start start/main_start.cpp)

# LIFAP1
grapic_add_executable(LIFAP1 LIFAP1/main_LIFAP1_TP3.cpp)

# Tetris
grapic_add_executable(tetris demo_tetris/main_tetris.cpp)

# Pacman
grapic_add_executable(pacman demo_pacman/Jeu.cpp demo_pacman/Terrain.cpp demo_pacman/Pacman.cpp demo_pacman/main_pacman.cpp)

# Minesweeper
grapic_add_executable(minesweeper demo_minesweeper/main_minesweeper.cpp)

# Checkers
grapic_add_executable(checkers demo_checkers/main_checkers.cpp)


#[[
You can add your own projects below. To do so, follow the examples above and a CMake target will be created for you
]]
