/*
Task 3: Midpoint Circle Drawing Algorithm
*/

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <stdio.h>
#include <stdlib.h>

int windowWidth = 800;
int windowHeight = 600;
int centerX = 400, centerY = 300, radius = 120;

void putPixel(int x, int y) {
    glBegin(GL_POINTS);
        glVertex2i(x, y);
    glEnd();
}

void plot8Points(int xc, int yc, int x, int y) {
    putPixel(xc + x, yc + y);
    putPixel(xc - x, yc + y);
    putPixel(xc + x, yc - y);
    putPixel(xc - x, yc - y);
    putPixel(xc + y, yc + x);
    putPixel(xc - y, yc + x);
    putPixel(xc + y, yc - x);
    putPixel(xc - y, yc - x);
}

void midpointCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int p = 1 - r;
    
    glColor3f(0.0, 1.0, 0.5);
    glPointSize(2.0);
    
    printf("Circle: center(%d,%d), radius=%d\n", xc, yc, r);
    
    plot8Points(xc, yc, x, y);
    
    while (x <= y) {
        x++;
        if (p < 0) {
            p = p + 2 * x + 1;
        } else {
            y--;
            p = p + 2 * x - 2 * y + 1;
        }
        plot8Points(xc, yc, x, y);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    midpointCircle(centerX, centerY, radius);
    
    // Mark center
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(8.0);
    putPixel(centerX, centerY);
    
    glFlush();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27 || key == 'q' || key == 'Q') exit(0);
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    printf("Task 3: Midpoint Circle Algorithm\nPress ESC or Q to quit\n");
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Task 3: Midpoint Circle");
    
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
