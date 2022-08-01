// Inspiration : https://www.geeksforgeeks.org/binary-heap/
// MOHAMMAD IMMAM
#include "iostream"
#include <climits>
using namespace std;
class BinaryHeap {
  int size;
  int capacity;
  long *arr;
  // If type == false : Min
  // If type == true : Max
  bool type;

public:
  BinaryHeap(int capacity, bool type);
  void Heapify(int i);
  int GetParent(int index) { return (index - 1) / 2; };
  int GetLeftChild(int index) { return (2 * index + 1); };
  int GetRightChild(int index) { return (2 * index + 2); };
  long Extract();
  long GetHead() { return arr[0]; };
  void DeleteIndex(int index);
  void Decrease(int index, int value);
  void Insert(long data);
  static void Swap(long *first, long *second) {
    long temp = *first;
    *first = *second;
    *second = temp;
  }
};

BinaryHeap::BinaryHeap(int capacity, bool type) {
  size = 0;
  this->capacity = capacity;
  this->type = type;
  arr = new long[capacity];
}

void BinaryHeap::Insert(long data) {
  if (size == capacity) {
    // If we don't want to increase dynamically, return here and don't increase
    cout << "Capacity of heap reached, increasing size.";
    capacity++;
  }
  size++;
  int index = size - 1;
  arr[index] = data;
  if (type == false) {
    // MIN HEAP
    while (index != 0 && arr[GetParent(index)] > arr[index]) {
      Swap(&arr[index], &arr[GetParent(index)]);
      index = GetParent(index);
    }
  } else {
    // MAX HEAP
    while (index != 0 && arr[GetParent(index)] < arr[index]) {
      Swap(&arr[index], &arr[GetParent(index)]);
      index = GetParent(index);
    }
  }
}

long BinaryHeap::Extract() {
  if (size <= 0) {
    return INT_MAX;
  }
  if (size == 1) {
    size--;
    return arr[0];
  }
  long head = arr[0];
  arr[0] = arr[size - 1];
  size--;
  Heapify(0);
  return head;
}

void BinaryHeap::Decrease(int index, int value) {
  arr[index] = value;
  if (type == false) {
    while (index != 0 && arr[GetParent(index)] > arr[index]) {
      Swap(&arr[index], &arr[GetParent(index)]);
      index = GetParent(index);
    }
  } else {
    while (index != 0 && arr[GetParent(index)] < arr[index]) {
      Swap(&arr[index], &arr[GetParent(index)]);
      index = GetParent(index);
    }
  }
}

void BinaryHeap::DeleteIndex(int index) {
  Decrease(index, INT_MIN);
  Extract();
}

void BinaryHeap::Heapify(int index) {
  int left = GetLeftChild(index);
  int right = GetRightChild(index);
  int smallest = index;
  if (type == false) {
    if (left < size && arr[left] < arr[index]) {
      smallest = left;
    }
    if (right < size && arr[right] < arr[smallest]) {
      smallest = right;
    }
    if (smallest != index) {
      Swap(&arr[index], &arr[smallest]);
      Heapify(smallest);
    }
  } else {
    if (left < size && arr[left] > arr[index]) {
      smallest = left;
    }
    if (right < size && arr[right] > arr[smallest]) {
      smallest = right;
    }
    if (smallest != index) {
      Swap(&arr[index], &arr[smallest]);
      Heapify(smallest);
    }
  }
}