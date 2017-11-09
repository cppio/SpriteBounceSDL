FLAGS := $(CFLAGS) $(CXXFLAGS)

ifeq ($(shell uname), Darwin)
FLAGS += -framework SDL2 -framework SDL2_image
else
FLAGS += -lSDL2 -lSDL2_image
endif

SpriteBounce: SpriteBounce.cpp
	$(CXX) SpriteBounce.cpp -o SpriteBounce $(FLAGS)
