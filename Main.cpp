#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VAO.h"
#include"EBO.h"

GLfloat vertices[] =
{
		//Positions			RGB for color				TexCoord
	0.0f, 0.0f, 0.0f,		1.0f,  0.0f, 0.0f,			0.0f, 0.0f,		
	1.0f, 0.0f, 0.0f,		1.0f,  1.5f, 0.0f,			1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,		1.0f,  1.0f, 0.0f,			1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,		1.0f,  0.0f, 1.0f,			0.0f, 1.0f
};

GLuint indices[] =
{
	 0, 1, 3,
	 1, 2, 3
};

int main()
{
	
	//-------------------------------------------------------------------------------------------------------------------------------------------------------------//
	//initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					//Telling glfw which version of opengl we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					// i.e. OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//telling glfw that we are using core profile i.e. means we only have modern functions
	GLFWwindow* window = glfwCreateWindow(800, 800, "Test Window", NULL, NULL);	// Create a GLFWwindow object of 800 by 800 pixcels, naming it "Test window"
	if (window == NULL)												//error check of the window fails to create6
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);									//Introduce the window into the current context
	gladLoadGL();													//load GLAD so it configures OpenGL
	glViewport(0, 0, 800, 800);										//Specify the viewport of OpenGL in the window
																	// in this case the viewport goes from x=0, y=0 to x=800, y=800
	//------------------------------------------------------------------------------------------------------------------------------------------------------------ --//
	
	Shader shaderProgram("default.vert", "default.frag");
	VAO VAO1;											// connecting newly formed VAO with the ID variable
	VAO1.Bind();										//Recording start, making it the curret active VAO

	VBO VBO1(vertices, sizeof(vertices));				//connecting to variable, changing target, copy data of vertices in GPU
	EBO EBO1(indices, sizeof(indices));					//connecting to variable, changing target, copy data of indices in GPU

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);								//how should gpu read the comma seperated values which is in VBO
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));				//and telling that these values are actually position of vertexes
																									//(by telling that they belong to attrib 0, in "default.vert" we already defined attrib 0 as position)*/
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
				//Now the VAO contains every info that we need	
				
	// Gets Id of a uniform called "scale" 	
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	//Textures importing
	Texture photo("img.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	photo.texUnit(shaderProgram, "tex0", 0);
	


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		shaderProgram.Activate();
		glUniform1f(uniID, 1.0f);								//only use uniforms after activating shader program
		photo.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);					//we are using gldrawElements because we are using indices provided by the bounded EBO
																				//use glDrawArrays if not using EBOs
		glfwSwapBuffers(window);
		glfwPollEvents();														// Take care of all GLFW events
	}

			//--------------------------CLEANUP RESOURCES-------------------------------//

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	photo.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);													//Delete window before ending the program
	glfwTerminate();															// Terminate GLFW before ending the program
	return 0;
}