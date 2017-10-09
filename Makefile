
# should be run from the directory of grapic
GRAPIC_HOME = .

	
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
else
	OS = linux
	PREMAKE4 = $(GRAPIC_HOME)/script/premake4.linux
	PREMAKE5 = $(GRAPIC_HOME)/script/premake5.linux
endif
endif


all: build/${OS}
	cd build/${OS} ; make
	
clean: build/${OS}
	cd build/${OS} ; make clean

build/${OS}: premake4.lua

doc: $(GRAPIC_HOME)/doc/* $(GRAPIC_HOME)/doc/images/* $(GRAPIC_HOME)/src/* FORCE
	cd doc ; doxygen

zip: version doc $(GRAPIC_HOME)/bin/remove_correction.exe premake
	$(GRAPIC_HOME)/script/make_zip.sh

version: FORCE
	sh $(GRAPIC_HOME)/script/inc_version.sh

web: version zip doc
	/c/Program\ Files\ \(x86\)/WinSCP/winscp.com -script=$(GRAPIC_HOME)/script/scp_script_toWeb.txt
	
bin/remove_correction.exe: $(GRAPIC_HOME)/script/remove_correction.cpp
	g++ -Wall $(GRAPIC_HOME)/script/remove_correction.cpp -o $(GRAPIC_HOME)/bin/remove_correction.exe

premake:
	rm -rf build
	$(PREMAKE4) --os=windows gmake
	$(PREMAKE4) --os=windows codeblocks
	$(PREMAKE4) --os=linux gmake
	$(PREMAKE4) --os=linux codeblocks
	$(PREMAKE4) --os=macosx gmake
	$(PREMAKE4) --os=macosx xcode4
	$(PREMAKE5) vs2015

premake-beta:
	rm -rf build
	$(PREMAKE4) --target=beta --os=windows gmake
	$(PREMAKE4) --target=beta --os=windows codeblocks
	$(PREMAKE4) --target=beta --os=linux gmake
	$(PREMAKE4) --target=beta --os=linux codeblocks
	$(PREMAKE4) --target=beta --os=macosx gmake
	$(PREMAKE4) --target=beta --os=macosx xcode4
	$(PREMAKE5) --target=beta  vs2015
	
	
define \n


endef

run: all
	@$(foreach dir,$(shell ls bin/*.exe),echo $(dir);)
	$(foreach x,$(shell ls bin/*.exe),$(x)${\n})

FORCE:
