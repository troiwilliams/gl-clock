/**
 * File:	gl-clock.cpp
 * Created:	15/04/2012	(DD/MM/YYYY)
**/
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <GL/glut.h>

void timer (int);
void initialize (void);
void display (void);
void drawClockPoints (void);
void drawTime (int, int, int);

void getClockPoints (void);
void getHrHandPoints (void);
void getSecMinHandPoints (void);
void calibrateClock (void);

const int REFRESH_RATE = 1000;

const int RADIUS = 95;
const int SEC_HAND_RAD = RADIUS;
const int MIN_HAND_RAD = RADIUS * 4 / 5;
const int HR_HAND_RAD = RADIUS * 3 / 5;

const int SIXTY = 60;
const int SIX = 6;

const int N_CLOCK_POINTS = 12;

const double DEG2RAD = M_PI / 180.0;

int sec_index = 0, min_index = 0, hr_index = 0;

int sinv_clck_markers[12];
int cosv_clck_markers[12];

int sinv_secs[60];
int cosv_secs[60];

int sinv_mins[60];
int cosv_mins[60];

int sinv_hrs[12];
int cosv_hrs[12];


int main (int argc, char** argv) {
	glutInit(&argc, argv);
	initialize();
	glutMainLoop();
	return 0;
}

void initialize (void) {
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(200, 200);
	glutCreateWindow("Clock");
	glutDisplayFunc(display);
	glutTimerFunc(REFRESH_RATE, timer, 0);
	glOrtho(-100, 100, -100, 100, -100, 100);
	glClearColor(0, 0, 0, 0);
	glColor3ub(0, 0, 0);
	
	calibrateClock();
	
	glPointSize(5);
	glLineWidth(2);
	getClockPoints();
	getSecMinHandPoints();
	getHrHandPoints();
	
}

void getClockPoints (void) {
	double theta = 0.0;
	int i = 0;
	do {
		cosv_clck_markers[i] = (int)(RADIUS * cos(theta * DEG2RAD));
		sinv_clck_markers[i] = (int)(RADIUS * sin(theta * DEG2RAD));
		i = i + 1;
		theta = theta + 30.0;
	} while (theta < 360);
}

void getHrHandPoints (void) {
	double theta = 0.0;
	int i = 0;
	do {
		cosv_hrs[i] = (int)(HR_HAND_RAD * cos((-theta + 90) * DEG2RAD));
		sinv_hrs[i] = (int)(HR_HAND_RAD * sin((-theta + 90) * DEG2RAD));
		i = i + 1;
		theta = theta + 30.0;
	} while (theta < 360);
}

void getSecMinHandPoints (void) {
	double theta = 0.0;
	int i = 0;
	do {
		cosv_secs[i] = (int)(SEC_HAND_RAD * cos((-theta + 90) * DEG2RAD));
		sinv_secs[i] = (int)(SEC_HAND_RAD * sin((-theta + 90) * DEG2RAD));
		cosv_mins[i] = (int)(MIN_HAND_RAD * cos((-theta + 90) * DEG2RAD));
		sinv_mins[i] = (int)(MIN_HAND_RAD * sin((-theta + 90) * DEG2RAD));
		i = i + 1;
		theta = theta + 6.0;
	} while (theta < 360);
}

void calibrateClock (void) {
	// get current time
	time_t now = time(0);
	tm *ltm = localtime(&now);
	sec_index = (ltm->tm_sec + 1) % SIXTY;
	min_index = (ltm->tm_min) % SIXTY;
	hr_index = (ltm->tm_hour) % 12;
}

void display (void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
		glColor3ub(255, 215, 0);
		drawClockPoints();
	glPopMatrix();
	glPushMatrix();
		drawTime(hr_index, min_index, sec_index);
	glPopMatrix();
	glutSwapBuffers();
	
	sec_index = (sec_index + 1) % SIXTY;
	if (sec_index % SIXTY == 0) {
		min_index = (min_index + 1) % SIXTY;
		if (min_index % SIXTY == 0) {
			hr_index = (hr_index + 1) % 12;
		}
		if (min_index % 5 == 0) {
			calibrateClock();
		}
	}
}

void timer (int x) {
	glutPostRedisplay();
	glutTimerFunc(REFRESH_RATE, timer, 0);
}

void drawClockPoints (void) {
	glBegin(GL_POINTS);
		for (int i = 0; i < N_CLOCK_POINTS; i++) {
			glVertex3i(cosv_clck_markers[i], sinv_clck_markers[i], 0);
		}
		glVertex3i(0, 0, 0);
	glEnd();
}

void drawTime (int hI, int mI, int sI) {
	glBegin(GL_LINES);
		// hour hand
		glColor3ub(255, 255, 255);
		glVertex3i(0, 0, 0);
		glVertex3i(cosv_hrs[hI], sinv_hrs[hI], 0);
		
		// minutes hand
		glColor3ub(139, 131, 120);
		glVertex3i(0, 0, 0);
		glVertex3i(cosv_mins[mI], sinv_mins[mI], 0);
		
		// seconds hand
		glColor3ub(230, 230, 250);
		glVertex3i(0, 0, 0);
		glVertex3i(cosv_secs[sI], sinv_secs[sI], 0);
	glEnd();
}