SDL_FLAGS = $(shell pkg-config --cflags --libs sdl2 sdl2_gfx)

SRC_FILE = main.cxx
EXECUTABLE = ant_war_direct

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC_FILE)
	g++ main.cxx boid.cpp pieBoid.cpp charBoid.cpp -o $(EXECUTABLE) $(SDL_FLAGS)

run: $(EXECUTABLE)
	./$(EXECUTABLE) 200

clean:
	rm -f $(EXECUTABLE)
	rm -f *.o
