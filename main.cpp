#include "Application.hpp"

#include <stdexcept>
#include <iostream>


int main()
{
	try
	{
		Application application;
		application.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}
}
//#pragma comment(lib,"opengl32.lib")
//#pragma comment(lib,"glu32.lib")
//
//#include <SFML/Graphics.hpp>
//#include <SFML/OpenGL.hpp>
//#include <gl/GLU.h>
//#include <iostream>
//
//
//
//
//////////////////////////////////////////////////////////////
///// Entry point of application
//////////////////////////////////////////////////////////////
//int main()
//{
//	// Create the main window
//	sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML OpenGL");
//
//	// Create a clock for measuring time elapsed
//	sf::Clock Clock;
//
//	//prepare OpenGL surface for HSR
//	glClearDepth(1.f);
//	glClearColor(0.3f, 0.3f, 0.3f, 0.f);
//	glEnable(GL_DEPTH_TEST);
//	glDepthMask(GL_TRUE);
//
//	//// Setup a perspective projection & Camera position
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(90.f, 1.f, 1.f, 300.0f);//fov, aspect, zNear, zFar
//
//
//
//	bool rotate = true;
//	float angle;
//
//	// Start game loop
//	while (App.isOpen())
//	{
//		// Process events
//		sf::Event Event;
//		while (App.pollEvent(Event))
//		{
//			// Close window : exit
//			if (Event.type == sf::Event::Closed)
//				App.close();
//
//			// Escape key : exit
//			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
//				App.close();
//
//			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::A)) {
//				rotate = !rotate;
//			}
//
//		}
//
//		//Prepare for drawing
//		// Clear color and depth buffer
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		// Apply some transformations for the cube
//		glMatrixMode(GL_MODELVIEW);
//		glLoadIdentity();
//		glTranslatef(0.f, 0.f, -200.f);
//
//		if (rotate) {
//			angle = Clock.getElapsedTime().asSeconds();
//		}
//		glRotatef(angle * 50, 1.f, 0.f, 0.f);
//		glRotatef(angle * 30, 0.f, 1.f, 0.f);
//		glRotatef(angle * 90, 0.f, 0.f, 1.f);
//
//
//
//		//Draw a cube
//		glBegin(GL_QUADS);//draw some squares
//		glColor3i(0, 1, 1);
//		glVertex3f(-50.f, -50.f, -50.f);
//		glVertex3f(-50.f, 50.f, -50.f);
//		glVertex3f(50.f, 50.f, -50.f);
//		glVertex3f(50.f, -50.f, -50.f);
//
//		glColor3f(0, 0, 1);
//		glVertex3f(-50.f, -50.f, 50.f);
//		glVertex3f(-50.f, 50.f, 50.f);
//		glVertex3f(50.f, 50.f, 50.f);
//		glVertex3f(50.f, -50.f, 50.f);
//
//		glColor3f(1, 0, 1);
//		glVertex3f(-50.f, -50.f, -50.f);
//		glVertex3f(-50.f, 50.f, -50.f);
//		glVertex3f(-50.f, 50.f, 50.f);
//		glVertex3f(-50.f, -50.f, 50.f);
//
//		glColor3f(0, 1, 0);
//		glVertex3f(50.f, -50.f, -50.f);
//		glVertex3f(50.f, 50.f, -50.f);
//		glVertex3f(50.f, 50.f, 50.f);
//		glVertex3f(50.f, -50.f, 50.f);
//
//		glColor3f(1, 1, 0);
//		glVertex3f(-50.f, -50.f, 50.f);
//		glVertex3f(-50.f, -50.f, -50.f);
//		glVertex3f(50.f, -50.f, -50.f);
//		glVertex3f(50.f, -50.f, 50.f);
//
//		glColor3f(1, 0, 0);
//		glVertex3f(-50.f, 50.f, 50.f);
//		glVertex3f(-50.f, 50.f, -50.f);
//		glVertex3f(50.f, 50.f, -50.f);
//		glVertex3f(50.f, 50.f, 50.f);
//
//		glEnd();
//
//		// Finally, display rendered frame on screen
//		App.display();
//	}
//
//	return EXIT_SUCCESS;
//}
//#include <iostream>
////#pragma comment(lib,"opengl32.lib")
//#pragma comment(lib,"glu32.lib")
//#include <SFML/Graphics.hpp>
//#include <gl/glew.h>
//#include <gl/GLU.h>
//
//int main()
//{
//	// Create window
//	sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML Perspective");
//
//	//load the texture
//	sf::Texture texture;
//	if (!texture.loadFromFile("images\\images.png"))
//	{
//		return EXIT_SUCCESS;
//	}
//	texture.setRepeated(true);
//
//	//initialise OPENGL
//	glClearDepth(1.0f);
//	glClearColor(1.f, 1.0f, 1.0f, 0.0f);
//	glEnable(GL_DEPTH_TEST);
//	glDepthMask(GL_TRUE);
//
//	//// Setup the camera
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(90.0f, 1.0f, 1.0f, 500.0f);
//
//	// Game loop
//	while (window.isOpen())
//	{
//		// Process events
//		sf::Event Event;
//		while (window.pollEvent(Event))
//		{
//			if (Event.type == sf::Event::Closed)
//			{
//				window.close();
//			}
//		}
//
//		// Clear the screen
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		// Preform the appropriate transformations
//		glMatrixMode(GL_MODELVIEW);
//		glLoadIdentity();
//		glTranslatef(0.f, 0.f, -150.f);
//
//		glRotatef(30.0f, 0.f, 1.f, 0.f);
//
//		glEnable(GL_TEXTURE_2D);
//		sf::Texture::bind(&texture);
//		//Draw a face
//		glBegin(GL_QUADS);
//		{
//			glColor3f(0, 0, 1);
//			glVertex3f(-50.f, -50.f, 50.f);
//			glVertex3f(-50.f, 50.f, 50.f);
//			glVertex3f(50.f, 50.f, 50.f);
//			glVertex3f(50.f, -50.f, 50.f);
//		}
//		glEnd();
//		sf::Texture::bind(NULL);
//
//		window.display();
//	}
//	return EXIT_SUCCESS;
//}