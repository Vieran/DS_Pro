/* graph lab */

// 1. design data structure
// 2. read from the file
// 3. find the shortest path in the graph --- dijkstra
// 4. calculate several centrality

// from this lab, we noticed that the filestream behaves different on Linux and Windows

#include <iostream>
#include <queue>
#include <stack>
#include <fstream>
#include <sstream>
using namespace std;

struct edge {
    int n1, n2;
    int weight;
    edge(int n1, int n2, int weight) : n1(n1), n2(n2), weight(weight) {}
    /*
    friend bool operator < (edge e1, edge e2) {
        return e1.weight > e2.weight;
    }
    */
};

struct graphNode {
    int id;
    vector<edge> edges;
    graphNode(){}
};

class socialNetwork
{
private:
    int nodeNum;
    graphNode *graph;
    double *cc;
    int *bc;

    void apply_dijkstra(int start_node);
    void update_dist(int *dist, int *path, vector<edge> &edges);
    void initialize();
public:
    socialNetwork(char **argv);
    ~socialNetwork() {
        delete [] graph;
        delete [] cc;
        delete [] bc;
    }
    void get_dc();
    void get_cc();
    void get_bc();
};

void socialNetwork::update_dist(int *dist, int *path, vector<edge> &edges) {
    for (vector<edge>::iterator i = edges.begin(); i != edges.end(); i++) {
        if (dist[(*i).n2] > dist[(*i).n1] + (*i).weight) {
            dist[(*i).n2] = dist[(*i).n1] + (*i).weight;
            path[(*i).n2] = (*i).n1;
        }
    }
}
// find node i's shortest path to all other nodes
void socialNetwork::apply_dijkstra(int start_node) {
    int path[nodeNum];
    int dist[nodeNum];
    for (int i = 0; i < nodeNum; i++)
        dist[i] = ~(1 << 31);
    bool explored_node[nodeNum] = {false};

    dist[start_node] = 0;
    path[start_node] = start_node;
    explored_node[start_node] = true;
    update_dist(dist, path, graph[start_node].edges);
    for (int i = 1; i < nodeNum; i++) {
        int min = ~(1 << 31);
        int min_node;
        for (int k = 0; k < nodeNum; k++)  // find the minimal node
            if (!explored_node[k] && min > dist[k]) {
                min_node = k;
                min = dist[min_node];
            }
        explored_node[min_node] = true;
        update_dist(dist, path, graph[min_node].edges);
    }

    // reverse and update cc and bc
    for (int i = 0; i < nodeNum; i++) {
        cc[start_node] += dist[i];
        int tmp = path[i];
        while (tmp != start_node) {
            bc[tmp]++;
            tmp = path[tmp];
        }
    }
}

// set dc, cc and bc as zero
void socialNetwork::initialize() {
    for (int i = 0; i < nodeNum; i++) {
        cc[i] = 0;
        bc[i] = 0;
    }
}

// construct the graph
socialNetwork::socialNetwork(char **argv){
    //ifstream file(argv[1]);
    ifstream file("data.txt");
    // malloc space for the graph
    file >> nodeNum;
    graph = new graphNode[nodeNum];
    cc = new double[nodeNum];
    bc = new int[nodeNum];

    // construct the graph
    int n1, n2, w;
    while (true) {
        file >> n1;
        if (file.peek() == ',')
            file.ignore();
        file >> n2;
        if (file.peek() == ',')
            file.ignore();
        file >> w;
        if (file.eof())
            break;
        
        // each edge is stored twice
        edge e1(n1, n2, w);
        graph[n1].id = n1;
        graph[n1].edges.push_back(e1);
        edge e2(n2, n1, w);
        graph[n2].id = n2;
        graph[n2].edges.push_back(e2);
    }
    initialize();

    for (int i = 0; i < nodeNum; i++)
        apply_dijkstra(i);
}

void socialNetwork::get_dc() {
    int max_degree = -1;
    int max_node = -1;
    for (int i = 0; i < nodeNum; i++) {
        int e_size = graph[i].edges.size();
        if (e_size > max_degree) {
            max_degree = e_size;
            max_node = i;
        }
    }
    cout << max_node << " " << max_degree << endl;
}

void socialNetwork::get_cc() {
    int node, min = ~(1 << 31);
    for (int i = 0; i < nodeNum; i++)
        if (cc[i] < min) {
            node = i;
            min = cc[i];
        }
    
    cout << node << " " << (nodeNum - 1) / double(min) << endl;
}

void socialNetwork::get_bc() {
    int node, max = 0;
    for (int i = 0; i < nodeNum; i++)
        if (bc[i] > max) {
            node = i;
            max = bc[i];
        }
    
    cout << node << " " << max / 2 << endl;
}

int main(int argc, char** argv) {
    socialNetwork sn(argv);
    sn.get_dc();
    sn.get_cc();
    sn.get_bc();
    return 0;
}