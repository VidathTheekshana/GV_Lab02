/*
Task 1: Basic Line Segment with OpenGL
Draw a straight 2D line segment using integer Cartesian coordinates
Start point: (180, 15), End point: (10, 145)
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

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.5);
    glLineWidth(3.0);
    
    glBegin(GL_LINES);
        glVertex2i(180, 15);
        glVertex2i(10, 145);
    glEnd();
    
    // Mark endpoints
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(8.0);
    glBegin(GL_POINTS);
        glVertex2i(180, 15);
        glVertex2i(10, 145);
    glEnd();
    
    glFlush();
    printf("Task 1: Line drawn from (180, 15) to (10, 145)\n");
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
    printf("Task 1: Basic Line Segment\nPress ESC or Q to quit\n");
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Task 1: Basic Line Segment");
    
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
