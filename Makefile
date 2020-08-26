
# should be run from the directory of grapic
GRAPIC_HOME = .

apps_linux = "${MAKECMDGOALS}"


ifeq ($(OS),Windows_NT)
	OS = windows
	PREMAKE4 = $(GRAPIC_HOME)/script/premake4.exe
	PREMAKE5 = $(GRAPIC_HOME)/script/premake5.exe
else
UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	OS = macosx
	PREMAKE4 = $(GRAPIC_HOME)/script/premake4.macosx
	PREMAKE5 = $(GRAPIC_HOME)/script/premake5.macosx
	apps_linux = $(shell basename --suffix=.make build/linux/*.make  )
else
	OS = linux
	PREMAKE4 = $(GRAPIC_HOME)/script/premake4.linux.sh
	PREMAKE5 = $(GRAPIC_HOME)/script/premake5.linux
	apps_linux = $(shell basename --suffix=.make build/linux/*.make  )
endif
endif


#$(info $(apps_linux))
#ifeq ($(wildcard build/*),)		# if the premake was never runned, apps_linux contains "*" => remove it
ifeq ($(apps_linux),*)		# if the premake was never runned, apps_linux contains "*" => remove it
	apps_linux = ""
endif
$(info $(apps_linux))




all: build/${OS} remove_quarantine
	cd build/${OS} ; make

remove_quarantine:
ifeq ($(UNAME_S),Darwin)
	xattr -rd com.apple.quarantine ./
endif
	
clean: build/${OS}
	rm -rf bin/*.exe apps/LIFAMI ; cd build/${OS} ; make clean

veryclean: clean
	rm -rf build doc/html doc/images doc/xml doc/index.html

build/${OS}: premake4.lua premake

doc: $(GRAPIC_HOME)/doc/* $(GRAPIC_HOME)/doc/images/* $(GRAPIC_HOME)/src/* FORCE
	cd doc ; doxygen

dos2unix:
	dos2unix script/*.sh doc/VERSION ; chmod 755 script/*.sh

zip: dos2unix clean version $(GRAPIC_HOME)/bin/remove_correction.exe premakeall
	$(GRAPIC_HOME)/script/make_zip.sh

version: FORCE
	#$(shell $(GRAPIC_HOME)/script/inc_version.sh))
	sh $(GRAPIC_HOME)/script/inc_version.sh

web-win: version zip doc
	/c/Program\ Files\ \(x86\)/WinSCP/winscp.com -script=$(GRAPIC_HOME)/script/scp_script_toWeb.txt

web: version zip doc web-force

web-force:
	rsync -ravuz --delete $(GRAPIC_HOME)/doc/ ameyer@connect.liris.cnrs.fr:/home-membres/ameyer/grapic/

#web-force:
#	rsync -az --delete $(GRAPIC_HOME)/doc/ ameyer@liris.cnrs.fr:/home/ameyer/grapic/
#scp -r $(GRAPIC_HOME)/doc ameyer@liris.cnrs.fr:/home/ameyer/grapic-new

bin/remove_correction.exe: $(GRAPIC_HOME)/script/remove_correction.cpp
	g++ -Wall $(GRAPIC_HOME)/script/remove_correction.cpp -o $(GRAPIC_HOME)/bin/remove_correction.exe

premakeall: remove_quarantine
	@echo "premakeall OS=$(OS)"
	rm -rf build ; chmod 755 script/premake*
	@echo "Generate all premake files"
	$(PREMAKE4) --os=windows codeblocks
	$(PREMAKE4) --os=windows gmake
	$(PREMAKE5) vs2015
	$(PREMAKE4) --os=linux gmake
	$(PREMAKE4) --os=linux codeblocks
	$(PREMAKE4) --os=macosx gmake
	$(PREMAKE4) --os=macosx xcode3
	$(PREMAKE5) xcode4

premake: remove_quarantine
	@echo "premake OS=$(OS)"
	rm -rf build ; chmod 755 script/premake*
	@echo "OS=$(OS)"
ifeq ($(OS),Windows_NT)
	$(PREMAKE4) --os=windows codeblocks
else ifeq ($(OS),linux)
	$(PREMAKE4) --os=linux gmake
	$(PREMAKE4) --os=linux codeblocks
else ifeq ($(OS),macosx)
	$(PREMAKE4) --os=macosx gmake
	$(PREMAKE4) --os=macosx xcode3
else
	@echo "ERROR: Your OS is not detected in the makefile"
endif

premake-beta: remove_quarantine
	rm -rf build ; chmod 755 script/premake*
	@echo "OS=$(OS)"
ifeq ($(OS),Windows_NT)
	$(PREMAKE4) --target=beta --os=windows codeblocks
else ifeq ($(OS),linux)
	$(PREMAKE4) --target=beta --os=linux gmake
	$(PREMAKE4) --target=beta --os=linux codeblocks
else ifeq ($(OS),macosx)
	$(PREMAKE4) --target=beta --os=macosx gmake
	$(PREMAKE4) --target=beta --os=macosx xcode3
else
	@echo "ERROR: Your OS is not detected in the makefile"
endif


lifami: remove_quarantine
	rm -rf build ; script/make_lifami.sh ; chmod 755 script/premake*
	@echo "OS=$(OS)"
ifeq ($(OS),Windows_NT)
	$(PREMAKE4) --os=windows --file=lifami.lua gmake
	$(PREMAKE4) --os=windows --file=lifami.lua codeblocks
	#$(PREMAKE5) --file=lifami.lua vs2015
else ifeq ($(OS),linux)
	$(PREMAKE4) --os=linux --file=lifami.lua gmake
	$(PREMAKE4) --os=linux --file=lifami.lua codeblocks
else ifeq ($(OS),macosx)
	$(PREMAKE4) --os=macosx --file=lifami.lua gmake
	$(PREMAKE4) --os=macosx --file=lifami.lua xcode3
	$(PREMAKE5) --file=lifami.lua xcode4
endif

define \n

endef

run: all
	@$(foreach dir,$(shell ls bin/*.exe),echo $(dir);)
	$(foreach x,$(shell ls bin/*.exe),$(x)${\n})

list:
	@echo "apps_linux=$(apps_linux)"	

${apps_linux}: %: remove_quarantine 
	@echo "Build app: app_linux=${apps_linux} OS=$(OS)"
	cd build/${OS} ; make -f $@.make

	
FORCE:

