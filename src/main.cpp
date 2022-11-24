#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static const int MATRIX_WIDTH = 70;
static const int MATRIX_HEIGHT = 20;
static const int PATTERN_WIDTH = 61;
static const int PATTERN_HEIGHT = 15;
static char MATRIX[MATRIX_WIDTH][MATRIX_HEIGHT];
static const char* PATTERN = "\
..........O..................................................\
.........O.O........................OO.......................\
..O....OOO.O.....O...................O...O..O......O.....OO..\
.O.O..O....OO...O.O..................O.OOO..OOO...O.O....O...\
.O.O..O.OO.......O....................O...OO...O.O..O......O.\
OO.OO.O.O.OOOOO.....O..................OO...O..O.O.OO.OO..OO.\
.O.O..O...O..O..O.......OO...OO...OO....OO.OO..O.O..O.O.O....\
.O..OO....OO......OOO.OO...OO...OO...OOO.....OOOO.OOO.O...OO.\
..O..OOO..O..O.OOOO...OO...OO...OO...OOO.OO..O....O.O....O..O\
...OO...O.O..O.....OO...OO...OO...OO......O............O...OO\
.....OO.O.OO.O.OO..O......................O........OO.O......\
.....O.OO.O..O.OO....O.................OO.O.O................\
...........OO.......OO..................O..OO................\
......................................O.O....................\
......................................OO.....................";

void renderBox() {
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);

    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();
    glFlush();
}

void renderSurface() {
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);

    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);

    glEnd();
    glFlush();
}

void handleWindowResize(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW!");
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(600, 400, "Game Of Life 3D", NULL, NULL);

    if (!window) {
        printf("Failed to create window!");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, handleWindowResize);

    if (glewInit()) {
        printf("Failed to initialize OpenGL!");
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, 600, 400);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // setup matrix
    for (int i = 0; i < MATRIX_WIDTH; i++) {
        for (int j = 0; j < MATRIX_HEIGHT; j++) {
            MATRIX[i][j] = 0;
        }
    }

    // draw pattern
    for (int i = 0; i < PATTERN_WIDTH; i++) {
        for (int j = 0; j < PATTERN_HEIGHT; j++) {
            if (PATTERN[j * PATTERN_WIDTH + i] == 'O') {
                MATRIX[i + 5][j + 2] = 1;
            }
        }
    }

    while (!glfwWindowShouldClose(window)) {

        // update matrix
        static int elapsed = 0; elapsed++;

        if (elapsed == 30) {
            elapsed = 0;
            char temp[MATRIX_WIDTH][MATRIX_HEIGHT];
            for (int i = 0; i < MATRIX_WIDTH; i++) {
                for (int j = 0; j < MATRIX_HEIGHT; j++) {
                    temp[i][j] = 0;
                }
            }
            for (int i = 1; i < MATRIX_WIDTH-1; i++) {
                for (int j = 1; j < MATRIX_HEIGHT-1; j++) {
                    char neighbors = 
                        MATRIX[i-1][j-1] +
                        MATRIX[i][j-1] +
                        MATRIX[i+1][j-1] +
                        MATRIX[i+1][j] +
                        MATRIX[i+1][j+1] +
                        MATRIX[i][j+1] +
                        MATRIX[i-1][j+1] +
                        MATRIX[i-1][j];

                    temp[i][j] = MATRIX[i][j];
                    if (neighbors < 2 || neighbors > 3) temp[i][j] = 0;
                    if (neighbors == 3) temp[i][j] = 1;
                }
            }
            for (int i = 0; i < MATRIX_WIDTH; i++) {
                for (int j = 0; j < MATRIX_HEIGHT; j++) {
                    MATRIX[i][j] = temp[i][j];
                }
            }

        }


        glClearColor(0.5f, 0.6f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // projection transformation
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        GLint windowRect[4];
        glGetIntegerv(GL_VIEWPORT, windowRect);

        float aspect = (float) windowRect[2] / windowRect[3];
        glFrustum(aspect, -aspect, -1.0f, 1.0f, 1, 100);

        // model transformation
        static float theta = 0.0f; theta += 0.5f;

        for (int i = 0; i < MATRIX_WIDTH; i++) {
            for (int j = 0; j < MATRIX_HEIGHT; j++) {
                if (MATRIX[i][j] == 1) {
                    glMatrixMode(GL_MODELVIEW);
                    glLoadIdentity();
                    glTranslatef(0.0f, 0.0f, -20.0f);
                    glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
                    glRotatef(theta, 0.0f, 1.0f, 0.0f);
                    glTranslatef(i - MATRIX_WIDTH / 2, 0.0f, j - MATRIX_HEIGHT / 2);
                    glScalef(0.5f, 0.5f, 0.5f);
                    
                    renderBox();
                }
            }
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -20.0f);
        glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(theta, 0.0f, 1.0f, 0.0f);
        glScalef(MATRIX_WIDTH, 1.0f, MATRIX_HEIGHT);

        renderSurface();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
