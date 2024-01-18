
# should be run from the directory of grapic
GRAPIC_HOME = .

apps_linux = "${MAKECMDGOALS}"


ifeq ($(OS),Windows_NT)
	OS = windows
	PREMAKE5 = $(GRAPIC_HOME)/script/premake5.exe
else
UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	OS = macosx
	OSGMAKE = ${OS}-gmake
	PREMAKE5 = $(GRAPIC_HOME)/script/premake5.macosx
	Filename="${Filename##*/}"
	apps_linux = $(shell ls build/${OSGMAKE}/*.make 2>/dev/null | cut -d '/' -f 3 | cut -d . -f 1  )				# ls build/macosx/*.make | cut -d '/' -f 3 | cut -d . -f 1   OR basename
else
	OS = linux
	OSGMAKE = ${OS}-gmake
	PREMAKE5 = $(GRAPIC_HOME)/script/premake5.linux
	apps_linux = $(shell ls build/${OSGMAKE}/*.make 2>/dev/null | cut -d '/' -f 3 | cut -d . -f 1 ) 				# basename --suffix=.make build/linux/*.make  )
endif
endif


#$(info $(apps_linux))
#ifeq ($(wildcard build/*),)		# if the premake was never runned, apps_linux contains "*" => remove it
ifeq ($(apps_linux),*)		# if the premake was never runned, apps_linux contains "*" => remove it
	apps_linux = ""
endif
# TO DEBUG apps_linux
#$(info (DEBUG) apps_linux=$(apps_linux))




all: build/${OSGMAKE} remove_quarantine
	cd build/${OSGMAKE} ; make

dir:
	mkdir -p bin
	
remove_quarantine:
ifeq ($(UNAME_S),Darwin)
	xattr -rd com.apple.quarantine ./*
endif
	
clean: build/${OSGMAKE}
	rm -rf bin/*.exe ; cd build/${OSGMAKE} ; make clean

doc-clean:
	rm -rf doc/html doc/xml

premake-clean: doc-clean
	rm -rf build ; rm -rf bin/* ; rm -rf obj ; chmod 755 script/premake*

build/${OSGMAKE}: premake5.lua premake

doc-gen: $(GRAPIC_HOME)/doc $(GRAPIC_HOME)/doc/images $(GRAPIC_HOME)/src FORCE
	cd doc ; doxygen

dos2unix:
	dos2unix -q script/*.sh doc/VERSION ; chmod 755 script/*.sh

version: FORCE
	sh $(GRAPIC_HOME)/script/inc_version.sh

web: doc-gen web-force

web-force:
	@echo "============================== WEB ================================="
	@echo "Continue?  [y/N]"
	@( read -p "\n" sure && \
		case "$$sure" in [yY]) \
			rsync -ravuz --delete $(GRAPIC_HOME)/doc/ ameyer@connect.liris.cnrs.fr:/home-membres/ameyer/grapic/;; \
		*) echo "stop";; \
		esac )

premake-all: premake-wincb20 premake-linux premake-macosx premake-winvs2019
	
premake-exec:
	chmod 755 script/premake*

premake-wincb20: remove_quarantine
	@echo "premake CB20 OS=$(OS)"
	cp extern/mingw-cb20/bin/*.dll bin
	$(PREMAKE5) --os=windows codeblocks
	$(PREMAKE5) --os=windows gmake

premake-winvs2019: remove_quarantine
	@echo "premake vs2019 OS=$(OS)"
	cp extern/visual2019/bin/*.dll bin
	$(PREMAKE5)  --os=windows vs2019
		
premake-linux: premake-exec remove_quarantine
	@echo "premake linux OS=$(OS)"
	$(PREMAKE5) --os=linux gmake
	$(PREMAKE5) --os=linux codeblocks

premake-macosx: premake-exec
	@echo "premake macosx OS=$(OS)"
	$(PREMAKE5) --os=macosx gmake
	$(PREMAKE5) --os=macosx xcode4

	
ifeq ($(OS),Windows_NT)
premake: remove_quarantine premake-cb20 dir
	@echo "premake Win OS=$(OS)"
else ifeq ($(OS),linux)
premake: remove_quarantine premake-linux dir
	@echo "premake linux OS=$(OS)"
else ifeq ($(OS),macosx)
premake: remove_quarantine premake-macosx dir
	@echo "premake macosx OS=$(OS)"
else
	@echo "ERROR: Your OS is not detected in the makefile"
endif

premake-beta: remove_quarantine
	rm -rf build ; chmod 755 script/premake*
	@echo "OS=$(OS)"
ifeq ($(OS),Windows_NT)
	$(PREMAKE5) --target=beta --os=windows codeblocks
else ifeq ($(OS),linux)
	$(PREMAKE5) --target=beta --os=linux gmake
	$(PREMAKE5) --target=beta --os=linux codeblocks
else ifeq ($(OS),macosx)
	$(PREMAKE5) --target=beta --os=macosx gmake
	$(PREMAKE5) --target=beta --os=macosx xcode4
else
	@echo "ERROR: Your OS is not detected in the makefile"
endif


lifami: remove_quarantine
	rm -rf build ; chmod 755 script/premake*
	@echo "OS=$(OS)"
ifeq ($(OS),Windows_NT)
	$(PREMAKE5) --os=windows --file=premake5.lua --lifami codeblocks
else ifeq ($(OS),linux)
	$(PREMAKE5) --os=linux --file=premake5.lua --lifami gmake
	$(PREMAKE5) --os=linux --file=premake5.lua --lifami codeblocks
else ifeq ($(OS),macosx)
	$(PREMAKE5) --os=macosx --file=premake5.lua --lifami gmake
	$(PREMAKE5) --os=macosx --file=premake5.lua --lifami xcode4
endif

define \n

endef

run: all
	@$(foreach dir,$(shell ls bin/*.exe),echo $(dir);)
	$(foreach x,$(shell ls bin/*.exe),$(x)${\n})

list:
	@echo "apps_linux=$(apps_linux)"	

EMCC = 	emcc -std=c++1z -Wimplicit-const-int-float-conversion -Isrc --preload-file data/ttf -s USE_SDL=2 -s FULL_ES2=1 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS="['png']"
emcc:
	echo "run: source /mnt/d/alex/CORRECTIONS/emsdk/emsdk_env.sh"
	${EMCC} src/Grapic.cpp apps/start/main_start.cpp -o bin/Start.html


${apps_linux}: %: remove_quarantine 
	@echo "Build app:\n   app_linux=${apps_linux}\n   OS=$(OS)"
	cd build/${OSGMAKE} ; make -f $@.make

	
FORCE:

