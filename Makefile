CXX     = g++
LDFLAGS = -lGL -lGLU -lglut -lX11

all:
	$(CXX) $(LDFLAGS) main.cc
