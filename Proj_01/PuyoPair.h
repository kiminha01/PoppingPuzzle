#pragma once
#include "Puyo.h"
class PuyoPair {
public:
	PuyoPair();
	void setPuyoPair(const Puyo& puyo1, const Puyo& puyo2);
	const Puyo& getFirstPuyo() const;
	const Puyo& getSecondPuyo() const;
	void move();
	void draw() const;
	void setpairPlaces(int p); 
	int getpairPlaces() const;
private:
	Puyo puyo1, puyo2;
	int place;
};
