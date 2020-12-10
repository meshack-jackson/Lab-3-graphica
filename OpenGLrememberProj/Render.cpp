

#include "Render.h"

#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <math.h>
#include <vector>
#include "Vec3.h"


double f(double a, double b, double t) { return a * (1 - t) + b * t; }

double f2(double p0, double p1, double p2, double p3, double t)
{
	return p0 * (1 - t) * (1 - t) * (1 - t) + 3 * t * p1 * (1 - t) * (1 - t) + 3 * t * t * p2 * (1 - t) + t * t * t * p3; 
}

double fer(double p1, double p4, double r1, double r4, double t)
{
	return p1 * (2 * t * t * t - 3 * t * t + 1) + p4 * (3 * t * t - 2 * t * t * t) + r1 * (t * t * t - 2 * t * t + t) + r4 * (t * t * t - t * t);
}

double f666(double p1, double p4, double r1, double r4, double t)
{
	return p1 * (2 * t * t * t - 3 * t * t + 1) + p4 * (3 * t * t - 2 * t * t * t) + r1 * (t * t * t - 2 * t * t + t) + r4 * (t * t * t - t * t); 
}



double t_max = 0;




double _factorial(double num) {
	double res = 1;
	for (int i = 1; i <= num; i++) {
		res *= i;
	}
	return res;
}



double _bernstein(double n, double i, double u) {
	return (_factorial(n) / (_factorial(i) * _factorial(n - i))) * pow(u, i) * pow(1 - u, n - i);
}






double* _bezierSurface(double points[4][4][3], double u, double v) {
	double* P = new double[3];

	P[0] = 0;
	P[1] = 0;
	P[2] = 0;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int c = 0; c < 3; c++) {
				P[c] += _bernstein(3, i, u) * _bernstein(3, j, v) * points[i][j][c];
			}
		}
	}
	return P;
}

void Draw_Cube(double P1[3])
{
	glBegin(GL_QUADS);

	glColor3d(0, 0, 0);
	glVertex3d(P1[0], P1[1], P1[2]);
	glVertex3d(P1[0] + 1, P1[1], P1[2]);
	glVertex3d(P1[0] + 1, P1[1] + 1, P1[2]);
	glVertex3d(P1[0], P1[1] + 1, P1[2]);

	glColor3d(0, 0, 0);
	glVertex3d(P1[0], P1[1], P1[2] + 1);
	glVertex3d(P1[0] + 1, P1[1], P1[2] + 1);
	glVertex3d(P1[0] + 1, P1[1] + 1, P1[2] + 1);
	glVertex3d(P1[0], P1[1] + 1, P1[2] + 1);


	glColor3d(0, 0, 0);
	glVertex3d(P1[0], P1[1], P1[2]);
	glVertex3d(P1[0], P1[1] + 1, P1[2]);
	glVertex3d(P1[0], P1[1] + 1, P1[2] + 1);
	glVertex3d(P1[0], P1[1], P1[2] + 1);

	glColor3d(0, 0, 0);
	glVertex3d(P1[0] + 1, P1[1], P1[2]);
	glVertex3d(P1[0] + 1, P1[1] + 1, P1[2]);
	glVertex3d(P1[0] + 1, P1[1] + 1, P1[2] + 1);
	glVertex3d(P1[0] + 1, P1[1], P1[2] + 1);

	glColor3d(0, 0, 0);
	glVertex3d(P1[0], P1[1], P1[2]);
	glVertex3d(P1[0] + 1, P1[1], P1[2]);
	glVertex3d(P1[0] + 1, P1[1], P1[2] + 1);
	glVertex3d(P1[0], P1[1], P1[2] + 1);

	glColor3d(0, 0, 0);
	glVertex3d(P1[0], P1[1] + 1, P1[2]);
	glVertex3d(P1[0] + 1, P1[1] + 1, P1[2]);
	glVertex3d(P1[0] + 1, P1[1] + 1, P1[2] + 1);
	glVertex3d(P1[0], P1[1] + 1, P1[2] + 1);
	glEnd();


}



void BezierSurface(double points[4][4][3]) {
	const double step = 0.1;
	const int size = 1 / 0.1 + 1;

	double* P;
	double calculated[size][size][3];
	int i = 0, j = 0;

	for (double u = 0; u <= 1; u += step) {
		for (double v = 0; v <= 1; v += step) {
			P = _bezierSurface(points, u, v);
			calculated[i][j][0] = P[0];
			calculated[i][j][1] = P[1];
			calculated[i][j][2] = P[2];

			delete[] P;
			j++;
		}
		j = 0;
		i++;
	}

	glPointSize(4);

	glBegin(GL_POINTS);
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			glVertex3dv(calculated[i][j]);
		}
	}
	glEnd();


	for (i = 0; i < size; i++) {
		glBegin(GL_LINE_STRIP);
		for (j = 0; j < size; j++) {
			glVertex3dv(calculated[i][j]);
		}
		glEnd();
	}

	for (j = 0; j < size; j++) {
		glBegin(GL_LINE_STRIP);
		for (i = 0; i < size; i++) {
			glVertex3dv(calculated[i][j]);
		}
		glEnd();
	}
}



void hermiteCurve(double P11[3], double P4[3], double R1[3], double R4[3])
{
	R1[0] = P11[0] + R1[0];
	R1[1] = P11[1] + R1[1];
	R1[2] = P11[2] + R1[2];

	R4[0] = P4[0] + R4[0];
	R4[1] = P4[1] + R4[1];
	R4[2] = P4[2] + R4[2];

	glBegin(GL_LINES);

	glVertex3dv(P11);
	glVertex3dv(R1);
	glEnd();

	glBegin(GL_LINES);
	glVertex3dv(P4);
	glVertex3dv(R4);
	glEnd();

	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
	for (double t = 0; t <= 1.0001; t += 0.01)
	{
		double PP[3];

		PP[0] = f2(P11[0], P4[0], R1[0], R4[0], t);
		PP[1] = f2(P11[1], P4[1], R1[1], R4[1], t);
		PP[2] = f2(P11[2], P4[2], R1[2], R4[2], t);

		glVertex3dv(PP);
	}
	glEnd();
	glLineWidth(1);

}


Vector3 ermitWithoutDraw(double P1[3], double P2[3], double P3[3], double P4[3], double t)
{
	Vector3 Vec;
	Vec.setCoords(f2(P1[0], P2[0], P3[0], P4[0], t), f2(P1[1], P2[1], P3[1], P4[1], t), f2(P1[2], P2[2], P3[2], P4[2], t));
	return Vec;
}


void curveH(double P1[3], double P2[3], double P3[3], double P4[3], double delta_time)
{
	static double t_max = 0;
	static bool flag = false;

	if (!flag)
	{
		t_max += delta_time / 1;
		if (t_max > 1)
		{
			t_max = 1;
			flag = !flag;
		}
	}
	else
	{
		t_max -= delta_time / 5;
		if (t_max < 0)
		{
			t_max = 0;
			flag = !flag;
		}
	}

	hermiteCurve(P1, P2, P3, P4);

	Vector3 P = ermitWithoutDraw(P1, P2, P3, P4, t_max);

	double A[] = { -0.2,-0.2,-0.2 };
	glPushMatrix();
	glTranslated(P.X(), P.Y(), P.Z());
	Draw_Cube(A);
	glPopMatrix();

	glColor3d(0, 1, 0);

}






void Render(double delta_time)
{

	t_max += delta_time / 5; 

	if (t_max > 1) t_max = 0; 

	double P1[] = { 0,0,0 }; 
	double P2[] = { 3,-6,7 };
	double P3[] = { 5,-6,-5 };
	double P4[] = { 8,-1,-3 };
	double P[3];


	

	glLineWidth(3);  

	glColor3d(0, 1, 0);

	glBegin(GL_LINE_STRIP);         
	for (double t = 0; t <= t_max; t += 0.01)
	{
		P[0] = f2(P1[0], P2[0], P3[0], P4[0], t);    
		P[1] = f2(P1[1], P2[1], P3[1], P4[1], t);
		P[2] = f2(P1[2], P2[2], P3[2], P4[2], t);
		glVertex3dv(P);    
	}   
		glEnd();   


		glColor3d(1, 0, 1);  
		glLineWidth(1); 
		

		glPointSize(10);
		glBegin(GL_POINTS); 
		glVertex3dv(P); 
		glEnd();  

		glPointSize(5);
		glColor3d(1, 0, 0); 
		glBegin(GL_POINTS); 
	
		glEnd();

	

		double r1[] = { 0,4,0 }; 
		double r2[] = { 0,2,6 };

		double r3[] = { 4,6,3 };
		double r4[] = { 8,8,8 };

		double P5[] = { 0,4,0 };;
		double P6[] = { 4,6,3 };

		double R1[] = { 0,-2,6 };
		double R2[] = { 4, 2, 5};
		
		double PPP[3];

		glBegin(GL_LINES);
		glVertex3dv(r1);
		glVertex3dv(r3);
		glVertex3dv(r4);
		glEnd();

		
		glLineWidth(3);  

		glColor3d(0, 0, 0);

		glBegin(GL_LINE_STRIP);
		for (double t = 0; t <= t_max; t += 0.01)
		{
			PPP[0] = fer(P5[0], P6[0], R1[0], R2[0], t);
			PPP[1] = fer(P5[1]-4, P6[1], R1[1], R2[1], t);
			PPP[2] = fer(P5[2], P6[2], R1[2], R2[2], t);
			glVertex3dv(PPP);   
		}
		glEnd();


		glColor3d(0, 0, 1);
		glLineWidth(3); 


		glPointSize(10);
		glBegin(GL_POINTS);
		glVertex3dv(PPP);
		glEnd();


			glBegin(GL_LINE_STRIP);
			for (double t = 0; t <= t_max; t += 0.01)
			{
				PPP[0] = fer(P5[0], P6[0], R1[0], R2[0], t);
				PPP[1] = fer(P5[1] - 4, P6[1]+2, R1[1]-4, R2[1]+3, t);
				PPP[2] = fer(P5[2], P6[2]+5, R1[2], R2[2], t);
				glVertex3dv(PPP);
			}
			glEnd();







		glColor3d(0, 0, 1);
		glLineWidth(3);


		glPointSize(10);
		glBegin(GL_POINTS);
		glVertex3dv(PPP);
		glEnd();

	


		glPointSize(5);
		glColor3d(1, 0, 0);
		glBegin(GL_POINTS);

		glEnd();
		



		double P20[] = { 5,0,7 };
		double P21[] = { -5,0,7 };
		double P22[] = { 0,8,7 };
		double P23[] = { 0,-8,7 };
		curveH(P20, P21, P22, P23, delta_time);



		double points[4][4][3] = {
		{{0, 9, 0}, {3, 9, -3}, {6, 9, -3}, {9, 9, 0}},
		{{0, 6, -3}, {3, 6, -3}, {6, 6, -3}, {9, 6, -3}},
		{{0, 3, -3}, {3, 3, -3}, {6, 3, 5}, {9, 3, -3}},
		{{0, 0, -2}, {3, 0, -3}, {6, 0, -3}, {9, 0, -2}}
		};
		BezierSurface(points);
		

}