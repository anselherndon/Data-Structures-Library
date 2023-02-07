/*
	CS201-001 Fall 2021
	Professor: Brandon Dixon

	Data Structures Library 
	Current Rev: Phase 3 
				11/19/2021
	
	Author: Ansel Herndon
	CWID:	11879859
*/

#include <iostream>
#include <stdlib.h>
#include <math.h>
using namespace std;


template<class keytype>

class FibHeap{

private:

    struct node{
        keytype key;
        int degree;
        node *child;
		node *parent;
        node *next;
        node *prev;
        bool root;
    };


    int numRoots = 0;       //size of root list
    node *head = NULL;
    node *tail = NULL;
    node *min = NULL;

    node *newNode(keytype k){
		node *newnode = new node;
		newnode->key = k;
		newnode->degree = 0;
		newnode->child = NULL;
		newnode->parent = NULL;
        newnode->next = NULL;       
        newnode->prev = NULL;
        newnode->root = true;
		return newnode;
	}

    //Not being used
    void swap(node *x, node *y){
        node temp = *x;
        *x = *y;
        *y = temp;
    }

    //Union helper function
    node *mergeHeap(node *x, node *y){
		if(x->key < y->key){
            if(y->next != NULL && y->prev != NULL){
                (y->prev)->next = y->next;
                (y->next)->prev = y->prev;
            }
            if(y->next == NULL) (y->prev)->next = NULL;
            if(y->prev == NULL) (y->next)->prev = NULL;
            y->prev = NULL;
            y->root = false;

            y->parent = x;
			y->next = x->child;
			x->child = y;
			x->degree++;            
            numRoots--;
			return x;
		}
		else {
            if(x->next != NULL && x->prev != NULL){
                (x->prev)->next = x->next;
                (x->next)->prev = x->prev;
            }
            if(x->next == NULL) (x->prev)->next = NULL;
            if(x->prev == NULL) (x->next)->prev = NULL;
            x->prev = NULL;
            x->root = false;

			x->parent = y;
			x->next = y->child;
			y->child = x;
			y->degree++; 
            numRoots--;
			return y;
		}
	}

    void consolidate(node *curr){ 
        //float maxRank = (log((double)numRoots) / (log(2.0)));
        //int N = maxRank;
        node *list[50];
        for(int i=0;i<50;i++) list[i] = NULL;

        while(curr != NULL){
            int d = curr->degree;
            if(list[d] == NULL){
                list[d] = curr;
                //cout << curr->key << " now represents B" << d << endl;
                curr = curr->next;
            }
            else{
                //cout << "Linking " <<  curr->key << " and " << list[d]->key;
                //cout << " beacause " << list[d]->degree << " = " << d << endl;
                if(curr->degree == list[d]->degree){
                    curr = mergeHeap(curr, list[d]);
                    list[d] = NULL;
                        //seg fault checkpoint
                        //printHeap(head);
                        //cout << endl << "------------------" << endl;
                }
                else curr = curr->next;
            }
        }
        //cout << "numRoots = " << numRoots << endl;
        //Now fix order of trees
        node *A[numRoots];
        int x = 0;
        //cout << "Root list: ";
        for(int i=0;i<50;i++){
            if(list[i] != NULL){
                A[x] = list[i];
                //cout << A[x]->key << " ";  
                if(A[x]->key < min->key) min = A[x];
                x++;
            }
        }
        //cout << endl;
        for(int j = 1; j < numRoots; j++){
            A[j-1]->next = A[j];
            A[j]->prev = A[j-1];
        }
        head = A[0];
        head->prev = NULL;
        tail = A[numRoots-1];
        tail->next = NULL;
    }

	node *copyHeap(node *curr){
		if (curr == NULL) return NULL;
    
		node *temp = newNode(curr->key);
		temp->degree = curr->degree;
        temp->root = curr->root;
		temp->child = copyHeap(curr->child);
		temp->next = copyHeap(curr->next);

		return temp;
	}

    //No longer being used
    node *copyNode(node *temp){
        node *copy = new node;
        copy->key = temp->key;
        copy->degree = temp->degree;
        copy->root = temp->root;
        copy->child = copyHeap(temp->child);
        return copy;
    }

    void deleteHeap(node *curr){
		if (curr == NULL) return;

		deleteHeap(curr->child);
		deleteHeap(curr->next);
		free(curr);
        numRoots--;
		return;
	}

    void printHeap(node *curr){
		if (curr == NULL) return;

        if(curr->root) cout << "B" << curr->degree << endl;
		cout << curr->key << " ";
		printHeap(curr->child);
        if(curr->next != NULL){
            if(curr->next->root) cout << endl << endl;
		    printHeap(curr->next);
        }
        else return;
	}


public:

    //Default constructor
    FibHeap(){
        head = NULL;
    }

    //Constructor
    FibHeap(keytype k[], int s){
        head = NULL;
		for (int i = 0; i < s; i++) {
			insert(k[i]);
		}
        consolidate(head);
    }

    //Copy Constructor
    FibHeap(FibHeap &og){
        head = copyHeap(og.head);
        numRoots = og.numRoots;
        //Set min and tail
        node *x = head;
        min = head;
        for(int i = 0; i < numRoots; i++){
            if(x->key < min->key) min = x;
            if(x->next == NULL) tail = x;
            x = x->next;
        }
	}

    //Copy assignment operator
    FibHeap& operator=(FibHeap& og){
		if(this != &og){
            head = copyHeap(og.head);
            numRoots = og.numRoots;
            //Set min and tail
            node *x = head;
            min = head;
            for(int i = 0; i < numRoots; i++){
                if(x->key < min->key) min = x;
                if(x->next == NULL) tail = x;
                x = x->next;
            }
		}
		return *this;
	}

    //Destructor
    ~FibHeap(){
        deleteHeap(head);
    }

    //Return the min key of the heap
    keytype peekKey(){
        return min->key;
    }

    //Remove node with min key and return the key
    keytype extractMin(){
        //extract key from min to return
        keytype minKey = min->key;
        //remove min from root list
        if(min == head && min == tail){     //There is only one tree
            if(min->child == NULL){    
                head = NULL;
                tail = NULL;
                return minKey;
            } 
            (min->child)->root = true;
            head = min->child;
            tail = min->child;
            //numRoots--;       //do not need to change numRoots because you are trading 1 for 1
            //Meld its children to root list
            node *curr = (min->child)->next;
            while(curr != NULL){
                curr->root = true;
                numRoots++;
                tail->next = curr;
                curr->prev = tail;
                tail = curr;
                curr = curr->next;
            }
        }
        else{
            if(min == head) head = min->next;
            if(min == tail) tail = min->prev;
            if(min->next != NULL && min->prev != NULL){
                    (min->prev)->next = min->next;
                    (min->next)->prev = min->prev;
            }
            if(min->next == NULL) (min->prev)->next = NULL;
            if(min->prev == NULL) (min->next)->prev = NULL;
            numRoots--;
            //Meld its children to root list
            if(min->child != NULL){
                node *curr = min->child;
                while(curr != NULL){
                    curr->root = true;
                    numRoots++;
                    tail->next = curr;
                    curr->prev = tail;
                    tail = curr;
                    curr = curr->next;
                }
            }
        }
        min = head;

        //****************************************************************
        //Check that min has been successfully removed before consolidate
/*            cout << "Min = " << minKey << endl;
            cout << "Fib Heap with min removed (before consolidate): " << endl;
            printKey();
            cout << "numRoots = " << numRoots << endl;
*/
        //****************************************************************

        //consolidate (updates min)
        consolidate(head);
        //return extracted key
        return minKey;
    }

    //Insert a node with key k. Do not merge trees of same degree
    void insert(keytype k){
        node *nNode = newNode(k);
        //Root list is empty
        if(head == NULL){
            head = nNode;
            tail = nNode;
            min = nNode;
        }
        //Root list is not empty
        else{
            tail->next = nNode;
            nNode->prev = tail;
            tail = nNode;
        }       
        
        //Check for min
        if(nNode->key < min->key){
            min = nNode;
        }

        numRoots++;
    }

    //Merge H2 into the current heap. ****Consume H2?
    void merge(FibHeap<keytype> &H2){      //currently causing "Aborted (core dumped)"
        H2.head->prev = tail;               //resolved by setting head of H2 to null at end of operation
        tail->next = H2.head;               //so that destructor does not get called twice for current and H2
        tail = H2.tail;
        if(H2.min->key < min->key){
            min = H2.min;
        }
        numRoots += H2.numRoots;
        H2.head = NULL;
        //Do I need these or is H2.head = NULL sufficient?
        H2.tail = NULL;
        H2.min = NULL;
    }

    //Start at head, output size of each tree first then print keys using preorder traversal 
    void printKey(){
        printHeap(head);
        cout << endl << endl;       
        //cout << "-------------------------------------------" << endl;    
    }
    
//These functions are for debugging purposes only
/*
    void printHead(){
        cout << "Head: " << head->key << endl;
    }
    void printTail(){
        cout << "Tail: " << tail->key << endl;
    }
    void printMin(){
        cout << "Min: " << min->key << endl;
    }
*/
};