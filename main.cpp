#include <iostream>
#include <vector>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <AL/al.h>
#include <AL/alc.h>


#include "Utils/controls.hpp"
#include "Utils/model.h"


using namespace glm;

ALCdevice *device;
ALCcontext *context;
ALuint source;
ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

GLFWwindow* window;
int windownH = 768;
int windownW = 1024;
int nbFrames = 0;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float timeCounter = 0;


void Timer();
void WindowSetUp();
void initOpenAL();

int main(void)
{
  // Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}
	WindowSetUp();

	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	initOpenAL();

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, windownW/2, windownH/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);


	// Create and compile our GLSL program from the shaders
	Shader programID( "Shaders/StandardShading.vertexshader", "Shaders/StandardShading.fragmentshader" );
	Model ovni("Model/Ovni/Ovni.dae");
	ovni.SetPosWorld(glm::vec3(0.0f,0.0f,0.0f));
	setCamaraPos(ovni.GetPosW());

	Model city("Model/city/city.dae");
	city.SetPosWorld(glm::vec3(0.0f,-50.0f,0.0f));

	programID.use();

	do{
    Timer();
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// Compute the MVP matrix from keyboard and mouse input

		//computeMatricesFromInputs(deltaTime);
		shipControl(deltaTime);
		glm::vec3 position = GetPosDelta();
		glm::vec3 currentPos = ovni.GetPosW();
		ovni.SetPosWorld(currentPos + position);

		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		programID.setMat4("P", ProjectionMatrix);
		// Use our shader
		programID.use();
		programID.setMat4("MVP", MVP);
		glm::vec3 lightPos = glm::vec3(4,4,4);
		programID.setVec3("LightPosition_worldspace", lightPos);
		programID.setMat4("V", ViewMatrix);
		//glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]); // This one doesn't change between objects, so this can be done once for all objects that use "programID"

			glm::mat4 modelO = glm::mat4(1.0f);
			modelO = glm::translate(modelO, ovni.GetPosW()); // translate it down so it's at the center of the scene
			modelO = glm::scale(modelO, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down

			programID.setMat4("M", modelO);
			ovni.Draw(programID);

			glm::mat4 modelC = glm::mat4(1.0f);
			modelC = glm::translate(modelC, city.GetPosW()); // translate it down so it's at the center of the scene
			modelC = glm::scale(modelC, glm::vec3(100.0f, 100.0f, 100.0f));	// it's a bit too big for our scene, so scale it down
			programID.setMat4("M", modelC);
			city.Draw(programID);



		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );
//OpenAL cleaning
	alcDestroyContext(context);
 	alcCloseDevice(device);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void WindowSetUp(){

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// Open a window and create its OpenGL context
		window = glfwCreateWindow( windownW, windownH, "CIS5013", NULL, NULL);

}

void Timer(){
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
	timeCounter += deltaTime;
	nbFrames += 1;
     if ( timeCounter >= 1.0 ){ // I
				 std::string newTitle ="CIS5013 " + std::to_string(nbFrames) + "FPS";
				 const char* title = newTitle.c_str();
				 glfwSetWindowTitle (window, title);
         nbFrames = 0;
				 timeCounter = 0;
     }
	 lastFrame = currentFrame;

}

void initOpenAL(){
	device = alcOpenDevice(NULL);
  if(!device) std::cout<<"audio device not found"<<std::endl;



	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context))std::cout<<"Context not created"<<std::endl;




	alListener3f(AL_POSITION, 0, 0, 1.0f);
	// check for errors
	alListener3f(AL_VELOCITY, 0, 0, 0);
	// check for errors
	alListenerfv(AL_ORIENTATION, listenerOri);
	// check for errors



alGenSources((ALuint)1, &source);
// check for errors

alSourcef(source, AL_PITCH, 1);
// check for errors
alSourcef(source, AL_GAIN, 1);
// check for errors
alSource3f(source, AL_POSITION, 0, 0, 0);
// check for errors
alSource3f(source, AL_VELOCITY, 0, 0, 0);
// check for errors
alSourcei(source, AL_LOOPING, AL_FALSE);
// check for errros

}
