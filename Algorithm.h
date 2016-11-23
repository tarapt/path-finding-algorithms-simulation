#include<bits/stdc++.h>
using namespace std;
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

double STRAIGHT = 1;
double DIAGONAL = sqrt(2);
state NIL = mp(-1, -1);
vector<ii> offs = { { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1,
		1 }, { 0, 1 }, { 1, 1 } }; //E, SE, S, SW, W, NW, N, NE
