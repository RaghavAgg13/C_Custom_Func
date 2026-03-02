#ifndef TREE_GENERATOR_H
#define TREE_GENERATOR_H

#include <vector>
#include <iostream>
using namespace std;

// A struct to hold the generated tree details
struct RandomTree {
    int root;           // The selected root (1-based index)
    int n;              // Number of nodes
    vector<vector<int>> adj; // Adjacency list (1-based indexing)

    // Helper to print the tree for debugging
    void print() const {
        cout << "Tree Details:\n";
        cout << "Root: " << root << "\n";
        cout << "Nodes: " << n << "\n";
        cout << "Edges:\n";
        for (int i = 1; i <= n; ++i) {
            cout << i << ": ";
            for (int neighbor : adj[i]) {
                cout << neighbor << " ";
            }
            cout << "\n";
        }
    }
};

/**
 * Generates a random tree with n nodes.
 * @param n Number of nodes in the tree.
 * @return RandomTree struct containing the root, size, and adjacency list.
 */
RandomTree generateRandomTree(int n);
void visualizeTree(const RandomTree& tree);

#endif