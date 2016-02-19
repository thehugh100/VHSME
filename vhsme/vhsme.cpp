#include "stdafx.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include <windows.h>
#include <WinUser.h>
#include <cstdlib>
#include <math.h>
#include <cmath>
#include <time.h>
#include <fstream>
#include "stb_image.h"
#include <minmax.h>
#include <chrono>
#include <string>
#include <string.h>
#include <ppl.h>
using namespace std;
int w = 1280;
int h = 720;
void changeSize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glEnable(GL_BLEND);
	::w = w;
	::h = h;
}
GLuint Textures[100];

struct color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;

	color(int nr, int ng, int nb)
	{
		//r = nr > 255 ? 255 : (nr < 0) ? 0 : nr;
		//g = ng > 255 ? 255 : (ng < 0) ? 0 : ng;
		//b = nb > 255 ? 255 : (nb < 0) ? 0 : nb;

		r = min(255, nr);
		g = min(255, ng);
		b = min(255, nb);


	}
};


inline void setPixel(unsigned char *data, int width, int height, int comp, int x, int y, color pixel)
{
	//pixel.r = pixel.r > 255 ? 255 : (pixel.r < 0) ? 0 : pixel.r;
	//pixel.g = pixel.g > 255 ? 255 : (pixel.g < 0) ? 0 : pixel.g;
	//pixel.b = pixel.b > 255 ? 255 : (pixel.b < 0) ? 0 : pixel.b;

	if (x >= 0 && x <= width && y >= 0 && y < height)
	{
		data[(y * width*comp) + (x*comp)] = pixel.r;
		data[(y * width*comp) + (x*comp) + 1] = pixel.g;
		data[(y * width*comp) + (x*comp) + 2] = pixel.b;
	}
	if (x > width && y >= 0 && y < height)
	{
		x = x%width;
		y++;
		data[(y * width*comp) + (x*comp)] = pixel.r;
		data[(y * width*comp) + (x*comp) + 1] = pixel.g;
		data[(y * width*comp) + (x*comp) + 2] = pixel.b;
	}
}
inline unsigned char clamp255(int in)
{
	if (in > 255)
		in = 255;
	if (in < 0)
		in = 0;
	return in;
}
inline void setPixel2(unsigned char *data, int width, int height, int comp, int x, int y, color pixel)
{
	//pixel.r = pixel.r > 255 ? 255 : (pixel.r < 0) ? 0 : pixel.r;
	//pixel.g = pixel.g > 255 ? 255 : (pixel.g < 0) ? 0 : pixel.g;
	//pixel.b = pixel.b > 255 ? 255 : (pixel.b < 0) ? 0 : pixel.b;

	if (x >= 0 && x <= width && y >= 0 && y < height)
	{
		float tcr = data[(y * width*comp) + (x*comp)];
		float tcg = data[(y * width*comp) + (x*comp) + 1];
		float tcb = data[(y * width*comp) + (x*comp) + 2];

		tcr += pixel.r;
		tcg += pixel.g;
		tcb += pixel.b;

		tcr = clamp255(tcr);
		tcg = clamp255(tcg);
		tcb = clamp255(tcb);

		data[(y * width*comp) + (x*comp)] = tcr;
		data[(y * width*comp) + (x*comp) + 1] = tcg;
		data[(y * width*comp) + (x*comp) + 2] = tcb;

	}
	if (x > width && y >= 0 && y < height)
	{
		x = x%width;
		y++;
		float tcr = data[(y * width*comp) + (x*comp)];
		float tcg = data[(y * width*comp) + (x*comp) + 1];
		float tcb = data[(y * width*comp) + (x*comp) + 2];

		tcr += pixel.r;
		tcg += pixel.g;
		tcb += pixel.b;

		tcr = clamp255(tcr);
		tcg = clamp255(tcg);
		tcb = clamp255(tcb);

		data[(y * width*comp) + (x*comp)] = tcr;
		data[(y * width*comp) + (x*comp) + 1] = tcg;
		data[(y * width*comp) + (x*comp) + 2] = tcb;
	}
}
inline unsigned char nclamp(int in)
{
	if (in > 255)
		in = 255;
	if (in < 0)
		in = 0;
	return in;
}



struct timer
{
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;
	void run()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	void split(string name)
	{
		end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> elapsed = end - start;
		std::cout << name << " \t\t" << elapsed.count() << "ms" << endl;
		run();
	}
	void stop()
	{
		end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> elapsed = end - start;
		cout << elapsed.count() << " milliseconds" << endl;
	}

};
timer globalT;


static unsigned long x = 123456789, y = 362436069, z = 521288629;
inline unsigned long xorshf96(void) {          //period 2^96-1
	unsigned long t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;

	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
}

void fuck(unsigned char *data, int width, int height, int comp)
{
	globalT.run();
	{
		float perc = width * 0.1;
		int x, y, n;
		unsigned char *play = stbi_load("play.png", &x, &y, &n, 0);
		int rx = 0;
		int ry = 0;
		for (int i = 0; i < x*y*n; i += n)
		{

			if (play[i + 3] == 255)
			{
				setPixel(data, width, height, 3, rx + (width - perc) - 512, ry+20, color(play[i], play[i + 1], play[i + 2]));
			}
			rx++;
			if (rx >= x)
			{
				if (rand() % 16)
					rx = 0;
				else
					rx = rand() % 2;
				ry++;
			}
		}
		delete play;
	}
	globalT.split("load and draw play symbol");
	unsigned char *newData = new unsigned char[width*height*comp];
	unsigned char *sampleData = new unsigned char[width * comp];
	int lines = 32;
	int startOffset = (width * 3) * lines;
	for (int i = startOffset; i < startOffset + (width * 3); i++)
	{
		sampleData[i - startOffset] = data[i];
	}
	for (int i = lines; i > 0; i--)
	{
		for (int a = 0; a < width; a++)
		{
			setPixel(data, width, height, comp, lines + a - i, i, color(sampleData[a * 3], sampleData[a * 3 + 1], sampleData[a * 3 + 2]));
		}
	}
	globalT.split("offset glitched lines\t");


	{
		int x = 0;
		int y = 0;

		float perc = width * 0.1;

		for (int i = 0; i < width * height * 3; i += 3)
		{

			if (y % 3 == 0)
			{
				data[i] *= 0.6;
				data[i + 1] *= 0.6;
				data[i + 2] *= 0.6;

			}

			x++;
			if (x >= width)
			{
				x = 0;
				y++;
			}
		}
	}

	int line = 0;
	int off2 = rand() % 360;
	for (int i = 0; i < width * height * 3; i += width * 3)
	{

		int startOffset = (line * 3 * width);
		for (int q = startOffset; q < startOffset + (width * 3); q++)
		{
			sampleData[q - startOffset] = data[q];
		}

		int lineOff = ((sin((line + off2) / 30.0f) + 1) * 2);

		for (int a = 0; a < width; a++)
		{
			setPixel(data, width, height, comp, a + lineOff, line, color(sampleData[a * 3], sampleData[a * 3 + 1], sampleData[a * 3 + 2]));
		}

		line++;
	}
	globalT.split("wavey lines            ");

	for (int i = 0; i < width * height * 3; i += 3)
	{

		int comp = (data[i] * 0.21) + (data[i] * 0.72) + (data[i] * 0.07);

		int or = data[i];
		int og = data[i + 1];
		int ob = data[i + 2];

		data[i] = (comp + or) / 2.0f;
		data[i + 1] = (comp + og) / 2.0f;
		data[i + 2] = (comp + ob) / 2.0f;

	}

	for (int i = 0; i < (width * height * 3); i += 3)
	{
		float r = data[i];
		float g = data[i + 1];
		float b = data[i + 2];



		data[i    ] = (float)(1 + sin(((r / 2)-60) * 3.1415 / 180.0f)) / 2.0f * 255;
		data[i + 1] = (float)(1 + sin(((g / 2)-60) * 3.1415 / 180.0f)) / 2.0f * 255;
		data[i + 2] = (float)(1 + sin(((b / 2)-60) * 3.1415 / 180.0f)) / 2.0f * 255;

	}

	for (int i = 0; i < (width * height * 3); i += 3)
	{
		int r = data[i];
		int g = data[i + 1];
		int b = data[i + 2];

		r *= 1.2;
		g *= 1.2;
		b *= 1.2;
		r = nclamp(r);
		g = nclamp(g);
		b = nclamp(b);

		r *= 0.8;
		g *= 0.8;
		b *= 0.8;
		r = nclamp(r);
		g = nclamp(g);
		b = nclamp(b);

		data[i] = r;
		data[i + 1] = g;
		data[i + 2] = b;
	}


	globalT.split("colour saturation        ");
	int light = 0;
	float time2 = 0;
	for (int i = 0; i < width * height * 3; i += 3)
	{

		int mix = data[i] + data[i + 1] + data[i + 2];
		if (light > 0)
			light--;
		if (data[i] > 200 && data[i + 1] > 200 && data[i + 2] > 200)
		{
			light = 100;
			time2 = 0;
		}

		if (light)
		{
			int ncr = clamp255(data[i] + max(25.0 - time2, 0));
			int ncg = clamp255(data[i + 1] + max(25.0 - time2, 0));
			int ncb = clamp255(data[i + 2] + max(25.0 - time2, 0));

			data[i] = ncr;
			data[i + 1] = ncg;
			data[i + 2] = ncb;

			time2 += 0.5;
		}
	}
	globalT.split("brightness persistance  ");
	int noiseLevel = 10;
	int noiseValue = 0;
	for (int i = 0; i < width * height * 3; i++)
	{
		noiseValue = data[i] - 10 + xorshf96() % 20;

		if (noiseValue > 255)
			noiseLevel = 255;
		if (noiseValue < 0)
			noiseValue = 0;
		data[i] = noiseValue;
	}
	globalT.split("adding noise             ");
	int blipCount = 150 + rand() % 800;
	for (int i = 0; i < blipCount; i++)
	{
		
		int sX = rand() % width;
		int sY = height - ((rand() % 20) * (rand() % 20) * 1.5+rand()%60) + pow((rand()%2000)/1000.0f, 2);
		int blipLength = 5 + (rand() % 500) * rand()%(1+sY/10);
		int blipWidth = 1 + rand() % 2;
		for (int a = 0; a < blipLength; a++)
		{
			for (int b = 0; b < blipWidth; b++)
			{
				unsigned char q = (float)(128 * (blipLength - (float)a) / blipLength) + 99 - rand() % 58;

				if (a == 0)
					q = 0;

				setPixel2(data, width, height, comp, sX + a, sY + b, color(q, q, q));
			}
		}
	}
	globalT.split("drawing blips\t\t");
	{
		unsigned char *avgmap = new unsigned char[width*height*comp];
		for (int a = 0; a < 1; a++)
		{
			for (int i = (width * 3) + 3; i < (width * height * comp) - (width * 3) - 3; i += 1)
			{
				int p = (data[i] + data[i - 3] + data[i + 3] + data[i - (width * 3)] + data[i + (width * 3)] + data[i - (width * 3) - 3] + data[i - (width * 3) + 3] + data[i + (width * 3) - 3] + data[i + (width * 3) + 3]) / 8.0f;

				avgmap[i] = clamp255(p);

			}
			for (int i = 0; i < width * height * 3; i++)
			{
				data[i] = avgmap[i];
			}
		}
		delete avgmap;
	}


	globalT.split("bluring screen\t\t");
	for (int i = 0; i < width * height * 3; i += width * 3)
	{
		if (rand() % (height / 16) == 0)
		{
			unsigned char *lineCopy = new unsigned char[width * 3];

			for (int a = 0; a < width * 3; a++)
			{
				lineCopy[a] = data[i + a];
			}
			int offset = 1 + rand() % 4;
			for (int a = 0; a < width - 1; a++)
			{
				for (int b = 0; b < 3; b++)
					setPixel(data, width, height, comp, a + offset, (i / (width * 3)) + b, color(lineCopy[a * 3], lineCopy[a * 3 + 1], lineCopy[a * 3 + 2]));

				//setPixel(data, width, height, comp, a + offset, (i / (width * 3)) + 1, color(lineCopy[a * 3], lineCopy[a * 3 + 1], lineCopy[a * 3 + 2]));
				//setPixel(data, width, height, comp, a + offset, (i / (width * 3)) + 2, color(lineCopy[a * 3], lineCopy[a * 3 + 1], lineCopy[a * 3 + 2]));
			}
			delete lineCopy;
		}
	}
	globalT.split("thick line glitches\t");


	//abberation
	for (int i = 0; i < width * height * comp; i += 3)
	{
		newData[i] = 0;
		newData[i + 1] = data[i + 1];
		newData[i + 2] = 0;
	}
	int offset = 9;
	int c = 0;
	for (int i = offset; i < width * height * comp; i += 3)
	{
		if (i%width == 0)
		{
			c = 4;
		}
		else
		{
			if (c == 0)
				data[i + 1] = newData[i + 1 - offset];
		}
		if (c > 0)
			c--;
	}
	globalT.split("chromic abberation\t");
	{
		int x = 0;
		int y = 0;

		float perc = width * 0.1;
		float lineBiasR = 0;
		float lineBiasG = 0;
		float lineBiasB = 0;

		for (int i = 0; i < width * height * 3; i += 3)
		{

			int newR = clamp255((float)data[i] + lineBiasR);
			int newG = clamp255((float)data[i + 1] + lineBiasG);
			int newB = clamp255((float)data[i + 2] + lineBiasB);

			data[i] = newR;
			data[i + 1] = newG;
			data[i + 2] = newB;

			x++;
			if (x >= width)
			{
				x = 0;
				y++;
				lineBiasR /= 1.028;
				lineBiasG /= 1.026;
				lineBiasB /= 1.025;

				if (rand() % 80 == 0)
				{
					lineBiasR = (rand() % 100) / 14.0f;
					lineBiasG = (rand() % 100) / 14.0f;
					lineBiasB = (rand() % 100) / 14.0f;
				}
			}
		}
	}

	{
		int x = 0;
		int y = 0;

		float perc = width * 0.1;

		for (int i = 0; i < width * height * 3; i += 3)
		{

			if (x < perc || x > width - perc)
			{
				data[i] = 0;
				data[i + 1] = 0;
				data[i + 2] = 0;

			}

			x++;
			if (x >= width)
			{
				x = 0;
				y++;
			}
		}
	}
	{
		unsigned char *avgmap = new unsigned char[width*height*comp];
		for (int a = 0; a < 1; a++)
		{
			for (int i = (width * 3) + 3; i < (width * height * comp) - (width * 3) - 3; i += 1)
			{
				int p = (data[i] + data[i - 3] + data[i + 3] + data[i - (width * 3)] + data[i + (width * 3)] + data[i - (width * 3) - 3] + data[i - (width * 3) + 3] + data[i + (width * 3) - 3] + data[i + (width * 3) + 3]) / 8.0f;

				avgmap[i] = clamp255(p);

			}
			for (int i = 0; i < width * height * 3; i++)
			{
				data[i] = avgmap[i];
			}
		}
		delete avgmap;
	}
	globalT.split("border & blur\t");
	delete sampleData;
	delete newData;

	globalT.split("finish scene\t\t");
}

void saveBMP(unsigned char *data, int width, int height, int components)
{

	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 19778;
	fileHead.bfSize = (width*height*components) + 54;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;
	fileHead.bfOffBits = 1078;

	BITMAPINFOHEADER infoHead;
	infoHead.biSize = 40;
	infoHead.biWidth = width;
	infoHead.biHeight = height;
	infoHead.biPlanes = 1;//could be error
	infoHead.biBitCount = 24;
	infoHead.biCompression = 0;
	infoHead.biSizeImage = 0;
	infoHead.biXPelsPerMeter = 0;
	infoHead.biYPelsPerMeter = 0;
	infoHead.biClrUsed = 0;
	infoHead.biClrImportant = 0;


	unsigned char buffer[14];
	unsigned char inf[40];

	memcpy(buffer, &fileHead, 14);
	memcpy(inf, &infoHead, 40);
	ofstream te("test.bmp", ios::binary);
	int xc = 0;
	int yc = 0;
	int c = 0;

	te << noskipws << buffer << noskipws << inf;
	te << noskipws << (int)0;
	for (int i = 0; i < width * height * components; i++)
	{
		te << noskipws << (unsigned char)data[i];

		c++;

		if (i % 3 == 0)
			xc++;

		if (xc >= width)
		{

			while (c % 4 != 0)
			{
				te << noskipws << (unsigned char)0;
				c++;
			}

			xc = 0;
			yc++;
			c = 0;
		}

	}

	te.close();
}

GLuint loadTexturePNG(const char *fname)
{
	int x, y, n;
	unsigned char *data = stbi_load(fname, &x, &y, &n, 0);
	fuck(data, x, y, 3);
	saveBMP(data, x, y, 3);
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	delete data;
	return textureId;
}

void drawRectangleTextured(float x, float y, float w, float h, GLuint tex)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Textures[tex]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2f(0, 0);
	glVertex2f(x, y);
	glTexCoord2f(1, 0);
	glVertex2f(x + w, y);
	glTexCoord2f(1, 1);
	glVertex2f(x + w, y + h);
	glTexCoord2f(0, 1);
	glVertex2f(x, y + h);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawRectangleTextured(0, 0, w, h, 0);
	//Textures[0] = loadTexturePNG("test.jpg");
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	//cin.get();
	srand(time(0));
	rand();
	rand();
	rand();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(w, h);
	glutCreateWindow("");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	Textures[0] = loadTexturePNG(argv[1]);

	glutMainLoop();
	return 1;
}