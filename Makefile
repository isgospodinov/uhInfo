CPP = g++
CPPFILES = $(wildcard src/*.cpp)
OBJS = $(CPPFILES:src/%.cpp=%.o)
BUILD = build
PROGRAM = uhInfo
GTKMMFLAGS = `pkg-config --cflags gtkmm-4.0`
GTKMMLIBS = `pkg-config --libs gtkmm-4.0`
UHIBFLAGS = -std=c++17 -Os -Wall# -g
ELIBS = -lpthread -ludisks2 -ldl
#MAXJ =
DT := $(shell date +%s)
EBTH = $(shell expr  $(shell nproc) - 1 )

.PHONY: all

all: buildafter

%.o:src/%.cpp
	@touch $@
	$(CPP) -c $(GTKMMFLAGS) $(UHIBFLAGS) $<

check: 
ifeq ($(origin MAXJ), undefined)
  ifeq ($(EBTH), 0)
    EBTH = 1
  endif
else
    EBTH = $(shell nproc)
endif

bmsg: check
	@echo 'Building with $(EBTH) threads'

cmsg:
	@echo '$(shell printf "Build time : %ds" $(shell expr $(shell date +%s) - $(DT)))'
	
building: buildbefore bmsg
	@$(MAKE) --no-builtin-variables --no-print-directory --jobs $(EBTH) realbuild
	
realbuild: $(OBJS)
	$(CPP) -o $(PROGRAM) $(OBJS) $(GTKMMLIBS) $(ELIBS)
	@mv -f $(PROGRAM) $(BUILD)
	
buildbefore:  
	@mkdir -p $(BUILD)
	@bash install.sh -ib

buildafter: building cmsg
	@bash install.sh -ia

clean:
	rm -rf $(BUILD)		
	rm -rf $(OBJS)

install:
	@bash install.sh -i

uninstall:
	@bash install.sh -ui

help:
	@bash install.sh
