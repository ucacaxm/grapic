
newoption {
   trigger     = "lifami",
   description = "activate creation of projects for LIFAMI",
}

newoption {
   trigger     = "lifami-beta",
   description = "activate creation of projects for LIFAMI-BETA",
}

function dump(o)
	if type(o) == 'table' then
	   local s = '{ '
	   for k,v in pairs(o) do
		  if type(k) ~= 'number' then k = '"'..k..'"' end
		  s = s .. '['..k..'] = ' .. dump(v) .. ','
	   end
	   return s .. '} '
	else
	   return tostring(o)
	end
end
 
function ostarget()
	-- print("Options:", dump(_OPTIONS))
	-- print("target:"..os.target())
	-- -- print(os.get())
	-- print("ceci=".._OPTIONS["os"])
	if _PREMAKE_VERSION >="5.0" then
		-- print("pre5 return ")
		-- print("pre5 return ".._OPTIONS["os"])
		-- print("pre5 return "..os.host())
		return os.target()
		-- return _OPTIONS["os"]
	else
		-- print("pre4 return "..os.get())
		return os.get() --_OPTIONS["os"]
	end
end



solution "grapic"

	if grapic_run_only_config then
		grapic_dir = path.getabsolute(".")
	else
		grapic_dir = "."
	end
	objdir = grapic_dir.."/obj"
	includedirs { grapic_dir.."/.", grapic_dir .. "/src" }

	configurations { "Grapic" }

	if not _ACTION then
		_ACTION="none"
	end

	print("Options:", dump(_OPTIONS))
	print("Action:", dump(_ACTION))


	location (grapic_dir .. "/build/" .. ostarget().."-".._ACTION)


	filter  { "system:windows" }
		defines { "WIN32", "NVWIDGETS_EXPORTS", "_USE_MATH_DEFINES", "_CRT_SECURE_NO_WARNINGS" }
		defines { "NOMINMAX" } -- allow std::min() and std::max() in vc++


	if _ACTION=="codeblocks" then
		require "script/premake-codeblocks/codeblocks"
	end


	if _ACTION=="vscode" then
		require "script/premake-vscode/vscode"
	end

	
	-- ##################### Windows: CB20 and gmake
	filter  { "system:windows", "action:codeblocks"}
		buildoptions { "-std=c++17" }
		buildoptions { "-g"}
		linkoptions { "-g"}
		buildoptions { "-W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-comment -Wno-unused-but-set-variable -Wno-narrowing" }
		includedirs { "extern/mingw-cb20/include", "extern/mingw-cb20/include/SDL2" }
		libdirs { grapic_dir .. "/extern/mingw-cb20/lib", grapic_dir .. "/extern/mingw-cb20/bin" }
		links { "mingw32", "SDL2main", "SDL2", "SDL2_image", "SDL2_ttf" }
	filter  { "system:windows", "action:gmake"}
		buildoptions { "-std=c++17" }
		buildoptions { "-g"}
		linkoptions { "-g"}
		buildoptions { "-W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-comment -Wno-unused-but-set-variable -Wno-narrowing" }
		includedirs { "extern/mingw-cb20/include", "extern/mingw-cb20/include/SDL2" }
		libdirs { grapic_dir .. "/extern/mingw-cb20/lib", grapic_dir .. "/extern/mingw-cb20/bin" }
		links { "mingw32", "SDL2main", "SDL2", "SDL2_image", "SDL2_ttf" }


	-- ##################### Linux
	filter  { "system:linux" }
		includedirs { "/usr/include/SDL2" }
		buildoptions { "-std=c++17" }
		buildoptions { "-ggdb"}
		linkoptions { "-ggdb"}
		buildoptions { "-W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-comment -Wno-unused-but-set-variable -Wno-narrowing" }
		links { "SDL2", "SDL2_image", "SDL2_ttf" }

	-- ##################### Windows VS2022
	filter  { "system:windows", "vs2022"}
		system "Windows"
		architecture "x64"
		includedirs { "extern/visual2023/include", "extern/visual2023/include/SDL2" }
		libdirs { grapic_dir .. "/extern/visual2023/lib" }
		links { "SDL2", "SDL2main", "SDL2_image", "SDL2_ttf"}


	-- ##################### MACOS
	filter  { "system:macosx" }
		buildoptions { "-W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-comment -Wno-narrowing" }
		buildoptions { "-std=c++17" }
		xcodebuildsettings {  ["ALWAYS_SEARCH_USER_PATHS"] = "YES" }
		includedirs {	grapic_dir .. "extern/macosx/**",
						grapic_dir .. "../../extern/macosx/**",
					 }
		-- externalincludedirs {	grapic_dir .. "extern/macosx/**",
		-- 						grapic_dir .. "../../extern/macosx/**"
	 	-- 					} 
		linkoptions {	"../../extern/macosx/SDL2.framework/Versions/A/SDL2",
						"../../extern/macosx/SDL2_image.framework/Versions/A/SDL2_image",
						"../../extern/macosx/SDL2_ttf.framework/Versions/A/SDL2_ttf",
						"-rpath @executable_path/../extern/macosx"
					}



function file_exists(name)
   local f=io.open(name,"r")
   if f~=nil then
      io.close(f)
      return true
   else
      print("File "..name.." does not exit => creation")
      local f=io.open(name,"w")
	  io.output(f)
	  io.write("// "..name)
	  if (string.find(name,".cpp")~=nil) then
	     io.write("\
#include<Grapic.h>\
\
using namespace grapic;\
\
\
int main(int , char**)\
{\
	winInit(\"vide\",500,500);\
	backgroundColor(120,70,200);\
	bool stop = false;\
	while( !stop )\
	{\
		winClear();\
		color(220,70,100);\
		rectangleFill(200,200, 300, 300);\
		stop = winDisplay();\
	}\
	winQuit();\
	return 0;\
}\
")
      end
	  io.close(f)
      return false
   end
end

-- description des fichiers communs
function make_project(name, _filesToCompile)
	filesToCompile = _filesToCompile
	--print("PROJECT="..name)
	if type(_filesToCompile)~="table" then
		filesToCompile = { _filesToCompile }
	end
	for i,name in ipairs(filesToCompile) do
		--print("NAME="..name)
		file_exists(name)
	end

	project(name)
		language "C++"
		kind "ConsoleApp"
		targetdir "bin"
		files { grapic_dir.."/src/Grapic.cpp",  grapic_dir.."/src/Grapic.h", }
		files { filesToCompile }
end


-- quand ce premake5.lua est inclus par un autre premake qui definit no_project=true (donc quand grapic est utilisé comme une lib),
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
make_project( "tuto9_Image",		"apps/tutorials/tuto9_Image.cpp" )
make_project( "tuto10_Demo",		"apps/tutorials/tuto10_Demo.cpp" )

-- demo
make_project( "demo_Minesweeper", 	"apps/demo_minesweeper/main_minesweeper.cpp" )
make_project( "demo_Tetris", 	"apps/demo_tetris/main_tetris.cpp" )
make_project( "demo_Checkers",	"apps/demo_checkers/main_checkers.cpp" )
make_project( "demo_Pacman", 	{ "apps/demo_pacman/Jeu.cpp",     "apps/demo_pacman/Terrain.cpp",
								"apps/demo_pacman/Jeu.h",       "apps/demo_pacman/Terrain.h",
								"apps/demo_pacman/Pacman.cpp",  "apps/demo_pacman/main_pacman.cpp",
								"apps/demo_pacman/Pacman.h" } )







if _OPTIONS["lifami"] then
	make_project( "LIFAMI_1_Complex", "apps/LIFAMI/TPComplex.cpp" )
	make_project( "LIFAMI_2_Interpolation", "apps/LIFAMI/TPInterpolation.cpp" )
	make_project( "LIFAMI_3_Particle", "apps/LIFAMI/TPParticle.cpp" )
	make_project( "LIFAMI_4_ParticleSpring", "apps/LIFAMI/TPParticleSpring.cpp" )
	make_project( "LIFAMI_5_GameOfLife", "apps/LIFAMI/TPGameOfLife.cpp" )
	make_project( "LIFAMI_6_WolfRabbit", "apps/LIFAMI/TPWolfRabbit.cpp" )
	make_project( "LIFAMI_7_ColorInsects", "apps/LIFAMI/TPColorInsects.cpp" )
	make_project( "LIFAMI_8_IceScream", "apps/LIFAMI/TPIceScream.cpp" )
end


if _OPTIONS["lifami-beta"] then
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
