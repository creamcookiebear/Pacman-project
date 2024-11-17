#include <iostream>
#include <vector>

#include "Agent.h"
#include "Light.h"
#include "Ghosts.h"
#include "Map.h"
#include <GL/freeglut.h>
#include "Constants.h"

using namespace std;

const int FPS = 20;
int sTime = 0;
int eTime = 0;

Pacman pacman;
Vector3f pacPos(0.f,0.f,0.f);

//Pinky pinky;
Blinky blinky;
//Inky inky;
//Clyde clyde;

std::vector<std::unique_ptr<Agent>> agents;

bool isPow = false;

Map map;

Light light(BOUNDARY_X, BOUNDARY_Y, BOUNDARY_X / 2, GL_LIGHT0);
//Texture texture("snu.png");

void initialize() {
	map.MapInitializer("map_source.csv"); //load map

	// Light
	light.setAmbient(0.5f, 0.5f, 0.5f, 1.0f);
	light.setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
	light.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);

	// PacMan
	Material mtlPac;
	mtlPac.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
	mtlPac.setAmbient(0.6f, 0.6f, 0.0f, 1.0f);
	mtlPac.setDiffuse(0.8f, 0.8f, 0.0f, 1.0f);
	mtlPac.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtlPac.setShininess(30.0f);

	//pacman.setIndexPosition(Map::width / 2, Map::hight / 2);
	pacman.setIndexPosition(4, 4);
	pacman.setVel(0.0f, 0.0f, 0.0f);
	pacman.setMTL(mtlPac);

	pacPos = pacman.getPos();
	//=======================================================
	// Ghost(s)
	//
	// Blinky
	Material mtlG1;
	mtlG1.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
	mtlG1.setAmbient(0.0f, 0.6f, 0.6f, 1.0f);
	mtlG1.setDiffuse(0.0f, 0.8f, 0.8f, 1.0f);
	mtlG1.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtlG1.setShininess(30.0f);

	blinky.setMTL(mtlG1);
	blinky.setIndexPosition(3, 4);
	blinky.setVel(1.f, 0.f, 0.f);

	//=======================================================
	// Handle all Agents at once
	agents.push_back(std::make_unique<Pacman>(pacman));
	agents.push_back(std::make_unique<Blinky>(blinky));

	//=======================================================
	// Map
	//
	Material mtlWall;
	mtlWall.setEmission(0.0f, 0.0f, 0.05f, 1.0f);
	mtlWall.setAmbient(0.0f, 0.0f, 0.2f, 1.0f);
	mtlWall.setDiffuse(0.0f, 0.0f, 0.5f, 1.0f);
	mtlWall.setSpecular(0.1f, 0.1f, 0.1f, 1.0f);
	mtlWall.setShininess(10.0f);

	Material mtlPellet;
	mtlPellet.setEmission(0.3f, 0.3f, 0.3f, 1.0f);   // Strong self-emission for a glowing effect
	mtlPellet.setAmbient(0.8f, 0.8f, 0.8f, 1.0f);    // Bright ambient color
	mtlPellet.setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);    // Pure white diffuse color
	mtlPellet.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);   // High-intensity specular reflection
	mtlPellet.setShininess(100.0f);                  // High shininess value for a sharp, reflective highlight

	Material mtlPower;
	mtlPower.setEmission(1.0f, 1.0f, 1.0f, 1.0f);   // Strong self-emission for a glowing effect
	mtlPower.setAmbient(0.8f, 0.8f, 0.8f, 1.0f);    // Bright ambient color
	mtlPower.setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);    // Pure white diffuse color
	mtlPower.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);   // High-intensity specular reflection
	mtlPower.setShininess(100.0f);                  // High shininess value for a sharp, reflective highlight
	
	map.setMtl_wall(mtlWall);
	map.setMtl_pellet(mtlPellet);
	map.setMtl_power(mtlPower);

	
}

void idle() {
	float frameDuration = 1000.0f / FPS; //  1000.0f / FPS
	eTime = glutGet(GLUT_ELAPSED_TIME);

	if (eTime - sTime > frameDuration) {
		/* Implement: update direction and move Agents */
		if ((pacman.getVel() == Vector3f() || pacman.isIndexPositionUpdated())) {
			cout << "called update vel" << endl;
			pacman.updateVel();
		}
		pacman.move();
		//cout << pacman.isIndexPositionUpdated() << endl;
		// 
		//cout << "vel: " << pacman.getVel()[0] << ", " << pacman.getVel()[1] << endl;

		sTime = eTime;
		glutPostRedisplay();
	}
}

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y, -100.0, 100.0);
	//gluOrtho2D(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw 2D
	/* Implement: (1) draw map, and (2) draw student ID and name */

	
	/*
	string c = "2020-14247 Shinyee Kang";
	glPushMatrix();
	glLineWidth(1.f);
	glTranslatef(LEFT_BOUNDARY - 3.f * BLOCK_SIZE, TOP_BOUNDARY + 3.f * BLOCK_SIZE, 0.0f);
	float scale_text = 20;
	glScalef(scale_text / 152.38f, scale_text / 152.38f, scale_text / 152.38f);
	for (int i = 0; i < c.size(); i++) {
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, c[i]);
	}
	glPopMatrix();
	*/

	// Draw 3D
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(light.getID());
	light.draw();

	map.draw();
	for (const auto& agent : agents){ // draw Pacman, Ghost(s)
		pacman.draw();
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(light.getID());

	glutSwapBuffers();
}

void keyboardDown(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27:
		exit(0);
	default:
		break;
	}

	glutPostRedisplay();
}

void specialKeyDown(int key, int x, int y) {
	/* Implement: set next directions */
	switch (key)
	{
	case GLUT_KEY_LEFT:
		pacman.setNextVel(DIRECTION::LEFT);
		cout << "Pressed: L" << endl;
		break;
	case GLUT_KEY_RIGHT:
		pacman.setNextVel(DIRECTION::RIGHT);
		cout << "Pressed: R" << endl;
		break;
	case GLUT_KEY_UP:
		pacman.setNextVel(DIRECTION::UP);
		cout << "Pressed: U" << endl;
		break;
	case GLUT_KEY_DOWN:
		pacman.setNextVel(DIRECTION::DOWN);
		cout << "Pressed: D" << endl;
		break;
	default:
		break;
	}
}

int main(int argc, char** argv) {
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(WINDOW_X, WINDOW_Y);
	glutInitWindowSize(WINDOW_W, WINDOW_H);
	glutCreateWindow("Move Pac-Man");

	initialize();

	// register callbacks
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboardDown);
	glutSpecialFunc(specialKeyDown);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}
