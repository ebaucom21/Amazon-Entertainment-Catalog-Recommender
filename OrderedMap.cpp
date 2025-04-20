#include<iostream>
#include<string>
#include<vector>
#include<utility>
#include<algorithm>



using namespace std;

class ordered_map {
public:

  class Node{
  public:

    Node *left, *right, *parent, *grandparent, *uncle;


    bool color; // True is red, false is Black
    string name;
    int rank;

    // A node will contain the its left, right, parent, uncle and grandparent.
    // Will also carry the name of show, color and its respective rank.
    Node(string showName, int r){

      left=nullptr;
      right=nullptr;
      parent= nullptr;
      grandparent= nullptr;
      uncle=nullptr;
      name = showName;
      color = true; // Insert it as red

      rank = r;
    }
  };

  class RedBlackTree{

    // Red black tree has a root we can check and will build from root
  public:
    Node *root;
    RedBlackTree(){
      root=nullptr;
    }

    //This insert will add a specific node to the tree based on recom value
    void insert(string name, int r){

      this->root = helperInsert( this->root ,name ,r);

    }

    // This will allow the RedBlackTree to get top k recoms, and sends it to top k for orderedmap
    vector<string>searchTopK(int k) {
      vector<string> result;
      int count =0;
      helperPostOrder(this->root, k, result, count);

      return result;
    }



  private:

    //This helperPostOrder will check top k and stop checking rest
    void helperPostOrder(Node* helpRoot, int k, vector<string>& result, int& count) {

      if (helpRoot==nullptr || count>=k) {
        return;
      }

      helperPostOrder(helpRoot->right,k, result, count);

      if (result.size()<k) {
        result.push_back(helpRoot->name);
        count++;
      }

      helperPostOrder(helpRoot->left,k, result, count);


    }


    // Allows for insert to check if it keeps the proporties of a red black tree
    Node* helperInsert(Node* helpRoot, string name, int r) {

        if (helpRoot==nullptr){
            return new Node(name, r);
        }
        
        //Updates grandparents, uncles and parents when inserting to the left
        else if(r <helpRoot->rank){

            
            if (helpRoot->parent!=nullptr){

            helpRoot->grandparent = helpRoot->parent;
            if(helpRoot->grandparent->right == helpRoot->parent){
                helpRoot->uncle= helpRoot->grandparent->left;
            }
            else{
                helpRoot->uncle= helpRoot->grandparent->right;
            }

            }

                helpRoot->parent = helpRoot;
                helpRoot->left = helperInsert(helpRoot->left, name, r);
        }
        
        //Updates grandparents, uncles and parents when inserting to the right
        else if(r>helpRoot->rank) {
          if (helpRoot->parent!=nullptr){

            helpRoot->grandparent = helpRoot->parent;
            if(helpRoot->grandparent->right == helpRoot->parent){
              helpRoot->uncle= helpRoot->grandparent->left;
            }
            else{
              helpRoot->uncle= helpRoot->grandparent->right;
            }
          }

          helpRoot->parent = helpRoot;
          helpRoot->right = helperInsert(helpRoot->right, name, r);
          
        }
        
        //This should not run but just in case
        else{
            cout << "This should not run" << endl;
            if (helpRoot->parent!=nullptr){

                helpRoot->grandparent = helpRoot->parent;
                if(helpRoot->grandparent->right == helpRoot->parent){
                    helpRoot->uncle= helpRoot->grandparent->left;
                }
                else{
                    helpRoot->uncle= helpRoot->grandparent->right;
                }

            }

            
        }

        //If parent's color is red something must be done
        if (helpRoot->parent->color == true){
            if(helpRoot->uncle == nullptr){

                if(helpRoot->grandparent->right == helpRoot->parent){
                    if(helpRoot->parent->right == helpRoot){
                        // Right right case
                        return leftRotate(helpRoot->grandparent);
                    }
                    else{
                        // Right left case
                        helpRoot->left =leftRotate(helpRoot->grandparent->left);
                        return rightRotate(helpRoot->grandparent);
                    }
    
                }

                else{
                    if(helpRoot->parent->left == helpRoot){
                        // Left left case
                        return rightRotate(helpRoot->grandparent);
                    }
    
    
                    else{
                        // Left right case
                        helpRoot->right =rightRotate(helpRoot->grandparent->right);
                        return leftRotate(helpRoot->grandparent);
                    }
                }
    
            }

            // If uncle is red we need to simply flip colors
            else if (helpRoot->uncle->color == true){
                return flipColors(helpRoot);
            }

            //If uncle is black or nullptr, will need to check if black
            else {
                // If this runs this means that uncle is black
                if(helpRoot->grandparent->right == helpRoot->parent){
                    if(helpRoot->parent->right == helpRoot){
                        // Right right case
                        return leftRotate(helpRoot->grandparent);
                    }
                    else{
                        // Right left case
                        helpRoot->left =leftRotate(helpRoot->grandparent->left);
                        return rightRotate(helpRoot->grandparent);
                    }
  
                }
                else{
                    if(helpRoot->parent->left == helpRoot){
                    // Left left case
                    return rightRotate(helpRoot->grandparent);
                    }
  
  
                    else{
                        // Left right case
                        helpRoot->right =rightRotate(helpRoot->grandparent->right);
                        return leftRotate(helpRoot->grandparent);
                    }
                }
            }

          
        }

      
    }

    // This flip colors so that gp is red, uncle black and parent black
    Node* flipColors(Node* helpRoot){

      helpRoot->grandparent->color = true;

      if(helpRoot->grandparent== root){
        helpRoot->grandparent->color= false;
      }

      helpRoot->parent->color= false;
      helpRoot->uncle->color= false;

    }

    Node* rightRotate(Node* helpRoot){
      // Get value left and the value that could be to that right
      Node* lowest = helpRoot->left;
      Node* grandChild = lowest->right;

      // Preform the rotation
      lowest->right = helpRoot;
      helpRoot->left = grandChild;

      //Updating the parent of each node
      lowest->parent = helpRoot->parent;
      helpRoot->parent = lowest;

      if (grandChild != nullptr) {
        grandChild->parent = helpRoot;
      }

      // This will update the grandparent and uncle pointed for each node
      if (lowest->parent != nullptr) {
        lowest->grandparent = lowest->parent->parent;

        if (lowest->grandparent != nullptr) {
          if (lowest->grandparent->left == lowest->parent) {
            lowest->uncle = lowest->grandparent->right;
          } else {
            lowest->uncle = lowest->grandparent->left;
          }
        }
      } else {
        lowest->grandparent = nullptr;
        lowest->uncle = nullptr;
      }

      helpRoot->grandparent = lowest->parent;

      //Updates color for all involved values
      lowest->color = false;
      helpRoot->color = true;
      if (lowest->left!= nullptr) {
        lowest->left->color = true;
      }

      // Returns the value on top now
      return lowest;

    }

    Node* leftRotate(Node* helpRoot){
      //Get the value right and gets a value that could be left of that
      Node* highest= helpRoot->right;
      Node* grandChild = highest->left;

      //Preforms the rotation
      highest->left = helpRoot;
      helpRoot->right = grandChild;

      //Updates parent's values
      highest->parent = helpRoot->parent;
      helpRoot->parent = highest;

      if (grandChild != nullptr) {
        grandChild->parent = helpRoot;
      }

      //Updates the grandparent's values
      if (highest->parent != nullptr) {
        highest->grandparent = highest->parent->parent;

        if (highest->grandparent != nullptr) {
          if (highest->grandparent->left == highest->parent) {
            highest->uncle = highest->grandparent->right;
          } else {
            highest->uncle = highest->grandparent->left;
          }
        }
      } else {
        highest->grandparent = nullptr;
        highest->uncle = nullptr;
      }

      helpRoot->grandparent = highest->parent;

      //Updates the colors of each node
      highest->color = false;
      helpRoot->color = true;
      if (highest->right!= nullptr) {
        highest->right->color = true;
      }

      //Returns new top
      return highest;
    }


  };

  // This is a public function that will allow for retrieving the top K recommended shows
  vector<string> getTopKRecom(int k) {
    return myRBT.searchTopK(k);
  }

  //Constructor for ordered map needs the data and user recom in order to be made.
  ordered_map(vector<vector<vector<string>>> data, vector<string> user){

    makeRecomVector(data);
    getRecomRating(user,data);
    buildRedBlackTree();

  }

private:

  vector<pair<string ,int>> recom;
  RedBlackTree myRBT;

  //Construction for redBlackTree, will insert a bunch of nodes into myRBT
  Node* buildRedBlackTree() {

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
        for(int k=0; k<shows[j].size(); k++){
          vector<string>& category = shows[j][k];
          string& keyword = user[i];

          //Will look if shows contains what the user speciifc, add 1 if it does
          auto it = find(category.begin(), category.end(), keyword[i]);
          if(it!=category.end()){
            int index= distance(category.begin(), it);
            recom[index] = {recom[index].first, recom[index].second+1};
          }

        }
      }
    }
  }

};