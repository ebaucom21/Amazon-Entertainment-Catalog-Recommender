#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include<algorithm>

using namespace std;

class RedBlackTree {
public:

  class Node{
  public:

    Node *left, *right, *parent;

    bool color; // True is red, false is Black
    string name;
    int rank;

    // A node will contain the its left, right, parent, color, name and rank.
    Node(string showName, int r){

      left=nullptr;
      right=nullptr;
      parent= nullptr;
      name = showName;
      color = true; // Insert it as red

      rank = r; // Rank is the number of matches it contains with other shows/ movies.
    }
  };

  //This class will allow for the construction of the red black tree maker.
  class RedBlackTreeMaker {
    public:

        Node *root;

        //Constructor for the red black tree maker, will initialize the root to nullptr.
        RedBlackTreeMaker(){
            root = nullptr;
        }

        //Breaks tree if not used.
        ~RedBlackTreeMaker() {
            deleteEach(root);
        }

        //This function will insert a node into the red black tree, and will run fixViolation to fix the tree if needed.
        void insert(string name, int rank) {

            Node *inserted = nullptr;
            root = insertHelper(root, nullptr, name, rank, inserted);
            fixViolation(inserted);

        }

        //This search function will search for the top K shows based on the number of matches it has with the user input.
        vector<string> searchTopK(int k) {
            vector<string> result;
            inOrderReverse(root, result, k);
            return result;
        }

    private:

        //This function will insert a node into the red black tree, and will return the root of the tree.
        Node* insertHelper(Node* root, Node* parent, string name, int rank, Node*& inserted) {

            if (root == nullptr) {
                inserted = new Node(name, rank);
                inserted->parent = parent;
                return inserted;
            }

            if (rank < root->rank) {
                root->left = insertHelper(root->left, root, name, rank, inserted);
            } 
            else {
                root->right = insertHelper(root->right, root, name, rank, inserted);
            }

            return root;
        }
        //This function will fix the violation of the red black tree, if it is needed.
        void fixViolation(Node*& node) {

          Node *parent = nullptr;
          Node *grandParent = nullptr;

          //This will run until we get to root or the color of the node is red or the parent is red.
          while ((node != root) && (node->color != false) && (node->parent->color == true)) {

              parent = node->parent;
              grandParent = node->parent->parent;

              //This will set uncle to be right or left of grandParent depending on the position of the parent.
              if (parent == grandParent->left) {

                  Node *uncle = grandParent->right;

                  //Will flip colors if uncle is red, and set node to grandParent.
                  if (uncle != nullptr && uncle->color == true) {
                      grandParent->color = true;
                      parent->color = false;
                      uncle->color = false;
                      node = grandParent;
                  //If uncle is black, we will rotate the tree to fix it.
                  } 
                  else {
                      //This is a left right case if below runs
                      if (node == parent->right) {
                          rotateLeft(root, parent);
                          node = parent;
                          parent = node->parent;
                      }
                      // Right means its a left left case if above did not run
                      rotateRight(root, grandParent);
                      swap(parent->color, grandParent->color);
                      node = parent;
                  }
              } 
              //This is the right case, if parent is right of grandParent.
              else {

                  Node *uncle = grandParent->left;

                  if ((uncle != nullptr) && (uncle->color == true)) {
                      grandParent->color = true;
                      parent->color = false;
                      uncle->color = false;
                      node = grandParent;
                  } 
                  
                  else {
                      //This is a right left case if below runs
                      if (node == parent->left) {
                          rotateRight(root, parent);
                          node = parent;
                          parent = node->parent;
                      }
                      //This is a right right case if above did not run
                      rotateLeft(root, grandParent);
                      swap(parent->color, grandParent->color);
                      node = parent;
                  }
              }
          }
          //This will set the root to black if it is red.
          root->color = false;
        }

        //This will rotate the tree to the left, and set the new root to nodeRight.
        void rotateLeft(Node* &root, Node* &node) {

            //nodeRight is what we are making the new root of the subtree.
            Node* nodeRight = node->right;
            node->right = nodeRight->left;

            if (node->right != nullptr)
                node->right->parent = node;

            //Have to update its parent to node's parent (node is old root of subtree)
            nodeRight->parent = node->parent;

            //If node is root, we have to set the new root to nodeRight.
            if (node->parent == nullptr)
                root = nodeRight;
            //If node is left of parent, we have to set the left of parent to nodeRight.
            else if (node == node->parent->left)
                node->parent->left = nodeRight;
            //If node is right of parent, we have to set the right of parent to nodeRight.
            else
                node->parent->right = nodeRight;

            //This will set the left of nodeRight to node, and set the parent of node to nodeRight, ending the rotation.
            nodeRight->left = node;
            node->parent = nodeRight;
        }

        //This will rotate the tree to the right, and set the new root to nodeLeft.
        void rotateRight(Node* &root, Node* &node) {
            //nodeLeft is what we are making the new root of the subtree.
            Node* nodeLeft = node->left;
            node->left = nodeLeft->right;

            if (node->left != nullptr)
                node->left->parent = node;
            //Have to update its parent to node's parent (node is old root of subtree)
            nodeLeft->parent = node->parent;

            //If node is root, we have to set the new root to nodeLeft.
            if (node->parent == nullptr)
                root = nodeLeft;
            //If node is left of parent, we have to set the left of parent to nodeLeft.
            else if (node == node->parent->left)
                node->parent->left = nodeLeft;
            //If node is right of parent, we have to set the right of parent to nodeLeft.
            else
                node->parent->right = nodeLeft;

            //This will set the right of nodeLeft to node, and set the parent of node to nodeLeft, ending the rotation.
            nodeLeft->right = node;
            node->parent = nodeLeft;
        }

        
        //Prints the revese inorder traversal of the tree, and will stop when it reaches k elements.
        //This will be used to get the top K shows based on the number of matches it has with the user input.
        void inOrderReverse(Node* node, vector<string>& result, int k) {

          if (node == nullptr || result.size() >= k) return;

          inOrderReverse(node->right, result, k);

          if (result.size() < k)
              result.push_back(node->name);
          
          inOrderReverse(node->left, result, k);
      }

      //This will recursively delete each node in the tree.
      void deleteEach(Node* node) {
          if (node == nullptr){
            return;
          } 

          deleteEach(node->left);
          deleteEach(node->right);

          delete node;
      }


  };

  // This is a public function that will allow for retrieving the top K recommended shows
  vector<string> getTopKRecom(int k) {
    return myRBT.searchTopK(k);
  }

  //Constructor for RedBlackTree, will take in the data and user input.
  RedBlackTree(vector<vector<vector<string>>> data, vector<string> user){

    makeRecomVector(data);
    getRecomRating(user,data);
    buildRedBlackTree();

  }

private:
  //This vector will hold the show's names and the number of mactehs it has with the user input.
  vector<pair<string ,int>> recom;

  //This tree will organize the shows based on the number of matches it has with the user input in a red black tree structure.
  RedBlackTreeMaker myRBT;

  //Construction for redBlackTree, will insert a bunch of nodes into myRBT
  void buildRedBlackTree() {

    for (int i=0; i<recom.size(); i++) {
      myRBT.insert(recom[i].first,recom[i].second);
    }
  }


  // This makes the recomVector, which gets show name and initializes all to 0.
  void makeRecomVector(vector<vector<vector<string>>> shows){
    for(int i=0; i<shows.size(); i++){
      recom.push_back({shows[i][0][0],0});
    }
  }

  //User is provided by user, shows is the whole data. Basically gets recom rating based on user input.

  void getRecomRating(vector<string> user, vector<vector<vector<string>>> shows){

    //For each user category
    for(int i=0; i<user.size(); i++){

      //For each vector in a specific category
      for(int j=0; j<shows.size(); j++){

        //For each vector with in that vector
        for(int k=1; k<shows[j].size(); k++){
          vector<string>& category = shows[j][k];
          string& keyword = user[i];

          //Will look if shows contains what the user speciifc, add 1 if it does
          auto it = find(category.begin(), category.end(), keyword);

          if(it!=category.end()){
            recom[j].second+=1;
          }

        }
      }
    }
    
  }

};