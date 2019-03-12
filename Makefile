
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
	mkdir -p build ; cd build/${OS} ; make

clean: build/${OS}
	rm -rf bin/*.exe apps/LIFAMI ; mkdir -p build ; cd build/${OS} ; make clean

build/${OS}: premake4.lua premake

doc: $(GRAPIC_HOME)/doc/* $(GRAPIC_HOME)/doc/images/* $(GRAPIC_HOME)/src/* FORCE
	cd doc ; doxygen

zip: clean version $(GRAPIC_HOME)/bin/remove_correction.exe premake
	$(GRAPIC_HOME)/script/make_zip.sh

version: FORCE
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

premake:
	rm -rf build ; chmod 755 script/premake*
	$(PREMAKE4) --os=windows gmake
	$(PREMAKE4) --os=windows codeblocks
	$(PREMAKE4) --os=linux gmake
	$(PREMAKE4) --os=linux codeblocks
	$(PREMAKE4) --os=macosx gmake
	$(PREMAKE4) --os=macosx xcode3
	$(PREMAKE5) vs2015
	$(PREMAKE5) xcode4

premake-beta:
	rm -rf build  ; chmod 755 script/premake*
	$(PREMAKE4) --target=beta --os=windows gmake
	$(PREMAKE4) --target=beta --os=windows codeblocks
	$(PREMAKE4) --target=beta --os=linux gmake
	$(PREMAKE4) --target=beta --os=linux codeblocks
	$(PREMAKE4) --target=beta --os=macosx gmake
	$(PREMAKE4) --target=beta --os=macosx xcode4
	$(PREMAKE5) --target=beta  vs2015

lifami:
	rm -rf build ; script/make_lifami.sh ; chmod 755 script/premake*
	$(PREMAKE4) --os=windows --file=lifami.lua gmake
	$(PREMAKE4) --os=windows --file=lifami.lua codeblocks
	$(PREMAKE4) --os=linux --file=lifami.lua gmake
	$(PREMAKE4) --os=linux --file=lifami.lua codeblocks
	$(PREMAKE4) --os=macosx --file=lifami.lua gmake
	$(PREMAKE4) --os=macosx --file=lifami.lua xcode3
	$(PREMAKE5) --file=lifami.lua vs2015
	$(PREMAKE5) --file=lifami.lua xcode4


define \n


endef

run: all
	@$(foreach dir,$(shell ls bin/*.exe),echo $(dir);)
	$(foreach x,$(shell ls bin/*.exe),$(x)${\n})

FORCE:
