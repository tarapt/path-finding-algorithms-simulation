#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <bits/stdc++.h>
using namespace std;
#define WHITE 1.0, 1.0, 1.0
#define BLACK 0.0, 0.0, 0.0
#define RED 1.0, 0.0, 0.0
#define GREEN 1.0, 1.0, 0.0
#define BLUE 0.0, 1.0, 0.0

#define F first
#define S second
#define pb push_back

GLint FPS = 24;
GLint window_width = 500;
GLint window_height = 500;
GLfloat left_pos = 0.0;
GLfloat right_pos = 1.0;
GLfloat bottom_pos = 0.0;
GLfloat top_pos = 1.0;
GLint game_width = 4;
GLint game_height = 4;

const double EPS = 1E-8;
const double ROOT_TWO = sqrt(2);
const int INF = 1000000000;
typedef pair<int, int> ii;
typedef vector<int> vi;
const ii NIL = make_pair(-1, -1);

int iteration_count;

class State {
public:
	ii pos = NIL;
	int value = 0;
	double distance = INF;
	ii parent;

	State() {
	}

	State(ii pos, int value) :
			pos(pos), value(value) {
	}
};

typedef vector<vector<State> > Grid;
ii source;
Grid grid;
vector<ii> path;

enum Direction {
	E, SE, S, SW, W, NW, N, NE
};

typedef struct Action_ {
	enum Direction direction;
	pair<int, int> offset;
	double step_cost;
} Action;

const Action actions[] = { { E, make_pair(0, 1), 1 }, { SE, make_pair(1, 1),
		ROOT_TWO }, { S, make_pair(1, 0), 1 },
		{ SW, make_pair(1, -1), ROOT_TWO }, { W, make_pair(0, -1), 1 }, { NW,
				make_pair(-1, -1), ROOT_TWO }, { N, make_pair(-1, 0), 1 }, { NE,
				make_pair(-1, 1), ROOT_TWO } };

const string separator("\n-----------------------------------------------\n");
const int ROCK = -1;
const int GEM = 1;
const int FREE = 0;

class Node {
public:
	ii pos;
	double distance;
	Direction priority;
	Node(ii pos, double distance, Direction priority) :
			pos(pos), distance(distance), priority(priority) {

	}

	Node(Node parent_node, Action action, ii position) {
		priority = action.direction;
		pos = position;
		double delay = iteration_count * EPS;
		distance = parent_node.distance + action.step_cost + delay;
	}
};

int get_new_row(Node popped, Action action, Grid &grid) {
	int new_row = (popped.pos.first + action.offset.first);
	return new_row;
}

void print_grid(Grid &grid, ii source) {
	cout << separator;
	for (int i = 0; i < (int) grid.size(); ++i) {
		cout << "\t";
		for (int j = 0; j < (int) grid[i].size(); ++j) {
			if (i == source.first && j == source.second)
				cout << setw(4) << "*S*";
			else if (grid[i][j].value == 1)
				cout << setw(4) << "*G*";
			else
				cout << setw(4) << grid[i][j].value;
		}
		cout << endl;
	}
	cout << separator;
}

class CompareNode {
public:
	bool operator()(Node &a, Node &b) {
		if (a.distance != b.distance)
			return a.distance > b.distance;
		else
			return a.priority > b.priority;
	}
};

bool goal_test(Node node, Grid &grid, ii &goal_state) {
	int row = node.pos.first;
	int col = node.pos.second;
	if (grid[row][col].value == GEM) {
		goal_state.first = row;
		goal_state.second = col;
		return true;
	}
	return false;
}

bool is_valid_position(ii pos, Grid &grid) {
	int max_r = grid.size();
	int max_c = grid[0].size();
	if (pos.first < 0 || pos.first >= max_r)
		return false;
	if (pos.second < 0 || pos.second >= max_c)
		return false;
	return true;
}

int get_new_col(Node popped, Action action, Grid &grid) {
	int new_col = (popped.pos.second + action.offset.second);
	return new_col;
}

void add_and_relax_neighbors(Node popped,
		priority_queue<Node, vector<Node>, CompareNode> &pq, Grid &grid) {
	for (Action action : actions) {
		int new_row = get_new_row(popped, action, grid);
		int new_col = get_new_col(popped, action, grid);
		if (is_valid_position(ii(new_row, new_col), grid)) {
			if (grid[new_row][new_col].value != ROCK) {
				Node neighbor(popped, action, make_pair(new_row, new_col));
				if (neighbor.distance < grid[new_row][new_col].distance) {
					grid[new_row][new_col].distance = neighbor.distance;
					grid[new_row][new_col].parent = popped.pos;
					pq.push(neighbor);
					iteration_count++;
				}
			}
		}
	}
}

ii dijkstra(ii source, Grid &grid) {
	Node s(source, 0, S);
	grid[source.first][source.second].distance = 0;
	priority_queue<Node, vector<Node>, CompareNode> pq;
	pq.push(s);
	ii goal_state = NIL;
	while (!pq.empty()) {
		Node popped = pq.top();
		pq.pop();
		int row = popped.pos.first;
		int col = popped.pos.second;
		if (popped.distance > grid[row][col].distance)
			continue; // Lazy Deletion
		if (goal_test(popped, grid, goal_state))
			break;
		add_and_relax_neighbors(popped, pq, grid);
	}
	return goal_state;
}

void print_stack(stack<ii> s) {
	while (!s.empty()) {
		ii t = s.top();
		cout << t.first << ' ' << t.second << ' ';
		s.pop();
	}
	cout << endl;
}

void print_path() {
	for(auto pos: path) {
        cout << "["<< pos.F<<","<< pos.S<< "] ";
	}
	cout << endl;
}

vector<ii> print_path(ii source, ii goal, Grid &grid) {
    vector<ii> path_vector;
	if (goal == NIL)
		cout << -1 << endl;
	else {
		stack<ii> states;
		ii state = goal;
		while (state != source) {
			states.push(state);
			state = grid[state.first][state.second].parent;
		}
		states.push(source);
		print_stack(states);

		while (!states.empty()) {
            ii t = states.top();
            path_vector.push_back(t);
            states.pop();
        }
	}
	return path_vector;
}

int RunMode = 1;      // Used as a boolean (1 or 0) for "on" and "off"

// The next global variable controls the animation's state and speed
int move_number = 0;
int AnimateStep = 2;

/*
* drawScene() handles the animation and the redrawing of the
*      graphics window contents.
*/
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (RunMode==1) {
        // Calculate animation parameters
        move_number++;
        if(move_number >= (int)path.size()){
            move_number = 0;
        }
    }
	GLfloat xSize = (right_pos - left_pos) / game_width;
	GLfloat ySize = (top_pos - bottom_pos) / game_height;

    ii bot_position = path[move_number];

	glBegin(GL_QUADS);
	for (GLint x = 0; x < game_width; ++x) {
		for (GLint y = 0; y < game_height; ++y) {
            int value  = grid[x][y].value;
            if(x == bot_position.S && y == bot_position.F){
                glColor3f(BLUE);
            }
             else if(x == source.S && y == source.F){
                glColor3f(GREEN);
            }
			else if (value == GEM) {
                glColor3f(BLACK);
            }
			else if (value == FREE)
				glColor3f(WHITE);
			else if (value == ROCK)
				glColor3f(RED);

			glVertex2f(x * xSize + left_pos, y * ySize + bottom_pos); // S-W point
			glVertex2f((x + 1) * xSize + left_pos, y * ySize + bottom_pos); // S-E point
			glVertex2f((x + 1) * xSize + left_pos, (y + 1) * ySize + bottom_pos); // N-W point
			glVertex2f(x * xSize + left_pos, (y + 1) * ySize + bottom_pos); // N-E point
		}
	}
	glEnd();
    // Flush the pipeline, swap the buffers
	glFlush();
	glutSwapBuffers();

    if ( RunMode==1 ) {
      glutPostRedisplay();   // Trigger an automatic redraw for animation
    }
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

// glutKeyboardFunc is called below to set this function to handle
//      all "normal" key presses.
void myKeyboardFunc( unsigned char key, int x, int y )
{
   switch ( key ) {
   case 'r':
      RunMode = 1-RunMode;      // Toggle to opposite value
      if ( RunMode==1 ) {
         glutPostRedisplay();
      }
      break;
   case 's':
      RunMode = 1;
      drawScene();
      RunMode = 0;
      break;
   case 27:   // Escape key
      exit(1);
   }
}
// glutSpecialFunc is called below to set this function to handle
//      all "special" key presses.  See glut.h for the names of
//      special keys.
void mySpecialKeyFunc( int key, int x, int y )
{
   switch ( key ) {
   case GLUT_KEY_UP:
      if ( AnimateStep > 3) {          // Do not reduce less than 2.
         AnimateStep--;
      }
      break;
   case GLUT_KEY_DOWN:
      if (AnimateStep<1000) {
         AnimateStep++;
      }
      break;
   }
}

void initialize_grid() {
    ifstream fin;
    fin.open("input.txt");
    int m, n;
    fin >> m >> n;
    game_height = m;
    game_width = n;
    grid.assign(m, vector<State>(n));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int value;
            fin >> value;
            State t(ii(i, j), value);
            grid[i][j] = t;
        }
    }
    fin >> source.first >> source.second;
    path = print_path(source, dijkstra(source, grid), grid);
    fin.close();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	initialize_grid();
    print_path();
	// We're going to animate it, so double buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Maze");
	glClearColor(1, 1, 1, 1);
	glutReshapeFunc(reshape);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(myKeyboardFunc);   // Handles "normal" ascii symbols
	glutSpecialFunc( mySpecialKeyFunc );      // Handles "special" keyboard keys
	cout << "Arrow keys control speed.  Press \"r\" to run,  \"s\" to single step." << endl;
	glutMainLoop();
	return 0;
}
