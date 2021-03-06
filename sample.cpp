﻿// The sample model.  You should build a file
// very similar to this for when you make your model.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <GL/glu.h>
#include "modelerglobals.h"
#include "metaball.h"

// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView 
{
public:
    SampleModel(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) { }

    virtual void draw();
};

void prepareTexture() {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load("oldFur.bmp", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{ 
    return new SampleModel(x,y,w,h,label); 
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
void SampleModel::draw()
{
    // This call takes care of a lot of the nasty projection 
    // matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
    ModelerView::draw();
	prepareTexture();
	// draw the floor
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_RED);
	glPushMatrix();
	glTranslated(-5,-4,-5);
	drawBox(10,0.01f,10);
	glPopMatrix();

	glEnable(GL_LIGHT0);
	if (VAL(LIGHTING) > 1) {
		glEnable(GL_LIGHT1);
		if (VAL(LIGHTING > 2))glEnable(GL_LIGHT2); else glDisable(GL_LIGHT2);;
	}
	else {
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
	}
	
	
	
	// ÙpperBody
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_WHITE);
	glPushMatrix();
	glRotated(VAL(ROTATE), 0.0, 1.0, 0.0);
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
	glTranslated(0, VAL(HEIGHT), 0);
	glScaled(1, 1, 1);

	if (VAL(DETAIL) > 1){
	glPushMatrix();
		//Torso
		glTranslated(-1, 1, 0);
		glTranslated(-0.125, -1.5, -0.125);
		glTranslated(0, -2.5, -0.3);
		glTranslated(0, -VAL(HEIGHT) / 2, 0);
		glTranslated(0, VAL(PELVIS) / 90.0 * 3, 0);
		glRotated(VAL(PELVIS), 1.0, 0.0, 0.0);
		drawBox(2.8125, 2.55+VAL(HEIGHT)/2, 2.8125);
		if (VAL(DETAIL) > 2) {
		glPushMatrix();
			//Tail
			glTranslated(1.45, 1, 0.0);
			drawSphere(0.45);
			//Hula Hoop
			setDiffuseColor(COLOR_RED);
			glTranslated(0, 0.5, 2);
			if (VAL(MOOD) == 2) {
				drawTorus(0, VAL(TORUS_INNER), VAL(TORUS_XSUB), VAL(TORUS_YSUB));
			}
			else drawTorus(VAL(TORUS_OUTTER), VAL(TORUS_INNER), VAL(TORUS_XSUB), VAL(TORUS_YSUB));
			setDiffuseColor(COLOR_WHITE);
		glPopMatrix();
		glPushMatrix();
			//Legs
			glTranslated(0, -VAL(HEIGHT) / 2, 0);
			glRotated(-VAL(PELVIS), 1.0, 0.0, 0.0);
			glTranslated(0, -VAL(PELVIS) / 90.0 * 3, 0);
			glTranslated(0.7, 1, 1.25);
			glRotated(90, 1.0, 0.0, 0.0);
			drawCylinder(2 + VAL(HEIGHT) / 2, 0.5, 0.5);
			glTranslated(1.35, 0, 0);
			drawCylinder(2 + VAL(HEIGHT) / 2, 0.5, 0.5);
		glPopMatrix();
		glPushMatrix();
			//Chest
			glTranslated(0, VAL(HEIGHT)/2, 0);
			glTranslated(0, 2.5, 0.25);
			drawBox(2.8125, 1.5, 2.25);
			if (VAL(DETAIL) > 3) {
			glPushMatrix();
				//Hand
				glRotated(160, 0.0, 0.0, 1.0);
				glTranslated(-0.2, -1.5, 0.25);
				glRotated(VAL(LEFT_HAND_ROTATE), 0.0, 0.0, 1.0);
				drawBox(1, 2.75, 1);
				glRotated(-VAL(LEFT_HAND_ROTATE), 0.0, 0.0, 1.0);
				glRotated(-320, 0.0, 0.0, 1.0);
				glTranslated(-2.5, 0.3, 0);
				if (VAL(MOOD) == 2) glRotated(22.5, 0.0, 0.0, 1.0);
				else if (VAL(MOOD) == 3) glRotated(45, 0.0, 0.0, 1.0);
				else glRotated(VAL(RIGHT_HAND_ROTATE), 0.0, 0.0, 1.0);
				drawBox(1, 2.75, 1);
				if (VAL(DETAIL) > 4) {
					glPushMatrix();
					//Dumbbell
					setDiffuseColor(COLOR_DARKBLUE);
					glTranslated(-0.5, 2.5, 0);
					glTranslated(0.7, 0.5, -0.5);
					drawBox(0.5, 0.5, 1.75);
					glTranslated(-0.7, -0.5, 0.5);
					MetaBall* tempBall = new MetaBall();
					if (VAL(MOOD) == 2) {
						tempBall->addBall(1, 0.8, -0.4+1, 0.7);
						tempBall->addBall(1, 0.8, 1.3 + -1, 0.7);
					}
					else if (VAL(MOOD) == 3) {
						tempBall->addBall(1, 0.8, -1.1, 0.7);
						tempBall->addBall(1, 0.8, 2.0, 0.7);
					}
					else {
						tempBall->addBall(1, 0.8, -0.4 - VAL(DUMBBELL), 0.7);
						tempBall->addBall(1, 0.8, 1.3 + VAL(DUMBBELL), 0.7);
					}
					tempBall->draw(1.0);				
					delete tempBall;
					glTranslated(0, -2.75, 0);
					setDiffuseColor(COLOR_WHITE);
				glPopMatrix();
				if (VAL(MOOD) == 2) glRotated(-22.5, 0.0, 0.0, 1.0);
				else glRotated(-VAL(RIGHT_HAND_ROTATE), 0.0, 0.0, 1.0);
				
			glPopMatrix();
			glPushMatrix();
				//Head
				glTranslated(0, VAL(HEAD) / 90.0 * 2, 0);
				glRotated(VAL(HEAD), 1.0, 0.0, 0.0);
				glTranslated(0.15, 1.5, 0);
				drawBox(2.5, 2.5, 2);
				glPushMatrix();
					if (VAL(DETAIL) > 5) {
					//Face
					glTranslated(0.5, 0.2, 0.75);
					drawBox(1.6, 1.2, 2);
					glPushMatrix();
						//Nose
						setDiffuseColor(COLOR_BLACK);
						glTranslated(0.8, 1.1, 2.0);
						drawSphere(0.25);
					glPopMatrix();
					glPushMatrix();
						glTranslated(0.5, 0.3, 2.0);
						drawBox(0.5, 0.1, 0.1);
						//Lips
						if (VAL(MOOD) == 2 ){
						glPushMatrix();
							glTranslated(-0.05, 0.0, 0);
							glRotated(45, 0.0, 0, 1.0);
							drawBox(0.1, 0.3, 0.1);
							glRotated(-45, 0.0, 0, 1.0);
							glTranslated(0.55, 0.05, 0);
							glRotated(-45, 0.0, 0, 1.0);
							drawBox(0.1, 0.3, 0.1);
						glPopMatrix();
						}
					glPopMatrix();
					}
				glPopMatrix();
				glPushMatrix();
					//Ear
					setDiffuseColor(COLOR_WHITE);
					glTranslated(0.3, 2.5, 1.0);
					drawSphere(0.35);
					glTranslated(1.9, 0, 0);
					drawSphere(0.35);
				glPopMatrix();
				glPushMatrix();
					//Eyes
					setDiffuseColor(COLOR_BLACK);
					glTranslated(0.6, 1.75, 2);
					drawSphere(0.15);
					glTranslated(1.3, 0, 0);
					drawSphere(0.15);
					if (VAL(MOOD) == 3){
					glPushMatrix();
						//Eyelash
						glTranslated(-0.1, 0.10, 0);
						glRotated(-70, 0, 0, 1);
						drawBox(0.1, 0.45, 0.1);
						glRotated(70, 0, 0, 1);
						glTranslated(-1.1, -0.1, 0);
						glRotated(70, 0, 0, 1);
						drawBox(0.1, 0.45, 0.1);
					glPopMatrix();
					}
				glPopMatrix();
				}
			glPopMatrix();
			}
		glPopMatrix();
		glTranslated(4.0, 6.5, 0);
		if (VAL(MOOD) == 3) {
			setDiffuseColor(COLOR_RED);
			drawStar(4, 1, 0.3);

		}
		else {
		setDiffuseColor(COLOR_YELLOW);
		drawStar(5, 1,0.3);
		}
		}
	
	}
	
	
	glPopMatrix();
	glPopMatrix();


}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];
    controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
    controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
    controls[HEIGHT] = ModelerControl("Height", 0, 2.5, 0.1, 0);
	controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);
	controls[LEFT_HAND_ROTATE] = ModelerControl("Left Hand Rotate", -70, 20, 1, 0);
	controls[RIGHT_HAND_ROTATE] = ModelerControl("Right Hand Rotate", -20, 70, 1, 0);
	controls[PELVIS] = ModelerControl("Pelvis Rotate", 0, 90, 1, 0);
	controls[HEAD] = ModelerControl("Head Rotate", -25, 25, 1, 0);
	controls[DUMBBELL] = ModelerControl("Dumbbell Transformation", -1, 0.7, 0.1f, 0);
	controls[TORUS_INNER] = ModelerControl("Hula Hoop Inner Radius", 0.1f, 2.0f, 0.1f, 0.2f);
	controls[TORUS_OUTTER] = ModelerControl("Hula Hoop Outter Radius", 0.0f, 5.0f, 0.5f, 4.0f);
	controls[TORUS_XSUB] = ModelerControl("Hula Hoop X-Subdivisions", 3, 45, 1, 30.0f);
	controls[TORUS_YSUB] = ModelerControl("Hula Hoop Y-Subdivisions", 3, 30, 1, 10.0f);
	controls[LIGHTING] = ModelerControl("Number of Light", 1, 3, 1, 2);
	controls[DETAIL] = ModelerControl("Detail", 1, 6, 1, 6);
	//2 = Happy 3 = WorkHard
	controls[MOOD] = ModelerControl("Mood Cycling", 1, 3, 1, 1);
	

	ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}
