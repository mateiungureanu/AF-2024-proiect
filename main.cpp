#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <climits>
using namespace std;

ifstream fin("graf.in");

int n, m, o;
bool visited[100];
int rosu[100];
int ranks[100];
int low[100];
int pairing[100];
int dist_cuplaj[100]; 
int a[100][100];
int flux[100][100];
vector<int> lista_adj[100];
vector<int> lista_adj_transpus[100];
vector<int> v[100];
vector<int> cost;
vector<int> grad(n, 0);
vector<int> culoare(n, -1);
vector<int> parent(100, -1);
vector<int> rang(100, 1);
vector<int> ciclu_eulerian;
vector<int> dist_dag(100, INT_MAX);
vector<bool> adaugat(n, false);
vector<pair<int, int>> adj[100];
stack<int> s;
queue<int> q;
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<> > q2;
priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<> > q3;
vector<vector<int>> componente;
vector<vector<int>> dist(100, vector<int>(100, INT_MAX));
vector<vector<bool>> culori_disponibile(n + 1, vector<bool>(7, true));

void matrice_adiacenta() {
    memset(a, 0, sizeof(a));
    fin >> n >> m >> o;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            a[i][j] = 0;
    for (int i = 0; i < m; i++) {
        int x, y;
        fin >> x >> y;
        a[x][y] = 1;
        if (o == 0)
            a[y][x] = 1;
    }
}

void afis_matrice() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << a[i][j] << " ";
        cout << endl;
    }
}

void lista_adiacenta() {
    fin >> n >> m >> o;
    for (int i = 0; i < n; i++)
        lista_adj[i].clear();
    for (int i = 0; i < m; i++) {
        int x, y;
        fin >> x >> y;
        lista_adj[x].push_back(y);
        if (o == 0)
            lista_adj[y].push_back(x);
    }
}

void afis_lista() {
    for (int i = 0; i < n; i++) {
        cout << i << ": ";
        for (auto j : lista_adj[i])
            cout << j << " ";
        cout << endl;
    }
}

void trecere_matrice_lista() {
    for (int i = 0; i < n; i++)
        lista_adj[i].clear();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (a[i][j] == 1)
                lista_adj[i].push_back(j);
}

void trecere_lista_matrice() {
    memset(a, 0, sizeof(a));
    for (int i = 0; i < n; i++)
        for (int j : lista_adj[i])
            a[i][j] = 1;
}

void DFS(int start) {
    fill(visited, visited + n, false);
    while (!s.empty()) {
        s.pop();
    }
    s.push(start);
    visited[start] = true;
    while (!s.empty()) {
        int nod = s.top();
        s.pop();
        cout << nod << " ";
        for (int vecin : lista_adj[nod]) {
            if (!visited[vecin]) {
                visited[vecin] = true;
                s.push(vecin);
            }
        }
    }
}

void BFS(int start) {
    fill(visited, visited + n, false);
    while (!q.empty()) {
        q.pop();
    }
    q.push(start);
    visited[start] = true;
    while (!q.empty()) {
        int nod = q.front();
        q.pop();
        cout << nod << " ";
        for (int vecin : lista_adj[nod]) {
            if (!visited[vecin]) {
                visited[vecin] = true;
                q.push(vecin);
            }
        }
    }
}

bool bipartit(int i) {
    fill(rosu, rosu + n, 0);
    while (!q.empty()) {
        q.pop();
    }
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
    for (int i = 0; i < n; i++) {
        if (rosu[i] == 0) {
            if (!bipartit(i))
                return false;
        }
    }
    return true;
}

bool bfs_cuplaj(int n, int m) {
    while (!q.empty()) {
        q.pop();
    }
    for (int u = 0; u < n; ++u) {
        if (pairing[u] == 0) {
            dist_cuplaj[u] = 0;
            q.push(u);
        } else {
            dist_cuplaj[u] = INT_MAX;
        }
    }
    bool found_augmenting_path = false;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (dist_cuplaj[u] < INT_MAX) {
            for (int v : lista_adj[u]) {
                if (pairing[v] == 0) {
                    found_augmenting_path = true;
                } else if (dist_cuplaj[pairing[v]] == INT_MAX) {
                    dist_cuplaj[pairing[v]] = dist_cuplaj[u] + 1;
                    q.push(pairing[v]);
                }
            }
        }
    }
    return found_augmenting_path;
}

bool dfs_cuplaj(int u) {
    if (u == 0) return true;
    for (int v : lista_adj[u]) {
        if (dist_cuplaj[pairing[v]] == dist_cuplaj[u] + 1) {
            if (dfs_cuplaj(pairing[v])) {
                pairing[u] = v;
                pairing[v] = u;
                return true;
            }
        }
    }
    dist_cuplaj[u] = INT_MAX;
    return false;
}

int cuplaj_maxim(int n, int m) {
    fill(pairing, pairing + n, 0);
    fill(dist_cuplaj, dist_cuplaj + n, INT_MAX);
    int max_matching = 0;
    while (bfs_cuplaj(n, m)) {
        for (int u = 0; u < n; u++) {
            if (pairing[u] == 0 && dfs_cuplaj(u)) {
                max_matching++;
            }
        }
    }
    return max_matching;
}

int muchii_critice(int nod, int p, int k) {
    visited[nod] = true;
    ranks[nod] = k;
    int adancime_minima = INT_MAX;
    for (int i = 0; i < lista_adj[nod].size(); i++) {
        if (lista_adj[nod][i] != p) {
            if (!visited[lista_adj[nod][i]]) {
                int x = muchii_critice(lista_adj[nod][i], nod, k + 1);
                adancime_minima = min(x, adancime_minima);
                if (x > ranks[lista_adj[nod][i]])
                    cout << nod << ' ' << lista_adj[nod][i] << '\n';
            } else {
                adancime_minima = min(adancime_minima, ranks[lista_adj[nod][i]]);
            }
        }
    }
    return adancime_minima;
}

void noduri_critice(int nod, int p, int k) {
    visited[nod] = true;
    ranks[nod] = low[nod] = k;
    int copii = 0;
    for (int i = 0; i < lista_adj[nod].size(); i++) {
        if (lista_adj[nod][i] != p) {
            if (!visited[lista_adj[nod][i]]) {
                copii++;
                noduri_critice(lista_adj[nod][i], nod, k + 1);
                low[nod] = min(low[nod], low[lista_adj[nod][i]]);
                if (low[lista_adj[nod][i]] >= ranks[nod] && p != -1) {
                    cout << nod << '\n';
                }
            } else {
                low[nod] = min(low[nod], ranks[lista_adj[nod][i]]);
            }
        }
    }
    if (p == -1 && copii > 1) {
        cout << nod << '\n';
    }
}

void tarjan(int nod) {
    fill(visited, visited + n, false);
    fill(ranks, ranks + n, -1);
    fill(low, low + n, -1);
    muchii_critice(nod, -1, 0);
    fill(visited, visited + n, false);
    fill(ranks, ranks + n, -1);
    noduri_critice(nod, -1, 0);
}

void lista_adiacenta_weighted() {
    for (int i = 0; i < n; i++) {
        adj[i].clear();
    }
    while (!q3.empty()) {
        q3.pop();
    }
    fin >> n >> m;
    for(int i = 0; i < m; i++) {
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
    fill(parent.begin(), parent.end(), -1);
    fill(rang.begin(), rang.end(), 1);
    while(!q3.empty()) {
        int w = get<0>(q3.top());
        int qa = get<1>(q3.top());
        int qb = get<2>(q3.top());
        q3.pop();
        if(gaseste_parinte(qa) != gaseste_parinte(qb)) {
            reuniune(qa, qb);
        }
    }
}

void prim() {
    fill(adaugat.begin(), adaugat.end(), false);
    while (!q3.empty()) {
        q3.pop();
    }
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
    fill(cost.begin(), cost.end(), INT_MAX);
    fill(parent.begin(), parent.end(), -1);
    while (!q2.empty()) {
        q2.pop();
    }
    cost[nod] = 0;
    q2.push({0, nod});
    while (!q2.empty()) {
        pair<int, int> f = q2.top();
        q2.pop();
        for (auto i : adj[f.second]) {
            if (cost[i.first] > cost[f.second] + i.second) {
                cost[i.first] = cost[f.second] + i.second;
                parent[i.first] = f.second;
                q2.push({cost[i.first], i.first});
            }
        }
    }
    for (int i = 0; i < n; i++) {
        if (cost[i] == INT_MAX) {
            cout << "inf " << endl;
        } else {
            cout << cost[i] << " ";
        }
    }
    cout << endl;
}

void bellman_ford(int nod) {
    fill(cost.begin(), cost.end(), INT_MAX);
    fill(parent.begin(), parent.end(), -1);
    cost[nod] = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int u = 0; u < n; u++) { 
            for (auto edge : adj[u]) {
                int v = edge.first;
                int weight = edge.second;
                if (cost[u] != INT_MAX && cost[u] + weight < cost[v]) {
                    cost[v] = cost[u] + weight;
                    parent[v] = u;
                }
            }
        }
    }
    for (int u = 0; u < n; u++) {
        for (auto edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (cost[u] != INT_MAX && cost[u] + weight < cost[v]) {
                cout << "Graful contine un ciclu de cost negativ" << endl;
                return;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        if (cost[i] == INT_MAX) {
            cout << "inf ";
        } else {
            cout << cost[i] << " ";
        }
    }
    cout << endl;
}

void floyd_warshall() {
    for (int i = 0; i < n; i++) {
        fill(dist[i].begin(), dist[i].end(), INT_MAX);
    }
    for (int i = 0; i < n; i++) {
        dist[i][i] = 0;
    }
    for (int u = 0; u < n; u++) {
        for (auto edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;
            dist[u][v] = weight;
        }
    }
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INT_MAX) {
                cout << "inf ";
            } else {
                cout << dist[i][j] << " ";
            }
        }
        cout << endl;
    }
}

void dfs_topologic(int nod) {
    visited[nod] = true;
    for (int i = 0; i < lista_adj[nod].size(); i++) {
        if (!visited[lista_adj[nod][i]]) {
            dfs_topologic(lista_adj[nod][i]);
        }
    }
    s.push(nod);
}

void sortare_topologica() {
    fill(visited, visited + n, false);
    while (!s.empty()) {
        s.pop();
    }
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfs_topologic(i);
        }
    }
    while (!s.empty()) {
        cout << s.top() << " ";
        s.pop();
    }
    cout << endl;
}

void sortare_topologica_dag() {
    fill(visited, visited + n, false);
    while (!s.empty()) {
        s.pop();
    }
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfs_topologic(i);
        }
    }
}

void dag_drum_minim(int start) {
    sortare_topologica_dag();
    dist_dag[start] = 0;
    while (!s.empty()) {
        int u = s.top();
        s.pop();
        if (dist_dag[u] != INT_MAX) {
            for (auto [v, cost] : adj[u]) {
                if (dist_dag[u] + cost < dist_dag[v]) {
                    dist_dag[v] = dist_dag[u] + cost;
                }
            }
        }
    }
    cout << "Distante minime de la nodul " << start << ":\n";
    for (int i = 0; i < n; i++) {
        if (dist_dag[i] == INT_MAX) {
            cout << "inf ";
        } else {
            cout << dist_dag[i] << " ";
        }
    }
    cout << endl;
}

void dfs_kosaraju(int nod) {
    visited[nod] = true;
    for (int vecin : lista_adj[nod]) {
        if (!visited[vecin]) {
            dfs_kosaraju(vecin);
        }
    }
    s.push(nod);
}

void dfs_transpus_kosaraju(int nod) {
    visited[nod] = true;
    componente.back().push_back(nod);
    for (int vecin : lista_adj_transpus[nod]) {
        if (!visited[vecin]) {
            dfs_transpus_kosaraju(vecin);
        }
    }
}

void kosaraju() {
    fill(visited, visited + n, false);
    while (!s.empty()) {
        s.pop();
    }
    for (int i = 0; i < n; i++) {
        lista_adj_transpus[i].clear();
    }
    componente.clear();
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfs_kosaraju(i);
        }
    }
    for (int u = 0; u < n; u++) {
        for (int v : lista_adj[u]) {
            lista_adj_transpus[v].push_back(u);
        }
    }
    fill(visited, visited + n, false);
    while (!s.empty()) {
        int nod = s.top();
        s.pop();
        if (!visited[nod]) {
            componente.push_back(vector<int>());
            dfs_transpus_kosaraju(nod);
        }
    }
    for (const auto& componenta : componente) {
        for (int nod : componenta) {
            cout << nod << " ";
        }
        cout << endl;
    }
}

bool bfs_flux(int s, int t) {
    fill(visited, visited + n, false);
    while (!q.empty()) {
        q.pop();
    }
    q.push(s);
    visited[s] = true;
    while (!q.empty()) {
        int i = q.front();
        q.pop();
        for (int j : lista_adj[i]) {
            if (!visited[j] && a[i][j] - flux[i][j] > 0) {
                parent[j] = i;
                visited[j] = true;
                q.push(j);
                if (j == t) {
                    return true;
                }
            }
        }
    }
    return false;
}

int edmonds_karp(int s, int t) {
    memset(flux, 0, sizeof(flux));
    int max_flow = 0;
    while (bfs_flux(s, t)) {
        int path_flow = INT_MAX;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, a[u][v] - flux[u][v]);
        }
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            flux[u][v] += path_flow;
            flux[v][u] -= path_flow;
        }
        max_flow += path_flow;
    }
    return max_flow;
}

bool dfs_flux(int s, int t) {
    fill(visited, visited + n, false);
    visited[s] = true;
    if (s == t) {
        return true;
    }
    for (int v : lista_adj[s]) {
        if (!visited[v] && a[s][v] - flux[s][v] > 0) {
            parent[v] = s;
            if (dfs_flux(v, t)) {
                return true;
            }
        }
    }
    return false;
}

int ford_fulkerson(int s, int t) {
    memset(flux, 0, sizeof(flux));
    fill(parent.begin(), parent.end(), -1);
    int max_flow = 0;
    while (true) {
        fill(visited, visited + n + 1, false);
        int path_flow = INT_MAX;
        if (!dfs_flux(s, t)) {
            break;
        }
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, a[u][v] - flux[u][v]);
        }
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            flux[u][v] += path_flow;
            flux[v][u] -= path_flow;
        }
        max_flow += path_flow;
    }
    return max_flow;
}

int min_cost_max_flow(int s, int t) {
    memset(flux, 0, sizeof(flux));
    int max_flow = 0;
    int min_cost = 0;
    while (true) {
        dijkstra(s);
        if (cost[t] == INT_MAX) {
            break;
        }
        int path_flow = INT_MAX;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, a[u][v] - flux[u][v]);
        }
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            flux[u][v] += path_flow;
            flux[v][u] -= path_flow;
            min_cost += path_flow * a[u][v];
        }
        max_flow += path_flow;
    }
    cout << "Flux maxim: " << max_flow << endl;
    cout << "Cost minim: " << min_cost << endl;
    return max_flow;
}

void sase_colorare() {
    fill(culoare.begin(), culoare.end(), 0);
    for (int i = 0; i < n; i++) {
        fill(culori_disponibile[i].begin(), culori_disponibile[i].end(), true);
    }
    for (int i = 0; i < n; i++) {
        grad[i] = lista_adj[i].size();
    }
    for (int i = 0; i < n; i++) {
        if (s.size() > 6) {
            if (grad[i] <= 5) {
                s.push(i);
                grad[i] = 0;
                for (int vecin : lista_adj[i]) {
                    grad[vecin]--;
                }
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        if (grad[i] > 0) {
            for (int vecin : lista_adj[i]) {
                if (culoare[vecin] != -1) {
                    culori_disponibile[i][culoare[vecin]] = false;
                }
            }
            for (int c = 0; c < 6; c++) {
                if (culori_disponibile[i][c]) {
                    culoare[i] = c;
                    break;
                }
            }
        }
    }
    while (!s.empty()) {
        int nod = s.top();
        s.pop();
        for (int vecin : lista_adj[nod]) {
            if (culoare[vecin] != -1) {
                culori_disponibile[nod][culoare[vecin]] = false;
            }
        }
        for (int c = 0; c < 6; c++) {
            if (culori_disponibile[nod][c]) {
                culoare[nod] = c;
                break;
            }
        }
    }
    cout << "Colorare finala:\n";
    for (int i = 0; i < n; i++) {
        cout << "nod " << i << " -> culoare " << culoare[i] << "\n";
    }
}

int levenshtein_distance(string cuvant1, string cuvant2) {
    int size1 = cuvant1.size();
    int size2 = cuvant2.size();
    for (int i = 0; i < n; i++) {
        fill(dist[i].begin(), dist[i].end(), INT_MAX);
    }
    for (int i = 0; i <= size1; i++) {
        dist[i][0] = i;
    }
    for (int j = 0; j <= size2; j++) {
        dist[0][j] = j;
    }
    for (int i = 1; i <= size1; i++) {
        for (int j = 1; j <= size2; j++) {
            if (cuvant1[i - 1] == cuvant2[j - 1]) {
                dist[i][j] = dist[i - 1][j - 1];
            } else {
                dist[i][j] = min({dist[i - 1][j] + 1, dist[i][j - 1] + 1, dist[i - 1][j - 1] + 1});
            }
        }
    }
    return dist[size1][size2];
}

void hierholzer() {
    for (int i = 1; i <= n; i++) {
        if (lista_adj[i].size() % 2 != 0) {
            cout << "Graful nu are circuit eulerian." << endl;
            return;
        }
    }
    int start = -1;
    for (int i = 1; i <= n; i++) {
        if (!lista_adj[i].empty()) {
            start = i;
            break;
        }
    }
    if (start == -1) {
        cout << "Graful este gol." << endl;
        return;
    }
    s.push(start);
    while (!s.empty()) {
        int nod = s.top();
        if (!lista_adj[nod].empty()) {
            int next_nod = lista_adj[nod].back();
            lista_adj[nod].pop_back();
            auto it = find(lista_adj[next_nod].begin(), lista_adj[next_nod].end(), nod);
            if (it != lista_adj[next_nod].end()) {
                lista_adj[next_nod].erase(it);
            }
            s.push(next_nod);
        } else {
            ciclu_eulerian.push_back(nod);
            s.pop();
        }
    }
    reverse(ciclu_eulerian.begin(), ciclu_eulerian.end());
    for (int nod : ciclu_eulerian) {
        cout << nod << " ";
    }
    cout << endl;
}

