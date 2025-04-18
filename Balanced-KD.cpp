#include <vector>
#include <queue>
#include <string>
#include <algorithm>
using namespace std;

class balancedKD {
    public:
        class Node {
            public:
                Node *left, *right;
                string name;
                vector<vector<string>> data;
                Node(vector<vector<string>> a) {
                    left = right = nullptr;
                    name = a[0][0]; // Assuming the first element is the name
                    a.erase(a.begin()); // Remove the name from the data vector
                    data = a;
                }
        };

        Node* head;
        // Constructor to create a balanced KD tree from the given data as a vector of vectors of strings
        balancedKD(vector<vector<vector<string>>> data) {
            head = buildKDTree(data, 0, data.size() - 1, 0);
        }
        
        // Finds the nearest K neighbors to a given vector
        // Vector contains - 
        /* 
        0 - Type
        1 - Actor/Directors
        2 - Rating
        3 - duration
        4 - Genre
        */
        vector<string> findNearestNeighbors(vector<string> target, int k) {
            vector<vector<string>> targetData(target.size(), vector<string>(1));
            for (int i = 0; i < target.size(); ++i) {
                targetData[i][0] = target[i]; // Change to 2D vector for the real function
            }
            return findNearestNeighbors(targetData, k);
        }

        vector<string> findNearestNeighbors(vector<vector<string>> target, int k) {
            priority_queue<Neighbor> result;
            findNearestNeighborsHelper(head, target, result, k, 0);
            // Turn the queue into a vector and return it
            vector<string> neighbors;
            while (!result.empty()) {
                neighbors.push_back(result.top().name);
                result.pop();
            }
            while (neighbors.size() < k) {
                neighbors.push_back("No results found"); // Fill with "No results found" if less than k neighbors found
            }
            return neighbors;
        }

        // Destructor to clean up the KD tree
        ~balancedKD() {
            queue<Node*> nodes;
            nodes.push(head);
            while (!nodes.empty()) {
                Node* current = nodes.front();
                nodes.pop();
                if (current) {
                    if (current->left != nullptr) nodes.push(current->left);
                    if (current->right != nullptr) nodes.push(current->right);
                    delete current;
                }
            }
        }

    private:

        struct Neighbor {
            int distance;
            string name;
            vector<vector<string>> data;
        
            // Custom comparison for max priority queue based on Task priority
            bool operator<(const Neighbor& other) const {
                return distance < other.distance; // For max heap, we want the largest distance at the top
            }
        };

        // Function to build the KD tree recursively from a vector of vectors of strings
        Node* buildKDTree(vector<vector<vector<string>>>& data, int start, int end, int depth) {
            if (data.empty() || start > end) return nullptr;
            int k = data[0].size(); // Number of dimensions
            int axis = depth % k; // Current axis to split on

            // Sort the data based on the current axis
            sort(data.begin() + start, data.begin() + end, [axis](const vector<vector<string>>& a, const vector<vector<string>>& b) {
                return a[axis][0] < b[axis][0];
            });

            // Find the median index
            int medianIndex = end-start / 2;
            Node* node = new Node(data[medianIndex]);

            // Recursively build the left and right subtrees
            node->left = buildKDTree(data, start, medianIndex-1, depth + 1);
            node->right = buildKDTree(data, medianIndex + 1, end, depth + 1);

            return node;
        }

        // Function to find the nearest k neighbors recursively
        // If the target has "None" as a value then search both subtrees    
        void findNearestNeighborsHelper(Node* node, vector<vector<string>> target, priority_queue<Neighbor>& result, int k, int depth) {
            if (node == nullptr) return;
            // Weights vector
            vector<int> weights(5);
            weights[0] = 300; // Type
            weights[1] = 10000;// Actor/Directors
            weights[2] = 200; // Rating
            weights[3] = 30; // Duration
            weights[4] = 50; // Genre
            // Calculate the distance between the current node and the target
            int distance = 0;
            for (int i = 0; i < target.size(); ++i) {
                for (int j = 0; j<target[i].size(); j++) {
                    if (target[i][j] != "None") {
                        if (j > node->data[i].size() || find(node->data[i].begin(),node->data[i].end(), target[i][j]) == node->data[i].end()) {
                            distance+= weights[i]; // Add the weight of the dimension if the target is not found in the node's data
                        }
                    }
                }
            }

            // Add the current node to the result if it is a valid neighbor
            Neighbor neighbor;
            neighbor.distance = distance;
            neighbor.name = (node->name);
            neighbor.data = node->data;
            result.push(neighbor);

            if (result.size() > k) {
                result.pop(); // Remove the farthest neighbor if we have more than k neighbors
            } 

            // Determine which side to search next based on the current axis
            int axis = depth % target.size();
            Node* nextNode = (target[axis][0] == "None" || node->data[axis][0] < target[axis][0]) ? node->left : node->right;
            Node* otherNode = (nextNode == node->left) ? node->right : node->left;

            // Search the next node
            findNearestNeighborsHelper(nextNode, target, result, k, depth + 1);

            // Check if we need to search the other side of the tree
            if ((result.size() < k || distance < result.top().distance+300) || target[axis][0] == "None") {
                // Search the other node if the distance is less than the current farthest neighbor
                // or if the target has "None" as a value
                findNearestNeighborsHelper(otherNode, target, result, k, depth + 1);
            }
        }
};
