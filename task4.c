/* Enhanced 3D Model Selection and Texture Mapping
 * Features: Multiple textures, lighting, camera controls, wireframe mode
 * Enhanced 3D graphics pipeline with advanced effects
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

// Window dimensions
int windowWidth = 1024;
int windowHeight = 768;

// Animation variables
float rotationX = 0.0f;
float rotationY = 0.0f;
float rotationZ = 0.0f;
float rotationSpeed = 1.0f;
int isAnimating = 1;

// Camera variables
float cameraDistance = 5.0f;
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;

// Texture variables
GLuint textureID;
int currentTexture = 0; // 0=checkerboard, 1=gradient, 2=grid, 3=plasma, 4=wood

// Rendering modes
int wireframeMode = 0;
int lightingEnabled = 1;
int showMultipleCubes = 0;

// Lighting variables
float lightPosition[4] = {2.0f, 2.0f, 2.0f, 1.0f};
float lightAmbient[4] = {0.3f, 0.3f, 0.3f, 1.0f};
float lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float lightSpecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};

// Material properties
float materialShininess = 50.0f;

// Create different texture patterns
void createCheckerboardTexture() {
    const int texWidth = 128;
    const int texHeight = 128;
    GLubyte textureData[texWidth][texHeight][3];
    
    printf("Creating enhanced checkerboard texture...\n");
    
    for (int i = 0; i < texWidth; i++) {
        for (int j = 0; j < texHeight; j++) {
            int checker = (((i & 0x10) == 0) ^ ((j & 0x10) == 0)) * 255;
            textureData[i][j][0] = (GLubyte) checker;
            textureData[i][j][1] = (GLubyte) checker;
            textureData[i][j][2] = (GLubyte) checker;
        }
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 
                 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
}

void createGradientTexture() {
    const int texWidth = 128;
    const int texHeight = 128;
    GLubyte textureData[texWidth][texHeight][3];
    
    printf("Creating rainbow gradient texture...\n");
    
    for (int i = 0; i < texWidth; i++) {
        for (int j = 0; j < texHeight; j++) {
            float u = (float)i / texWidth;
            float v = (float)j / texHeight;
            
            textureData[i][j][0] = (GLubyte) (255 * (0.5f + 0.5f * sin(u * 6.28f)));
            textureData[i][j][1] = (GLubyte) (255 * (0.5f + 0.5f * cos(v * 6.28f)));
            textureData[i][j][2] = (GLubyte) (255 * (0.5f + 0.5f * sin((u + v) * 3.14f)));
        }
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 
                 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
}

void createGridTexture() {
    const int texWidth = 128;
    const int texHeight = 128;
    GLubyte textureData[texWidth][texHeight][3];
    
    printf("Creating enhanced grid texture...\n");
    
    for (int i = 0; i < texWidth; i++) {
        for (int j = 0; j < texHeight; j++) {
            int isGrid = (i % 16 == 0 || j % 16 == 0);
            int isDiagonal = ((i + j) % 32 < 16);
            
            if (isGrid) {
                textureData[i][j][0] = 255;
                textureData[i][j][1] = 255;
                textureData[i][j][2] = 0;
            } else if (isDiagonal) {
                textureData[i][j][0] = 100;
                textureData[i][j][1] = 150;
                textureData[i][j][2] = 255;
            } else {
                textureData[i][j][0] = 50;
                textureData[i][j][1] = 50;
                textureData[i][j][2] = 100;
            }
        }
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 
                 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
}

void createPlasmaTexture() {
    const int texWidth = 128;
    const int texHeight = 128;
    GLubyte textureData[texWidth][texHeight][3];
    
    printf("Creating plasma texture...\n");
    
    for (int i = 0; i < texWidth; i++) {
        for (int j = 0; j < texHeight; j++) {
            float x = (float)i / texWidth;
            float y = (float)j / texHeight;
            
            float plasma = sin(x * 10) + sin(y * 10) + 
                          sin((x + y) * 10) + sin(sqrt(x*x + y*y) * 10);
            plasma = (plasma + 4.0f) / 8.0f; // Normalize to 0-1
            
            textureData[i][j][0] = (GLubyte) (255 * plasma);
            textureData[i][j][1] = (GLubyte) (255 * (1.0f - plasma));
            textureData[i][j][2] = (GLubyte) (255 * fabs(sin(plasma * 3.14159f)));
        }
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 
                 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
}

void createWoodTexture() {
    const int texWidth = 128;
    const int texHeight = 128;
    GLubyte textureData[texWidth][texHeight][3];
    
    printf("Creating wood texture...\n");
    
    for (int i = 0; i < texWidth; i++) {
        for (int j = 0; j < texHeight; j++) {
            float x = (float)i / texWidth;
            float y = (float)j / texHeight;
            
            float distance = sqrt((x - 0.5f) * (x - 0.5f) + (y - 0.5f) * (y - 0.5f));
            float wood = sin(distance * 40.0f + x * 20.0f) * 0.3f + 0.7f;
            
            textureData[i][j][0] = (GLubyte) (139 * wood); // Brown
            textureData[i][j][1] = (GLubyte) (69 * wood);
            textureData[i][j][2] = (GLubyte) (19 * wood);
        }
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 
                 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
}

// Initialize texture
void initTexture() {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Create initial texture
    createCheckerboardTexture();
    printf("Enhanced texture system initialized with ID: %d\n", textureID);
}

// Switch texture based on current selection
void switchTexture() {
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    switch(currentTexture) {
        case 0:
            createCheckerboardTexture();
            printf("Switched to enhanced checkerboard texture\n");
            break;
        case 1:
            createGradientTexture();
            printf("Switched to rainbow gradient texture\n");
            break;
        case 2:
            createGridTexture();
            printf("Switched to enhanced grid texture\n");
            break;
        case 3:
            createPlasmaTexture();
            printf("Switched to plasma texture\n");
            break;
        case 4:
            createWoodTexture();
            printf("Switched to wood texture\n");
            break;
        default:
            currentTexture = 0;
            createCheckerboardTexture();
            break;
    }
}

// Initialize lighting
void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess);
    
    printf("Enhanced lighting system initialized\n");
}

// Draw a textured cube
void drawTexturedCube(float size) {
    glBegin(GL_QUADS);
    
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);
    
    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, -size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, -size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, -size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, -size);
    
    // Top face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);
    
    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, -size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, -size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);
    
    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, -size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, -size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size, size, size);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, size);
    
    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, size, size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, -size);
    
    glEnd();
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up camera
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -cameraDistance);
    glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);
    
    // Toggle wireframe mode
    if (wireframeMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_TEXTURE_2D);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    
    // Toggle lighting
    if (lightingEnabled && !wireframeMode) {
        glEnable(GL_LIGHTING);
    } else {
        glDisable(GL_LIGHTING);
    }
    
    if (showMultipleCubes) {
        // Draw multiple cubes in a formation
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                glPushMatrix();
                glTranslatef(i * 2.5f, j * 2.5f, 0.0f);
                glRotatef(rotationX + i * 30, 1.0f, 0.0f, 0.0f);
                glRotatef(rotationY + j * 30, 0.0f, 1.0f, 0.0f);
                glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);
                
                // Different colors for each cube
                float r = (i + 1) * 0.5f;
                float g = (j + 1) * 0.5f;
                float b = 1.0f - (r + g) * 0.5f;
                glColor3f(r, g, b);
                
                drawTexturedCube(0.8f);
                glPopMatrix();
            }
        }
    } else {
        // Draw single rotating cube
        glPushMatrix();
        glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
        glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);
        
        glColor3f(1.0f, 1.0f, 1.0f);
        drawTexturedCube(1.0f);
        glPopMatrix();
    }
    
    glutSwapBuffers();
}

// Animation update function
void update(int value) {
    if (isAnimating) {
        rotationX += rotationSpeed;
        rotationY += rotationSpeed * 0.7f;
        rotationZ += rotationSpeed * 0.3f;
        
        if (rotationX > 360.0f) rotationX -= 360.0f;
        if (rotationY > 360.0f) rotationY -= 360.0f;
        if (rotationZ > 360.0f) rotationZ -= 360.0f;
        
        glutPostRedisplay();
    }
    glutTimerFunc(16, update, 0); // ~60 FPS
}

// Keyboard input handling
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC key
            exit(0);
            break;
        case ' ': // Space - toggle animation
            isAnimating = !isAnimating;
            printf("Animation %s\n", isAnimating ? "enabled" : "disabled");
            break;
        case 't': // Toggle texture
            currentTexture = (currentTexture + 1) % 5;
            switchTexture();
            glutPostRedisplay();
            break;
        case 'w': // Toggle wireframe
            wireframeMode = !wireframeMode;
            printf("Wireframe mode %s\n", wireframeMode ? "enabled" : "disabled");
            glutPostRedisplay();
            break;
        case 'l': // Toggle lighting
            lightingEnabled = !lightingEnabled;
            printf("Lighting %s\n", lightingEnabled ? "enabled" : "disabled");
            glutPostRedisplay();
            break;
        case 'm': // Toggle multiple cubes
            showMultipleCubes = !showMultipleCubes;
            printf("Multiple cubes mode %s\n", showMultipleCubes ? "enabled" : "disabled");
            glutPostRedisplay();
            break;
        case '+': // Increase rotation speed
            rotationSpeed += 0.5f;
            printf("Rotation speed: %.1f\n", rotationSpeed);
            break;
        case '-': // Decrease rotation speed
            rotationSpeed = (rotationSpeed > 0.5f) ? rotationSpeed - 0.5f : 0.0f;
            printf("Rotation speed: %.1f\n", rotationSpeed);
            break;
        case 'r': // Reset camera and rotation
            cameraDistance = 5.0f;
            cameraAngleX = 0.0f;
            cameraAngleY = 0.0f;
            rotationX = rotationY = rotationZ = 0.0f;
            printf("View reset\n");
            glutPostRedisplay();
            break;
    }
}

// Special key handling (arrow keys)
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            cameraAngleX -= 5.0f;
            break;
        case GLUT_KEY_DOWN:
            cameraAngleX += 5.0f;
            break;
        case GLUT_KEY_LEFT:
            cameraAngleY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            cameraAngleY += 5.0f;
            break;
        case GLUT_KEY_PAGE_UP:
            cameraDistance -= 0.5f;
            if (cameraDistance < 1.0f) cameraDistance = 1.0f;
            break;
        case GLUT_KEY_PAGE_DOWN:
            cameraDistance += 0.5f;
            break;
    }
    glutPostRedisplay();
}

// Window reshape handling
void reshape(int width, int height) {
    if (height == 0) height = 1;
    
    windowWidth = width;
    windowHeight = height;
    
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / height, 0.1f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW);
    printf("Window resized to %dx%d\n", width, height);
}

// Print control instructions
void printInstructions() {
    printf("\n=== Enhanced 3D Textured Cube Controls ===\n");
    printf("ESC       - Exit program\n");
    printf("SPACE     - Toggle animation\n");
    printf("T         - Switch texture (5 different textures)\n");
    printf("W         - Toggle wireframe mode\n");
    printf("L         - Toggle lighting\n");
    printf("M         - Toggle multiple cubes mode\n");
    printf("+/-       - Increase/decrease rotation speed\n");
    printf("R         - Reset view\n");
    printf("Arrow Keys - Rotate camera\n");
    printf("Page Up/Down - Zoom in/out\n");
    printf("==========================================\n\n");
}

// Initialize OpenGL settings
void initGL() {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    
    // Enable alpha blending for transparency effects
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    printf("Enhanced OpenGL initialized\n");
}

// Main function
int main(int argc, char** argv) {
    printf("Enhanced 3D Textured Cube Demo Starting...\n");
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Enhanced 3D Textured Cube with Multiple Effects");
    
    initGL();
    initTexture();
    initLighting();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, update, 0);
    
    printInstructions();
    
    printf("Starting main loop...\n");
    glutMainLoop();
    
    return 0;
}
