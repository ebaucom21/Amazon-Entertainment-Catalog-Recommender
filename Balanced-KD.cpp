#include <vector>
using namespace std;

class balancedKD {
    public:
        Node* head;
        // Constructor to create a balanced KD tree from the given data as a vector of vectors of strings
        balancedKD(vector<vector<string>> data) {
            head = buildKDTree(data, 0, data.size() - 1, 0);
        }
        // Finds the nearest K neighbors to a given vector
        vector<string> findNearestNeighbors(vector<string> target, int k) {
            vector<string> result;
            return findNearestNeighbor(head, target, result, k, 0);
        }


    private:
        class Node {
            public:
                Node *left, *right;
                vector<string> data;
                Node(vector<string> a) {
                    left = right = nullptr;
                    data = a;
                }
        };

        // Function to build the KD tree recursively from a vector of vectors of strings
        Node* buildKDTree(vector<vector<string>> data&, int start, int end, int depth) {
            if (data.empty()) return nullptr;
            int k = data[0].size(); // Number of dimensions
            int axis = depth % k; // Current axis to split on

            // Sort the data based on the current axis
            sort(data.begin()+start, data.end()+end, [axis](const vector<string>& a, const vector<string>& b) {
                return a[axis] < b[axis];
            });

            // Find the median index
            int medianIndex = data.size() / 2;
            Node* node = new Node(data[medianIndex]);

            // Recursively build the left and right subtrees
            node->left = buildKDTree(data, start, medianIndex-1, depth + 1);
            node->right = buildKDTree(data, medianIndex + 1, end, depth + 1);

            return node;
        }

        // Function to find the nearest k neighbors recursively
        vector<string> findNearestNeighbor(Node* node, vector<string> target, vector<string> result&, int k, int depth) {
            
        }
}