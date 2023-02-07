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
using namespace std;

#ifndef Include_CDA
#define Include_CDA
#include "CDA.cpp"
#endif

template<class keytype>

class Heap{

private:

    struct node{
        keytype key;
        int index;      //unused
    };

    CDA<node> minHeap;

    void swap(node &x, node &y){
        node temp = x;
        x = y;
        y = temp;
    }

    void heapify(node arr[], int s, int curr){
        int root = curr;
        int left = 2 * curr + 1;
        int right = 2 * curr + 2;

        if (left < s && arr[left].key < arr[root].key){
            root = left;
        }
		if (right < s && arr[right].key < arr[root].key){
            root = right;
        }
		if (root != curr) {
			swap(arr[curr], arr[root]);
			heapify(arr, s, root);
		}
    }

    void bubbleUp(int i){
        int pNode = (i - 1) / 2;
        if(minHeap[pNode].key > minHeap[i].key){
            swap(minHeap[pNode], minHeap[i]);
            bubbleUp(pNode);
        }
    }

    void bubbleDown(int pNode){
        int s = minHeap.Length();
        int left = pNode * 2 + 1;
        int right = left + 1;
        int min = left;

        if(left >= s) return;   //out of bounds check

        if(right < s && minHeap[left].key > minHeap[right].key){
            min = right;
        }
        if(minHeap[pNode].key > minHeap[min].key){
            swap(minHeap[pNode], minHeap[min]);
            bubbleDown(min);
        }
    }

public:
    //Default Constructor
    Heap(){
       minHeap = CDA<node> ();
    }

    //Constructor
    Heap(keytype k[], int s){
        node *arr = new node[s];
        for(int i = 0; i < s; i++){
            node *temp = new node;
            temp->key = k[i];
            temp->index = i;
            arr[i] = *temp;
        }
        for(int j = (s/2) - 1; j >= 0; j--){
            heapify(arr, s, j);
        }
        minHeap = CDA<node> ();
        for(int k = 0; k < s; k++) {
            minHeap.AddEnd(arr[k]);
        }
    }

    //This might be unnecessary since CDA is included  
    //Copy constructor
/*    Heap(const Heap &og){
        int size = og.Length();
        node *newArr = new node[size];
        for(int i = 0; i < size; i++){
            node *temp = new node;
            temp->key = og[i].key;
            temp->index = i;
            newArr[i] = *temp;
        }
        for(int j = (size/2) - 1; j >= 0; j--){
            heapify(newArr, size, j);
        }

        minHeap = CDA<node> ();
        for(int k = 0; k < size; k++) minHeap.AddEnd(newArr[k]);
    }

    //Copy assignment operator
    Heap& operator=(const Heap &og){
        if(this != &og){
            int size = og.Length();
            node *newArr = new node[size];
            for(int i = 0; i < size; i++){
                node *temp = new node;
                temp->key = og[i].key;
                temp->index = i;
                newArr[i] = *temp;
            }
            for(int j = (size/2) - 1; j >= 0; j--){
                heapify(newArr, size, j);
            }

            minHeap = CDA<node> ();
            for(int k = 0; k < size; k++) minHeap.AddEnd(newArr[k]);
        }
        return *this;
    }

    Heap& operator[](int i){
		if(i > minHeap.Length() || i < 0){
			cout << "Index: " << i << " is out of bounds" << endl;
			return -1;
		}
		else{
			return minHeap[i];
		}
	}
*/
    //Destructor
    ~Heap(){
        //delete[] minHeap;
    }

    //return min key (aka root)
    keytype peekKey(){ 
        return minHeap[0].key;
    }

    //Remove node with minimum key from the heap and return the key
    keytype extractMin(){
        keytype k = minHeap[0].key;     //min key = root
        int s = minHeap.Length() - 1;
        swap(minHeap[0], minHeap[s]);
        minHeap.DelEnd();
        bubbleDown(0);
        return k;
    }

    //Insert node with key k and fix properties
    void insert(keytype k){
        //Create new node
        node *temp = new node;
        temp->key = k;
        temp->index = minHeap.Length();
        //Insert new node
        minHeap.AddEnd(*temp);
        bubbleUp(minHeap.Length() - 1);
    }

    void printKey(){
        for (int i = 0; i < minHeap.Length(); i++) cout << minHeap[i].key << " ";
		cout << endl;
    }

};