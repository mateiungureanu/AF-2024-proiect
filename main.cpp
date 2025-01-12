#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

ifstream fin("graf.in");
int n, m, o;
int viz[100];
int rosu[100];
bool visited[100];
int ranks[100];
int a[100][100];
vector<int> lista_adj[100];
vector<int> v[100];
vector<int> cost;
vector<int> parent(100, -1);
vector<int> rang(100, 1);
vector<pair<int, int>> adj[100];
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<> > q2;
priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<> > q3;

void matrice_adiacenta() {
    fin >> n >> m >> o;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            a[i][j] = 0;
    for (int i = 1; i <= m; i++) {
        int x, y;
        fin >> x >> y;
        a[x][y] = 1;
        if (o == 0)
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

void lista_adiacenta() {
    fin >> n >> m >> o;
    for (int i = 1; i <= n; i++)
        lista_adj[i].clear();
    for (int i = 1; i <= m; i++) {
        int x, y;
        fin >> x >> y;
        lista_adj[x].push_back(y);
        if (o == 0)
            lista_adj[y].push_back(x);
    }
}

void afis_lista() {
    for (int i = 1; i <= n; i++) {
        cout << i << ": ";
        for (auto j : lista_adj[i])
            cout << j << " ";
        cout << endl;
    }
}

void trecere_matrice_lista() {
    for (int i = 1; i <= n; i++)
        lista_adj[i].clear();
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (a[i][j] == 1)
                lista_adj[i].push_back(j);
}

void trecere_lista_matrice() {
    for (int i = 1; i <= n; i++)
        for (int j : lista_adj[i])
            a[i][j] = 1;
}

void DFS(int i) {
    viz[i] = 1;
    cout << i << " ";
    for (int j : lista_adj[i]) {
        if (viz[j] == 0) {
            DFS(j);
        }
    }
}

void BFS(int start) {
    queue<int> q;
    q.push(start);
    viz[start] = 1;

    while (!q.empty()) {
        int i = q.front();
        q.pop();
        cout << i << " ";

        for (int j : lista_adj[i]) {
            if (!viz[j]) {
                viz[j] = 1;
                q.push(j);
            }
        }
    }
}

bool bipartit(int i) {
    queue<int> q;
    q.push(i);
    rosu[i] = 1;
    while (!q.empty()) {
        int j = q.front();
        q.pop();
        for (int vecin : lista_adj[j]) {
            if (rosu[vecin] == 0) {
                rosu[vecin] = -rosu[j];
                q.push(vecin);
            } else if (rosu[vecin] == rosu[j]) {
                return false;
            }
        }
    }
    return true;
}

bool verifica() {
    for (int i = 1; i <= n; i++) {
        if (rosu[i] == 0) {
            if (!bipartit(i))
                return false;
        }
    }
    return true;
}

int muchii_critice(int nod, int p, int k) {
    visited[nod] = true;
    int adancime_minima = INT_MAX;
    ranks[nod] = k;
    for (int i = 0; i < v[nod].size(); i++) {
        if (v[nod][i] != p) {
            if (!visited[v[nod][i]]) {
                int x = muchii_critice(v[nod][i], nod, k + 1);
                adancime_minima = min(x, adancime_minima);
                if (x > ranks[v[nod][i]])
                    cout << nod << ' ' << v[nod][i] << '\n';
            } else {
                adancime_minima = min(adancime_minima, ranks[v[nod][i]]);
            }
        }
    }
    return adancime_minima;
}

void lista_adiacenta_weighted() {
    fin >> n >> m;
    for(int i = 1; i <= m; i++) {
        int x, y, w;
        fin >> x >> y >> w;
        adj[x].push_back({y, w});
        adj[y].push_back({x, w});
        q3.push({w, x, y});
    }
}

int gaseste_parinte(int k) {
    while(parent[k] != -1) {
        k = parent[k];
    }
    return k;
}

void reuniune(int x, int y) {
    int rootX = gaseste_parinte(x);
    int rootY = gaseste_parinte(y);
    if(rootX != rootY) {
        if(rang[rootX] > rang[rootY]) {
            parent[rootY] = rootX;
            rang[rootX] += rang[rootY];
        }
        else {
            parent[rootX] = rootY;
            rang[rootY] += rang[rootX];
        }
        cout << x << " " << y << endl;
    }
}

void kruskal() {
    while(!q3.empty()) {
        int w = get<0>(q3.top()), qa = get<1>(q3.top()), qb = get<2>(q3.top());
        q3.pop();
        if(gaseste_parinte(qa) != gaseste_parinte(qb)) {
            reuniune(qa, qb);
        }
    }
}

void prim() {
    vector<bool> adaugat(n + 1, false);
    adaugat[1] = true;
    for (auto [x, w] : adj[1]) {
        if (!adaugat[x]) {
            q3.push({w, 1, x});
        }
    }
    while (!q3.empty()) {
        int qa = get<1>(q3.top()), qb = get<2>(q3.top());
        q3.pop();
        if (!adaugat[qb]) {
            cout << qa << " " << qb << endl;
            adaugat[qb] = true;
            for (auto [x, w] : adj[qb]) {
                if (!adaugat[x]) {
                    q3.push({w, qb, x});
                }
            }
        }
    }
}

void dijkstra(int nod) {
    cost.resize(n + 1, INT_MAX);
    cost[nod] = 0;
    q2.push({0, nod});

    while (!q2.empty()) {
        pair<int, int> f = q2.top();
        q2.pop();

        for (auto i : adj[f.second]) {
            if (cost[i.first] > cost[f.second] + i.second) {
                cost[i.first] = cost[f.second] + i.second;
                q2.push({cost[i.first], i.first});
            }
        }
    }
    for (int i = 1; i <= n; i++) {
        if (cost[i] == INT_MAX) {
            cout << "Inf " << endl;
        } else {
            cout << cost[i] << " ";
        }
    }
    cout << endl;
}

void reset_all() {
    fill(viz, viz + n + 1, 0);
    fill(rosu, rosu + n + 1, 0);
    fill(visited, visited + n + 1, false);
    fill(ranks, ranks + n + 1, -1);
    fill(parent.begin(), parent.end(), -1);
    fill(rang.begin(), rang.end(), 1);
    cost.clear();
}
