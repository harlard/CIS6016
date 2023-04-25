#OBJS specifies which files to compile as part of the project
OBJS = main.cpp Utils/controls.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
OPENGL_FLAGS =  -lGLEW -lGLU -lm -lGL -lstb
GLSL_FLAGS = -lglslang -lOSDependent -lHLSL -lOGLCompiler -lSPVRemapper
GLFW_FLAGS = -lglfw -lXrandr -lpthread -lxcb -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl
ASSIMP_FLAGS = -lassimp -lstdc++ -lz
OPENAL_FLAGS = -lopenal
ODE_FLAGS = -lode

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = test
#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(OPENGL_FLAGS) $(GLFW_FLAGS) $(GLSL_FLAGS) $(ASSIMP_FLAGS) $(OPENAL_FLAGS) $(ODE_FLAGS) -o $(OBJ_NAME)
