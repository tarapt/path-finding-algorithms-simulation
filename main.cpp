#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <bits/stdc++.h>
using namespace std;
#define WHITE 1.0, 1.0, 1.0
#define DESTINATION 1.0, 0.0, 0.0
#define OBSTACLE 0.5, 0.0, 0.4
#define BACKGROUND 0.8, 0.0, 0.9
#define CURRENT 0.1, 0.2, 0.4
#define SOURCE 0.3, 0.6, 0.0
#define VISITED 0.0, 0.0, 0.9
#define FINAL_PATH 0.2, 0.3, 0.1
#define BORDER 0.0, 0.0, 0.0

#define F first
#define S second
#define pb push_back
#define mp make_pair
#define epsilon 1e-12

typedef pair<int, int> ii;
typedef vector<int> vi;
typedef vector<vector<int> > vvi;
typedef ii state;
typedef pair<double, state> pq_entry;
typedef struct {
	double cost;
	ii parent;
} state_info;
// Global Variables
char title[] = "Maze";
int refreshMillis = 300;                 // Refresh period in milliseconds
ii source, destination;
bool taking_input = false, source_input = false, dest_input = false;
vvi maze, visited;
vector<ii> shortest_path, exploration;
const int ROCK = -1, GEM = 1, FREE = 0;

GLfloat BORDER_WIDTH = 3.0;
GLint window_width = 700;
GLint window_height = 500;
GLfloat left_pos = 0.0;
GLfloat right_pos = 1.0;
GLfloat bottom_pos = 0.0;
GLfloat top_pos = 1.0;
GLint MAZE_WIDTH;
GLint MAZE_HEIGHT;

// Global variables
double STRAIGHT = 1;
double DIAGONAL = sqrt(2);
state NIL = mp(-1, -1);

vector<ii> offsets = { { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1,
		1 }, { 0, 1 }, { 1, 1 } }; //E, SE, S, SW, W, NW, N, NE

bool valid(int i, int j, int r, int c) {
	if (i >= 0 && j >= 0 && i < r && j < c)
		return true;
	return false;
}

pair<vector<ii>, vector<ii> > findPathByDjikstra(ii s, ii d, vvi grid) {
	vector<ii> path;
	vector<ii> exploration;
	priority_queue<pq_entry, vector<pq_entry>, greater<pq_entry> > pq;
	map<state, state_info> m;
	double iter = 0;
	pq.push(mp(0, s));
	iter++;
	m[s].cost = 0;
	m[s].parent = mp(-1, -1);
	while (!pq.empty()) {
		pq_entry front = pq.top();
		state pop = front.S;
		exploration.pb(pop);
		double pcost = front.F;
		if (pop == d) {
			state temp = d;
			while (1) {
				path.pb(temp);
				if (temp == s)
					break;
				temp = m[temp].parent;
			}
			reverse(path.begin(), path.end());
			break;
		}
		pq.pop();
		if (pcost > m[pop].cost)
			continue;
		// add neighbors
		for (int i = 0; i < (int) offsets.size(); i++) {
			int ni = pop.F + offsets[i].F;
			int nj = pop.S + offsets[i].S;
			if (valid(ni, nj, grid.size(), grid[0].size())) {
				if (grid[ni][nj] != -1) {
					state child;
					child.F = ni;
					child.S = nj;
					double path_cost = m[pop].cost;
					double delay = iter * epsilon;
					if (abs(offsets[i].F) == 1 && abs(offsets[i].S) == 1)
						path_cost += DIAGONAL;
					else
						path_cost += STRAIGHT;
					path_cost += delay;
					if (m.find(child) == m.end() || path_cost < m[child].cost) {
						m[child].cost = path_cost;
						m[child].parent = pop;
						pq.push(mp(path_cost, child));
						iter++;
					}
				}

			}
		}

	}
	if (m.find(d) == m.end())
		cout << "No Path Found";
	cout << '\n';
	return mp(exploration, path);
}

void print_vector(vector<ii> path) {
	for(auto pos: path) {
        cout << "("<< pos.F<<","<< pos.S<< ") ";
	}
	cout << endl;
}
void print_pair(ii p) {
    printf("(%d,%d)\n", p.F, p.S);
}
bool paused = false;
int move_number = 0;

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ii bot_position;
	bool explore_finished = false;
    if (!paused) {
        // Calculate animation parameters
        int b = shortest_path.size();
        int a = exploration.size();
        if(move_number >= (int)(a + b)){
            paused = true;
            bot_position = destination;
        } else if(move_number == a) {
            cout << "Goal found!" << endl;
            explore_finished = true;
            bot_position=shortest_path[move_number - a];
            move_number++;
        } else if(move_number> a){
            bot_position=shortest_path[move_number - a];
            move_number++;
        } else {
            bot_position=exploration[move_number];
            move_number++;
        }
    }
	GLfloat xSize = (right_pos - left_pos) / MAZE_WIDTH;
	GLfloat ySize = (top_pos - bottom_pos) / MAZE_HEIGHT;

	glBegin(GL_QUADS);
	for (GLint x = 0; x < MAZE_WIDTH; ++x) {
		for (GLint y = 0; y < MAZE_HEIGHT; ++y) {
            int value  = maze[y][x];
            if(taking_input){
               if(value == ROCK)
                    glColor3f(OBSTACLE);
               else if (value == FREE)
                    glColor3f(BACKGROUND);
               else if(x == source.S && y == source.F)
                    glColor3f(SOURCE);
               else if (value == GEM)
                    glColor3f(DESTINATION);
            } else {
                if(explore_finished && x == bot_position.S && y == bot_position.F) {
                    print_pair(bot_position);
                    visited[y][x] = 1;
                    glColor3f(FINAL_PATH);
                } else if(x == bot_position.S && y == bot_position.F){
                    print_pair(bot_position);
                    visited[y][x] = 1;
                    glColor3f(CURRENT);
                } else if(x == source.S && y == source.F){
                    glColor3f(SOURCE);
                } else if (value == GEM) {
                    glColor3f(DESTINATION);
                } else if (visited[y][x] == 1)
                    glColor3f(VISITED);
                else if (value == FREE)
                    glColor3f(BACKGROUND);
                else if (value == ROCK)
                    glColor3f(OBSTACLE);
            }
			glVertex2f(x * xSize + left_pos, y * ySize + bottom_pos); // S-W point
			glVertex2f((x + 1) * xSize + left_pos, y * ySize + bottom_pos); // S-E point
			glVertex2f((x + 1) * xSize + left_pos, (y + 1) * ySize + bottom_pos); // N-W point
			glVertex2f(x * xSize + left_pos, (y + 1) * ySize + bottom_pos); // N-E point
		}
	}
	glEnd();
	glColor3f(BORDER);
	glLineWidth(BORDER_WIDTH);
	glBegin(GL_LINES);
	for (GLint x = 0; x < MAZE_WIDTH; ++x) {
		for (GLint y = 0; y < MAZE_HEIGHT; ++y) {
            glBegin(GL_LINE_LOOP);
            glVertex2f(x * xSize + left_pos, y * ySize + bottom_pos); // S-W point
			glVertex2f((x + 1) * xSize + left_pos, y * ySize + bottom_pos); // S-E point
			glVertex2f((x + 1) * xSize + left_pos, (y + 1) * ySize + bottom_pos); // N-W point
			glVertex2f(x * xSize + left_pos, (y + 1) * ySize + bottom_pos); // N-E point
            glEnd();
		}
	}
    // Flush the pipeline, swap the buffers
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

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 'r':
    case 'R':
        paused = !paused;      // Toggle to opposite value
        if (!paused) {
         glutPostRedisplay();
        }
    break;
    case 's':
    case 'S':
        paused = false;
        drawScene();
        paused = true;
        break;
    case 'f':
    case 'F':
        if(taking_input && !source_input && !dest_input) {
            printf("Click on the source position, then press [F/f].\n");
            source_input = true;
        } else if(taking_input && source_input && !dest_input){
            source_input = false;
            dest_input = true;
            printf("Click on the destination position, then press [F/f].\n");
        } else if(taking_input && !source_input && dest_input) {
            dest_input = false;
            taking_input = false;
            printf("Finished taking input.\n");
            pair<vector<ii>, vector<ii> > data = findPathByDjikstra(source, destination, maze);
            exploration = data.F;
            shortest_path = data.S;
            paused = false;
        }
        break;
    /*case 27:   // Escape key
        exit(1);*/
   }
}
// glutSpecialFunc is called below to set this function to handle
//      all "special" key presses.  See glut.h for the names of
//      special keys.
void mySpecialKeyFunc( int key, int x, int y )
{
   switch ( key ) {
   case GLUT_KEY_UP:
      if ( refreshMillis > 25) {          // Do not reduce less than 2.
         refreshMillis -= 20;
      }
      break;
   case GLUT_KEY_DOWN:
      if (refreshMillis<1000) {
         refreshMillis += 20;
      }
      break;
   }
}

void take_input() {
    printf("Rows: ");
    cin >> MAZE_HEIGHT;
    printf("Columns: ");
    cin >> MAZE_WIDTH;
    printf("Fill the board.");
    maze.assign(MAZE_HEIGHT, vi(MAZE_WIDTH, 0));
    visited.assign(MAZE_HEIGHT, vi(MAZE_WIDTH, 0));
    cout << "Task Finished (Press F/f): " << endl;
    taking_input = true;
}

void initialize_grid() {
    ifstream fin;
    fin.open("input.txt");
    int m, n;
    fin >> m >> n;
    MAZE_HEIGHT = m;
    MAZE_WIDTH = n;
    maze.assign(m, vi(n));
    visited.assign(m, vi(n, 0));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            fin >> maze[i][j];
            if(maze[i][j] == 1) {
                destination = ii(i,j);
                print_pair(destination);
            }
        }
    }
    fin >> source.first >> source.second;
    pair<vector<ii>, vector<ii>> data = findPathByDjikstra(source, destination, maze);
    exploration = data.F;
    shortest_path = data.S;
    //print_vector(shortest_path);
    fin.close();
}
/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(BACKGROUND, 1.0); // Set background (clear) color to black
}

/* Callback handler for mouse event */
void mouse(int button, int state, int x, int y) {
    double cell_width = glutGet(GLUT_WINDOW_WIDTH) /(double) MAZE_WIDTH;
	double cell_height = glutGet(GLUT_WINDOW_HEIGHT) /(double) MAZE_HEIGHT;
    int row = (MAZE_HEIGHT - 1) - (int) (y / cell_height);
    int col = (int) (x / cell_width);

    printf("Cell clicked: ");print_pair(ii(row, col));

    if(row < 0 || col < 0 || row >= MAZE_HEIGHT || col >= MAZE_WIDTH) {
        return;
    }
    if(taking_input && !source_input && !dest_input){
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
          if (maze[row][col] == ROCK) {
                maze[row][col] = FREE;
          } else {
                maze[row][col] = ROCK;
          }
        }
    } else if(taking_input && source_input){
        if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
          source = ii(row, col);
          printf("Source = ");
          print_pair(source);
        }
    } else if(taking_input && dest_input){
        if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
          maze[row][col] = GEM;
          destination = ii(row, col);
          printf("Destination = ");
          print_pair(destination);
        }
    }
}

/* Called back when the timer expired */
void Timer(int value) {
    if(!paused)
        glutPostRedisplay();    // Post a paint request to activate display()
    glutTimerFunc(refreshMillis, Timer, 0); // subsequent timer call at milliseconds
}

int main(int argc, char **argv) {
    //initialize_grid();
    take_input();
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(title);                    // Create window with given title
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(myKeyboardFunc);           // Handles "normal" ASCII symbols
	glutSpecialFunc( mySpecialKeyFunc );        // Handles "special" keyboard keys
	glutMouseFunc(mouse);                       // Register callback handler for mouse event
	initGL();                                   // Our own OpenGL initialization
	glutTimerFunc(0, Timer, 0);                 // First timer call immediately
	glutMainLoop();                             // Enter event-processing loop
	return 0;
}
