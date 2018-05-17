
solution "grapic"

	--  ugly hack to use clang
	-- premake.gcc.cc  = 'clang'
	-- premake.gcc.cxx = 'clang++'

	configurations { "debug", "release" }
	-- platforms { "native", "x32", "x64"  }
	-- platforms { "x64", "x32"  }
	-- platforms { "native" }

	--~ location ("build/" .. os.get() .. "/" .. _ACTION)

	if grapic_run_only_config then
		grapic_dir = path.getabsolute(".")
	else
		grapic_dir = "."
	end

	
	location (grapic_dir .. "/build/" .. os.get())

	objdir = grapic_dir.."/obj"
	includedirs { grapic_dir.."/.", grapic_dir .. "/src" }


	configuration { "linux" }
		includedirs { "/usr/include/SDL2" }
		buildoptions { "-std=c++11" }
		buildoptions { "-ggdb"}
		linkoptions { "-ggdb"}
		buildoptions { "-W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-comment -Wno-unused-but-set-variable -Wno-narrowing" }
		links { "SDL2", "SDL2_image", "SDL2_ttf" }

		

	configuration { "windows" }
		defines { "WIN32", "NVWIDGETS_EXPORTS", "_USE_MATH_DEFINES", "_CRT_SECURE_NO_WARNINGS" }
		defines { "NOMINMAX" } -- allow std::min() and std::max() in vc++ :(((

	configuration { "windows", "gmake" }
		buildoptions { "-std=c++11" }
		buildoptions { "-W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-comment -Wno-unused-but-set-variable -Wno-narrowing" }
		buildoptions { "-g"}
		linkoptions { "-g"}
		includedirs { grapic_dir .. "/extern/mingw/include", grapic_dir .. "/extern/mingw/include/SDL2" }
		libdirs { grapic_dir .. "/extern/mingw/lib" }
		links { "mingw32", "SDL2main", "SDL2", "SDL2_image", "SDL2_ttf" }

	configuration { "windows", "codeblocks" }
		buildoptions { "-std=c++11" }
		buildoptions { "-g"}
		linkoptions { "-g"}
		buildoptions { "-W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-comment -Wno-unused-but-set-variable -Wno-narrowing" }
		includedirs { "extern/mingw/include", "extern/mingw/include/SDL2" }
		libdirs { grapic_dir .. "/extern/mingw/lib" }
		links { "mingw32", "SDL2main", "SDL2", "SDL2_image", "SDL2_ttf" }

	configuration { "windows", "vs2015"}
		if _PREMAKE_VERSION >="5.0" then
			system "Windows"
			architecture "x64"
		end
		buildoptions { "-g"}
		includedirs { "extern/visual2015/include", "extern/visual2015/include/SDL2" }
		libdirs { grapic_dir .. "/extern/visual2015/lib" }
		links { "SDL2", "SDL2main", "SDL2_image", "SDL2_ttf"}

		
		
	configuration "macosx"
		buildoptions { "-W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-comment -Wno-narrowing" }
		buildoptions { "-std=c++11" }
		includedirs {	"extern/macosx/SDL2.framework/Versions/A/Headers/",
						"extern/macosx/SDL2.framework/Versions/A/Headers/SDL2",
						"extern/macosx/SDL2_image.framework/Versions/A/Headers",
						"extern/macosx/SDL2_ttf.framework/Versions/A/Headers" }
		linkoptions {	"../../extern/macosx/SDL2.framework/Versions/A/SDL2",
						"../../extern/macosx/SDL2_image.framework/Versions/A/SDL2_image",
						"../../extern/macosx/SDL2_ttf.framework/Versions/A/SDL2_ttf",
						"-rpath @executable_path/../extern/macosx"
		}




-- description des fichiers communs
function make_project(name, filesToCompile)
	project(name)
		language "C++"
		kind "ConsoleApp"
		targetdir "bin"
		files { grapic_dir.."/src/Grapic.cpp",  grapic_dir.."/src/Grapic.h", }
		files { filesToCompile }
end


-- quand ce premake4.lua est inclus par un autre premake qui definit no_project=true (donc quand gkit2light est utilisé comme une lib),
-- ceci stoppe la creation des projects suivants (tuto, etc.)
if grapic_run_only_config then
	do return end
end


-- description des projets
make_project( "Start", "apps/start/main_start.cpp" )


-- tuto
make_project( "tuto1_Simple", 		"apps/tutorials/tuto1_Simple.cpp" )
make_project( "tuto2_FirstLoop", 	"apps/tutorials/tuto2_FirstLoop.cpp" )
make_project( "tuto3_Drawing", 		"apps/tutorials/tuto3_Drawing.cpp" )
make_project( "tuto4_Keyboard", 	"apps/tutorials/tuto4_Keyboard.cpp" )
make_project( "tuto5_Mouse", 		"apps/tutorials/tuto5_Mouse.cpp" )
make_project( "tuto6_Menu", 		"apps/tutorials/tuto6_Menu.cpp" )
make_project( "tuto7_Animation", 	"apps/tutorials/tuto7_Animation.cpp" )
make_project( "tuto8_Plot",			"apps/tutorials/tuto8_Plot.cpp" )
make_project( "tuto9_Demo",			"apps/tutorials/tuto9_Demo.cpp" )

-- demo
make_project( "demo_Minesweeper", 	"apps/demo_minesweeper/main_minesweeper.cpp" )
make_project( "demo_Tetris", 	"apps/demo_tetris/main_tetris.cpp" )
make_project( "demo_Checkers",	"apps/demo_checkers/main_checkers.cpp" )
make_project( "demo_Pacman", 	{ "apps/demo_pacman/Jeu.cpp",     "apps/demo_pacman/Terrain.cpp",
								"apps/demo_pacman/Jeu.h",       "apps/demo_pacman/Terrain.h",
								"apps/demo_pacman/Pacman.cpp",  "apps/demo_pacman/main_pacman.cpp",
								"apps/demo_pacman/Pacman.h" } )




newoption {
   trigger     = "target",
   value       = "TGT",
   description = "Choose a particular sub-list of projects",
   allowed = {
      { "standard",	"All project except beta" },
      { "beta",  	"Future project" },
   }
}



if not _OPTIONS["target"] then
   _OPTIONS["target"] = "standard"
end

-- zzz (future projects)
if _OPTIONS["target"] == "beta" then
	make_project( "zzz_Fluid", 								"apps/zzz_fluid/main_fluid.cpp" )
	make_project( "zzz_particles", 							"apps/zzz_particles/main_particles.cpp" )
	make_project( "zzz_lifami_TD1_Complex",					"apps/zzz_lifami/T1_TD1-TP-TP_Complex.cpp" )
	make_project( "zzz_lifami_TD2_Interpolation",			"apps/zzz_lifami/T2_TD2-TP-TP_Interpolation.cpp" )
	make_project( "zzz_lifami_TD3_ParticlesBasic",			"apps/zzz_lifami/T3_TD3-TP-TP_ParticlesBasic.cpp" )
	make_project( "zzz_lifami_TD3_Particles",				"apps/zzz_lifami/T3_TD3-TP-TP_Particles.cpp" )
	make_project( "zzz_lifami_TD4_MassSpring",				"apps/zzz_lifami/T3_TD4-TP_MassSpring.cpp" )
	make_project( "zzz_lifami_TD4_Asteroid",				"apps/zzz_lifami/T3_TD4-TP_Particles.cpp" )
	make_project( "zzz_lifami_TD4-Fluid",					"apps/zzz_lifami/T3_TD4-TP_fluid.cpp" )
	make_project( "zzz_lifami_TD6_TP-TP_ProiesPredateurs",	"apps/zzz_lifami/T4_TD6_TP-TP_ProiesPredateurs.cpp" )
	make_project( "zzz_lifami_TD7-TP-TP_ColorInsects",		"apps/zzz_lifami/T4_TD7-TP-TP_ColorInsects.cpp" )
	make_project( "zzz_lifami_TD8-TP-TP_EcoIcecreamShop",	"apps/zzz_lifami/T5_TD8-TP-TP_EcoIcecreamShop.cpp" )
	make_project( "zzz_lifami_TPnote_Water",				"apps/zzz_lifami/T9_TPnote_Water.cpp" )
	make_project( "zzz_lifami_test",						"apps/zzz_lifami/test.cpp" )
end
