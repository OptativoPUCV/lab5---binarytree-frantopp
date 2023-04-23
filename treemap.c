#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}

TreeNode * createTreeNode(void* key, void * value) { 
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap* tree = (TreeMap *) malloc(sizeof(TreeMap));
  tree->root = NULL;
  tree->lower_than = lower_than;
  return tree;
}

Pair * searchTreeMap(TreeMap * tree, void* key) {
  TreeNode* auxiliar = tree->root;
  
  while (auxiliar != NULL){
     if (is_equal(tree, key, auxiliar->pair->key) == 1) {
      tree->current = auxiliar;
      return auxiliar->pair;
    }

    else if (tree->lower_than(key,auxiliar->pair->key) == 0) auxiliar = auxiliar->right;
    else if (tree->lower_than(key,auxiliar->pair->key) == 1) auxiliar = auxiliar->left;
  }
    return NULL;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {
  TreeNode* nuevoNode = createTreeNode(key, value);

  if (tree->root == NULL) {
    tree->root = nuevoNode;
    tree->current = nuevoNode;
    return;
  }

  TreeNode* current = tree->root;
  TreeNode* parent = NULL;

  while(current != NULL){
    if(is_equal(tree, key, current->pair->key) == 1) {
        return;
      }

    parent = current;

    if(tree->lower_than(key,current->pair->key) == 0) {
        current = current->right;
      } else current = current->left;
  }

  nuevoNode->parent = parent;
    if (tree->lower_than(key, parent->pair->key) == 0) {
      parent->right = nuevoNode;
    } else {
      parent->left = nuevoNode;
    }
    
  tree->current = nuevoNode;
}

TreeNode * minimum(TreeNode * x){
  if (x == NULL) return NULL;
  
  while(x->left != NULL) {
    x = x->left;
  }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  if(node == NULL) return;

  if(node->left == NULL && node->right == NULL){
    if(tree->lower_than(node->pair->key,node->parent->pair->key) == 0) node->parent->right = NULL;
    else node->parent->left = NULL;
    free(node);
    return;
   }

  else if(node->left == NULL || node->right == NULL){
    TreeNode* hijo = node->left ? node->left : node->right;
    if (node->parent->left == node) node->parent->left = hijo;
    else node->parent->right = hijo;
    hijo->parent = node->parent;
    free(node);
    return;
  }

  else{
    TreeNode *minimo = minimum(node->right);
    node->pair->key = minimo->pair->key;
    node->pair->value = minimo->pair->value;
    removeNode(tree, minimo);
  }
  return;

}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode* current = tree->root;
  current = minimum(current);
  return current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  if (tree->current == NULL) return NULL;
  if (tree->current->right != NULL) {
    TreeNode * current = tree->current->right;
    while (current->left != NULL) {
      current = current->left;
    }
    tree->current = current;
    return current->pair;
  }
  
  TreeNode *inicial = tree->current->parent;
  while (inicial != NULL && tree->current == inicial->right) {
    tree->current = inicial;
    inicial = inicial->parent;
  }

  tree->current = inicial;
    if (inicial == NULL) return NULL;
    return inicial->pair;
  
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
  if(tree->root == NULL) return NULL;
  tree->current = tree->root;
  TreeNode *ub_node = NULL;

    return NULL;
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}