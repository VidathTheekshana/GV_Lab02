/*
Task 2: Bresenham's Line Drawing Algorithm
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
#include <math.h>

int windowWidth = 800;
int windowHeight = 600;

void putPixel(int x, int y) {
    glBegin(GL_POINTS);
        glVertex2i(x, y);
    glEnd();
}

void bresenhamLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int x, y, xEnd;
    
    if (x1 > x2) {
        x = x2; y = y2; xEnd = x1;
    } else {
        x = x1; y = y1; xEnd = x2;
    }
    
    int p = 2 * dy - dx;
    glColor3f(0.0, 1.0, 0.5);
    glPointSize(3.0);
    
    printf("Bresenham: (%d,%d) to (%d,%d), slope=%.2f\n", x1, y1, x2, y2, (float)dy/dx);
    
    putPixel(x, y);
    
    while (x < xEnd) {
        x++;
        if (p >= 0) {
            y = (y1 < y2) ? y + 1 : y - 1;
            p = p + 2 * dy - 2 * dx;
        } else {
            p = p + 2 * dy;
        }
        putPixel(x, y);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Example with slope between 0 and 1
    bresenhamLine(100, 150, 500, 350);
    
    // Mark endpoints
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(8.0);
    putPixel(100, 150);
    putPixel(500, 350);
    
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
    printf("Task 2: Bresenham's Line Algorithm\nPress ESC or Q to quit\n");
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Task 2: Bresenham Algorithm");
    
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
