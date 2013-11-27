GCC_OPTIONS= -g -Wall -pedantic -std=c++11 -I ./include/ -Wno-unused-function -Wno-unused-but-set-variable -Wno-sign-compare
LIBS= -lglut -lGLEW -lGL -lGLU -lm 
OPTIONS= $(GCC_OPTIONS) $(LIBS) 
TARGET = flight_sim

OBJ=
SRC= geometry.cpp windmill.cpp camera.cpp game.cpp

$(TARGET):
	g++ main.cpp ./Common/InitShader.cpp $(SRC)  $(OPTIONS) -o $(TARGET)

clean:
	rm -f $(TARGET)


#-Wno-unused-function

# -Wunused-function
# -Wunused-label
# -Wunused-parameter
# -Wunused-value
# -Wunused-variable
# -Wunused (=all of the above)
