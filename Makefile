
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
	rm -rf bin/*.exe ; cd build/${OS} ; make clean

cleandoc:
	rm -rf doc/html doc/images doc/xml doc/index.html

cleanpremake: cleandoc
	rm -rf build ; rm -rf bin/* ; rm -rf obj ; chmod 755 script/premake*

build/${OS}: premake4.lua premake

doc: $(GRAPIC_HOME)/doc/* $(GRAPIC_HOME)/doc/images/* $(GRAPIC_HOME)/src/* FORCE
	cd doc ; doxygen

dos2unix:
	dos2unix script/*.sh doc/VERSION ; chmod 755 script/*.sh

zip: dos2unix version $(GRAPIC_HOME)/bin/remove_correction.exe
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
	
premake-WinCB17: remove_quarantine cleanpremake
	@echo "premake CB17 OS=$(OS)"
	cp extern/mingw-cb17/bin/*.dll bin
	$(PREMAKE4) --os=windows --cb-version=cb17 codeblocks

premake-WinCB20: remove_quarantine cleanpremake
	@echo "premake CB20 OS=$(OS)"
	cp extern/mingw-cb20/bin/*.dll bin
	$(PREMAKE4) --os=windows --cb-version=cb20 codeblocks

premake-WinVS2015: remove_quarantine cleanpremake
	@echo "premake vs2015 OS=$(OS)"
	cp extern/visual2015/bin/*.dll bin
	$(PREMAKE5)  --os=windows vs2015
		
premake-linux: remove_quarantine cleanpremake
	@echo "premake linux OS=$(OS)"
	$(PREMAKE4) --os=linux gmake
	$(PREMAKE4) --os=linux codeblocks

premake-macos: cleanpremake
	@echo "premake macos OS=$(OS)"
	$(PREMAKE4) --os=macosx gmake
	$(PREMAKE5) xcode4

	
ifeq ($(OS),Windows_NT)
premake: remove_quarantine premake-cb20
	@echo "premake Win OS=$(OS)"
else ifeq ($(OS),linux)
premake: remove_quarantine premake-linux
	@echo "premake linux OS=$(OS)"
else ifeq ($(OS),macosx)
premake: remove_quarantine premake-macos
	@echo "premake macos OS=$(OS)"
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
	$(PREMAKE4) --os=windows --file=premake4.lua --lifami codeblocks
else ifeq ($(OS),linux)
	$(PREMAKE4) --os=linux --file=premake4.lua --lifami gmake
	$(PREMAKE4) --os=linux --file=premake4.lua --lifami codeblocks
else ifeq ($(OS),macosx)
	$(PREMAKE4) --os=macosx --file=premake4.lua --lifami gmake
	$(PREMAKE4) --os=macosx --file=premake4.lua --lifami xcode3
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

