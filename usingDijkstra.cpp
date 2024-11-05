#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
using namespace std;

struct Edge {
    int to;   // 연결된 역의 번호
    int time; // 이동 시간
};

const int MAX = 1001; // 이 문제에서 무한대를 뜻하는 수임.
vector<vector<Edge>> graph; // 역의 인접 리스트
unordered_map<int, string> station_names; // 역 번호와 역 이름을 매핑

// 다익스트라 알고리즘을 이용하여 최단 경로를 찾음
void dijkstra(int start, vector<int>& dist, vector<int>& prev) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (const auto& edge : graph[u]) {
            int v = edge.to;
            int time = edge.time;

            if (dist[u] + time < dist[v]) {
                dist[v] = dist[u] + time;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

// 최단 경로를 출력
void printShortestPath(int start, int dest, const vector<int>& prev) {
    stack<int> path;
    int at = dest;
    while (at != -1) {
        path.push(at);
        at = prev[at];
    }

    cout << path.size() << " ";
    while (!path.empty()) {
        cout << station_names[path.top()];
        path.pop();
        if (!path.empty()) cout << " ";
    }
    cout << endl;
}

int main() {
    int n, q;
    cin >> n >> q;

    graph.resize(n + 1); // 그래프의 크기를 조정

    unordered_map<int, int> last_station_of_line; // 각 노선의 마지막 역 번호를 저장하는 맵

    for (int i = 0; i < n; ++i) {
        int id, line, time;
        string name;
        cin >> id >> line >> name >> time;

        station_names[id] = name; // 역 이름 저장

        if (last_station_of_line.find(line) != last_station_of_line.end()) {
            int prev_id = last_station_of_line[line];
            graph[prev_id].push_back({id, time});
            graph[id].push_back({prev_id, time});
        }

        last_station_of_line[line] = id; // 현재 역을 해당 노선의 마지막 역으로 업데이트
    }

    // 환승역 처리
    unordered_map<string, vector<int>> name_to_ids;
    for (const auto& [id, name] : station_names) {
        name_to_ids[name].push_back(id);
    }

    for (const auto& [name, ids] : name_to_ids) {
        for (size_t i = 0; i < ids.size(); ++i) {
            for (size_t j = i + 1; j < ids.size(); ++j) {
                graph[ids[i]].push_back({ids[j], 0});
                graph[ids[j]].push_back({ids[i], 0});
            }
        }
    }

    // 질의 처리
    for (int i = 0; i < q; ++i) {
        char choice;
        int depart, dest;
        cin >> choice >> depart >> dest;

        vector<int> dist(n + 1, MAX);
        vector<int> prev(n + 1, -1);
        dijkstra(depart, dist, prev);

        if (choice == 'A') {
            if (dist[dest] == MAX || depart == dest) {
                cout << "None" << endl;
            } else {
                cout << dist[dest] << endl;
            }
        } else if (choice == 'B') {
            if (dist[dest] == MAX || depart == dest) {
                cout << "None" << endl;
            } else {
                printShortestPath(depart, dest, prev);
            }
        }
    }

    return 0;
}