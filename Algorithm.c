#include "Algorithm.h"
bool valid(int i, int j) {
	if (i >= 0 && j >= 0 && i < r && j < c)
		return true;
	return false;
}
vector<ii> findPathByDjikstra(ii s, ii d, vvi grid) {
	vector<ii> path;
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
		}
		pq.pop();
		if (pcost > m[pop].cost)
			continue;
		// add neighbors
		for (int i = 0; i < (int) offs.size(); i++) {
			int ni = pop.F + offs[i].F;
			int nj = pop.S + offs[i].S;
			if (valid(ni, nj)) {
				if (grid[ni][nj] != -1) {
					state child;
					child.F = ni;
					child.S = nj;
					double path_cost = m[pop].cost;
					double delay = iter * epsilon;
					if (abs(offs[i].F) == 1 && abs(offs[i].S) == 1)
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
	return path;
}
