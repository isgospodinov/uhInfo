CPP = c++
BUILD = build
PROGRAM = $(BUILD)/uhInfo
OBJS = $(patsubst src/%.cpp,$(BUILD)/%.o,$(wildcard */*.cpp)) \
               $(patsubst src/dar/%.cpp,$(BUILD)/%.o,$(wildcard */dar/*.cpp)) \
                            $(patsubst src/dlg/%.cpp,$(BUILD)/%.o,$(wildcard */dlg/*.cpp))
CMPGO = $(CPP) -c -o $@ $(BFLAGS) $< 
BFLAGS = `pkg-config --cflags gtkmm-4.0 udisks2` -std=c++17 -Os -Wall# -g
GTKMMLIBS = `pkg-config --libs gtkmm-4.0`
ELIBS = -lpthread -ludisks2 -ldl
DT := $(shell date +%s)

$(BUILD)/%.o:*/%.cpp ; $(CMPGO)
$(BUILD)/%.o:*/*/%.cpp ; $(CMPGO)

.PHONY: all

all: bldaf	

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
