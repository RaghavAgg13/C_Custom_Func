#include "TreeGenerator.h"
#include <random>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <fstream> // Needed for file writing
#include <cstdlib> // Needed for system()
using namespace std;

RandomTree generateRandomTree(int n) {
    RandomTree tree;
    tree.n = n;
    
    // Handle edge case for empty or single node tree
    if (n <= 0) return tree;
    if (n == 1) {
        tree.root = 1;
        tree.adj.resize(2); // index 0 unused, index 1 empty
        return tree;
    }

    // Resize adjacency list for 1-based indexing (size n + 1)
    tree.adj.resize(n + 1);

    // 1. Setup Random Number Generator
    // We use a high-quality mersenne_twister_engine
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 rng(seed);

    // 2. Create a permutation of nodes 1 to N
    // We shuffle the node labels so the tree structure doesn't always "grow" from node 1.
    vector<int> nodes(n);
    iota(nodes.begin(), nodes.end(), 1); // Fill with 1, 2, ..., n
    shuffle(nodes.begin(), nodes.end(), rng);

    // 3. Tree Growth Algorithm
    // - We treat nodes[0] as the initial "tree".
    // - For every subsequent node (nodes[i]), we attach it to a random node
    //   that is ALREADY in the tree (nodes[0...i-1]).
    // - This guarantees connectedness and NO loops.
    for (int i = 1; i < n; ++i) {
        // Pick a random index from the previously added nodes (0 to i-1)
        uniform_int_distribution<int> dist(0, i - 1);
        int parentIndex = dist(rng);

        int u = nodes[i];           // The new node
        int v = nodes[parentIndex]; // The existing node

        // Add undirected edge
        tree.adj[u].push_back(v);
        tree.adj[v].push_back(u);
    }

    // 4. Select a random root (or just pick nodes[0])
    uniform_int_distribution<int> rootDist(0, n - 1);
    tree.root = nodes[rootDist(rng)];

    return tree;
}

void visualizeTree(const RandomTree& tree) {
    std::ofstream htmlFile("tree_viz.html");
    
    // 1. Write HTML Header & CSS
    htmlFile << R"(
    <!DOCTYPE html>
    <html>
    <head>
        <title>Random Tree Visualization</title>
        <script type="text/javascript" src="https://unpkg.com/vis-network/standalone/umd/vis-network.min.js"></script>
        <style type="text/css">
            #mynetwork { width: 100%; height: 90vh; border: 1px solid lightgray; }
            body { font-family: sans-serif; text-align: center; }
        </style>
    </head>
    <body>
        <h2>Tree Visualization (Root: )" << tree.root << R"()</h2>
        <div id="mynetwork"></div>
        <script type="text/javascript">
            var nodes = new vis.DataSet([
    )";

    // 2. Write Nodes (Javascript Array)
    for (int i = 1; i <= tree.n; ++i) {
        std::string color = (i == tree.root) ? "#ff9999" : "#97c2fc"; // Red for root, Blue for others
        std::string label = std::to_string(i);
        // JS format: { id: 1, label: '1', color: '...' },
        htmlFile << "{ id: " << i << ", label: '" << label << "', color: '" << color << "' },\n";
    }

    htmlFile << "]);\n";
    htmlFile << "var edges = new vis.DataSet([\n";

    // 3. Write Edges (Javascript Array)
    // We iterate 1 to n. Since adj is undirected (u-v and v-u), 
    // we only print if u < v to avoid duplicates in the visual.
    for (int u = 1; u <= tree.n; ++u) {
        for (int v : tree.adj[u]) {
            if (u < v) {
                htmlFile << "{ from: " << u << ", to: " << v << " },\n";
            }
        }
    }

    // 4. Write JS Boilerplate to render
    htmlFile << R"(
            ]);
            var container = document.getElementById('mynetwork');
            var data = { nodes: nodes, edges: edges };
            var options = {
                layout: {
                    hierarchical: {
                        enabled: true,
                        direction: "UD", // Up-Down
                        sortMethod: "directed"
                    }
                },
                edges: { smooth: false },
                physics: { stabilization: false }
            };
            var network = new vis.Network(container, data, options);
        </script>
    </body>
    </html>
    )";

    htmlFile.close();
    std::cout << "Visualization saved to tree_viz.html\n";

    // 5. Open the file automatically
    // Windows command:
    system("start tree_viz.html");
    // If on Mac use: system("open tree_viz.html");
    // If on Linux use: system("xdg-open tree_viz.html");
}