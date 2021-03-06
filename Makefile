ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell sh -c 'uname -s 2>/dev/null || echo not')
endif

#OBJS specifies which files to compile as part of the project 
OBJS = ./*cpp
#HEADS = ./*.h

#CC specifies which compiler we're using 
CC = g++

ASTYLE = aStyle

SRCS=$(wildcard ./src/*.cpp)
PROGS = $(patsubst ./src/%.cpp, ./build/%.o,$(SRCS))

# -w suppresses all warnings 
# -Wl,-subsystem,windows gets rid of the console window 
COMPILER_FLAGS = -w -fpermissive

LIBRARY_PATHS = -L ./lib

INCLUDE_PATHS = -I ./include

#LINKER_FLAGS specifies the libraries we're linking against 
LINKER_FLAGS = -static-libgcc -static-libstdc++ -lpthread

WINAPI = -lmingw32 -lkernel32 -lm -ldxguid -ldxerr8 -luser32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lgdi32 -lcomdlg32 -lwinspool 
WINAPI+= -lcomctl32 -luuid -lrpcrt4 -ladvapi32 -lwsock32 -lshlwapi -lversion -ldbghelp -lwinpthread

MOHO = -lMoho
 
#OBJ_NAME specifies the name of our exectuable 
OBJ_NAME = English.exe 

WX = -lwxmsw31u_richtext -lwxbase31u_xml -lwxexpat -lwxregexu -lwxscintilla -lwxbase31u_net
WX+= -lwxmsw31u_adv -lwxmsw31u_aui -lwxmsw31u_gl -lwxmsw31u_html -lwxmsw31u_media -lwxmsw31u_propgrid
WX+= -lwxmsw31u_qa -lwxmsw31u_ribbon -lwxmsw31u_stc -lwxmsw31u_webview -lwxmsw31u_xrc
WX+= -lwxmsw31u_core -lwxbase31u -luxtheme -loleacc -lodbc32 -lwxjpeg -lwxpng -lwxtiff -lwxzlib

FUNC_CALL_PARAMS_FLAGS = -mpush-args -mno-accumulate-outgoing-args -mno-stack-arg-probe

#Monolithic compile.
all : 
	$(CC) -O3 -s -static $(OBJS) $(HEADS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(WX) $(FUNC_CALL_PARAMS_FLAGS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(WINAPI) -o $(OBJ_NAME)

all_individual : $(PROGS)
./build/%.o: ./src/%.cpp
	$(CC) -c -static $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(WX) $(FUNC_CALL_PARAMS_FLAGS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(WINAPI) -o $@ $<
	
Moho: 
	$(MAKE) all -C ./src
	
#Building a static lib out of src files. Benefits - faster compile time. Makes project modular.
all_sl: Moho
	$(CC) -O3 -static main.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(MOHO) $(WX) $(FUNC_CALL_PARAMS_FLAGS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(WINAPI) -o $(OBJ_NAME)
	
clean_f:
	find . -type f -name '*.orig' -delete
	
clean_o:
	$(MAKE) clean -C ./src
	find . -type f -name '*.o' -delete
	
.IGNORE format:
	$(ASTYLE) --style=allman --indent=tab ./*.cpp, *.h
	$(ASTYLE) --style=allman --indent=tab ./src/*.cpp, *.h
	$(ASTYLE) --style=allman --indent=tab --recursive ./src/*.cpp, *.h
	
	
	
	
	