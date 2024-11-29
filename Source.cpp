#include <iostream>
#include <vector>

#include "Agent.h"
#include "Light.h"
#include "Ghosts.h"
#include "Map.h"
#include "SpecialCase.h"
#include <GL/freeglut.h>
#include "Constants.h"
#include <string>
#include <fstream>

using namespace std;

const int FPS = FPS_SET;
int sTime = 0;
int eTime = 0;

Pacman pacman;
Vector3f pacPos(0.f,0.f,0.f);
Vector3i pacVel(0, 0, 0);
Vector3f blinkyPos(0.f, 0.f, 0.f);

Pinky pinky;
Blinky blinky;
Inky inky;
Clyde clyde;

std::vector<Ghost*> ghosts;

std::vector<std::string> lines;

enum class GAME_STATE {Normal, Respawn, GameOver, GameClear};

GAME_STATE game_state = GAME_STATE::Normal;
//GAME_STATE game_state = GAME_STATE::GameClear;

int PacmanRespawnCnt = 0;
const int PacmanMaxRespawnCnt = 2 * FPS_SET;

int EndCnt = 0;
const int MaxEndCnt = 316;

//bool bIsGameOver = false;
bool isPow = false;

bool bstart = true; // for debug

Map map;

Light light(BOUNDARY_X, BOUNDARY_Y, BOUNDARY_X / 2, GL_LIGHT0);
//Texture texture("snu.png");

void readFile(const char* filename) {
	std::ifstream file(filename);
	if (!file) {
		std::cerr << "파일을 열 수 없습니다: " << filename << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string line;
	while (std::getline(file, line)) {
		lines.push_back(line); // 한 줄씩 읽어서 벡터에 저장
	}
	file.close();
}

void initialize() {
	map.MapInitializer("reflected_pacman_map.csv"); //load map
	readFile("end.txt");

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
	pacman.setIndexPosition(13, 7);
	pacman.setVel(0.0f, 0.0f, 0.0f);
	pacman.setMTL(mtlPac);

	pacPos = pacman.getPos();
	//=======================================================
	// Ghost(s)
	//
	// Blinky
	Material mtlG1;
	mtlG1.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
	mtlG1.setAmbient(0.6f, 0.0f, 0.0f, 1.0f);
	mtlG1.setDiffuse(0.8f, 0.0f, 0.0f, 1.0f);
	mtlG1.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtlG1.setShininess(30.0f);

	blinky.setMTL(mtlG1);
	blinky.setIndexPosition(16, 19);//26, 29
	blinky.setVel(1.f, 0.f, 0.f);

	// Pinky
	Material mtlG2;
	mtlG2.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
	mtlG2.setAmbient(0.6f, 0.0f, 0.6f, 1.0f);
	mtlG2.setDiffuse(0.8f, 0.0f, 0.8f, 1.0f);
	mtlG2.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtlG2.setShininess(30.0f);

	pinky.setMTL(mtlG2);
	pinky.setIndexPosition(11, 15);
	pinky.setVel(0.f, 1.f, 0.f);
	
	// Inky
	Material mtlG3;
	mtlG3.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
	mtlG3.setAmbient(0.0f, 0.6f, 0.6f, 1.0f);
	mtlG3.setDiffuse(0.0f, 0.8f, 0.8f, 1.0f);
	mtlG3.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtlG3.setShininess(30.0f);

	inky.setMTL(mtlG3);
	inky.setIndexPosition(13, 17);
	inky.setVel(0.f, -1.f, 0.f);

	// Clyde
	Material mtlG4;
	mtlG4.setEmission(0.2f, 0.2f, 0.2f, 1.0f);
	mtlG4.setAmbient(0.7286f, 0.4714f, 0.0f, 1.0f); //0.7286f, 0.4714f, 0.0f, 1.0f
	mtlG4.setDiffuse(0.9714f, 0.6285f, 0.0f, 1.0f); //0.9714f, 0.6285f, 0.0f, 1.0f
	mtlG4.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtlG4.setShininess(30.0f);

	clyde.setMTL(mtlG4);
	clyde.setIndexPosition(15, 15);
	clyde.setVel(0.f, 1.f, 0.f);
	//=======================================================
	// Handle all Ghosts at once
	ghosts.push_back(&blinky);
	ghosts.push_back(&pinky);
	ghosts.push_back(&inky);
	ghosts.push_back(&clyde);

	//=======================================================
	// Map
	//
	Material mtlWall;
	mtlWall.setEmission(0.1f, 0.1f, 0.4f, 1.0f);
	mtlWall.setAmbient(0.0f, 0.0f, 0.2f, 1.0f);
	mtlWall.setDiffuse(0.1f, 0.1f, 0.7f, 1.0f);
	mtlWall.setSpecular(0.1f, 0.1f, 0.1f, 1.0f);
	mtlWall.setShininess(10.0f);

	Material mtlPellet;
	mtlPellet.setEmission(0.3f, 0.3f, 0.3f, 1.0f);
	mtlPellet.setAmbient(0.8f, 0.8f, 0.8f, 1.0f);
	mtlPellet.setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	mtlPellet.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtlPellet.setShininess(100.0f);

	Material mtlPower;
	mtlPower.setEmission(1.0f, 1.0f, 1.0f, 1.0f);
	mtlPower.setAmbient(0.8f, 0.8f, 0.8f, 1.0f);
	mtlPower.setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	mtlPower.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	mtlPower.setShininess(100.0f);
	
	map.setMtl_wall(mtlWall);
	map.setMtl_pellet(mtlPellet);
	map.setMtl_power(mtlPower);

	
}

void resetStage() {
	pacman.setIndexPosition(13, 7);
	pacman.setVel(0.0f, 0.0f, 0.0f);
	pacPos = pacman.getPos();

	blinky.setIndexPosition(16, 19);
	blinky.setVel(1.f, 0.f, 0.f);

	pinky.setIndexPosition(11, 15);
	pinky.setVel(0.f, 1.f, 0.f);
	pinky.respawnTick = 5 * FPS_SET;
	pinky.bisRespawn = true;

	inky.setIndexPosition(13, 17);
	inky.setVel(0.f, -1.f, 0.f);
	inky.respawnTick = 10 * FPS_SET;
	inky.bisRespawn = true;

	clyde.setIndexPosition(15, 15);
	clyde.setVel(0.f, 1.f, 0.f);
	clyde.respawnTick = 15 * FPS_SET;
	clyde.bisRespawn = true;
}

void idle() {
	float frameDuration = 1000.0f / FPS; //  1000.0f / FPS
	eTime = glutGet(GLUT_ELAPSED_TIME);

	if (eTime - sTime > frameDuration) {
		/* Implement: update direction and move Agents */

		if (MaxEndCnt == EndCnt) game_state = GAME_STATE::GameClear;

		switch (game_state) {
		
		case(GAME_STATE::Normal): {
			//========================================
			// Pacman Move handle
			//
			// If Pacman is Not respawn (normal state)
			if (!pacman.isRespawn()) {
				if (pacman.isIndexPositionUpdated()) {
					cout << "called update vel" << endl;
					pacman.updateVel();
				}
				else {}
				pacman.move();
			}
			// If Pacman is at Respawn
			else {
				// code for Pacman respawn state
			}

			blinkyPos = blinky.getPos(); // this is for Inky path finding

			for (Ghost* ghost : ghosts) {
				if (!ghost->isRespawn()) {
					if ((ghost->isIndexPositionUpdated() || ghost->getVel() == Vector3f())) {
						cout << "called update vel" << endl;
						ghost->updateVel();
					}
					else {}
					ghost->move();

					if ((pacPos - ghost->getPos()).norm() < (BLOCK_SIZE / 3.f * 2.f)) {
						// if Pacman catch Ghost
						if (isPow) {
							ghost->setIsRespawn(true);
							pacman.increasePoint(50);
						}
						else {
							// 팩맨 사망 처리
							/////////////////////////////////
							pacman.decreaseLifes();
							if (pacman.getLifes() > 0) game_state = GAME_STATE::Respawn;
							else game_state = GAME_STATE::GameOver;
							PacmanRespawnCnt = PacmanMaxRespawnCnt;
							resetStage();

							break;
						}
					}
				}
				// If ghost is at Respawn
				else {
					// code for ghost respawn state
					ghost->move();
				}
			}
			break;
		}

		case(GAME_STATE::Respawn): {
			PacmanRespawnCnt--;
			if (PacmanRespawnCnt == 0) game_state = GAME_STATE::Normal;
			break;
		}
		case(GAME_STATE::GameOver): {
			break;
		}
		case(GAME_STATE::GameClear): {
			break;
		}
		}

		sTime = eTime;
		glutPostRedisplay();
	}
}

void displayText(float x, float y, void* font, const char* text) {
	glRasterPos2f(x, y); // Set the text position
	for (const char* c = text; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void displayText(float x, float y, void* font, string str) {
	glRasterPos2f(x, y); // Set the text position
	for (char c : str) {
		glutBitmapCharacter(font, c);
	}
}

void displayBitmapText(float x, float y, const char* text) {
	glRasterPos2f(x, y); // 출력 위치 설정
	for (const char* c = text; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c); // 비트맵 문자 출력
		//glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
	}
}

void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (game_state) {

	case (GAME_STATE::Normal): {
		bool orthogonalView = false;
		bool tiltView = true;
		if (orthogonalView) {
			if (!tiltView) {
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y, -100.0, 100.0);
				//gluOrtho2D(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
			}

			else {
				// Set up the orthogonal projection
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y, -100.0, 100.0);
				// gluOrtho2D(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();

				// Apply a rotation of 15 degrees downward about the X-axis
				glRotatef(-30.0, 1.0, 0.0, 0.0);
			}
		}
		else {
			// Set up the perspective projection
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			//gluPerspective(45.0, (double)BOUNDARY_X / (double)BOUNDARY_Y*3, 0.1, BLOCK_SIZE*400.0);
			gluPerspective(35.0, (double)BOUNDARY_X / (double)BOUNDARY_Y * 5, 0.1, BLOCK_SIZE * 200.0);
			// Switch to the modelview matrix and set the camera
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			// Position the camera
			gluLookAt(
				0.0, 0.0, BLOCK_SIZE * 60.f,  // Camera position (x, y, z)
				0.0, 0.0, 0.0,   // Look at point (center x, y, z)
				0.0, 1.0, 0.0    // Up vector (x, y, z)
			);

			// Apply a rotation to tilt the view down 15 degrees
			glRotatef(-30.0, 1.0, 0.0, 0.0);
		}

		// Draw 3D
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(light.getID());
		light.draw();

		map.draw();

		pacman.draw();

		for (Ghost* ghost : ghosts) { // draw Ghost(s)
			ghost->draw();
		}

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(light.getID());

		// Bitmap draw
		glColor3f(1.0, 1.0, 1.0); // White color

		string text3 = "LIFES :";
		string text4 = std::to_string(pacman.getLifes());
		
		displayText(-200.f, 200.0f, GLUT_BITMAP_TIMES_ROMAN_24, text3 + text4);

		string text1 = "SCORE :";
		string text2 = std::to_string(pacman.getPoint());
		displayText(-200.f, 150.0f, GLUT_BITMAP_TIMES_ROMAN_24, text1 + text2);
		break;
	}

	case(GAME_STATE::Respawn): {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y, -100.0, 100.0);
		//gluOrtho2D(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glColor3f(1.0, 1.0, 1.0); // White color

		displayText(-90.f, 50.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Caught by Ghost");

		displayText(-90.f, -100.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Respawn In 2 sec");
		break;
	}

	case (GAME_STATE::GameOver): {// Game Over Screen
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y, -100.0, 100.0);
		//gluOrtho2D(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glColor3f(1.0, 1.0, 1.0); // White color

		displayText(-75.f, 50.0f, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER");
		
		string text1 = "SCORE : ";
		string text2 = std::to_string(pacman.getPoint());
		displayText(-75.f, -50.0f, GLUT_BITMAP_TIMES_ROMAN_24, text1 + text2);
		break;
		}
	case (GAME_STATE::GameClear): {// Game Clear Screen
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-BOUNDARY_X, BOUNDARY_X, -BOUNDARY_Y, BOUNDARY_Y, -100.0, 100.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glColor3f(1.0, 1.0, 1.0); // White color
		float y = 280.f; // 텍스트 출력 시작 위치 (y)
		for (const auto& line : lines) {
			displayBitmapText(-250.f, y, line.c_str()); // 각 줄을 출력
			y -= 13.f; // 다음 줄로 이동
		}

		string text1 = "SCORE : ";
		string text2 = std::to_string(pacman.getPoint());
		displayText(110.f, 150.0f, GLUT_BITMAP_TIMES_ROMAN_24, text1 + text2);
		displayText(130.f, 90.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Merry");
		displayText(110.f, 60.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Christmas!");
		break;
	}
	}

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
	glutInitWindowPosition(WINDOW_X, WINDOW_Y-150);
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
