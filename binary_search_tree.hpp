#ifndef BST_ASSIGNMENT_HPP
#define BST_ASSIGNMENT_HPP

#include <algorithm>

#include <iostream>

template < typename T >
  class BST {
    public:
      class Node {
        public:
          T key;
        int height = 0;
        Node * left = nullptr;
        Node * right = nullptr;
        Node * parent = nullptr;
        Node() {}
        Node(T k, Node * input_node = nullptr) {
          key = k;
          parent = input_node;
        }
      };

    private:
      Node * root_ = nullptr;
    unsigned int size_ = 0;

    public:
      BST();
    ~BST();
    void insert(T k);
    Node * successor(T k);
    void delete_min();
    void erase(T k);
    void rotate_right(Node * node);
    //*** End of methods for you to implement
    unsigned size();
    void print();
    Node * find(T k);
    std::vector < T > make_vec();
    std::vector < int > your_postorder_heights();
    std::vector < int > real_postorder_heights();
    T get_root_value();
    Node * min();

    private:
      int fix_height(Node * node);
    Node * erase(Node * node, T k); //Custom erase method 
    // The rest of these functions are already implemented
    void delete_subtree(Node * node);
    Node * min(Node * node);
    void print(Node * node);
    void make_vec(Node * node, std::vector < T > & vec);
    void your_postorder_heights(Node * node, std::vector < int > & vec);
    int real_postorder_heights(Node * node, std::vector < int > & vec);
  };

template < typename T > // Default constructor
  BST < T > ::BST() {}

template < typename T > // Destructor
  BST < T > ::~BST() {
    delete_subtree(root_);
  }

template < typename T > // Fix height
  int BST < T > ::fix_height(Node * node) {
    int h = 0, lh = 0, rh = 0;
    if (node != nullptr) {
      lh = fix_height(node -> left);
      rh = fix_height(node -> right);
      (lh >= rh) ? h = lh + 1: h = rh + 1;
      node -> height = h - 1;
    }
    return h;
  }

template < typename T >
  typename BST < T > ::Node * BST < T > ::erase(Node * node, T k) { //Erase 
    if (node == nullptr) {
      return nullptr; //If empty return nullptr
    }
    if (k < node -> key) { //key is smaller than start, go left
      node -> left = erase(node -> left, k);
    } else if (k > node -> key) { //key is greater than start, go left
      node -> right = erase(node -> right, k);
    } else { // key is root
      if (node -> left == nullptr && node -> right == nullptr) {
        if (node == root_) {
          root_ = nullptr;
        }
        return nullptr;
      } else if (node -> left == nullptr) { //If child is right...
        Node * temp = node -> right; //Replace node
        if (node == root_) {
          root_ = temp;
        }
        delete node;
        return temp;
      } else if (node -> right == nullptr) { //If child is left...
        Node * temp = node -> left; //Replace node 
        if (node == root_) {
          root_ = temp;
        }
        delete node;
        return temp;
      }
      Node * temp = successor(node -> key); //Find the successor
      node -> key = temp -> key; //Replace node value with successor value
      node -> right = erase(node -> right, node -> key); //Delete the successor from its original position and return new right child
    }
    return node;
  };

template < typename T >
  void BST < T > ::delete_subtree(Node * node) { // Delete sub-tree
    if (node == nullptr) {
      return;
    }
    delete_subtree(node -> left);
    delete_subtree(node -> right);
    delete node;
  }

template < typename T > // Insert function
  void BST < T > ::insert(T k) {
    Node * node = root_; // node will iterate down through the tree starting from the root
    Node * prev_node = node; // prev_node will hold node's parent
    bool went_right;
    if (node == nullptr) {
      root_ = new Node(k);
      ++size_;
      return;
    }
    while (node != nullptr) {
      prev_node = node;
      if (k < node -> key) {
        node = node -> left;
        went_right = false;
      } else if (k > node -> key) {
        node = node -> right;
        went_right = true;
      } else {
        return; // item already in set
      }
    }
    if (went_right){ // new node is either left or right child of prev_node
      prev_node -> right = new Node(k, prev_node);
    } else {
      prev_node -> left = new Node(k, prev_node);
    }
    ++size_;
    fix_height(root_);
  }

template < typename T > // Successor function
  typename BST < T > ::Node * BST < T > ::successor(T k) {
    Node * node = find(k);
    Node * successor = nullptr;
    if (node -> right != nullptr) { // Node has right child; successor is min of right tree
      successor = min(node->right);
    } else { // Find most recent node which turned left before parent
      Node * temp = root_;
      while (temp != node) {
        if (k < temp -> key) { // Go left
          successor = temp;
          temp = temp -> left;
        }
        if (k > temp -> key) {
          temp = temp -> right; // Go right
        }
      }
    }
    return successor;
  }

template < typename T > // Delete min function
  void BST < T > ::delete_min() {
    if (root_ == nullptr) {
      return; // if tree is empty just return.
    }
    Node * min_node = min();
    erase(min_node -> key);
  }

template < typename T >
  void BST < T > ::erase(T k) {
    erase(root_, k);
    --size_;
    fix_height(root_);
  };

template < typename T > // Rotate right function
  void BST < T > ::rotate_right(Node * node) {
    Node * move_up_node = node -> left; // Find node which moves up (left of node)
    node -> left = move_up_node -> right; // Left node becomes moved right
    move_up_node -> parent = node -> parent; // Node's parent becomes move parent 
    if (node!=root_) { // If node is not the root, rotate
      if (node -> parent -> key > node -> key) { // If node's parent > node then node is left of parent
        node -> parent -> left = move_up_node;
      } else {
        node -> parent -> right = move_up_node;
      }
    } else {
      root_ = move_up_node;
    }
    move_up_node -> right = node; // Move->R = node
    node -> parent = move_up_node; // Node's parent becomes move
    fix_height(root_);
  }

template < typename T >
  typename BST < T > ::Node * BST < T > ::min() {
    if (root_ == nullptr) {
      return root_;
    } else {
      return min(root_);
    }
  }

template < typename T >
  typename BST < T > ::Node * BST < T > ::min(Node * node) {
    while (node -> left != nullptr) {
      node = node -> left;
    }
    return node;
  }

template < typename T >
  typename BST < T > ::Node * BST < T > ::find(T k) {
    Node * node = root_;
    while (node != nullptr && node -> key != k) {
      node = k < node -> key ? node -> left : node -> right;
    }
    return node;
  }

template < typename T >
  unsigned BST < T > ::size() {
    return size_;
  }

template < typename T >
  void BST < T > ::print() {
    print(root_);
  }

template < typename T >
  void BST < T > ::print(Node * node) {
    if (node == nullptr) {
      return;
    }
    print(node -> left);
    std::cout << node -> key << " height " << node -> height << '\n';
    print(node -> right);
  }

template < typename T >
  typename std::vector < T > BST < T > ::make_vec() {
    std::vector < T > vec;
    vec.reserve(size_);
    make_vec(root_, vec);
    return vec;
  }

template < typename T >
  void BST < T > ::make_vec(Node * node, std::vector < T > & vec) {
    if (node == nullptr) {
      return;
    }
    make_vec(node -> left, vec);
    vec.push_back(node -> key);
    make_vec(node -> right, vec);
  }

template < typename T >
  void BST < T > ::your_postorder_heights(Node * node, std::vector < int > & vec) {
    if (node == nullptr) {
      return;
    }
    your_postorder_heights(node -> left, vec);
    your_postorder_heights(node -> right, vec);
    vec.push_back(node -> height);
  }

template < typename T >
  int BST < T > ::real_postorder_heights(Node * node, std::vector < int > & vec) {
    if (node == nullptr) {
      return -1;
    }
    int left_height = real_postorder_heights(node -> left, vec);
    int right_height = real_postorder_heights(node -> right, vec);
    int node_height = 1 + std::max(left_height, right_height);
    vec.push_back(node_height);
    return node_height;
  }

template < typename T >
  typename std::vector < int > BST < T > ::your_postorder_heights() {
    std::vector < int > vec;
    vec.reserve(size_);
    your_postorder_heights(root_, vec);
    return vec;
  }

template < typename T >
  typename std::vector < int > BST < T > ::real_postorder_heights() {
    std::vector < int > vec;
    vec.reserve(size_);
    real_postorder_heights(root_, vec);
    return vec;
  }

template < typename T >
  T BST < T > ::get_root_value() {
    if (root_ == nullptr) {
      // std::cout << "Calling get_root_value on empty tree\n";
      T dummy {};
      return dummy;
    } else {
      return root_ -> key;
    }
  }

#endif
