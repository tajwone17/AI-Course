#include <iostream>
#include <vector>
#include <climits>
using namespace std;

struct Node
{
    bool isLeaf;
    int value;
    vector<Node *> children;

    // Leaf node
    Node(int val) : isLeaf(true), value(val) {}

    // Internal node
    Node(vector<Node *> ch) : isLeaf(false), value(0), children(ch) {}
};

int minmax(Node *node, bool maxPlayer, int alpha, int beta)
{
    if (node->isLeaf)
        return node->value;

    if (maxPlayer)
    {
        int best = INT_MIN;
        for (Node *child : node->children)
        {
            int val = minmax(child, false, alpha, beta);
            best = max(best, val);
            alpha = max(alpha, best);
            if (beta <= alpha)
                break; // beta cutoff
        }
        return best;
    }
    else
    {
        int best = INT_MAX;
        for (Node *child : node->children)
        {
            int val = minmax(child, true, alpha, beta);
            best = min(best, val);
            beta = min(beta, best);
            if (beta <= alpha)
                break; // alpha cutoff
        }
        return best;
    }
}

Node *buildTreeFromUserInput()
{
    cout << "Building the game tree from user input..." << endl;

    cout << "\nIs the root a MAX (1) or MIN (0) node? ";
    bool rootIsMax;
    cin >> rootIsMax;

    cout << "Enter the total number of internal nodes (non-leaf nodes): ";
    int internalNodeCount;
    cin >> internalNodeCount;

    cout << "Enter the total number of leaf nodes: ";
    int leafNodeCount;
    cin >> leafNodeCount;

    // Create storage for all nodes
    vector<Node *> nodes(internalNodeCount + leafNodeCount, nullptr);

    // First create all leaf nodes
    cout << "\n--- Creating leaf nodes ---" << endl;
    for (int i = 0; i < leafNodeCount; i++)
    {
        cout << "Enter value for leaf node " << (internalNodeCount + i) << ": ";
        int value;
        cin >> value;
        nodes[internalNodeCount + i] = new Node(value);
    }

    // Now create internal nodes starting from the bottom-most level
    cout << "\n--- Creating internal nodes ---" << endl;
    for (int i = internalNodeCount - 1; i >= 0; i--)
    {
        cout << "For internal node " << i << ":" << endl;
        cout << "Enter number of children: ";
        int childCount;
        cin >> childCount;

        vector<Node *> children;
        for (int j = 0; j < childCount; j++)
        {
            cout << "Enter child " << (j + 1) << " node ID: ";
            int childId;
            cin >> childId;

            if (childId < 0 || childId >= nodes.size() || nodes[childId] == nullptr)
            {
                cout << "Invalid node ID. Please enter a valid node ID that's already defined." << endl;
                j--; // Retry
                continue;
            }

            children.push_back(nodes[childId]);
        }

        nodes[i] = new Node(children);
    }

    cout << "\nTree construction complete!" << endl;
    return nodes[0]; // Return the root
}

// Set of pointers to track which nodes have been deleted
void cleanupTree(Node *node, vector<Node *> &visited)
{
    if (!node)
        return;

    // Check if we've already processed this node
    for (Node *visitedNode : visited)
    {
        if (node == visitedNode)
        {
            return; // Already visited
        }
    }

    // Mark as visited
    visited.push_back(node);

    // Recursively delete children
    if (!node->isLeaf)
    {
        for (Node *child : node->children)
        {
            cleanupTree(child, visited);
        }
    }
}

// Display the tree structure (helpful for debugging)
void printTree(Node *node, int depth, bool isMax)
{
    if (!node)
        return;

    // Print indentation
    for (int i = 0; i < depth; i++)
    {
        cout << "  ";
    }

    // Print node info
    if (node->isLeaf)
    {
        cout << "Leaf: " << node->value << endl;
    }
    else
    {
        cout << (isMax ? "MAX" : "MIN") << " Node with " << node->children.size() << " children" << endl;

        // Print children
        for (size_t i = 0; i < node->children.size(); i++)
        {
            printTree(node->children[i], depth + 1, !isMax);
        }
    }
}

int main()
{
    cout << "Alpha-Beta Pruning Algorithm" << endl;
    cout << "===========================" << endl;

    // Build the tree using user input
    Node *root = buildTreeFromUserInput();

    // Ask if user wants to see the tree structure
    char showTree;
    cout << "\nDo you want to see the tree structure? (y/n): ";
    cin >> showTree;

    if (showTree == 'y' || showTree == 'Y')
    {
        cout << "\nTree Structure:" << endl;
        cout << "==============" << endl;
        bool rootIsMax;
        cout << "Is the root a MAX node? (1 for yes, 0 for no): ";
        cin >> rootIsMax;
        printTree(root, 0, rootIsMax);
    }

    // Run Alpha-Beta pruning
    int result = minmax(root, true, INT_MIN, INT_MAX);
    cout << "\nResult of Alpha-Beta Pruning: " << result << endl;

    // Cleanup - using a vector of pointers to avoid double-deletion
    vector<Node *> visitedNodes;
    cleanupTree(root, visitedNodes);

    // Delete all nodes in the visitedNodes vector
    for (Node *node : visitedNodes)
    {
        delete node;
    }

    return 0;
}
