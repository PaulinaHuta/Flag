/*
Paulina Huta 
*/
#include "glut.h"
#include <stdlib.h>
#include <time.h>
#include <vector>

#define MULTIPLIER 0.1
const int N = 100;
const int W = 600;// window width
const int H = 600;// window height
using namespace std;

typedef struct {
	int x, y;
} POINT_2D;

bool pixsMat[W][H];
const double PI = 3.14159265359;


typedef struct {
	int r, g, b; // 0..255
}MY_COLOR;

POINT_2D top_left = { 0,100 };
POINT_2D bottom_right = { W,50 };
MY_COLOR blue = { 0,0,255 };//BLUE
void MyFloodFill(int line, int col);
POINT_2D top_left2 = { 0,H - 50 };
POINT_2D bottom_right2 = { W,H - 100 };
bool isInParameter(POINT_2D topLeft, POINT_2D  bottomRight, POINT_2D isInside);
void drawStrip(POINT_2D topLeft, POINT_2D bottomRight, MY_COLOR color1, double dist1);
unsigned char pix[H][W][3]; // red,green and blue layers
double offset = 0;
void DrawLineMatYesNo(POINT_2D p1, POINT_2D p2);
void setTriangle(POINT_2D center, int radius);
void DrawLineMatYesNo(POINT_2D p1, POINT_2D p2) {
	int i, j, start, stop;
	double a, b;

	if (p1.x != p2.x)
	{
		a = ((double)p2.y - p1.y) / (p2.x - p1.x);
		if (fabs(a) < 1)
		{
			b = p1.y - a * p1.x;
			if (p1.x < p2.x)
			{
				start = p1.x;
				stop = p2.x;
			}
			else
			{
				stop = p1.x;
				start = p2.x;
			}
			for (j = start; j <= stop; j++)
			{
				i = a * j + b;// y = ax+b
				pixsMat[i][j] = true;
			}
		}
		else // fabs(slope) is > 1
		{
			a = 1 / a;
			b = p1.x - a * p1.y;
			if (p1.y < p2.y)
			{
				start = p1.y;
				stop = p2.y;
			}
			else
			{
				start = p2.y;
				stop = p1.y;
			}
			for (i = start; i <= stop; i++)
			{
				j = a * i + b;
				pixsMat[i][j] = true;

			}
		}
	}
}

void drawVerticleLine(int x, int y_start, int y_end)
{
	if (y_start > y_end)
	{
		int temp = y_start;
		y_start = y_end;
		y_end = temp;
	}

	for (int i = y_start; i < y_end; i++)
		pixsMat[i][x] = true;
}

bool ColorEquals(MY_COLOR c1, MY_COLOR c2) {
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}
void MyFloodFill(int line, int col)
{
	vector <POINT_2D> stack; // stack of 2D points
	POINT_2D apoint = {line, col};
	stack.push_back(apoint);

	while (!stack.empty())
	{
		apoint = stack.back();
		stack.pop_back();
		line = apoint.x;
		col = apoint.y;

		// 1. paint(line,col,new_color)
		pixsMat[line][col] = true;

		// 2. try to go up
		if (pixsMat[line + 1][col] == false) {
			stack.push_back({ line + 1, col });
			//pixsMat[line + 1][col] = true;

		}
		// 3. try to go right
		if (pixsMat[line][col + 1] == false)
		{
			stack.push_back({ line , col + 1 });
		}

		//4. try to go down
		if (pixsMat[line - 1][col] == false)
		{
			stack.push_back({ line - 1, col });
		}

		//4.left
		if (pixsMat[line][col - 1] == false)
		{
			stack.push_back({ line, col - 1 });
		}

	}// while
}

void init()
{
	int i, j;
	glClearColor(1, 1, 1, 1);// color of window background

	srand(time(NULL));

	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
		{
			pix[i][j][0] = 255; // R
			pix[i][j][1] = 0; // G
			pix[i][j][2] = 0; // B
		}

	//up
	DrawLineMatYesNo({ 0,H }, { W,H });
	drawVerticleLine(0, H, H - 110);
	DrawLineMatYesNo({ 0,H - 110 }, { W,H - 110 });
	drawVerticleLine(W, H - 110, H);
	MyFloodFill(H - 1, W - 1);

	//down
	DrawLineMatYesNo({ 0, 0 }, { W, 0 });
	drawVerticleLine(W, 0, 110);
	DrawLineMatYesNo({ 0, 110 }, { W, 110 });
	drawVerticleLine(0, 110, 0);
	MyFloodFill(1, 1);

	int small_radius = 130;
	int big_radius = 170;


	setTriangle(POINT_2D{ W / 2, H / 2 }, -big_radius);
	setTriangle(POINT_2D{ W / 2, H / 2 }, -small_radius);
	MyFloodFill(H / 2 - big_radius + 3, W / 2);


	setTriangle(POINT_2D{ W / 2, H / 2 }, small_radius);
	setTriangle(POINT_2D{ W / 2, H / 2 }, big_radius);
	MyFloodFill(H / 2 - small_radius * sin(PI / 6) - 3, W / 2);


	MyFloodFill(H / 2 - small_radius * sin(PI / 6) - 3, W / 2 + small_radius * cos(PI / 6));
	MyFloodFill(H / 2 - small_radius * sin(PI / 6) - 3, W / 2 - small_radius * cos(PI / 6));
	MyFloodFill(H / 2 + big_radius - 3, W / 2);

	int x = W / 2 - sin(PI / 6) * small_radius + 4;
	int y = H / 2 + sin(PI / 6) * small_radius - 3;
	MyFloodFill(y, x);

	x = W / 2 + sin(PI / 6) * small_radius - 4;
	y = H / 2 + sin(PI / 6) * small_radius - 3;
	MyFloodFill(y, x);
	
}

void setTriangle(POINT_2D center, int radius)
{
	POINT_2D left = { (int)(center.x - radius * cos(PI / 6)), (int)(center.y - radius * sin(PI / 6)) };
	POINT_2D right = { (int)(center.x + radius * cos(PI / 6)), (int)(center.y - radius * sin(PI / 6)) };
	POINT_2D top = { (int)(center.x), (int)(center.y + radius) };

	DrawLineMatYesNo(left, right);
	DrawLineMatYesNo(right, top);
	DrawLineMatYesNo(top, left);

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	glDrawPixels(W, H, GL_RGB, GL_UNSIGNED_BYTE, pix);
	glutSwapBuffers(); // show all
}

void idle()
{
	int i, j;
	double dist, dist1;

	offset += 0.03;

	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
		{
			dist = sqrt((double)i * i + j * j);
			dist1 = sqrt((double)(H / 2 - i) * (H / 2 - i) + (-W / 2 - j) * (-W / 2 - j));
			pix[i][j][2] = 90 + 52 * (1 + sin(dist1 / 50.0 - offset));
			if (pixsMat[i][j] == true)
			{
				pix[i][j][0] = 0;
				pix[i][j][1] = 0;

			}
			else
			{
				pix[i][j][0] = 90 + 52 * (1 + sin(dist1 / 50.0 - offset));
				pix[i][j][1] = 90 + 52 * (1 + sin(dist1 / 50.0 - offset));
			}

		}

	glutPostRedisplay(); // go to display
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		pix[H - y][x][1] = 255;
	}
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(((glutGet(GLUT_SCREEN_WIDTH)) - W) / 2, 0);
	glutCreateWindow("Israel Flag");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	init();
	glutMainLoop();
}


void drawStrip(POINT_2D topLeft, POINT_2D bottomRight, MY_COLOR color1, double dist1)
{

	float startX = topLeft.x;
	float startY = topLeft.y;
	float finishX = bottomRight.x;
	float finishY = bottomRight.y;
	int i, j;
	for (j = startX; j < finishX; j++)
		for (i = finishY; i < startY; i++)
		{
			pix[i][j][0] = color1.r * (1 + sin(dist1 / 50.0 - offset)); // R
			pix[i][j][1] = color1.g * (1 + sin(dist1 / 50.0 - offset)); // G
			pix[i][j][2] = color1.b * (1 + sin(dist1 / 50.0 - offset)); // B
		}
}

bool isInParameter(POINT_2D topLeft, POINT_2D  bottomRight, POINT_2D isInside)
{

	if (isInside.x <topLeft.x || isInside.x> bottomRight.x)
		return false;
	if (isInside.y > topLeft.y || isInside.y < bottomRight.y)
		return false;

	return true;

}