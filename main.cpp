#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <bits/stdc++.h>
using namespace std;
#define WHITE 1.0, 1.0, 1.0
#define BLACK 0.0, 0.0, 0.0
#define RED 1.0, 0.0, 0.0
#define GREEN 1.0, 1.0, 0.0

#define F first
#define S second
#define pb push_back

typedef vector<int> vi;
typedef vector<vector<int> > vvi;
typedef pair<int, int> ii;
typedef ii State;
typedef pair<double, State> Node;
typedef struct {
	State parent;
	double cost;
} StateInfo;

const vvi NIL(10, vi(10, 0));
const int ROCK = -1, GOAL = 1, FREE = 0;
const double EPS = 1E-12;

State source;

GLint FPS = 24;
GLint window_width = 500;
GLint window_height = 500;
GLfloat left_pos = 0.0;
GLfloat right_pos = 1.0;
GLfloat bottom_pos = 0.0;
GLfloat top_pos = 1.0;
GLint game_width = 4;
GLint game_height = 4;

vector<vector<int> > grid(game_height, vector<int>(game_width, 0));

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	GLfloat xSize = (right_pos - left_pos) / game_width;
	GLfloat ySize = (top_pos - bottom_pos) / game_height;

	glBegin(GL_QUADS);
	for (GLint x = 0; x < game_width; ++x) {
		for (GLint y = 0; y < game_height; ++y) {
            if(x == source.S && y == source.F){
                glColor3f(GREEN);
            }
			else if (grid[x][y] == GOAL)
				glColor3f(BLACK);
			else if (grid[x][y] == FREE)
				glColor3f(WHITE);
			else if (grid[x][y] == ROCK)
				glColor3f(RED);

			glVertex2f(x * xSize + left_pos, y * ySize + bottom_pos); // S-W point
			glVertex2f((x + 1) * xSize + left_pos, y * ySize + bottom_pos); // S-E point
			glVertex2f((x + 1) * xSize + left_pos, (y + 1) * ySize + bottom_pos); // N-W point
			glVertex2f(x * xSize + left_pos, (y + 1) * ySize + bottom_pos); // N-E point
		}
	}
	glEnd();

	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h) {
	window_width = w;
	window_height = h;

	glViewport(0, 0, window_width, window_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left_pos, right_pos, bottom_pos, top_pos);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

void update(int value) {
}

void key(unsigned char key, int x, int y) {
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case '+':
            break;
        case '-':
            break;
    }
    glutPostRedisplay();
}

void initialize_grid() {
    ifstream fin;
    fin.open("input.txt");
    int m, n;
    fin >> m >> n;
    game_height = m;
    game_width = n;
    grid.assign(m, vi(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            fin >> grid[i][j];
        }
    }
    fin >> source.F >> source.S;
    fin.close();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	grid[0][1] = -1;
	grid[0][2] = -1;
	grid[2][1] = -1;
	grid[3][1] = -1;
	grid[3][3] = 1;
	initialize_grid();
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Maze");
	glClearColor(1, 1, 1, 1);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutMainLoop();
	return 0;
}
