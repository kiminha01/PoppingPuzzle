#include "PuyoPair.h"
#include "Puyo.h"
#include <iostream>
#include <ctime>

PuyoPair::PuyoPair() {
	setPuyoPair(Puyo(1,2, 0), Puyo(2,3, 0));
	//setPuyoPair(Puyo(1, 1, 5), Puyo(1, 1, 5));
}



void PuyoPair::setPuyoPair(const Puyo& sph1, const Puyo& sph2) {
	puyo1 = sph1; puyo2 = sph2;
}
const Puyo& PuyoPair::getFirstPuyo() const {
	return puyo1;
}
const Puyo& PuyoPair::getSecondPuyo() const {
	return puyo2;
}
void PuyoPair::move() {
	puyo1.move();
	puyo2.move();
}
void PuyoPair::draw() const {
	puyo1.draw();
	puyo2.draw();
}

void PuyoPair::setpairPlaces(int p) {
	place = p;
}
int PuyoPair::getpairPlaces() const {
	return place;
}
