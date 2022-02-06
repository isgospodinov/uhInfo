CPP = g++
PROGRAM = uhInfo
CPPFILES = main.cpp hth.cpp mwnd.cpp mwndui.cpp smdialog.cpp util.cpp hInfapp.cpp hgpu.cpp hmonit.cpp proc.cpp procun.cpp sysens.cpp ud2mon.cpp hdwera.cpp prefsdlg.cpp sensmon.cpp aboutdlg.cpp
OBJS = $(CPPFILES:.cpp=.o)
BUILD = build
GTKMMFLAGS = `pkg-config --cflags gtkmm-4.0`
GTKMMLIBS = `pkg-config --libs gtkmm-4.0`
UHIBFLAGS = -std=c++17 -Os -Wall# -g
ELIBS = -pthread -ludisks2 -ldl #-B/usr/local/libexec/mold

DT := $(shell date +%s)
EBTH = $(shell nproc)

.PHONY: all

all: buildafter

%.o: %.cpp
	$(CPP) -c -o $@ $(GTKMMFLAGS) $(UHIBFLAGS) $^

bmsg:
	@echo 'Building with $(EBTH) threads'
	
cmsg:
	@echo '$(shell printf "Build time : $(shell expr $(shell date +%s) - $(DT))s\n" )'
	
building: buildbefore bmsg
	@$(MAKE) --no-print-directory --jobs $(EBTH) realbuild
	
realbuild: $(OBJS)
	$(CPP) -o $(PROGRAM) $(OBJS) $(GTKMMLIBS) $(ELIBS)
	@mkdir -p $(BUILD)
	@mv -f $(PROGRAM) $(BUILD)

buildbefore:
	@bash install.sh -ib

buildafter: building cmsg
	@bash install.sh -ia

clean:
	rm -rf $(OBJS)
	rm -rf $(BUILD)		

install:
	@bash install.sh -i

uninstall:
	@bash install.sh -ui

help:
	@bash install.sh
