#include <vector>
#include <queue>
#include <string>
using namespace std;

class balancedKD {
    public:
        Node* head;
        // Constructor to create a balanced KD tree from the given data as a vector of vectors of strings
        // TODO: Refactor for a 3D vector of vectors of strings
        balancedKD(vector<vector<string>> data) {
            head = buildKDTree(data, 0, data.size() - 1, 0);
        }
        // Finds the nearest K neighbors to a given vector
        // TODO: Describe the target vector
        // 
        // TODO: Refactor to use a vector of vectors of strings
        vector<string> findNearestNeighbors(vector<string> target, int k) {
            priority_queue<Neighbor> result;
            findNearestNeighborsHelper(head, target, result, k, 0);
            // Turn the queue into a vector and return it
            vector<string> neighbors;
            while (!result.empty()) {
                neighbors.push_back(result.front().data);
                result.pop();
            }
            return neighbors;
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

        struct Neighbor {
            int distance;
            vector<string> data;
        
            // Custom comparison for max priority queue based on Task priority
            bool operator<(const Neighbor& other) const {
                return distance > other.distance; // For max heap, we want the largest distance at the top
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
        // If the target has "None" as a value then search both subtrees
        // TODO: Describe the target vector

    
        void findNearestNeighborsHelper(Node* node, vector<string> target, priority_queue<Neighbor>& result, int k, int depth) {
            if (node == nullptr) return;

            // Calculate the distance between the current node and the target
            int distance = 0;
            for (int i = 0; i < target.size(); ++i) {
                if (target[i] != "None") {
                    if (node->data[i] != target[i]) {
                        distance++;
                    }
                }
            }

            // Add the current node to the result if it is a valid neighbor
            Neighbor neighbor;
            neighbor.distance = distance;
            neighbor.data = node->data;
            result.push(neighbor);

            if (result.size() < k) {
                result.pop(); // Remove the farthest neighbor if we have more than k neighbors
            } 

            // Determine which side to search next based on the current axis
            int axis = depth % target.size();
            Node* nextNode = (target[axis] == "None" || node->data[axis] < target[axis]) ? node->left : node->right;
            Node* otherNode = (nextNode == node->left) ? node->right : node->left;

            // Search the next node
            findNearestNeighbor(nextNode, target, result, k, depth + 1);

            // Check if we need to search the other side of the tree
            if ((result.size() < k || abs(stoi(node->data[axis]) - stoi(target[axis])) < result.top().distance) || target[axis] == "None") {
                // Search the other node if the distance is less than the current farthest neighbor
                // or if the target has "None" as a value
                findNearestNeighbor(otherNode, target, result, k, depth + 1);
            }
        }
}