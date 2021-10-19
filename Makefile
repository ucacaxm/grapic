
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
	OSGMAKE = ${OS}-gmake
	PREMAKE4 = $(GRAPIC_HOME)/script/premake4.macosx
	PREMAKE5 = $(GRAPIC_HOME)/script/premake5.macosx
	Filename="${Filename##*/}"
	apps_linux = $(shell ls build/${OSGMAKE}/*.make 2>/dev/null | cut -d '/' -f 3 | cut -d . -f 1  )				# ls build/macosx/*.make | cut -d '/' -f 3 | cut -d . -f 1   OR basename
else
	OS = linux
	OSGMAKE = ${OS}-gmake
	PREMAKE4 = $(GRAPIC_HOME)/script/premake4.linux.sh
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
	xattr -rd com.apple.quarantine ./
endif
	
clean: build/${OSGMAKE}
	rm -rf bin/*.exe ; cd build/${OSGMAKE} ; make clean

doc-clean:
	rm -rf doc/html doc/xml

premake-clean: doc-clean
	rm -rf build ; rm -rf bin/* ; rm -rf obj ; chmod 755 script/premake*

build/${OSGMAKE}: premake4.lua premake

docgen: $(GRAPIC_HOME)/doc $(GRAPIC_HOME)/doc/images $(GRAPIC_HOME)/src FORCE
	cd doc ; doxygen

dos2unix:
	dos2unix -q script/*.sh doc/VERSION ; chmod 755 script/*.sh

zip: dos2unix version $(GRAPIC_HOME)/bin/remove_correction.exe
	$(GRAPIC_HOME)/script/make_zip.sh
	chmod 644 doc/download/*

version: FORCE
	sh $(GRAPIC_HOME)/script/inc_version.sh

web: zip docgen web-force

web-force:
	@echo "==============================ERRORS================================="
	cat tmp/errors.log
	@echo "==============================ERRORS================================="
	@echo "Continue?  [y/N]"
	@( read -p "\n" sure && \
		case "$$sure" in [yY]) \
			rsync -ravuz --delete $(GRAPIC_HOME)/doc/ ameyer@connect.liris.cnrs.fr:/home-membres/ameyer/grapic/; \
			touch tmp/rsync.ok;; \
		*) echo "stop";; \
		esac )

bin/remove_correction.exe: $(GRAPIC_HOME)/script/remove_correction.cpp dir
	g++ -Wall $(GRAPIC_HOME)/script/remove_correction.cpp -o $(GRAPIC_HOME)/bin/remove_correction.exe

premake-all: premake-wincb20 premake-linux premake-macosx premake-winvs2019
	
premake-exec:
	chmod 755 script/premake*

premake-wincb17: remove_quarantine
	@echo "premake CB17 OS=$(OS)"
	cp extern/mingw-cb17/bin/*.dll bin
	$(PREMAKE4) --os=windows --cb-version=cb17 codeblocks

premake-wincb20: remove_quarantine
	@echo "premake CB20 OS=$(OS)"
	cp extern/mingw-cb20/bin/*.dll bin
	$(PREMAKE4) --os=windows --cb-version=cb20 codeblocks
	$(PREMAKE4) --os=windows --cb-version=cb20 gmake

premake-winvs2019: remove_quarantine
	@echo "premake vs2019 OS=$(OS)"
	cp extern/visual2019/bin/*.dll bin
	$(PREMAKE5)  --os=windows vs2019
		
premake-linux: premake-exec remove_quarantine
	@echo "premake linux OS=$(OS)"
	$(PREMAKE4) --os=linux gmake
	$(PREMAKE4) --os=linux codeblocks

premake-macosx: premake-exec
	@echo "premake macosx OS=$(OS)"
	$(PREMAKE4) --os=macosx gmake
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
	rm -rf build ; chmod 755 script/premake*
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
	@echo "Build app:\n   app_linux=${apps_linux}\n   OS=$(OS)"
	cd build/${OSGMAKE} ; make -f $@.make

	
FORCE:

