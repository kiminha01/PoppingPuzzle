#include <iostream>
#include <string>
#include <ctime>
#include "PuyoPair.h"
#include "Puyo.h"
#include "Light.h"
#include "Stage.h"
#include "Texture.h"
#include <string>
#include <vector>
#include "soloud.h"
#include "soloud_wav.h"
#include <fstream>
#include <time.h>
#include "GL/glut.h"
#include <algorithm>
using namespace std;

const float scaleFactor = 5.f;
void displayStrokeCharacters(void* font, string c, float x, float y,  float size, float r=1.f, float g=1.f, float b=1.f) {
	glPushMatrix();

	glLineWidth(2);

	glPushMatrix();
	glColor3f(1-r, 1-g, 1-b);
	glTranslatef(x-strokeSpace, y,0.f);
	glScalef(1 / size, 1 / size, 1 / size);
	for (int i = 0; i < c.size(); i++)
		glutStrokeCharacter(font, c[i]);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1 - r, 1 - g, 1 - b);
	glTranslatef(x+ strokeSpace, y, 0.f);
	glScalef(1 / size, 1 / size, 1 / size);
	for (int i = 0; i < c.size(); i++)
		glutStrokeCharacter(font, c[i]);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1 - r, 1 - g, 1 - b);
	glTranslatef(x , y- strokeSpace, 0.f);
	glScalef(1 / size, 1 / size, 1 / size);
	for (int i = 0; i < c.size(); i++)
		glutStrokeCharacter(font, c[i]);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1 - r, 1 - g, 1 - b);
	glTranslatef(x , y+ strokeSpace, 0.f);
	glScalef(1 / size, 1 / size, 1 / size);
	for (int i = 0; i < c.size(); i++)
		glutStrokeCharacter(font, c[i]);
	glPopMatrix();

	glColor3f(r, g, b);
	glTranslatef(x , y, 0.f);
	glScalef(1 / size, 1 / size, 1 / size);
	for (int i = 0; i < c.size(); i++)
		glutStrokeCharacter(font, c[i]);
	glPopMatrix();
}
Puyo puyos[2];
Puyo next_puyos[2];
pairPlaces pair_places;
Light light(0, 0, boundaryX, GL_LIGHT0);
Puyo gamespace[gameBoardY][gameBoardX];
bool visited[gameBoardY][gameBoardX];
int adjacentCount[gameBoardY][gameBoardX];
int limit[6] = { 0 };
enum State { down, pause, pop, popping, drop,changeLevel,start,lose,win,board0, board1 };
State now = State::start;
Texture startBackground;
Texture nextBackground;
Texture gameOverBackground;
Texture winBackground;
Texture blackBackground;
Texture level1bg;
Texture level2bg;
Texture level3bg;
int frame = 0;

int score = 0;
int level = 1;
int popNum=0;
int normalKeyArray[256] = { 0 };
int specialKeyArray[256] = { 0 };
clock_t timeNow=clock();

SoLoud::Soloud soloud;
SoLoud::Wav music;
SoLoud::Wav popSounds[maxPopSound];
bool backgroundMusic = true;

string name = "";

vector<pair<string, int>> leaderboard;

void readLeaderboard() {
	leaderboard.clear();
	ifstream file(leaderboardFile);
	string s;
	int i;
	while (file >> s >> i) {
		leaderboard.push_back(make_pair(s, i));
	}
}

void writeLeaderboard() {
	ofstream file(leaderboardFile);
	for (int i = 0; i < leaderboard.size(); i++) {
		file << leaderboard[i].first << " " << leaderboard[i].second << endl;
	}
}

bool addLeaderboard() {
	for (int i = 0; i < leaderboard.size(); i++) {
		if (leaderboard[i].second < score) {
			leaderboard.insert(leaderboard.begin() + i, make_pair("\n", score));
			if (leaderboard.size() > leaderboardSize) {
				leaderboard.pop_back();
			}
			return true;
		}
	}
	if (leaderboard.size() < leaderboardSize) {
		leaderboard.push_back(make_pair("\n", score));
		return true;
	}
	return false;
}

void initializePair() {
	/* Implement */

	srand((unsigned int)time(0));
	int b = puyoColors::blue;
	int a = puyoColors::red;
	Puyo p1 = Puyo(rand() % (b - a + 1) + a,2, 12);
	Puyo p2= Puyo(rand() % (b - a + 1) + a,2, 13);
	Puyo p3 = Puyo(rand() % (b - a + 1) + a, 5, 11);
	Puyo p4 = Puyo(rand() % (b - a + 1) + a, 5, 11);
	p1.setFollowV(defaultV * (1 + (level - 1) * speedUp));
	p2.setFollowV(defaultV * (1 + (level - 1) * speedUp));
	p3.setFollowV(defaultV * (1 + (level - 1) * speedUp));
	p4.setFollowV(defaultV * (1 + (level - 1) * speedUp));
	p1.setVelocity(Vector3f(p1.getVelocity()[0], p1.getVelocity()[1] * (1 + (level - 1) * speedUp), p1.getVelocity()[2]));
	p2.setVelocity(Vector3f(p2.getVelocity()[0], p2.getVelocity()[1] * (1 + (level - 1) * speedUp), p2.getVelocity()[2]));
	p3.setVelocity(Vector3f(p3.getVelocity()[0], p3.getVelocity()[1] * (1 + (level - 1) * speedUp), p3.getVelocity()[2]));
	p4.setVelocity(Vector3f(p4.getVelocity()[0], p4.getVelocity()[1] * (1 + (level - 1) * speedUp), p4.getVelocity()[2]));

	p3.setCenter(Vector3f(p3.getCenter()[0]+2*PUYOSIZE*3, gameBoundaryY -PUYOSIZE*4, PUYOSIZE * 2));
	p4.setCenter(Vector3f(p4.getCenter()[0]+2*PUYOSIZE*3, gameBoundaryY -2*PUYOSIZE, PUYOSIZE * 2));
	puyos[0] = p1;
	puyos[1] = p2;
	next_puyos[0] = p3;
	next_puyos[1] = p4;
	pair_places = pairPlaces::top;
}

void updatePair() {
	int b = puyoColors::green + min(3, level);
	int a = puyoColors::red;
	Puyo p1 = Puyo(rand() % (b - a + 1) + a, 5, 11);
	Puyo p2 = Puyo(rand() % (b - a + 1) + a, 5, 11);
	p1.setFollowV(defaultV * (1 + (level - 1) * speedUp));
	p2.setFollowV(defaultV * (1 + (level - 1) * speedUp));
	p1.setVelocity(Vector3f(p1.getVelocity()[0], p1.getVelocity()[1] * (1 + (level - 1) * speedUp), p1.getVelocity()[2]));
	p2.setVelocity(Vector3f(p2.getVelocity()[0], p2.getVelocity()[1] * (1 + (level - 1) * speedUp), p2.getVelocity()[2]));

	p1.setCenter(Vector3f(p1.getCenter()[0] + 2 * PUYOSIZE * 3, gameBoundaryY  - PUYOSIZE * 4, PUYOSIZE * 2));
	p2.setCenter(Vector3f(p2.getCenter()[0] + 2 * PUYOSIZE * 3, gameBoundaryY - 2 * PUYOSIZE, PUYOSIZE * 2));

	puyos[0] = next_puyos[0];
	puyos[1] = next_puyos[1];
	puyos[0].setX(2);
	puyos[0].setY(12);
	puyos[1].setX(2);
	puyos[1].setY(13);
	puyos[0].updateDest();
	puyos[1].updateDest();
	puyos[0].setCenter(puyos[0].getDest());
	puyos[1].setCenter(puyos[1].getDest());
	next_puyos[0] = p1;
	next_puyos[1] = p2;
	pair_places = pairPlaces::top;
}

// TODO calculatePopPuyo 이후 점수 계산
static void calculateScore() {
	for (int y = 0; y < gameBoardY; y++) {
		for (int x = 0; x < gameBoardX; x++) {
			if (adjacentCount[y][x] >= minPopCount)
				score += adjacentCount[y][x]*popNum*popNum;
		}
		
	}
}
void initializeTexture() {
	startBackground.initTexture("start.jpg", 1);
	nextBackground.initTexture("clear.png", 2);
	gameOverBackground.initTexture("over.png", 3);
	winBackground.initTexture("win.png", 4);
	blackBackground.initTexture("podium2.png", 5);
	level1bg.initTexture("bg1.png",	6);
	level2bg.initTexture("bg2.png", 7);
	level3bg.initTexture("bg3.png", 8);


}

void initialize() {
	light.setAmbient(1.f, 1.f, 1.f, 1.0);
	light.setDiffuse(1.f, 1.f, 1.f, 1.0);
	light.setSpecular(1.0, 1.0, 1.0, 1.0);


	for (int y = 0; y < gameBoardY; y++) {
		for (int x = 0; x < gameBoardX; x++) {
			gamespace[y][x] = Puyo(puyoColors::empty, x, y);
		}
	}
	score = 0;
	memset(limit, 0, sizeof(limit));
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutIgnoreKeyRepeat(true);
	
	
	initializePair();
}

void specialKeyDown(int key, int x, int y) {
	if (key < 0 && key >= 256) {
		return;
	}
	specialKeyArray[key] = keyRepeatFrame;
}

void keyboardDown(unsigned char key, int x, int y) {
	normalKeyArray[key] = keyRepeatFrame;
}

void specialKeyUp(int key, int x, int y) {
	if (key < 0 && key >= 256) {
		return;
	}
	specialKeyArray[key] = 0;
}

void keyboardUp(unsigned char key, int x, int y) {
	normalKeyArray[key] = 0;
}

int recursiveCount(int x, int y, int color) {
	if (visited[y][x]) {
		return 0;
	}
	if (color == puyoColors::empty || color != gamespace[y][x].getColor()) {
		return 0;
	}
	visited[y][x] = true;
	int count = 1;
	if (x > 0) {
		count += recursiveCount(x - 1, y, color);
	}
	if (x < gameBoardX - 1) {
		count += recursiveCount(x + 1, y, color);
	}
	if (y > 0) {
		count += recursiveCount(x, y - 1, color);
	}
	if (y < gameBoardY - 1) {
		count += recursiveCount(x, y + 1, color);
	}
	adjacentCount[y][x] = count;
	return count;
}

void recursiveApply(int x, int y, int color, int n) {
	if (visited[y][x]) {
		return;
	}
	if (color == puyoColors::empty || color != gamespace[y][x].getColor()) {
		return;
	}
	visited[y][x] = true;
	n = max(n, adjacentCount[y][x]);
	adjacentCount[y][x] = n;
	if (x > 0) {
		recursiveApply(x - 1, y, color, n);
	}
	if (x < gameBoardX - 1) {
		recursiveApply(x + 1, y, color, n);
	}
	if (y > 0) {
		recursiveApply(x, y - 1, color, n);
	}
	if (y < gameBoardY - 1) {
		recursiveApply(x, y + 1, color, n);
	}
}

bool checkPop() {
	for (int y = 0; y < gameBoardY; y++) {
		for (int x = 0; x < gameBoardX; x++) {
			if (adjacentCount[y][x] >= minPopCount) {
				return true;
			}
		}
	}
	return false;
}

bool popPuyos() {
	bool popped = false;
	for (int y = 0; y < gameBoardY; y++) {
		for (int x = 0; x < gameBoardX; x++) {
			if (adjacentCount[y][x] >= minPopCount) {
				gamespace[y][x] = Puyo(puyoColors::empty, x, y);
				limit[x]--;
				popped = true;
			}
		}
	}
	return popped;
}

void dropPuyos() {
	for (int y = 1; y < gameBoardY; y++) {
		for (int x = 0; x < gameBoardX; x++) {
			int i = y - 1;
			while (i >= 0 && gamespace[i][x].getColor() == puyoColors::empty) {
				gamespace[i][x] = gamespace[i + 1][x];
				gamespace[i + 1][x] = Puyo(puyoColors::empty, x, y);
				gamespace[i][x].setY(i);
				gamespace[i][x].updateDest();
				i--;
			}
		}
	}
}

// Calculates adjacentCount, then returns whether or not to pop
bool calculatePopPuyo() {
	memset(visited, 0, sizeof(visited));
	memset(adjacentCount, 0, sizeof(adjacentCount));
	for (int y = 0; y < gameBoardY; y++) {
		for (int x = 0; x < gameBoardX; x++) {
			recursiveCount(x, y, gamespace[y][x].getColor());
			
		}
	}
	
	memset(visited, 0, sizeof(visited));
	for (int y = 0; y < gameBoardY; y++) {
		for (int x = 0; x < gameBoardX; x++) {
			recursiveApply(x, y, gamespace[y][x].getColor(), 0);
		}
	}
	return checkPop();
		
}

static void putPuyo(Puyo& puyo, float v) {
	int x = puyo.getX(), y = limit[puyo.getX()]++;
	gamespace[y][x] = puyo;
	gamespace[y][x].setY(y);
	gamespace[y][x].setVelocity(Vector3f(0.f, 0.f, 0.f));
	gamespace[y][x].setFollowV(v);
	gamespace[y][x].updateDest();
}

void keyboardDownHandler(unsigned char key, int x, int y) {
	Puyo& puyo1 = puyos[0];
	Puyo& puyo2 = puyos[1];
	Vector3f center1 = puyo1.getDest();
	Vector3f center2 = puyo2.getDest();
	if (key == 'm') {
		if (backgroundMusic) {
			soloud.stopAudioSource(music);
			backgroundMusic = !backgroundMusic;
		}
		else {
			soloud.play(music);
			backgroundMusic = !backgroundMusic;

		}
	}
	if (now != State::down) {
		return;
	}
	switch (key)
	{
	case ' ':
		if (pair_places == pairPlaces::top) {
			putPuyo(puyo1, hardDropV);
			putPuyo(puyo2, hardDropV);
		}
		else {
			putPuyo(puyo2, hardDropV);
			putPuyo(puyo1, hardDropV);
		}
		now = State::drop;
	}

	//glutPostRedisplay();
}

void specialKeyDownHandler(int key, int x, int y) {
	Puyo& puyo1 = puyos[0];
	Puyo& puyo2 = puyos[1];
	Vector3f center1 = puyo1.getDest();
	Vector3f center2 = puyo2.getDest();
	if (puyo1.getisMoving() && puyo2.getisMoving()) {
		switch (key) {

		case GLUT_KEY_UP:
			switch (pair_places) {

			case pairPlaces::right:

				if (center1[1] - 3 * PUYOSIZE >= limit[puyo2.getX()] * 2 * PUYOSIZE - gameBoundaryY) {
					center2 = center1;
					center2[1] = center1[1] - PUYOSIZE * 2;
					puyo2.setDest(center2);
					puyo2.setX(puyo1.getX());
					pair_places = pairPlaces::bottom;
				}

				break;
			case pairPlaces::bottom:
				if (center1[0] - 3 * PUYOSIZE >= -gameBoundaryX &&
					limit[puyo1.getX() - 1] * 2 * PUYOSIZE - gameBoundaryY < center1[1] - 0.6 * PUYOSIZE) {
					center2 = center1;
					center2[0] = center1[0] - PUYOSIZE * 2;
					puyo2.setDest(center2);
					puyo2.setX(puyo1.getX() - 1);
					pair_places = pairPlaces::left;
				}

				break;
			case pairPlaces::left:
				center2 = center1;
				center2[1] = center1[1] + PUYOSIZE * 2;
				puyo2.setDest(center2);
				puyo2.setX(puyo1.getX());
				pair_places = pairPlaces::top;


				break;

			case pairPlaces::top:
				if (center1[0] + 3 * PUYOSIZE <= gameBoundaryX &&
					limit[puyo1.getX() + 1] * 2 * PUYOSIZE - gameBoundaryY < center1[1] + PUYOSIZE) {
					center2 = center1;
					center2[0] = center1[0] + PUYOSIZE * 2;
					puyo2.setDest(center2);
					puyo2.setX(puyo1.getX() + 1);
					pair_places = pairPlaces::right;
				}

				break;
			}
			break;
		case GLUT_KEY_DOWN:
			if ((center1[1] - 3 * PUYOSIZE >= limit[puyo1.getX()] * 2 * PUYOSIZE - gameBoundaryY) && (center2[1] - 3 * PUYOSIZE >= limit[puyo2.getX()] * 2 * PUYOSIZE - gameBoundaryY)) {
				center1[1] -= PUYOSIZE * 2;
				puyo1.setDest(center1);
				center2[1] -= PUYOSIZE * 2;
				puyo2.setDest(center2);
			}
			if ((center1[1] - 3 * PUYOSIZE < limit[puyo1.getX()] * 2 * PUYOSIZE - gameBoundaryY)) {
				center1[1] = limit[puyo1.getX()] * 2 * PUYOSIZE - gameBoundaryY;
				puyo1.setDest(center1);
			}
			if ((center2[1] - 3 * PUYOSIZE < limit[puyo2.getX()] * 2 * PUYOSIZE - gameBoundaryY)) {
				center2[1] = limit[puyo2.getX()] * 2 * PUYOSIZE - gameBoundaryY;
				puyo2.setDest(center2);
			}
			break;
		case GLUT_KEY_LEFT:

			if ((center1[0] - 3 * PUYOSIZE >= -gameBoundaryX) &&
				(center2[0] - 3 * PUYOSIZE >= -gameBoundaryX) &&
				limit[puyo1.getX() - 1] * 2 * PUYOSIZE - gameBoundaryY < center1[1] - 0.6f * PUYOSIZE &&
				limit[puyo2.getX() - 1] * 2 * PUYOSIZE - gameBoundaryY < center2[1] - 0.6f * PUYOSIZE) {
				center1[0] -= PUYOSIZE * 2;
				puyo1.setDest(center1);
				center2[0] -= PUYOSIZE * 2;
				puyo2.setDest(center2);
				puyo2.setX(puyo2.getX() - 1);
				puyo1.setX(puyo1.getX() - 1);
			}
			break;
		case GLUT_KEY_RIGHT:

			if ((center1[0] + 3 * PUYOSIZE <= gameBoundaryX) &&
				(center2[0] + 3 * PUYOSIZE <= gameBoundaryX) &&
				limit[puyo1.getX() + 1] * 2 * PUYOSIZE - gameBoundaryY < center1[1] - 0.6f * PUYOSIZE &&
				limit[puyo2.getX() + 1] * 2 * PUYOSIZE - gameBoundaryY < center2[1] - 0.6f * PUYOSIZE) {
				center1[0] += PUYOSIZE * 2;
				puyo1.setDest(center1);
				center2[0] += PUYOSIZE * 2;
				puyo2.setDest(center2);
				puyo1.setX(puyo1.getX() + 1);
				puyo2.setX(puyo2.getX() + 1);
			}
			break;
		default:
			break;
		}
	}
}


void goDown() {
	Puyo &puyo1 = puyos[0];
	Puyo &puyo2 = puyos[1];
	Vector3f center1 = puyo1.getDest();
	Vector3f center2 = puyo2.getDest();
	puyo1.move();
	puyo2.move();
	if (center1[1] - puyo1.getRadius() <= limit[puyo1.getX()] * 2 * puyo1.getRadius() - gameBoundaryY
		|| center2[1] - puyo2.getRadius() <= limit[puyo2.getX()] * 2 * puyo2.getRadius() - gameBoundaryY) {
		if (pair_places == pairPlaces::top) {
			putPuyo(puyo1, defaultV);
			putPuyo(puyo2, defaultV);
		}
		else {
			putPuyo(puyo2, defaultV);
			putPuyo(puyo1, defaultV);
		}
		now = State::drop;
	}
}

void drawDown() {
	for (int i = 0; i < gameBoardY; i++) {
		for (int j = 0; j < gameBoardX; j++) {
			if (gamespace[i][j].getColor() != puyoColors::empty) {
				Puyo p = gamespace[i][j];
				p.setDest(Vector3f(-gameBoundaryX + j * 2. * PUYOSIZE + PUYOSIZE, -gameBoundaryY + PUYOSIZE * 2 * i + PUYOSIZE, 0));
				p.draw();
			}
		}
	}
}

Vector3f getCoordinateSum(int x, int y, int color) {
	if (visited[y][x]) {
		return Vector3f(0.f, 0.f, 0.f);
	}
	if (color == puyoColors::empty || color != gamespace[y][x].getColor()) {
		return Vector3f(0.f, 0.f, 0.f);
	}
	visited[y][x] = true;
	Vector3f v = gamespace[y][x].getCenter();
	if (x > 0) {
		v = v + getCoordinateSum(x - 1, y, color);
	}
	if (x < gameBoardX - 1) {
		v = v + getCoordinateSum(x + 1, y, color);
	}
	if (y > 0) {
		v = v + getCoordinateSum(x, y - 1, color);
	}
	if (y < gameBoardY - 1) {
		v = v + getCoordinateSum(x, y + 1, color);
	}
	return v;
}

void drawLights() {
	if (now != State::popping) {
		return;
	}
	memset(visited, 0, sizeof(visited));
	int lights = 1;
	for (int i = 0; i < gameBoardY; i++) {
		for (int j = 0; j < gameBoardX; j++) {
			int color = gamespace[i][j].getColor();
			if (color == puyoColors::empty) {
				continue;
			}
			if (adjacentCount[i][j] < minPopCount) {
				continue;
			}
			Vector3f v = getCoordinateSum(j, i, color);
			if (v.len() < 1E-6f) {
				continue;
			}
			v = (1.f / adjacentCount[i][j]) * v;
			Light l = Light(v[0], v[1], lightZ, GL_LIGHT0 + lights);

			float r = gamespace[i][j].getRadius();
			float brightness = r * (PUYOSIZE - r) * (maxLight / (PUYOSIZE * PUYOSIZE));
			l.setAsMaterial(gamespace[i][j].getMTL(), brightness);
			l.setAttenuation(0.1f);
			l.draw();
			if (++lights >= 8) {
				return;
			}
		}
	}
}

// drops the puyos (gravity), then returns whether drop has finished
bool calculateDrop() {
	bool finished = true;
	for (int y = 0; y < gameBoardY; y++) {
		for (int x = 0; x < gameBoardX; x++) {
			if (gamespace[y][x].getColor() != puyoColors::empty) {
				gamespace[y][x].updateDest();
				if (!gamespace[y][x].isAtDest()) {
					gamespace[y][x].move();
					finished = false;
				}
				else {
					gamespace[y][x].setFollowV(defaultV);
				}
			}
		}
	}
	return finished;
}

// does pop animation and returns whether popping finished
bool popAnimation() {
	bool finished = true;
	for (int y = 0; y < gameBoardY; y++) {
		for (int x = 0; x < gameBoardX; x++) {
			if (adjacentCount[y][x] >= minPopCount) {
				if (gamespace[y][x].getRadius() > 0.f) {
					gamespace[y][x].setRadius(max(gamespace[y][x].getRadius() - (PUYOSIZE * 0.05f), 0.f));
					finished = false;
				}
			}
		}
	}
	if (finished) {
		popPuyos();
		dropPuyos();
		return true;
	}
	return false;
}

void handleInput() {
	for (int i = 0; i < 256; i++) {
		if (normalKeyArray[i] > 0) {
			if (++normalKeyArray[i] > keyRepeatFrame) {
				keyboardDownHandler(i, 0, 0);
				normalKeyArray[i] = 1;
			}
		}
		if (specialKeyArray[i] > 0) {
			if (++specialKeyArray[i] > keyRepeatFrame) {
				specialKeyDownHandler(i, 0, 0);
				specialKeyArray[i] = 1;
			}
		}
	}
}

void leaderboardKeyDown(unsigned char key, int x, int y) {
	if (key == '\b') {
		if (name.length() > 0) {
			name = name.substr(0, name.length() - 1);
		}
	}
	else if ((key == '\r' || key == '\n')&&name.length()>0) {
		now = State::board1;
		for (int i = 0; i < leaderboard.size(); i++) {
			if (leaderboard[i].first == "\n") {
				leaderboard[i].first = name;
			}
		}
		writeLeaderboard();
		glutKeyboardFunc(keyboardDown);
	}
	else {
		if (name.length() < 12&&key!='\r'&&key!=' ') {
			name = name + string(1, key);
		}
	}
}

void idle() {
	
	if (((double)clock() - (double)timeNow) / CLOCKS_PER_SEC >= 1 / 66.f){
		handleInput();
		timeNow = clock();

		if (now == State::start) {
			if (normalKeyArray['s']) {
				now = State::down;
			}
		}
		if (now == State::lose) {
			if (normalKeyArray['r']) {
				initialize();
				now = State::down;
			}
		}
		if (now == State::win) {
			if (frame > 120) {
				readLeaderboard();
				if (addLeaderboard()) {
					now = State::board0;
					glutKeyboardFunc(leaderboardKeyDown);
				}
				else {
					now = State::board1;
				}
			}
		}
		if (now == State::board1) {
			if (normalKeyArray['r']) {
				level = 1;
				score = 0;
				initialize();
				now = State::down;
			}
		}
		if (now == State::down)
			goDown();
		if (now == State::pause) {
			popNum = 0;
			updatePair();
			now = State::down;
			if (score > level * levelScore && level < 3) {
				now = State::changeLevel;
				score = 0;
				level++;
				frame = 0;
			}
			for (int i = 0; i < gameBoardX; i++) {
				if (limit[i] >= 12) {
					if (level == 3 && score >= 3 * levelScore) {
						now = State::win;
						frame = 0;
					}
					else {
						now = State::lose;
						score = 0;
						level = 1;
					}
				}
			}
		}
		if (now == State::changeLevel) {
			if (frame > 120) {
				initialize();
				now = State::drop;
			}
		}
		if (now == State::pop) {
			if (calculatePopPuyo()) {
				now = State::popping;
				soloud.play(popSounds[min(maxPopSound - 1, popNum)]);
				popNum += 1;
				calculateScore();


			}
			else {
				now = State::pause;
				idle();
				return;
			}
		}
		if (now == State::popping) {
			if (popAnimation()) {
				now = State::drop;
			}
		}
		if (now == State::drop) {
			if (calculateDrop()) {
				now = State::pop;
			}
		}
		glutPostRedisplay();

	}
}

void displayGame(){
	// Draw 3D
	if(level==1)
		drawSquareWithTexture(level1bg);
	else if (level == 2)
		drawSquareWithTexture(level2bg);
	else if (level == 3)
		drawSquareWithTexture(level3bg);


	displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, "Score: " + to_string(score),
		-gameBoundaryX - (PUYOSIZE * 2) * 6, -gameBoundaryY + (PUYOSIZE * 2) * 1, scaleFactor);
	displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, "Level: " + to_string(level),
		-gameBoundaryX - (PUYOSIZE * 2) * 6, -gameBoundaryY + (PUYOSIZE * 2) * 1.75f, scaleFactor);
	displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, "Next",
		-gameBoundaryX + 5 * 2 * PUYOSIZE + PUYOSIZE + 2 * PUYOSIZE * 3-1.78*PUYOSIZE,
		gameBoundaryY - 4 * PUYOSIZE - 1.5 * PUYOSIZE-1.75*PUYOSIZE, scaleFactor);
	displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, "m: BGM On/Off",
		gameBoundaryX + PUYOSIZE * 2, -gameBoundaryY + (PUYOSIZE * 2) * 1.75f+1*2*PUYOSIZE, scaleFactor);
	if (level == 3) {
		if (level * levelScore - score > 0) {
			displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN,  "To Win: "+ to_string(max(0, level * levelScore - score)),
				gameBoundaryX + PUYOSIZE * 2, -gameBoundaryY + (PUYOSIZE * 2) * 1.75f, scaleFactor);
		}
		else {
			displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN,  " Go for",
				gameBoundaryX + (float)(PUYOSIZE * 2*1.5), -gameBoundaryY + (PUYOSIZE * 2) * 1.75f, scaleFactor);
			displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, "Highscore!",
				gameBoundaryX + PUYOSIZE * 2, -gameBoundaryY + (PUYOSIZE * 2) * 1.75f-2*PUYOSIZE, scaleFactor);
		}
	}
	else
	displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, "To Next: " + to_string(max(0, level * levelScore - score)),
		gameBoundaryX+PUYOSIZE*2  , -gameBoundaryY + (PUYOSIZE * 2) * 1.75f, scaleFactor);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(light.getID());

	light.draw();
	drawLights();
	drawBackground();
	//drawNextStage();
	next_puyos[0].draw();
	next_puyos[1].draw();
	drawHideCube();
	if (now == State::down) {
		puyos[0].draw();
		puyos[1].draw();
	}
	/* Implement: draw a pair of spheres */
	drawDown();

	for (int i = 0; i < 8; i++) {
		glDisable(GL_LIGHT0 + i);
	}
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	drawStage();
}

void display() {
	frame++;
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (now == State::start) {
		drawBackgroundWithTexture(startBackground);
		if ((frame % 60) < 30) {
			displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, "Press S",
				-80, -270, scaleFactor);
		}
	}
	else if (now == State::changeLevel) {
		drawBackgroundWithTexture(nextBackground);
	}
	else if (now == State::lose) {
		drawBackgroundWithTexture(gameOverBackground);
		if ((frame % 60) < 30) {
			displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, "Press R",
				-80, -270, scaleFactor);
		}
	}
	else if (now == State::win) {
		drawBackgroundWithTexture(winBackground);
		displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, "Score: " + to_string(score),
			-110, -290, scaleFactor);
	}
	else if (now == State::board0) {
		drawBackgroundWithTexture(blackBackground);
		displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, "Leaderboard",
			-110, 300, scaleFactor);
		for (int i = 0; i < leaderboard.size(); i++) {
			displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, to_string(i+1),
				-300, 250 - i * 50, scaleFactor);
			displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, leaderboard[i].first == "\n" ? name : leaderboard[i].first,
				-200, 250 - i * 50, scaleFactor);
			displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, to_string(leaderboard[i].second),
				100, 250 - i * 50, scaleFactor);
		}
		if ((frame % 60) < 30) {
			displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, "Enter your name!",
				-140, -300, scaleFactor);
		}
	}
	else if (now == State::board1) {
		drawBackgroundWithTexture(blackBackground);
		displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, "Leaderboard",
			-110, 300, scaleFactor);
		for (int i = 0; i < leaderboard.size(); i++) {
			displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, to_string(i+1),
				-300, 250 - i * 50, scaleFactor);
			displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, leaderboard[i].first,
				-200, 250 - i * 50, scaleFactor);
			displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, to_string(leaderboard[i].second),
				100, 250 - i * 50, scaleFactor);
		}
		if ((frame % 60) < 30) {
			displayStrokeCharacters(GLUT_STROKE_MONO_ROMAN, "Press R to restart",
				-140, -300, scaleFactor);
		}
	}
	else {
		displayGame();
	}

	glutSwapBuffers();

}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);

	float wRatio = (float)w / WIDTH;
	float hRatio = (float)h / HEIGHT;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-boundaryX * wRatio, boundaryX * wRatio, -boundaryY * hRatio, boundaryY * hRatio, -100.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv) {
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(350, 50);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("PuyoPuyo_final");
	soloud.init();
	music.load("music.mp3");
	for (int i = 0; i < maxPopSound; i++) {
		popSounds[i].load(("pop1 (" + to_string(i + 1) + ").mp3").c_str());
	}
	music.setLooping(true);
	soloud.play(music);
	initializeTexture();
	initialize();

	// register callbacks
	glutKeyboardFunc(keyboardDown);
	glutSpecialFunc(specialKeyDown);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialUpFunc(specialKeyUp);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	timeNow = clock();
	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}
