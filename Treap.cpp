/*
	CS201-001 Fall 2021

	Data Structures Library 
	Current Rev: Phase 2
				10/15/2021
	
	Author: Ansel Herndon
*/

/* 	
	Revisions:
		Successor and Predecessor:
			Added cases for when pred/succ is not in a subtree of the node in question
		Remove Function
			Error in rotateRight helper function. Resolved. 

*/

#include <iostream>
#include <stdlib.h>
#include <cstdlib>
using namespace std;

template<typename K>

class Treap {

private:
	
	struct treapNode {
		K key;
		float priority;
		int rank;
		int size;
		treapNode *left;
		treapNode *right;
	};

	treapNode *root = NULL;

	treapNode *newNode(K k, float p){
		treapNode *temp = new treapNode;
		temp->key = k;
		temp->priority = p;
		temp->left = NULL;
		temp->right = NULL;
		temp->size = 1;
		return temp;
	}

	treapNode *rotateRight(treapNode *y){
		treapNode *x = y->left;
		treapNode *temp = x->right;

		x->right = y;
		y->left = temp;

		//adjust size
		y->size = subsize(y->left) + subsize(y->right) + 1;
		x->size = subsize(x->left) + subsize(x->right) + 1;

		return x;
	}

	treapNode *rotateLeft(treapNode *x){
		treapNode *y = x->right;
		treapNode *temp = y->left;

		y->left = x;
		x->right = temp;

		//adjust size
		x->size = subsize(x->left) + subsize(x->right) + 1;
		y->size = subsize(y->left) + subsize(y->right) + 1;

		return y;
	}

	// Returns the size of the subtree that stems from any given node (Node included)
	int subsize(treapNode *x){
		if (x == NULL) return 0;
		else return x->size;
	}

	//Function for inserting a new node 
	treapNode *insertNode(treapNode *root, K k, float p){
		// if no root exists, create new node as root
		if(!root) return newNode(k, p);

		// if key < root
		if(k < root->key){
			root->left = insertNode(root->left, k, p);
			//fix properties
			if (root->left != NULL && root->left->priority < root->priority){
				root = rotateRight(root);
			}           
		}

		// if key > root
		else{
			root->right = insertNode(root->right, k, p);
			//fix properties
			if (root->right != NULL && root->right->priority < root->priority){
				root = rotateLeft(root);
			} 
		}

		//adjust size
		root->size = subsize(root->left) + subsize(root->right) + 1;

		return root;
	}

	//no longer being used
	void fixTreap(treapNode *curr, K k){
		treapNode *prev = NULL;

		// new node is in the left subtree
		if(k < curr->key){
				prev = curr;
				curr = curr->left;
				fixTreap(curr, k);
			}	

		// new node is in the right subtree
		else if(k > curr->key){
			prev = curr;
			curr = curr->right;
			fixTreap(curr, k);
		}

		// when you land on the new node
		else{
			// current node is a left child of previous node
			if(curr->key < prev->key){
				if(curr->priority < prev->priority) rotateRight(prev);
			}

			// current node is a right child of previous node
			if(curr->key > prev->key){
				if(curr->priority < prev->priority) rotateLeft(prev);
			}
		}

	}

	// helper function to determine the rank of a given node
	int detRank(treapNode *temp, K k){
		if(temp == NULL) return 0;

		else if(k > temp->key){
			return 1 + subsize(temp->left) + detRank(temp->right, k); 
		}
		else if(k < temp->key){
			return detRank(temp->left, k);
		}
		else if(k == temp->key){
			return temp->size - subsize(temp->right);
		}

		else return 0;
	}

	//Preorder traversal: root -> left -> right
	void printPre(treapNode *parent){
		if(parent == NULL) return;
		cout << parent->key << " ";
		printPre(parent->left);
		printPre(parent->right);
	}

	//Postorder Traversal: left -> right -> root
	void printPost(treapNode *parent){
		if(parent == NULL) return;
		printPost(parent->left);
		printPost(parent->right);
		cout << parent->key << " ";
	}

	//Inorder Traversal: left -> root -> right
	void printIn(treapNode *parent){
		if(parent == NULL) return;
		printIn(parent->left);
		cout << parent->key << " ";
		printIn(parent->right);
	}

	void clear(treapNode *x){
		if(x == NULL) return;
		clear(x->left);
		if(x->left != NULL) delete(x->left);
		clear(x->right);
		if(x->right != NULL) delete(x->right);
	}

	// Basically just newNode but also takes care of children
	treapNode *copyNode(treapNode *x){
		treapNode *newnode = newNode(x->key, x->priority);

		if(x->left != NULL){
			newnode->left = copyNode(x->left);
		}
		if(x->right != NULL){
			newnode->right = copyNode(x->right);
		}

		newnode->size = x->size;
		return newnode;
	}

	//Find predecessor node for delete function
	//Right-most child of the left subtree
	treapNode *predNode(treapNode *x){
		treapNode *curr = root;
		treapNode *prev = NULL;

		while(curr != NULL){
				if(x->key < curr->key){
					curr = curr->left;
				}
				else if(x->key > curr->key){
					prev = curr;			
					curr = curr->right;
				}
				else break;
			}
			//Node not found
			if(curr == NULL) return x;

			//If node has no left child
			if(curr->left == NULL){
				if(prev == NULL) return x; 
				else return prev;
			}
			//If node has a left subtree, find right-most child
			else{
				curr = curr->left;
				while(curr->right != NULL){
					curr = curr->right;
				}
				return curr;
			}
	}

	treapNode *delNode(treapNode *curr, K k){
		if(curr == NULL) return curr;

		// Find node with key k
		if (k < curr->key) curr->left = delNode(curr->left, k);
		else if (k > curr->key) curr->right = delNode(curr->right, k);
		//Node has been found
		else{
			//Node has no children
			if(curr->left == NULL && curr->right == NULL){
				delete(curr);
				curr = NULL;
				root->size -= 1;
			}
			// Left child of the node to be removed is NULL
			else if(curr->left == NULL){
				treapNode *temp = curr->right;
				delete(curr);
				curr = temp;
				root->size -= 1;
			}
			// Right child of node to be removed is NULL
			else if(curr->right == NULL){
				treapNode *temp = curr->left;
				delete(curr);
				curr = temp;
				root->size -= 1;
			}
			// Node to be removed has two children
			// Replace k and p with key and priority of the predecessor (Requirement)
			else{
			/*	Does not fix properties
				treapNode *pred = predNode(curr);
				curr->key = pred->key;
				curr->priority = pred->priority;
				curr->left = delNode(curr->left, curr->key);	
			*/
				if(curr->left->priority < curr->right->priority) {
					curr = rotateRight(curr);
					curr = delNode(curr, k);
				}
				else {
					curr = rotateLeft(curr);
					curr = delNode(curr, k);
				}
												
			}			
		}
		//curr->size = subsize(curr->left) + subsize(curr->right) + 1;					
		return curr;
	}
	

	//Recursive helper function for search
	float searchNode(treapNode *temp, K k){
		if(temp == NULL || temp->key == k) return temp->priority;
		
		else if(temp->key < k){
			return searchNode(temp->right, k);
		}

		else{
			return searchNode(temp->left, k);
		}
	}


public:

		Treap(){
			root = NULL;
		}

		//build tree O(s lg s)
		Treap(K k[], float p[], int s){	
			//initialize root
			root = NULL;
			//run insert function s times
			for(int i = 0; i < s; i++){
				root = insertNode(root, k[i], p[i]);
			}

			//root->size = s;
		}

		~Treap(){
			clear(root);
			delete root;
		}

		//copy constructor
		Treap(const Treap &og){
			root = copyNode(og.root);
		}

		// copy assign operator
		Treap& operator=(const Treap& og){
			root = copyNode(og.root);
			return *this;
		}

		float search(K k){
			return searchNode(root, k);
		}

		void insert(K k, float p){
			//insert new node based on key
			root = insertNode(root, k, p);
		}

		//insert node with random priority O(lg n)
		void insert(K k){	
			float p = rand() % 1;
			root = insertNode(root, k, p);
		}

		// delete node with key k 	O(lg n)
		// Return 1 if successful, 0 otherwise
		int remove(K k){
			int ogSize = subsize(root);

			root = delNode(root, k);	
			
			if(ogSize == root->size) return 0;
			return 1;
		}

		// Search for node first then use detRank function?
		int rank(K k){
			return detRank(root, k);
		}

		K select(int pos){
			//Out of bounds argument
				if(pos < 1 || pos > subsize(root)){
					return 0;
				}

			treapNode *temp = root;
			int curr; 
 
			while(temp != NULL){
				curr = subsize(temp->left) + 1;
				if(pos < curr) temp = temp->left;
				else if(pos > curr){
					temp = temp->right;
					pos -= curr;
				}
				else break;
			}

			return temp->key;
		}

		// If successor exists, return succ->key
		// else return k
		K successor(K k){
			treapNode *prev = NULL;
			treapNode *curr = root; 

			// find node with key k
			while(curr != NULL){
				if(k > curr->key){
					curr = curr->right;
				}
				else if(k < curr->key){
					prev = curr;			
					curr = curr->left;
				}
				else break;
			}
			//Node not found, return k
			if(curr == NULL) return k;

			//If node has no right child
			if(curr->right == NULL){
				if(prev==NULL) return k;
				else return prev->key;
			}
			//If node has a right subtree, find left-most child
			else{
				curr = curr->right;
				while(curr->left != NULL){
					curr = curr->left;
				}
				return curr->key;
			}
		}

		// If predecessor exists, return predecessor->key
		// else return k
		K predecessor(K k){	
			treapNode *curr = root;
			treapNode *prev = NULL;

			//Find node with key k
			while(curr != NULL){
				if(k < curr->key){
					curr = curr->left;
				}
				else if(k > curr->key){
					prev = curr;			
					curr = curr->right;
				}
				else break;
			}
			//Node not found, return k
			if(curr == NULL) return k;

			//If node has no left child
			if(curr->left == NULL){
				if(prev == NULL) return k; 
				else return prev->key;
			}
			//If node has a left subtree, find right-most child
			else{
				curr = curr->left;
				while(curr->right != NULL){
					curr = curr->right;
				}
				return curr->key;
			}
		}

		int size(){
			return root->size;
		}

		void preorder(){
			printPre(root);
			cout << endl;
		}

		void inorder(){
			printIn(root);
			cout << endl;
		}

		void postorder(){
			printPost(root);
			cout << endl;
		}


};
