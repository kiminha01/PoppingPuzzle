#include "Puyo.h"
#include "Stage.h"
void drawStage(){
	glColor3f(1., 1., 1.);

	glLineWidth(1.5f);
	glBegin(GL_LINES);
	glVertex2f(-gameBoundaryX, gameBoundaryY);// A
	glVertex2f(-gameBoundaryX, -gameBoundaryY); // B

	glVertex2f(-gameBoundaryX, -gameBoundaryY); // B
	glVertex2f(gameBoundaryX, -gameBoundaryY); // C

	glVertex2f(gameBoundaryX, -gameBoundaryY); // C
	glVertex2f(gameBoundaryX, gameBoundaryY) ;// D

	glVertex2f(gameBoundaryX, gameBoundaryY);// D
	glVertex2f(-gameBoundaryX, gameBoundaryY);// A
	glVertex2f(-gameBoundaryX + 5 * 2 * PUYOSIZE + PUYOSIZE + 2 * PUYOSIZE * 3 - PUYOSIZE * 1.5,
		gameBoundaryY  - 2 * PUYOSIZE + PUYOSIZE * 1.5);// A
	glVertex2f(-gameBoundaryX + 5 * 2 * PUYOSIZE + PUYOSIZE + 2 * PUYOSIZE * 3 - PUYOSIZE * 1.5,
		gameBoundaryY  - 4 * PUYOSIZE - 1.5 * PUYOSIZE); // B

	glVertex2f(-gameBoundaryX + 5 * 2 * PUYOSIZE + PUYOSIZE + 2 * PUYOSIZE * 3 - PUYOSIZE * 1.5,
		gameBoundaryY  - 4 * PUYOSIZE - 1.5 * PUYOSIZE); // B
	glVertex2f(-gameBoundaryX + 5 * 2 * PUYOSIZE + PUYOSIZE + 2 * PUYOSIZE * 3 + 1.5 * PUYOSIZE,
		gameBoundaryY  - 4 * PUYOSIZE - 1.5 * PUYOSIZE); // C

	glVertex2f(-gameBoundaryX + 5 * 2 * PUYOSIZE + PUYOSIZE + 2 * PUYOSIZE * 3 + 1.5 * PUYOSIZE,
		gameBoundaryY  - 4 * PUYOSIZE - 1.5 * PUYOSIZE);// C
	glVertex2f(-gameBoundaryX + 5 * 2 * PUYOSIZE + PUYOSIZE + 2 * PUYOSIZE * 3 + 1.5 * PUYOSIZE,
		gameBoundaryY  - 2 * PUYOSIZE + PUYOSIZE * 1.5);// D

	glVertex2f(-gameBoundaryX + 5 * 2 * PUYOSIZE + PUYOSIZE + 2 * PUYOSIZE * 3 + 1.5 * PUYOSIZE,
		gameBoundaryY- 2 * PUYOSIZE + PUYOSIZE * 1.5);// D
	glVertex2f(-gameBoundaryX + 5 * 2 * PUYOSIZE + PUYOSIZE + 2 * PUYOSIZE * 3 - PUYOSIZE * 1.5,
		gameBoundaryY - 2 * PUYOSIZE + PUYOSIZE * 1.5);// A
	//glDisable(GL_COLOR);
	glEnd();
}
void drawBackground() {
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 6; j++) {
			Material mtl;
			mtl.setEmission(0.f, 0.f, 0.f, 0.f);
			if ((i + j) % 2 == 0) {
				mtl.setAmbient(0.15f, 0.15f, 0.15f, 1.f);
				mtl.setDiffuse(0.15f, 0.15f, 0.15f, 1.f);
				mtl.setSpecular(0.6f, 0.6f, 0.6f, 1.f);
				mtl.setShininess(3.f);
			}
			else {
				mtl.setAmbient(0.1f, 0.1f, 0.1f, 1.f);
				mtl.setDiffuse(0.1f, 0.1f, 0.1f, 1.f);
				mtl.setSpecular(0.2f, 0.2f, 0.2f, 1.f);
				mtl.setShininess(0.f);
			}
			glShadeModel(GL_SMOOTH);
			glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission());
			glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient());
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse());
			glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular());
			glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

			glPushMatrix();
			float center[2] = { -gameBoundaryX + PUYOSIZE * 2. * j + PUYOSIZE,-gameBoundaryY + PUYOSIZE * 2. * i + PUYOSIZE };
			glTranslatef(center[0], center[1], -PUYOSIZE - 2);

			glutSolidCube(PUYOSIZE * 2);
			glPopMatrix();
		}
	}

}

void drawHideCube() {
	Material mtl;
	mtl.setEmission(0.f, 0.f, 0.f, 0.f);
	mtl.setAmbient(0.f, 0.f, 0.f, 1.f);
	mtl.setDiffuse(0.f, 0.f, 0.f, 1.f);
	mtl.setSpecular(0.f, 0.f, 0.f, 1.f);
	mtl.setShininess(0.f);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission());
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

	float center[2] = { 0, gameBoundaryX + gameBoundaryY };

	glPushMatrix();
	glTranslatef(center[0], center[1], PUYOSIZE - gameBoundaryX);

	glutSolidCube(gameBoundaryX * 2);
	glPopMatrix();
}