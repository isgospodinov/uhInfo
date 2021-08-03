CPP = g++
PROGRAM = uhInfo
CPPFILES = main.cpp hth.cpp mwnd.cpp mwndui.cpp smdialog.cpp util.cpp hInfapp.cpp hgpu.cpp hmonit.cpp proc.cpp procun.cpp sysens.cpp ud2mon.cpp hdwera.cpp prefsdlg.cpp sensmon.cpp aboutdlg.cpp
OBJS = $(CPPFILES:.cpp=.o)
BUILD = build
GTKMMFLAGS = `pkg-config --cflags gtkmm-3.0`
GTKMMLIBS = `pkg-config --libs gtkmm-3.0`
UHIBFLAGS = -std=c++14 -Os -Wall# -g
ELIBS = -pthread -ludisks2 -ldl

all: $(PROGRAM)

%.o: %.cpp
	$(CPP) -c -o $@ $(GTKMMFLAGS) $(UHIBFLAGS) $^

$(PROGRAM): $(OBJS)
	$(CPP) -o $(PROGRAM) $(OBJS) $(GTKMMLIBS) $(ELIBS)
	@mkdir -p $(BUILD)
	@mv -f $(PROGRAM) $(BUILD)

clean:
	rm -rf $(OBJS)
	rm -rf $(BUILD)		

install:
	bash install.sh -i	

uninstall:
	bash install.sh -ui

help:
	bash install.sh
