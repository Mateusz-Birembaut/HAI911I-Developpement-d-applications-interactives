#ifndef SphereSelectionTool_H
#define SphereSelectionTool_H
#include "Vec3.h"

#include <GL/glut.h>
#include <GL/gl.h>


void drawSphere1(float x, float y, float z, float radius, int slices, int stacks)
{
    if (stacks < 2) { stacks = 2; }
    if (stacks > 30) { stacks = 30; }
    if (slices < 3) { slices = 3; }
    if (slices > 30) { slices = 30; }
    // Pas essentiel ...

    int Nb = slices * stacks + 2;
    std::vector<Vec3> points(Nb);

    Vec3 centre(x, y, z);

    float sinP, cosP, sinT, cosT, Phi, Theta;
    points[0] = Vec3(0, 0, 1);
    points[Nb - 1] = Vec3(0, 0, -1);

    for (int i = 1; i <= stacks; i++)
    {
        Phi = 90 - (float)(i * 180) / (float)(stacks + 1);
        sinP = sinf(Phi * 3.14159265 / 180);
        cosP = cosf(Phi * 3.14159265 / 180);

        for (int j = 1; j <= slices; j++)
        {
            Theta = (float)(j * 360) / (float)(slices);
            sinT = sinf(Theta * 3.14159265 / 180);
            cosT = cosf(Theta * 3.14159265 / 180);

            points[j + (i - 1) * slices] = Vec3(cosT * cosP, sinT * cosP, sinP);
        }
    }

    int k1, k2;
    glBegin(GL_TRIANGLES);
    for (int i = 1; i <= slices; i++)
    {
        k1 = i;
        k2 = (i % slices + 1);
        glNormal3f(points[0][0], points[0][1], points[0][2]);
        glVertex3f((centre + radius * points[0])[0], (centre + radius * points[0])[1], (centre + radius * points[0])[2]);
        glNormal3f(points[k1][0], points[k1][1], points[k1][2]);
        glVertex3f((centre + radius * points[k1])[0], (centre + radius * points[k1])[1], (centre + radius * points[k1])[2]);
        glNormal3f(points[k2][0], points[k2][1], points[k2][2]);
        glVertex3f((centre + radius * points[k2])[0], (centre + radius * points[k2])[1], (centre + radius * points[k2])[2]);

        k1 = (stacks - 1) * slices + i;
        k2 = (stacks - 1) * slices + (i % slices + 1);
        glNormal3f(points[k1][0], points[k1][1], points[k1][2]);
        glVertex3f((centre + radius * points[k1])[0], (centre + radius * points[k1])[1], (centre + radius * points[k1])[2]);
        glNormal3f(points[Nb - 1][0], points[Nb - 1][1], points[Nb - 1][2]);
        glVertex3f((centre + radius * points[Nb - 1])[0], (centre + radius * points[Nb - 1])[1], (centre + radius * points[Nb - 1])[2]);
        glNormal3f(points[k2][0], points[k2][1], points[k2][2]);
        glVertex3f((centre + radius * points[k2])[0], (centre + radius * points[k2])[1], (centre + radius * points[k2])[2]);
    }
    glEnd();

    glBegin(GL_QUADS);
    for (int j = 1; j < stacks; j++)
    {
        for (int i = 1; i <= slices; i++)
        {
            k1 = i + (j - 1) * slices;
            k2 = (i % slices + 1) + (j - 1) * slices;
            glNormal3f(points[k2][0], points[k2][1], points[k2][2]);
            glVertex3f((centre + radius * points[k2])[0], (centre + radius * points[k2])[1], (centre + radius * points[k2])[2]);
            glNormal3f(points[k1][0], points[k1][1], points[k1][2]);
            glVertex3f((centre + radius * points[k1])[0], (centre + radius * points[k1])[1], (centre + radius * points[k1])[2]);

            k1 = i + (j)*slices;
            k2 = (i % slices + 1) + (j)*slices;
            glNormal3f(points[k1][0], points[k1][1], points[k1][2]);
            glVertex3f((centre + radius * points[k1])[0], (centre + radius * points[k1])[1], (centre + radius * points[k1])[2]);
            glNormal3f(points[k2][0], points[k2][1], points[k2][2]);
            glVertex3f((centre + radius * points[k2])[0], (centre + radius * points[k2])[1], (centre + radius * points[k2])[2]);
        }
    }
    glEnd();
}

struct SphereSelectionTool
{
	float radius;
	Vec3 center;
	bool isAdding;
	bool isActive;

	SphereSelectionTool() : radius(1.0), center(0.0, 0.0, 0.0), isAdding(false), isActive(false) {}


	void initSphere(const Vec3& pCenter, const float &pRadius)
	{
		center = pCenter;
		radius = pRadius;
	}

	void updateSphere(float pRadius)
	{
		radius = pRadius;
	}

	bool contains (const Vec3& p)
	{
		// is point p in sphere (center_x, center_y, center_z), radius) ?
		return (center - p).norm() <= radius;
	}


	void draw() {
        if(! isActive) return;
	    // draw Sphere
		drawSphere1( center[0] , center[1] , center[2] , radius , 10 , 10 );
	}
};
#endif