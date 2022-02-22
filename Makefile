CPP = g++
BUILD = build
PROGRAM = $(BUILD)/uhInfo
CPPFILES = $(wildcard src/*.cpp)
OBJS = $(CPPFILES:src/%.cpp=$(BUILD)/%.o)
GTKMMFLAGS = `pkg-config --cflags gtkmm-4.0`
GTKMMLIBS = `pkg-config --libs gtkmm-4.0`
UHIBFLAGS = -std=c++17 -Os -Wall# -g
ELIBS = -lpthread -ludisks2 -ldl
DT := $(shell date +%s)

.PHONY: all

all: bldaf

$(BUILD)/%.o:src/%.cpp
	$(CPP) -c -o $@ $(GTKMMFLAGS) $(UHIBFLAGS) $<

cmsg:
	@echo '$(shell printf "** Build time : %ds **" $(shell expr $(shell date +%s) - $(DT)))'
	
bldst: bldbf
	@$(MAKE) --no-print-directory --jobs $(shell nproc) bldrl
	
bldrl: $(OBJS)
	$(CPP) -o $(PROGRAM) $(OBJS) $(GTKMMLIBS) $(ELIBS)
	
bldbf:  
	@mkdir -p $(BUILD)
	@bash install.sh -ib

bldaf: bldst cmsg
	@bash install.sh -ia

clean:
	$(RM) -r $(BUILD)

install:
	@bash install.sh -i

uninstall:
	@bash install.sh -ui

help:
	@bash install.sh
