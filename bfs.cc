#include <iostream>
#include <queue>
#include <vector>

using namespace std;

static const int MAX = 5;

const int X[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };
const int Y[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };

typedef vector <pair <bool, int> > Fila;
typedef vector <Fila> Mapa;
typedef pair <int, int> parint;

bool valid (int x, int y) {
	return x >= 0 and x < MAX and y >= 0 and y < MAX;
}

void prova (Mapa& m, queue<parint>& q, int x, int y, int d) {
	if (valid(x,y)) {
		if (not m[x][y].first) {
			q.push(parint(x,y));
			if (m[x][y].second == 0) m[x][y].second = d+1;
		}
	}
}

int main() {
	int f, c;
	while (cin >> f >> c) {
		Mapa m(20, Fila(20));
		for (int i = 0; i < MAX; ++i) {
			for (int j = 0; j < MAX; ++j) {
				m[i][j].first = false;
				m[i][j].second = 0;
			}
		}
		queue <parint> q;
		q.push(parint(f,c));
		while (not q.empty()) {
			parint p = q.front(); q.pop();
			if (not m[p.first][p.second].first) {
				m[p.first][p.second].first = true;
				int d = m[p.first][p.second].second;
				for (int i = 0; i < 8; ++i) {
					int xx = p.first + X[i];
					int yy = p.second + Y[i];
					prova(m, q, xx, yy, d);
				}
			}
		}
		for (int i = 0; i < MAX; ++i) {
			for (int j = 0; j < MAX; ++j) cout << m[i][j].second << " ";
			cout << endl;
		}
	}
}