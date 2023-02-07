/*
	CS201-001 Fall 2021
	Professor: Brandon Dixon

	Data Structures Library 
	Current Rev: Phase 3 
				(This version of CDA is being used as a support file for phase 3)
	
	Author: Ansel Herndon
	CWID:	11879859
*/
/* 
	Persisting Issues:
		Copy constructor does not appear to work
		Search and Select is inconsistent
			This may be an issue with the shadow array,
			indices are likely inaccurate
		Doesn't seem to work with non-integer datatypes
		Quicksort is struggling to organize head and tail values

	Revisions:
		Fixed partition function to use real indices not relative
		Sorted using mergeSort instead of Quicksort
		Arguments in recursive call for quick select were wrong

*/

#include <iostream>
#include <stdlib.h>
using namespace std;

template<class T>

class CDA {

private:
	int size;
	int cap; //capacity
	int head;
	int tail;
	T *arr;
	T garbage;
	
	//function to increase the size of the internal array
	void sizeInc(){
		int newCapacity = (this->cap) * 2;
		T *newArray = new T[newCapacity];
		for(int i = 0; i < this->size; i++){
			newArray[i] = this->arr[indices(i)];
		}
		delete[] this->arr;

		this->cap = newCapacity;
		this->arr = newArray;
		this->head = 0;
	}

	//function to reduce the size of the internal array
	void sizeDec(){
		int newCapacity = this->cap/2;
		T *newArray = new T[newCapacity];

		for (int i = 0; i < this->size; i++) {
			newArray[i] = this->arr[indices(i)];
		}

		delete[] this->arr;

		this->cap = newCapacity;
		this->arr = newArray;
		this->head = 0;
	}

	//Return the real index based on index relative to head
	int indices(int i){

		if (i < 0 || i > this->size-1) {
			return -1;
		}

		else return (this->head + i) % this->cap;
	}

	void swap(T *x, T *y){
		T temp = *x;
		*x = *y;
		*y = temp;
	}

	//Partition function for quick sort and quick select
	int partition(int left, int right){
		if(right <= left) return left;
		int index = left;
		//int pivotIndex = left + rand() % (right - left + 1);
		T pivotKey = this->arr[indices(right)];

		for(int i = left; i < right; i++){
			if(this->arr[indices(i)] <= pivotKey){
				swap(&this->arr[indices(index)], &this->arr[indices(i)]);
				index++;
			}
		}
		swap(&this->arr[indices(index)], &this->arr[indices(right)]);
		return index;
	}

	T qSelect(int k, int l, int r){
		if(l == r) return this->arr[indices(l)];
		if(k < 0 || k > this->size-1) return garbage;

		int pivotIndex = partition(l, r);

		if(pivotIndex - l == k - 1) return this->arr[indices(pivotIndex)];
		else if(pivotIndex - l > k - 1) return qSelect(k, l, pivotIndex - 1);
		else return qSelect(k - pivotIndex + l - 1, pivotIndex + 1, r); 		
	}

	//No longer using
	void quickSort(int l, int r){		
		if(l < r) {
			int pivotIndex = partition(l, r);
			quickSort(l, pivotIndex - 1);
			quickSort(pivotIndex + 1, r);
		}
	}

	//Merge Sort Functions

	void merge(int l, int pivot, int r) {
		T* temp = new T[r - l + 1];
		int sLeft = l; //starting min

		int rPivot = pivot; //right side pivot
		int lPivot = pivot - 1; //left side pivot
		int i;
		for (i = 0; l <= lPivot && r >= rPivot; i++) {
			if (this->arr[indices(rPivot)] < this->arr[indices(l)]) {
				temp[i] = this->arr[indices(rPivot)];
				rPivot++;
			}
			else {
				temp[i] = this->arr[indices(l)];
				l++;
			}
		}

		for (i = 0; l <= lPivot; i++, l++) temp[i] = this->arr[indices(l)];
		for (i = 0; r >= rPivot; i++, rPivot++) temp[i] = this->arr[indices(rPivot)];

		for (int j = 0; j < i; j++) {
			this->arr[indices(j + sLeft)] = temp[j];
		}
        delete [] temp;
	}

	void mergeSort(int l, int r) {
		if (l >= r) return;
		int pivot = (r + l + 1) / 2;
		mergeSort(l, pivot-1);
		mergeSort(pivot, r);
		merge(l, pivot, r);
	}	

public:

	//Default
	CDA(){
		this->size = 0;
		this->cap = 1;
		this->head = 0;
		this->arr = new T[cap];
		//this->garbage = 0;
	}

	//Parameterized
	CDA(int s){
		this->size = s;
		this->cap = s;
		this->head = 0;
		this->tail = s - 1;
		this->arr = new T[cap];
		this->garbage = 0;
	}

	CDA(int s, T init){
		this->size = s;
		this->cap = s;
		this->head = 0;
		this->tail = s - 1;
		this->arr = new T[cap];
		this->garbage = 0;
		for(int i=0;i<size;i++) this->arr[i] = init;
	}

	//Copy constructor
	CDA(const CDA &og){
		size = og.size;
		cap = og.cap;
		head = og.head;
		tail = size - 1;
		T *newArr = new T[cap];

		for(int i = 0; i < size; i++){
			newArr[i] = og.arr[indices(i)];
		}
		head = 0;
		this->arr = newArr;
	}

	// copy assign operator
	CDA& operator=(const CDA& og) {
		if(this != &og){
			size = og.size;
			cap = og.cap;
			head = og.head;
			T *newArr = new T[cap];

			for (int i = 0; i < size; i++) {
				newArr[i] = og.arr[indices(i)];
			}
			head = 0;
			arr = newArr;
		}
		return *this;
	}


	//Destructor
	~CDA(){
		delete[] this->arr;
	}


	T& operator[](int i){
		if(i > size || i < 0){
			cout << "Index: " << i << " is out of bounds" << endl;
			return garbage;
		}
		else{
			int index = (this->head + i) % this->cap;
			return this->arr[index];
		}
	}

	void AddEnd(T v){		//amortized O(1)
		/* pseudo
		if size == cap, cap *= 2
		else 
			size += 1
			shift left 1
			tail->value = new value
		*/

		if(this->size == this->cap){
			this->sizeInc();
			this->AddEnd(v);
		}
		else{
			this->arr[(this->head + this->size) % this->cap] = v;
			this->size += 1;
		}

	}

	void AddFront(T v){	//amortized O(1)
		/* pseudo
		if size == cap, cap *= 2
		else 
			size += 1
			shift left 1
			head->value = new value
		*/

		if(this->size == this->cap){
			this->sizeInc();
		}

		this->head = (this->head + this->cap - 1) % this->cap;
		this->arr[this->head] = v;
		this->size += 1;

	}

	void DelEnd(){		//amortized O(1)
		/*pseudo: 
		delete tail->value
		size -= 1
		if cap == 4, break
		if cap / 4 >= size,
			cap = cap / 2;
		*/

			this->tail = (this->tail - 1) % this->cap;
			this->size -= 1;

			if (((double) this->size)/((double) this->cap) <= .25) {
				this->sizeDec();
			}

	}

	void DelFront(){	//amortized O(1)
		/*pseudo: 
		delete head->value
		size -= 1
		if cap == 4, break
		if cap / 4 >= size,
			cap = cap / 2;
		*/

			this->head = (this->head + 1) % this->cap;
			this->size -= 1;

			if (((double) this->size)/((double) this->cap) <= .25) {
				this->sizeDec();
			}

	}

	int Length(){ 
		return this->size;
	}

	int Capacity(){
		return this->cap;
	} 

	T Select(int k){	//return the kth smallest element,	O(size)
		//Quick Select
		return qSelect(k, 0, size - 1);

	}

	void Sort(){ //choose a sorting algorithm with worst case O(size lg size)

		mergeSort(0, this->size - 1);
		
	}

	int Search(T e){ //linear search O(size)

		for(int i = 0; i < this->size; i++){
			if (this->arr[indices(i)] == e){	//fix: originally was comparing e to arr[i] causing incorrect output
				//return indices(arr[i]);
				return i;
			}
		}

		return -1;
	}

	int BinSearch(T e){ // O(lg size)

		int min = 0;
		int max = this->size - 1;

		if(this->arr[max] < this->arr[min]) return -1;

		int midpoint = (max-min) / 2;

		while(e != this->arr[indices(midpoint)]){
			if(this->arr[indices(midpoint)] < e){
				min = midpoint + 1;
				midpoint = (max + min) / 2;
			}

			if(this->arr[indices(midpoint)] > e){
				max = midpoint - 1;
				midpoint = (max + min) / 2;
			}

			if(max < min) break;
		}

		if(e == this->arr[indices(midpoint)]) return midpoint;
		else return 1 - e;

	}

};


