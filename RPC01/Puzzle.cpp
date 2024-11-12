#include <bits/stdc++.h>
using namespace std;

struct Robot {
    int x, y, dir; // posición y dirección
};

struct State {
    int x, y, dir; // Estado del robot
    int forward, bumps; // Contadores de comandos "adelante" y choques
};

int dx[] = {0, 1, 0, -1}; // N, E, S, W
int dy[] = {-1, 0, 1, 0}; // N, E, S, W

int c, r, e; // columnas, filas y salida
vector<vector<int>> horizontal, vertical; // paredes
map<tuple<int, int, int>, bool> visited;

// Función para verificar si un movimiento es válido
bool validMove(int x, int y, int dir) {
    if (x < 1 || x > c || y < 1 || y > r) return false; // Fuera del tablero
    if (dir == 0 && (y == 1 || horizontal[x][y - 1])) return false; // N
    if (dir == 1 && (x == c || vertical[x][y])) return false;       // E
    if (dir == 2 && (y == r || horizontal[x][y])) return false;     // S
    if (dir == 3 && (x == 1 || vertical[x - 1][y])) return false;   // W
    return true;
}

// BFS para encontrar el camino más corto de un robot
pair<int, vector<int>> bfs(Robot start) {
    queue<State> q;
    q.push({start.x, start.y, start.dir, 0, 0});
    visited[{start.x, start.y, start.dir}] = true;

    while (!q.empty()) {
        State cur = q.front();
        q.pop();

        // Verificar si el robot llegó a la salida
        if (cur.x == e && cur.y == r) {
            return {cur.forward, {cur.dir}};
        }

        // Generar posibles movimientos
        for (int d = 0; d < 4; d++) {
            State next = cur;
            next.dir = d;

            // Intentar mover hacia adelante
            if (d == cur.dir && validMove(next.x, next.y, d)) {
                next.x += dx[d];
                next.y += dy[d];
                next.forward++;
            } else if (d == cur.dir) {
                next.bumps++;
            }

            if (!visited[{next.x, next.y, next.dir}]) {
                visited[{next.x, next.y, next.dir}] = true;
                q.push(next);
            }
        }
    }
    return {-1, {}}; // No se debería alcanzar aquí
}

// Simula los movimientos de un robot aplicados a otro
int simulateMovements(const vector<int> &movements, Robot &robot) {
    int bumps = 0;
    for (int move : movements) {
        if (validMove(robot.x, robot.y, move)) {
            robot.x += dx[move];
            robot.y += dy[move];
        } else {
            bumps++;
        }
    }
    return bumps;
}

int main() {
    cin >> c >> r >> e;

    Robot r1, r2;
    char d1, d2;

    cin >> r1.x >> r1.y >> d1 >> r2.x >> r2.y >> d2;
    r1.dir = (d1 == 'N' ? 0 : d1 == 'E' ? 1 : d1 == 'S' ? 2 : 3);
    r2.dir = (d2 == 'N' ? 0 : d2 == 'E' ? 1 : d2 == 'S' ? 2 : 3);

    int h, v;
    cin >> h;
    horizontal.assign(c + 1, vector<int>(r + 1, 0));
    vertical.assign(c + 1, vector<int>(r + 1, 0));

    for (int i = 0; i < h; i++) {
        int x, y;
        cin >> x >> y;
        horizontal[x][y] = 1;
    }
    cin >> v;
    for (int i = 0; i < v; i++) {
        int x, y;
        cin >> x >> y;
        vertical[x][y] = 1;
    }

    // Sacar primero el robot 1
    visited.clear();
    auto [moves1, path1] = bfs(r1);
    Robot tempR2 = r2;
    int bumps1 = simulateMovements(path1, tempR2);

    // Sacar primero el robot 2
    visited.clear();
    auto [moves2, path2] = bfs(r2);
    Robot tempR1 = r1;
    int bumps2 = simulateMovements(path2, tempR1);

    // Comparar resultados
    if (moves1 < moves2 ) {
        cout << moves1 << " " << bumps1 << endl;
    } else {
        cout << moves2 << " " << bumps2 << endl;
    }

    return 0;
}
