#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

ifstream fin("graf.in");
int a[100][100];
int n, m;
vector<int> adj[100];

void matrice_adiacenta(int p) {
    fin >> n >> m;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            a[i][j] = 0;
    for (int i = 1; i <= m; i++) {
        int x, y;
        fin >> x >> y;
        a[x][y] = 1;
        if (p == 0)
            a[y][x] = 1;
    }
}

void afis_matrice() {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++)
            cout << a[i][j] << " ";
        cout << endl;
    }
}

void lista_adiacenta(int p) {
    fin >> n >> m;
    for (int i = 1; i <= n; i++)
        adj[i].clear();
    for (int i = 1; i <= m; i++) {
        int x, y;
        fin >> x >> y;
        adj[x].push_back(y);
        if (p == 0)
            adj[y].push_back(x);
    }
}

void afis_lista() {
    for (int i = 1; i <= n; i++) {
        cout << i << ": ";
        for (auto j : adj[i])
            cout << j << " ";
        cout << endl;
    }
}

void trecere_matrice_lista() {
    for (int i = 1; i <= n; i++)
        adj[i].clear();
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (a[i][j] == 1)
                adj[i].push_back(j);
}

void trecere_lista_matrice() {
    for (int i = 1; i <= n; i++)
        for (int j : adj[i])
            a[i][j] = 1;
}
