add_rules("mode.debug", "mode.release")

add_requires("libsdl", "libsdl_ttf", "libsdl_image")
add_packages("libsdl", "libsdl_ttf", "libsdl_image")

set_languages("c++17")
add_includedirs("src")


-- Library
target("grapic")
	set_kind("static")	
	add_files("src/*.cpp")

-- Tutorials
target("tuto1_Simple")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/tutorials/tuto1_Simple.cpp")

target("tuto2_FirstLoop")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/tutorials/tuto2_FirstLoop.cpp")

target("tuto3_Drawing")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/tutorials/tuto3_Drawing.cpp")

target("tuto4_Keyboard")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/tutorials/tuto4_Keyboard.cpp")

target("tuto5_Mouse")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/tutorials/tuto5_Mouse.cpp")

target("tuto6_Menu")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/tutorials/tuto6_Menu.cpp")

target("tuto7_Animation")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/tutorials/tuto7_Animation.cpp")

target("tuto8_Plot")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/tutorials/tuto8_Plot.cpp")

target("tuto9_Image")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/tutorials/tuto9_Image.cpp")

target("tuto10_Demo")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/tutorials/tuto10_Demo.cpp")

-- LIFAPI
target("LIFAPI_TP")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/LIFAP1/LIFAPI_TP.cpp")

target("LIFAPI_TP3")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/LIFAP1/LIFAP1_TP3.cpp")

-- Demo
target("Checkers")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/demo_checkers/main_checkers.cpp")

target("MineSweeper")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/demo_minesweeper/main_minesweeper.cpp")

target("Pacman")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/demo_pacman/*.cpp")

target("Tetris")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/demo_tetris/main_tetris.cpp")

-- Main
target("start")
	add_deps("grapic")
	set_kind("binary")
	add_files("apps/start/main_start.cpp")
