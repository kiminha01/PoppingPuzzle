#pragma once
#include "Shape3D.h"
#include "Vector3f.h"
#include "Material.h"
//#include "GL/glut.h"
#define WIDTH 960.f
#define HEIGHT 960.f

#define boundaryX (WIDTH/2)
#define boundaryY (HEIGHT/2)

#define gameBoundaryY (boundaryY*360/640.f)
#define PUYOSIZE (gameBoundaryY*1.f/12)

#define gameBoundaryX (PUYOSIZE*6.f)

#define puyoX0 (-gameBoundaryX+0)
#define puyoX1 (-gameBoundaryX+1*PUYOSIZE *2)
#define puyoX2 (-gameBoundaryX+2*PUYOSIZE*2)
#define puyoX3 (-gameBoundaryX+3*PUYOSIZE * 2)
#define puyoX4 (-gameBoundaryX+4*PUYOSIZE * 2)
#define puyoX5 (-gameBoundaryX+5*PUYOSIZE * 2)
#define passiveV (-PUYOSIZE/24.f)

#define maxPopSound 8

#define gameBoardX 6
#define gameBoardY 14
#define minPopCount 4
#define hardDropV 45
#define defaultV 7
#define maxLight 80.f
#define lightZ 30.f
#define keyRepeatFrame 13
#define strokeSpace 2.1f
#define leaderboardSize 10
#define speedUp 0.62f
#define levelScore 20


#define leaderboardFile "leader.txt"
enum puyoColors { empty = 0, red, green, blue, yellow, purple };
enum pairPlaces{right, top, left, bottom};
class Puyo :public Shape3D {
public:
	Puyo();
	Puyo(int c,int s, int Y);
	//Sphere(float r, int sl, int st);
	void setRadius(float r);
	float getRadius() const;
	void setSlice(int sl);
	void setStack(int st);
	int getY() const;
	void setY(int y);
	int getX() const;
	void setX(int x);
	void updateDest();
	bool isAtDest();
	void setColor(int c);
	int getColor() const;
	virtual void draw() const;
	const bool getisMoving() const;
	void setisMoving(bool t) ;
private:
	int x, y;
	int puyoType;
//	float color[3];
	float radius;
	int slice;
	int stack;
	bool isMoving;

};

