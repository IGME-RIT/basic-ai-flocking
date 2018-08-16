/*
File Name: main.cpp
Copyright © 2018
Original authors: Sanketh Bhat
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Description:
2D rigidbody engine
*/


#include "GLRender.h"
#include "GameObject.h"
#include "AgentManager.h"


#pragma region program specific Data members

float timestep = .016;


glm::vec3 mousePos;

GameObject* objA;
GameObject* objB;
GameObject* objC;
GameObject* objD;
GameObject* objE;
GameObject* objF;

Model* meshA;


// vector of scene bodies
std::vector<GameObject*> bodies;


#pragma endregion

glm::vec3 GetMousePos()
{

	double x, y;
	glfwGetCursorPos(window, &x, &y);

	//normalized device coordinates
	float NDCX = (x / 400.0f) - 1.0f;
	float NDCY = -(y / 400.0f) + 1.0f;

	return glm::vec3(NDCX, NDCY, 0.0f);


}

#pragma region util_functions

void update()
{

	target = GetMousePos();
	target.z = 0;

	Flocking(bodies);

	for each (GameObject* body in bodies)
	{
		body->update(timestep, PV);
	}
}


// This function is used to handle key inputs.
// It is a callback funciton. i.e. glfw takes the pointer to this function (via function pointer) and calls this function every time a key is pressed in the during event polling.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Sets the current window to a close flag when ESC is pressed
	if (key == GLFW_KEY_ESCAPE && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
	{
		glfwSetWindowShouldClose(window, 1);
	}
}
#pragma endregion

//Makes a 2D polygon based on the vertex number arguement
Model* setupModel(int n, glm::vec4 color, float size = 0.25f)
{
	std::vector<GLuint> indices;
	std::vector<VertexFormat> vertices;
	VertexFormat center(glm::vec3(0.0f, 0.0f, 0.0f), color);
	
	//Only add indices if you drawing a polygon with more than 3 points.
	if (n > 3)
	{
		//Indices are added in threes to form tris
		for (int i = 1; i < n+1; i++)
		{
			indices.push_back(0); //Start at the center
			if(i==n) //If we are at the last last vertex, go back to the first  non-center vertex and add it
			{
				indices.push_back(i);
				indices.push_back(1);

			}
			else
			{	//Adds current vertex and the next one
				indices.push_back(i);
				indices.push_back(i + 1);

			}

		}

		//Only 3+ point polygons need a center vertex
		vertices.push_back(center);
	}	

	float theta = 360.0f / n;
	

	//vertex generation
	for (int i = 0; i < n; i++)
	{
		//The point at angle theta  are fed into the buffer.
		vertices.push_back(VertexFormat(glm::vec3(size * cos(glm::radians(i*theta)), size * sin(glm::radians(i*theta)), 0.0f), color));
		
	}
	 return new Model(vertices.size(), vertices.data(), indices.size(), indices.data());

}


void main()
{
	// Initializes most things needed before the main loop
	init();

	// Sends the funtion as a funtion pointer along with the window to which it should be applied to.

	glfwSetKeyCallback(window, key_callback);

	
	//Creating models
	meshA = setupModel(3, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),0.05f);

	

	//Sets up bodies in the scene
	objA = new GameObject(meshA);
	objB = new GameObject(meshA);
	objC = new GameObject(meshA);
	objD = new GameObject(meshA);
	objE = new GameObject(meshA);
	objF = new GameObject(meshA);


	bodies.push_back(objA);
	bodies.push_back(objB);
	bodies.push_back(objC);
	bodies.push_back(objD);
	bodies.push_back(objE);
	bodies.push_back(objF);
	float x, y;
	
	for each (GameObject* body in bodies)
	{
		x=(rand() % 200 - 100)/100.0f;
		y= (rand() % 200 - 100) / 100.0f;
		body->Position(glm::vec3(x,y, 0));
	}

	// Enter the main loop.
	while (!glfwWindowShouldClose(window))
	{

		// Call to update() which will update the gameobjects.
		update();

		// Call the render function(s).
		renderScene();

		for each (GameObject* body in bodies)
		{
			body->render(uniMVP);
		}
		// Swaps the back buffer to the front buffer
		// Remember, you're rendering to the back buffer, then once rendering is complete, you're moving the back buffer to the front so it can be displayed.
		glfwSwapBuffers(window);

		// Checks to see if any events are pending and then processes them.
		glfwPollEvents();

	}

	for each (GameObject* body in bodies)
	{
		delete body;
	}
	delete meshA;
	//Cleans shaders and the program and frees up GLFW memory
	cleanup();

	return;
}