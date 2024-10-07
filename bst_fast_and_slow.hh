//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// UD CISC220 Lab #5 
// two ways to implement a binary search tree...
// September, 2023 -- starter code (Prof. Christopher Rasmussen)

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

#ifndef BST_HH
#define BST_HH

#include <cstdlib>
#include <iostream>
#include <vector>
#include "bstnode.hh"

using namespace std;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// BSTREE_FAST -- THIS IS THE ONE YOU NEED TO COMPLETE
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// a binary search tree, implemented as a linked structure

template<typename T>
class BSTree_Fast
{
public:
  
  BSTree_Fast() { root = NULL; numNodes = 0; }   // make an empty BST

  void insert(T &);                              // put a key into the BST 
  void remove(T &);                              // delete, if it exists, a key from the BST 
  int frequency(T &);                            // how many of given key does BST contain?
  const T & findMin();                           // what is the smallest key in the BST?
  const T & findMax();                           // what is the biggest key in the BST?
  int size()  { return numNodes; }               // number of nodes in the tree
  int computeHeight();                           // longest path from root to leaf
  
private:
  BSTNode<T> *root;                              // pointer to the node at the root of the tree
  int numNodes;                                  // this must be updated with each insert and remove
                                                 // a node whose "number" variable is > 1 is not counted multiple times
  
  BSTNode<T> *insert(BSTNode<T> *, T &);         // helper for recursive insert
  BSTNode<T> *remove(BSTNode<T> *, T &);         // helper for recursive remove
  BSTNode<T> *findMinNode(BSTNode<T> *);         // helper for findMin
  BSTNode<T> *findMaxNode(BSTNode<T> *);         // helper for findMax
  int computeHeight(BSTNode<T> *);               // helper for recursive height computation
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// Helper function to find minimum node in a subtree
template<typename T>
BSTNode<T> * BSTree_Fast<T>::findMinNode(BSTNode<T> *node)
{
  if (node == nullptr)
  {
    return nullptr;
  }
  else if (node->getLeft() == nullptr)
  {
    return node;
  }
  return findMinNode(node->getLeft());
}

//----------------------------------------------------------------------------

// Helper function to find maximum node in a subtree
template<typename T>
BSTNode<T> * BSTree_Fast<T>::findMaxNode(BSTNode<T> *node)
{
  if (node == nullptr)
  {
    return nullptr;
  }
  else if (node->getRight() == nullptr)
  {
    return node;
  }
  return findMaxNode(node->getRight());
}

//----------------------------------------------------------------------------

// Compute height of the BST
template<typename T>
int BSTree_Fast<T>::computeHeight()
{
  return computeHeight(root);
}

// Recursive helper function to compute the height
template<typename T>
int BSTree_Fast<T>::computeHeight(BSTNode<T> *node)
{
  if (node == nullptr)
    return -1;
  int leftHeight = computeHeight(node->getLeft());
  int rightHeight = computeHeight(node->getRight());
  return 1 + max(leftHeight, rightHeight);
}

//----------------------------------------------------------------------------

// Insert a key into the BST
template<typename T>
void BSTree_Fast<T>::insert(T & key)
{
  root = insert(root, key);
}

// Recursive helper function for insert
template<typename T>
BSTNode<T> * BSTree_Fast<T>::insert(BSTNode<T> *node, T & key)
{
  if (node == nullptr)
  {
    numNodes++;
    return new BSTNode<T>(key);
  }

  if (key < node->getKey())
    node->setLeft(insert(node->getLeft(), key));
  else if (key > node->getKey())
    node->setRight(insert(node->getRight(), key));
  else
    node->setNumber(node->getNumber() + 1);  // Increment count if the key already exists

  return node;
}

//----------------------------------------------------------------------------

// Remove a key from the BST
template<typename T>
void BSTree_Fast<T>::remove(T & key)
{
  root = remove(root, key);
}

// Recursive helper function for remove
template<typename T>
BSTNode<T> * BSTree_Fast<T>::remove(BSTNode<T> *node, T & key)
{
  if (node == nullptr)
    return nullptr;

  if (key < node->getKey())
  {
    node->setLeft(remove(node->getLeft(), key));
  }
  else if (key > node->getKey())
  {
    node->setRight(remove(node->getRight(), key));
  }
  else
  {
    // Node with two children
    if (node->getLeft() && node->getRight())
    {
      BSTNode<T> *temp = findMinNode(node->getRight());
      node->setKey(temp->getKey());
      node->setNumber(temp->getNumber());
      node->setRight(remove(node->getRight(), temp->getKey()));
    }
    // Node with one or zero children
    else
    {
      BSTNode<T> *temp = node;
      node = (node->getLeft() != nullptr) ? node->getLeft() : node->getRight();
      delete temp;
      numNodes--;
    }
  }
  return node;
}

//----------------------------------------------------------------------------

// Find the frequency of a key in the BST
template<typename T>
int BSTree_Fast<T>::frequency(T & key)
{
  BSTNode<T> *current = root;
  while (current != nullptr)
  {
    if (key == current->getKey())
      return current->getNumber();
    else if (key < current->getKey())
      current = current->getLeft();
    else
      current = current->getRight();
  }
  return 0;  // Key not found
}

//----------------------------------------------------------------------------

// Return the smallest key in the BST
template<typename T>
const T & BSTree_Fast<T>::findMin()
{
  if (root == nullptr)
  {
    cout << "ERROR\n";
    exit(1);
  }
  return findMinNode(root)->getKey();
}

//----------------------------------------------------------------------------

// Return the biggest key in the BST
template<typename T>
const T & BSTree_Fast<T>::findMax()
{
  if (root == nullptr)
  {
    cout << "ERROR\n";
    exit(1);
  }
  return findMaxNode(root)->getKey();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// BSTREE_SLOW -- THIS ONE IS ALREADY WRITTEN
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// a binary search tree, implemented as an unordered STL vector

template<typename T>
class BSTree_Slow
{
public:
  
  BSTree_Slow() { treeVec.clear(); }            // make an empty BST

  void insert(T &);                             // put a key into the BST 
  void remove(T &);                             // delete, if it exists, a key from the BST
  int frequency(T &);                           // how many of given key does BST contain?
  const T & findMin();                          // what is the smallest key in the BST?
  const T & findMax();                          // what is the biggest key in the BST?
  int size() { return treeVec.size(); }         // number of nodes in the tree
  int computeHeight();                          // longest path from root to leaf
  void print();                                 // print every node in vector order -- this is here for debugging purposes
  
private:

  vector < BSTNode<T> * > treeVec;              // vector of pointers to nodes in the "tree"

};

//----------------------------------------------------------------------------

// height means nothing in this "flat" implementation, so return error code

template<typename T>
void BSTree_Slow<T>::print()
{
  for (int i = 0; i < treeVec.size(); i++)
    cout << i << " " << treeVec[i]->getKey() << " " << treeVec[i]->getNumber() << endl;
}

//----------------------------------------------------------------------------

// height means nothing in this "flat" implementation, so return error code

template<typename T>
int BSTree_Slow<T>::computeHeight()
{
  return -1;
}

//----------------------------------------------------------------------------

// put key into the tree

template<typename T>
void BSTree_Slow<T>::insert(T & key)
{
  // if it's already in the "tree", just increment the count at that node
  
  for (int i = 0; i < treeVec.size(); i++)
    if (treeVec[i]->getKey() == key) {
      treeVec[i]->setNumber(treeVec[i]->getNumber() + 1);
      return;
    }

  // else, put a new node at the end of the vector
  
  treeVec.push_back(new BSTNode<T>(key));
}

//----------------------------------------------------------------------------

// if key is in the tree, get rid of its node

template<typename T>
void BSTree_Slow<T>::remove(T & key)
{
  for (int i = 0; i < treeVec.size(); i++)
    if (treeVec[i]->getKey() == key) {
      treeVec.erase(treeVec.begin() + i);
      return;
    }
}

//----------------------------------------------------------------------------

// how many of this key are in the tree?

template<typename T>
int BSTree_Slow<T>::frequency(T & key)
{
  for (int i = 0; i < treeVec.size(); i++)
    if (treeVec[i]->getKey() == key)
      return treeVec[i]->getNumber();

  return 0;  // it's not in the tree
}

//----------------------------------------------------------------------------

// find the minimum key in the tree

template<typename T>
const T & BSTree_Slow<T>::findMin()
{
  if (treeVec.size() == 0) {
    cout << "ERROR\n";
    exit(1);
  }
  
  int minIndex = 0;
  for (int i = 1; i < treeVec.size(); i++)
    if (treeVec[i]->getKey() < treeVec[minIndex]->getKey())
      minIndex = i;

  return treeVec[minIndex]->getKey();
}

//----------------------------------------------------------------------------

// find the maximum key in the tree

template<typename T>
const T & BSTree_Slow<T>::findMax()
{
  if (treeVec.size() == 0) {
    cout << "ERROR\n";
    exit(1);
  }
  
  int maxIndex = 0;
  for (int i = 1; i < treeVec.size(); i++)
    if (treeVec[i]->getKey() > treeVec[maxIndex]->getKey())
      maxIndex = i;

  return treeVec[maxIndex]->getKey();
}

//----------------------------------------------------------------------------

#endif
