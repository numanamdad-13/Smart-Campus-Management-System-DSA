#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <functional>
#include <cctype>

using namespace std;

//  Formatting Helpers

void printBanner(const string& title) {
    cout << "\n========================================================\n";
    cout << "  " << title << "\n";
    cout << "========================================================\n";
}

void printLine(const string& ch = "-", int len = 56) {
    for (int i = 0; i < len; i++) cout << ch;
    cout << "\n";
}

//  MODULE 1: GRAPH — Navigation

const int INF = numeric_limits<int>::max();

struct Edge {
    int to, weight;
};

class CampusGraph {
    int V;
    vector<vector<Edge>> adj;
    vector<string> locationNames;

public:
    CampusGraph(int vertices, const vector<string>& names)
        : V(vertices), adj(vertices), locationNames(names) {
    }

    void addEdge(int u, int v, int w) {
        adj[u].push_back({ v, w });
        adj[v].push_back({ u, w });
    }

    // DIJKSTRA'S SHORTEST PATH FROM SOURCE TO ALL 
    void dijkstra(int src) {
        vector<int> dist(V, INF), prev(V, -1);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        dist[src] = 0;
        pq.push({ 0, src });

        while (!pq.empty()) {
            auto temp = pq.top(); pq.pop();
            int d = temp.first;
            int u = temp.second;

            if (d > dist[u]) continue;

            for (auto& e : adj[u]) {
                if (dist[u] != INF && dist[u] + e.weight < dist[e.to]) {
                    dist[e.to] = dist[u] + e.weight;
                    prev[e.to] = u;
                    pq.push({ dist[e.to], e.to });
                }
            }
        }

        printBanner("DIJKSTRA — Shortest Paths from " + locationNames[src]);
        cout << left << setw(30) << "Destination" << setw(12) << "Distance" << "Path\n";
        printLine();
        for (int i = 0; i < V; i++) {
            if (i == src) continue;
            cout << left << setw(30) << locationNames[i];
            if (dist[i] == INF) {
                cout << setw(12) << "Unreachable" << "N/A\n";
            }
            else {
                cout << setw(12) << (to_string(dist[i]) + "m");
                vector<int> path;
                for (int x = i; x != -1; x = prev[x]) path.push_back(x);
                reverse(path.begin(), path.end());
                for (int j = 0; j < (int)path.size(); j++) {
                    if (j) cout << " -> ";
                    cout << locationNames[path[j]];
                }
                cout << "\n";
            }
        }
    }

    // ── FIND SINGLE SHORTEST PATH (Fixed Bug) ──
    pair<vector<int>, int> shortestPath(int src, int dst) {
        vector<int> dist(V, INF), prev(V, -1);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        dist[src] = 0;
        pq.push({ 0, src });

        while (!pq.empty()) {
            auto temp = pq.top(); pq.pop();
            int d = temp.first;
            int u = temp.second;

            if (d > dist[u]) continue;

            for (auto& e : adj[u]) {
                if (dist[u] != INF && dist[u] + e.weight < dist[e.to]) {
                    dist[e.to] = dist[u] + e.weight;
                    prev[e.to] = u;
                    pq.push({ dist[e.to], e.to });
                }
            }
        }

        vector<int> path;
        if (dist[dst] == INF) return { {}, INF };

        for (int x = dst; x != -1; x = prev[x]) path.push_back(x);
        reverse(path.begin(), path.end());
        return { path, dist[dst] };
    }

    void bfs(int src) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(src);
        visited[src] = true;

        printBanner("BFS Traversal from " + locationNames[src]);
        cout << "Order visited: ";
        while (!q.empty()) {
            int u = q.front(); q.pop();
            cout << locationNames[u];
            for (auto& e : adj[u]) {
                if (!visited[e.to]) {
                    visited[e.to] = true;
                    q.push(e.to);
                }
            }
            if (!q.empty()) cout << " -> ";
        }
        cout << "\n";
    }

    void dfsHelper(int u, vector<bool>& visited, vector<string>& result) {
        visited[u] = true;
        result.push_back(locationNames[u]);
        for (auto& e : adj[u]) {
            if (!visited[e.to]) dfsHelper(e.to, visited, result);
        }
    }

    void dfs(int src) {
        vector<bool> visited(V, false);
        vector<string> result;
        dfsHelper(src, visited, result);

        printBanner("DFS Traversal from " + locationNames[src]);
        cout << "Order visited: ";
        for (int i = 0; i < (int)result.size(); i++) {
            if (i) cout << " -> ";
            cout << result[i];
        }
        cout << "\n";
    }

    void displayMap() {
        printBanner("CAMPUS MAP — Adjacency List");
        for (int u = 0; u < V; u++) {
            cout << "[" << u << "] " << locationNames[u] << "\n";
            for (auto& e : adj[u]) {
                cout << "    <-> " << locationNames[e.to] << " (" << e.weight << "m)\n";
            }
        }
    }

    int getV() const { return V; }
    const vector<string>& getNames() const { return locationNames; }
};


//  MODULE 2: BST — Location Management

struct Location {
    string name;
    string type;
    int capacity;
    string contact;

    bool operator<(const Location& o) const { return name < o.name; }
};

struct BSTNode {
    Location data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(const Location& d) : data(d), left(nullptr), right(nullptr) {}
};

class LocationBST {
    BSTNode* root;

    BSTNode* insert(BSTNode* node, const Location& loc) {
        if (!node) return new BSTNode(loc);
        if (loc.name < node->data.name) node->left = insert(node->left, loc);
        else if (loc.name > node->data.name) node->right = insert(node->right, loc);
        else { cout << "  Location already exists.\n"; }
        return node;
    }

    BSTNode* search(BSTNode* node, const string& name) const {
        if (!node || node->data.name == name) return node;
        return name < node->data.name ? search(node->left, name) : search(node->right, name);
    }

    BSTNode* minNode(BSTNode* node) {
        while (node->left) node = node->left;
        return node;
    }

    BSTNode* deleteNode(BSTNode* node, const string& name) {
        if (!node) return nullptr;
        if (name < node->data.name)       node->left = deleteNode(node->left, name);
        else if (name > node->data.name)  node->right = deleteNode(node->right, name);
        else {
            if (!node->left) { BSTNode* tmp = node->right; delete node; return tmp; }
            if (!node->right) { BSTNode* tmp = node->left;  delete node; return tmp; }
            BSTNode* succ = minNode(node->right);
            node->data = succ->data;
            node->right = deleteNode(node->right, succ->data.name);
        }
        return node;
    }

    void inorder(BSTNode* node, vector<Location>& result) const {
        if (!node) return;
        inorder(node->left, result);
        result.push_back(node->data);
        inorder(node->right, result);
    }

    void destroyTree(BSTNode* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    LocationBST() : root(nullptr) {}
    ~LocationBST() { destroyTree(root); }

    void insert(const Location& loc) { root = insert(root, loc); }
    void remove(const string& name) { root = deleteNode(root, name); }

    Location* search(const string& name) const {
        BSTNode* n = search(root, name);
        return n ? &n->data : nullptr;
    }

    void displayAll() const {
        printBanner("LOCATION DIRECTORY (BST Inorder)");
        vector<Location> locs;
        inorder(root, locs);
        if (locs.empty()) { cout << "  No locations stored.\n"; return; }
        cout << left << setw(28) << "Name" << setw(12) << "Type" << setw(12) << "Capacity" << "Contact\n";
        printLine();
        for (auto& l : locs) {
            cout << left << setw(28) << l.name << setw(12) << l.type << setw(12) << l.capacity << l.contact << "\n";
        }
    }

    vector<Location> getAllSorted() const {
        vector<Location> locs;
        inorder(root, locs);
        return locs;
    }
};

//  MODULE 3: QUEUE — Request Handling

struct Request {
    int id;
    string user;
    string description;
    string type;
    int priority;

    string priorityStr() const {
        if (priority == 1) return "HIGH";
        if (priority == 2) return "MEDIUM";
        return "LOW";
    }
};

class RequestQueue {
    queue<Request> q;
    int nextId;

public:
    RequestQueue() : nextId(1001) {}

    void enqueue(const string& user, const string& desc, const string& type, int priority) {
        q.push({ nextId++, user, desc, type, priority });
        cout << "  Request #" << nextId - 1 << " queued successfully.\n";
    }

    void processNext() {
        if (q.empty()) {
            cout << "  No pending requests.\n";
            return;
        }
        Request r = q.front(); q.pop();
        printBanner("PROCESSING Request #" + to_string(r.id));
        cout << "  User       : " << r.user << "\n";
        cout << "  Type       : " << r.type << "\n";
        cout << "  Priority   : " << r.priorityStr() << "\n";
        cout << "  Description: " << r.description << "\n";
        cout << "  Request processed and removed from queue.\n";
    }

    void displayAll() const {
        printBanner("PENDING REQUESTS (Queue)");
        if (q.empty()) { cout << "  Queue is empty.\n"; return; }
        queue<Request> temp = q;
        cout << left << setw(8) << "ID" << setw(16) << "User" << setw(14) << "Type" << setw(10) << "Priority" << "Description\n";
        printLine();
        while (!temp.empty()) {
            Request r = temp.front(); temp.pop();
            cout << left << setw(8) << r.id << setw(16) << r.user << setw(14) << r.type << setw(10) << r.priorityStr() << r.description << "\n";
        }
    }

    int size() const { return (int)q.size(); }
    bool empty() const { return q.empty(); }
};

//  MODULE 4: STACK — Search History & Undo

struct SearchRecord {
    string query;
    string result;
    string timestamp;
};

class SearchHistoryStack {
    stack<SearchRecord> st;
    int counter;

public:
    SearchHistoryStack() : counter(0) {}

    void push(const string& query, const string& result) {
        counter++;
        string ts = "Search #" + to_string(counter);
        st.push({ query, result, ts });
        cout << "  Search saved to history.\n";
    }

    void undo() {
        if (st.empty()) {
            cout << "  History is empty — nothing to undo.\n";
            return;
        }
        SearchRecord r = st.top(); st.pop();
        cout << "  Undone: [" << r.timestamp << "] " << r.query << " -> " << r.result << "\n";
    }

    void peek() const {
        if (st.empty()) {
            cout << "  No recent search.\n"; return;
        }
        SearchRecord r = st.top();
        cout << "  Last search: [" << r.timestamp << "] " << r.query << " -> " << r.result << "\n";
    }

    void displayAll() const {
        printBanner("SEARCH HISTORY (Stack — top to bottom)");
        if (st.empty()) { cout << "  History is empty.\n"; return; }
        stack<SearchRecord> temp = st;
        cout << left << setw(12) << "Entry" << setw(24) << "Query" << "Result\n";
        printLine();
        while (!temp.empty()) {
            SearchRecord r = temp.top(); temp.pop();
            cout << left << setw(12) << r.timestamp << setw(24) << r.query << r.result << "\n";
        }
    }

    int size() const { return (int)st.size(); }
};

//  MODULE 5: HASH TABLE — Fast Location Lookup

struct HashNode {
    string key;
    Location value;
    HashNode* next;
    HashNode(const string& k, const Location& v)
        : key(k), value(v), next(nullptr) {
    }
};

class HashTable {
    static const int TABLE_SIZE = 17;
    HashNode* table[TABLE_SIZE];

    int hash(const string& key) const {
        unsigned long h = 5381;
        for (char c : key) h = ((h << 5) + h) + tolower(c);
        return (int)(h % TABLE_SIZE);
    }

    string toLower(string s) const {
        for (char& c : s) c = tolower(c);
        return s;
    }

public:
    HashTable() { fill(table, table + TABLE_SIZE, nullptr); }

    ~HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode* cur = table[i];
            while (cur) { HashNode* tmp = cur; cur = cur->next; delete tmp; }
        }
    }

    void insert(const Location& loc) {
        string key = toLower(loc.name);
        int idx = hash(key);
        HashNode* cur = table[idx];
        while (cur) {
            if (cur->key == key) { cur->value = loc; return; }
            cur = cur->next;
        }
        HashNode* node = new HashNode(key, loc);
        node->next = table[idx];
        table[idx] = node;
    }

    Location* search(const string& name) const {
        string key = toLower(name);
        int idx = hash(key);
        HashNode* cur = table[idx];
        while (cur) {
            if (cur->key == key) return &cur->value;
            cur = cur->next;
        }
        return nullptr;
    }

    void remove(const string& name) {
        string key = toLower(name);
        int idx = hash(key);
        HashNode* cur = table[idx];
        HashNode* prev = nullptr;
        while (cur) {
            if (cur->key == key) {
                if (prev) prev->next = cur->next;
                else       table[idx] = cur->next;
                delete cur;
                cout << "  Removed from hash table.\n";
                return;
            }
            prev = cur; cur = cur->next;
        }
        cout << "  Not found in hash table.\n";
    }

    void displayTable() const {
        printBanner("HASH TABLE (Separate Chaining)");
        for (int i = 0; i < TABLE_SIZE; i++) {
            cout << "  [" << setw(2) << i << "] ";
            if (!table[i]) { cout << "- (empty)\n"; continue; }
            HashNode* cur = table[i];
            while (cur) {
                cout << "\"" << cur->value.name << "\"";
                if (cur->next) cout << " -> ";
                cur = cur->next;
            }
            cout << "\n";
        }
    }

    int count() const {
        int n = 0;
        for (int i = 0; i < TABLE_SIZE; i++)
            for (HashNode* c = table[i]; c; c = c->next) n++;
        return n;
    }
};

//  MODULE 6: SORTING & SEARCHING

class SortSearch {
public:
    void mergeSort(vector<Location>& arr, int l, int r) {
        if (l >= r) return;
        int mid = l + (r - l) / 2;
        mergeSort(arr, l, mid);
        mergeSort(arr, mid + 1, r);
        merge(arr, l, mid, r);
    }

    void merge(vector<Location>& arr, int l, int mid, int r) {
        vector<Location> left(arr.begin() + l, arr.begin() + mid + 1);
        vector<Location> right(arr.begin() + mid + 1, arr.begin() + r + 1);
        int i = 0, j = 0, k = l;
        while (i < (int)left.size() && j < (int)right.size()) {
            if (left[i].name <= right[j].name) arr[k++] = left[i++];
            else                                arr[k++] = right[j++];
        }
        while (i < (int)left.size())  arr[k++] = left[i++];
        while (j < (int)right.size()) arr[k++] = right[j++];
    }

    void bubbleSortByDistance(vector<pair<string, int>>& arr) {
        int n = arr.size();
        for (int i = 0; i < n - 1; i++)
            for (int j = 0; j < n - i - 1; j++)
                if (arr[j].second > arr[j + 1].second)
                    swap(arr[j], arr[j + 1]);
    }

    int binarySearch(const vector<Location>& arr, const string& name) {
        int lo = 0, hi = (int)arr.size() - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (arr[mid].name == name)       return mid;
            else if (arr[mid].name < name)   lo = mid + 1;
            else                             hi = mid - 1;
        }
        return -1;
    }

    int linearSearch(const vector<Location>& arr, const string& name) {
        for (int i = 0; i < (int)arr.size(); i++)
            if (arr[i].name == name) return i;
        return -1;
    }

    void displaySortedByName(vector<Location> locs) {
        mergeSort(locs, 0, (int)locs.size() - 1);
        printBanner("LOCATIONS SORTED BY NAME (Merge Sort)");
        cout << left << setw(6) << "#" << setw(28) << "Name" << setw(14) << "Type" << "Capacity\n";
        printLine();
        for (int i = 0; i < (int)locs.size(); i++) {
            cout << left << setw(6) << (i + 1) << setw(28) << locs[i].name << setw(14) << locs[i].type << locs[i].capacity << "\n";
        }
    }

    void displaySortedByDistance(vector<pair<string, int>> items) {
        bubbleSortByDistance(items);
        printBanner("LOCATIONS SORTED BY DISTANCE (Bubble Sort)");
        cout << left << setw(6) << "#" << setw(34) << "Location" << "Distance\n";
        printLine();
        for (int i = 0; i < (int)items.size(); i++) {
            cout << left << setw(6) << (i + 1) << setw(34) << items[i].first << items[i].second << "m\n";
        }
    }
};

//  CAMPUS SYSTEM — Main Controller

class CampusSystem {
    CampusGraph* graph;
    LocationBST     bst;
    HashTable       hashTable;
    RequestQueue    requestQueue;
    SearchHistoryStack historyStack;
    SortSearch      sortSearch;

    void initCampus() {
        vector<Location> locs = {
            {"Admin Block",        "Admin",     500,  "Ext: 100"},
            {"Library",            "Academic",  600,  "Ext: 110"},
            {"CS Department",      "Academic",  300,  "Ext: 120"},
            {"EE Department",      "Academic",  300,  "Ext: 121"},
            {"Main Cafeteria",     "Facility",  400,  "Ext: 130"},
            {"Sports Complex",     "Sports",    1000, "Ext: 140"},
            {"Hostel Block A",     "Residence", 200,  "Ext: 150"},
            {"Hostel Block B",     "Residence", 200,  "Ext: 151"},
            {"Main Gate",          "Security",  50,   "Ext: 101"},
            {"Medical Centre",     "Health",    100,  "Ext: 160"},
            {"Lecture Hall A",     "Academic",  250,  "Ext: 122"},
            {"Parking Area",       "Facility",  300,  "Ext: 102"},
        };

        for (auto& l : locs) { bst.insert(l); hashTable.insert(l); }

        graph = new CampusGraph(12, {
            "Admin Block", "Library", "CS Department",
            "EE Department", "Main Cafeteria", "Sports Complex",
            "Hostel Block A", "Hostel Block B", "Main Gate",
            "Medical Centre", "Lecture Hall A", "Parking Area"
            });

        graph->addEdge(8, 0, 150);
        graph->addEdge(8, 11, 80);
        graph->addEdge(0, 1, 200);
        graph->addEdge(0, 2, 300);
        graph->addEdge(0, 3, 310);
        graph->addEdge(1, 2, 120);
        graph->addEdge(2, 3, 50);
        graph->addEdge(2, 10, 80);
        graph->addEdge(3, 10, 90);
        graph->addEdge(0, 4, 180);
        graph->addEdge(4, 5, 250);
        graph->addEdge(4, 9, 100);
        graph->addEdge(5, 6, 150);
        graph->addEdge(6, 7, 60);
        graph->addEdge(9, 0, 220);
        graph->addEdge(11, 1, 170);

        requestQueue.enqueue("Ahmed", "Navigate to Library from CS", "Navigation", 2);
        requestQueue.enqueue("Sara", "Electricity issue in EE Dept", "Maintenance", 1);
        requestQueue.enqueue("Ali", "Medical appointment booking", "Info", 2);
    }

    void printLocationNames() const {
        const auto& names = graph->getNames();
        cout << "\n  Available Campus Locations:\n";
        for (int i = 0; i < (int)names.size(); i++)
            cout << "    [" << i << "] " << names[i] << "\n";
    }

    int getValidNode(const string& prompt) const {
        printLocationNames();
        cout << prompt;
        int idx; cin >> idx; cin.ignore();
        if (idx < 0 || idx >= graph->getV()) {
            cout << "  Invalid index.\n";
            return -1;
        }
        return idx;
    }

public:
    CampusSystem() : graph(nullptr) { initCampus(); }
    ~CampusSystem() { delete graph; }

    void menuNavigation() {
        int choice;
        while (true) {
            printBanner("NAVIGATION MODULE");
            cout << "  1. Find Shortest Path (Dijkstra)\n";
            cout << "  2. Campus Map (All connections)\n";
            cout << "  3. BFS Traversal\n";
            cout << "  4. DFS Traversal\n";
            cout << "  5. All Shortest Paths from Source\n";
            cout << "  0. Back\n";
            cout << "\n  Choice: ";
            cin >> choice; cin.ignore();

            if (choice == 0) break;

            if (choice == 1) {
                int s = getValidNode("  Enter SOURCE index: ");
                int d = getValidNode("  Enter DESTINATION index: ");
                if (s < 0 || d < 0) continue;
                pair<vector<int>, int> result = graph->shortestPath(s, d);
                vector<int> path = result.first;
                int cost = result.second;
                printBanner("SHORTEST PATH");
                if (cost == INF) {
                    cout << "  No path found.\n";
                    historyStack.push(graph->getNames()[s] + " -> " + graph->getNames()[d], "Unreachable");
                }
                else {
                    cout << "  Total distance: " << cost << "m\n";
                    cout << "  Route: ";
                    for (int i = 0; i < (int)path.size(); i++) {
                        if (i) cout << " -> ";
                        cout << graph->getNames()[path[i]];
                    }
                    cout << "\n";
                    historyStack.push(graph->getNames()[s] + " -> " + graph->getNames()[d], to_string(cost) + "m");
                }
            }
            else if (choice == 2) {
                graph->displayMap();
            }
            else if (choice == 3) {
                int s = getValidNode("  Enter START index for BFS: ");
                if (s >= 0) graph->bfs(s);
            }
            else if (choice == 4) {
                int s = getValidNode("  Enter START index for DFS: ");
                if (s >= 0) graph->dfs(s);
            }
            else if (choice == 5) {
                int s = getValidNode("  Enter SOURCE index: ");
                if (s >= 0) graph->dijkstra(s);
            }
        }
    }

    void menuLocations() {
        int choice;
        while (true) {
            printBanner("LOCATION MANAGEMENT (BST)");
            cout << "  1. Add Location\n";
            cout << "  2. Search Location (BST)\n";
            cout << "  3. Search Location (Hash Table — O(1))\n";
            cout << "  4. Delete Location\n";
            cout << "  5. List All Locations (BST Inorder)\n";
            cout << "  6. Display Hash Table\n";
            cout << "  0. Back\n";
            cout << "\n  Choice: ";
            cin >> choice; cin.ignore();

            if (choice == 0) break;

            if (choice == 1) {
                Location loc;
                cout << "  Name     : "; getline(cin, loc.name);
                cout << "  Type     : "; getline(cin, loc.type);
                cout << "  Capacity : "; cin >> loc.capacity; cin.ignore();
                cout << "  Contact  : "; getline(cin, loc.contact);
                bst.insert(loc);
                hashTable.insert(loc);
                cout << "  Location added.\n";
            }
            else if (choice == 2) {
                cout << "  Search name: ";
                string name; getline(cin, name);
                Location* l = bst.search(name);
                if (l) {
                    cout << "  Found (BST):\n";
                    cout << "  Name: " << l->name << "  Type: " << l->type << "  Capacity: " << l->capacity << "  Contact: " << l->contact << "\n";
                    historyStack.push("BST search: " + name, "Found");
                }
                else {
                    cout << "  Not found (BST).\n";
                    historyStack.push("BST search: " + name, "Not found");
                }
            }
            else if (choice == 3) {
                cout << "  Search name: ";
                string name; getline(cin, name);
                Location* l = hashTable.search(name);
                if (l) {
                    cout << "  Found (Hash O(1)):\n";
                    cout << "  Name: " << l->name << "  Type: " << l->type << "  Capacity: " << l->capacity << "  Contact: " << l->contact << "\n";
                    historyStack.push("Hash search: " + name, "Found");
                }
                else {
                    cout << "  Not found (Hash Table).\n";
                    historyStack.push("Hash search: " + name, "Not found");
                }
            }
            else if (choice == 4) {
                cout << "  Delete name: ";
                string name; getline(cin, name);
                bst.remove(name);
                hashTable.remove(name);
                cout << "  Deleted from both BST and Hash Table.\n";
            }
            else if (choice == 5) {
                bst.displayAll();
            }
            else if (choice == 6) {
                hashTable.displayTable();
                cout << "  Total entries: " << hashTable.count() << "  |  Load factor: " << fixed << setprecision(2) << (double)hashTable.count() / 17.0 << "\n";
            }
        }
    }

    void menuRequests() {
        int choice;
        while (true) {
            printBanner("REQUEST HANDLING (Queue)");
            cout << "  1. Add New Request\n";
            cout << "  2. Process Next Request\n";
            cout << "  3. View All Pending Requests\n";
            cout << "  4. Queue Size\n";
            cout << "  0. Back\n";
            cout << "\n  Choice: ";
            cin >> choice; cin.ignore();

            if (choice == 0) break;
            if (choice == 1) {
                string user, desc, type;
                int priority;
                cout << "  Your name   : "; getline(cin, user);
                cout << "  Request type (Navigation/Info/Emergency/Maintenance): "; getline(cin, type);
                cout << "  Description : "; getline(cin, desc);
                cout << "  Priority (1=High, 2=Medium, 3=Low): "; cin >> priority; cin.ignore();
                if (priority < 1 || priority > 3) priority = 2;
                requestQueue.enqueue(user, desc, type, priority);
            }
            else if (choice == 2) {
                requestQueue.processNext();
            }
            else if (choice == 3) {
                requestQueue.displayAll();
            }
            else if (choice == 4) {
                cout << "  Pending requests: " << requestQueue.size() << "\n";
            }
        }
    }

    void menuHistory() {
        int choice;
        while (true) {
            printBanner("SEARCH HISTORY (Stack)");
            cout << "  1. View Full History\n";
            cout << "  2. Peek Last Search\n";
            cout << "  3. Undo Last Search\n";
            cout << "  4. History Size\n";
            cout << "  0. Back\n";
            cout << "\n  Choice: ";
            cin >> choice; cin.ignore();

            if (choice == 0) break;
            if (choice == 1) historyStack.displayAll();
            else if (choice == 2) historyStack.peek();
            else if (choice == 3) historyStack.undo();
            else if (choice == 4) cout << "  History entries: " << historyStack.size() << "\n";
        }
    }

    void menuSortSearch() {
        int choice;
        while (true) {
            printBanner("SORT & SEARCH MODULE");
            cout << "  1. Sort Locations by Name (Merge Sort)\n";
            cout << "  2. Sort Locations by Distance from a Node (Bubble Sort)\n";
            cout << "  3. Binary Search by Name\n";
            cout << "  4. Linear Search by Name\n";
            cout << "  0. Back\n";
            cout << "\n  Choice: ";
            cin >> choice; cin.ignore();

            if (choice == 0) break;

            vector<Location> locs = bst.getAllSorted();
            if (locs.empty()) { cout << "  No locations in BST.\n"; continue; }

            if (choice == 1) {
                sortSearch.displaySortedByName(locs);
            }
            else if (choice == 2) {
                int src = getValidNode("  Source index for distances: ");
                if (src < 0) continue;
                vector<pair<string, int>> items;
                const auto& names = graph->getNames();

                // Fixed inner loop to prevent logic freezing
                for (int i = 0; i < graph->getV(); i++) {
                    if (i == src) continue;
                    pair<vector<int>, int> result2 = graph->shortestPath(src, i);
                    int c = result2.second;
                    if (c < INF) items.push_back({ names[i], c });
                    else         items.push_back({ names[i], 99999 });
                }
                sortSearch.displaySortedByDistance(items);
            }
            else if (choice == 3) {
                vector<Location> sorted = locs;
                sortSearch.mergeSort(sorted, 0, (int)sorted.size() - 1);
                cout << "  Search name: ";
                string name; getline(cin, name);
                int idx = sortSearch.binarySearch(sorted, name);
                if (idx >= 0) {
                    cout << "  Found at index " << idx << ": " << sorted[idx].name << " | " << sorted[idx].type << "\n";
                    historyStack.push("Binary search: " + name, "Found at idx " + to_string(idx));
                }
                else {
                    cout << "  Not found (Binary Search).\n";
                    historyStack.push("Binary search: " + name, "Not found");
                }
            }
            else if (choice == 4) {
                cout << "  Search name: ";
                string name; getline(cin, name);
                int idx = sortSearch.linearSearch(locs, name);
                if (idx >= 0) {
                    cout << "  Found at index " << idx << ": " << locs[idx].name << "\n";
                    historyStack.push("Linear search: " + name, "Found");
                }
                else {
                    cout << "  Not found (Linear Search).\n";
                    historyStack.push("Linear search: " + name, "Not found");
                }
            }
        }
    }

    void run() {
        printBanner("SMART CAMPUS MANAGEMENT SYSTEM\n  CSC-162 DSA Project");

        int choice;
        while (true) {
            printBanner("MAIN MENU");
            cout << "  1. Navigation Module     (Graph: Dijkstra, BFS, DFS)\n";
            cout << "  2. Location Management   (BST: insert, search, delete)\n";
            cout << "  3. Request Handling      (Queue: enqueue, dequeue)\n";
            cout << "  4. Search History        (Stack: push, pop, undo)\n";
            cout << "  5. Sort & Search         (Merge Sort, Bubble Sort, Binary/Linear Search)\n";
            cout << "  0. Exit\n";
            cout << "\n  Choice: ";
            cin >> choice; cin.ignore();

            switch (choice) {
            case 1: menuNavigation(); break;
            case 2: menuLocations();  break;
            case 3: menuRequests();   break;
            case 4: menuHistory();    break;
            case 5: menuSortSearch(); break;
            case 0:
                cout << "\n  Thank you! Goodbye.\n";
                return;
            default:
                cout << "  Invalid choice. Try again.\n";
            }
        }
    }
};




int main() {
    CampusSystem system;
    system.run();
    return 0;
}