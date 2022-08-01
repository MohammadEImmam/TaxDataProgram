#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

//If we go this route, which I think is the right one, have to change the key array to type zipNode in main node class
struct zipNode{
    int zipCode;
    string state;

    vector<int> grossIncomeVec;
    vector<int> taxesPaidVec;
    vector<int> taxAmountVec;
public:
    zipNode(){
        this->zipCode = 0;
    }

    void setZipCode(int inZip){
        this->zipCode = inZip;
    }

    void setZipState(string inState){
        this->state = inState;
    }

    void pushGrossInc(vector<int> grossIncome){
        this->grossIncomeVec = grossIncome;
    }

    void pushTaxesPaid(vector<int> taxesPaid){
        this->taxesPaidVec = taxesPaid;
    }

    void pushTaxAmount(vector<int> taxAmount){
        this->taxAmountVec = taxAmount;
    }

    void pushState(string inState){
        this->state = inState;
    }

    vector<int> getGrossInc(){
        return this->grossIncomeVec;
    }

    vector<int> getTaxesPaid(){
        return this->taxesPaidVec;
    }

    vector<int> getTaxAmount(){
        return this->taxAmountVec;
    }

    string getState(){
        return this->state;
    }
};


struct Node{
    Node *parents;
    Node **children;

    bool isLeaf;

    //string state;
    zipNode* zipCode; //key array
    int size;
    int num;
    //vector<int> grossIncomeVec;
    //vector<int> taxesPaidVec;
    //vector<int> taxAmountVec;
public:
    Node(int sizeIn){ //sizeIn is the degree
        this->num = 0;
        this->size = sizeIn;
        this->zipCode = new zipNode[sizeIn];
        for(int i = 0; i < sizeIn - 1; i++){
           zipCode[i] = *new zipNode();
        }

        Node** childrenTemp = new Node*[sizeIn + 1];
       for(int i = 0; i < sizeIn; i++){
           childrenTemp[i] = nullptr;
        }
        this->parents = nullptr;
        this->children = childrenTemp;
        isLeaf = false;
    }
};

class BTree{
    Node *root = nullptr;
    int size; //determines number of children, degree
    int numNodes = 0;
public:
    //public stuff
    BTree(int sizeIn){
        this->root = nullptr;
        this->size = sizeIn;
    }

    Node *getRoot(){
        return this->root;
    }

    ~BTree(){
        delete this->root;
    }

    void insert(int key, vector<int> grossIncomeVec, vector<int> taxesPaidVec, vector<int> taxAmountVec, string inState){
          this->numNodes++;

        if(this->root == nullptr){
            //cout << "Tree empty!" << endl;
            root = new Node(this->size);
            root->zipCode[0].zipCode = key;
            root->zipCode[0].pushGrossInc(grossIncomeVec);
            root->zipCode[0].pushTaxesPaid(taxesPaidVec);
            root->zipCode[0].pushTaxAmount(taxAmountVec);
            root->zipCode[0].pushState(inState);
            root->isLeaf = true;
            root->num = 1;
        } else {
            //cout  << "Tree not empty!" << endl;
            Node *currNode = this->root;
            Node *parent;
            //find the proper leaf node
            while(!currNode->isLeaf){
                parent = currNode;
                for(int i = 0; i < currNode->num; i++){
                    if(key < currNode->zipCode[i].zipCode){
                        currNode = currNode->children[i];
                        break;
                    }
                    //check overflow
                    if(i == currNode->num - 1){
                        currNode = currNode->children[i + 1];
                        break;
                    }
                }
            }
            //if there is still space in the leaf node for a new key
            if (currNode->num < this->size){
                //cout << "No leaf overflow" << endl;
                int count = 0;
                //check overflow, as well as finding correct index of new key
                while(count < currNode->num && key > currNode->zipCode[count].zipCode){
                    count++;
                }
                //this will move all indexes up one until we reach correct index, count
                if(currNode->zipCode[count].zipCode != 0){
                    for(int i = currNode->num; i > count; i--){
                        currNode->zipCode[i] = currNode->zipCode[i - 1];
                    }
                }
                //cout << "count: " << count << endl;
                currNode->zipCode[count].zipCode = key;
                currNode->zipCode[count].pushGrossInc(grossIncomeVec);
                currNode->zipCode[count].pushTaxesPaid(taxesPaidVec);
                currNode->zipCode[count].pushTaxAmount(taxAmountVec);
                currNode->zipCode[count].pushState(inState);
                currNode->num++;
                currNode->children[currNode->num] = currNode->children[currNode->num - 1];
                currNode->children[currNode->num - 1] = nullptr;
                //cout << "currNode->num: " << currNode->num << endl;
            } else {
                //overflow in leaf node, need to split node
                /*
                 * Suggestion: B+ tree uses a set number of keys and children nodes, if we have more than one zip code per Node, might be worth making a new node struct/class
                 * to store the NEW zip code Nodes with those nodes having a single internal zip code and respective vectors
                 */

                //cout << "Overflow in leaf" << endl;
                Node *newNode = new Node(this->size);
                //zipNode tempKeys[this->size + 1];
                vector<zipNode> tempKeys(this->size + 1);
                //int mid = currNode->size / 2;
                //cout << "In overflow case mid: " << mid << endl;
                for(int i = 0; i < this->size; i++){
                    tempKeys[i] = currNode->zipCode[i];
                    //cout << "tempKeys[i]: " << tempKeys[i] << endl;
                }

                int count = 0;
                //check overflow, as well as finding correct index of new key
                while(count < currNode->size && key > currNode->zipCode[count].zipCode && currNode->zipCode[count].zipCode != 0){
                    count++;
                }
                //this will move all indexes up one until we reach correct index, count
                for(int i = this->size; i > count; i--){
                    tempKeys[i] = tempKeys[i - 1];
                }
                tempKeys[count].zipCode = key;
                tempKeys[count].pushGrossInc(grossIncomeVec);
                tempKeys[count].pushTaxesPaid(taxesPaidVec);
                tempKeys[count].pushTaxAmount(taxAmountVec);
                tempKeys[count].pushState(inState);
                newNode->isLeaf = true;
                currNode->num = (size + 1) / 2;
                newNode->num = (size + 1) - ((size + 1) / 2);
                currNode->children[currNode->num] = newNode;
                newNode->children[newNode->num] = currNode->children[this->size];
                currNode->children[this->size] = nullptr;
                //set first half of keys to currnode
                for(int i = 0; i < currNode->num; i++){
                    currNode->zipCode[i] = tempKeys[i];
                }
                //set other half of keys to new node
                int temp = currNode->num;
                for(int i = 0; i < newNode->num; i++){
                    newNode->zipCode[i] =  tempKeys[temp];
                    temp++;
                }
                if(currNode != root){
                    //call to add intermediary node
                    splitNonLeaf(newNode->zipCode[0].zipCode, parent, newNode);
                } else {
                    Node *newRoot = new Node(this->size);
                    newRoot->zipCode[0] = newNode->zipCode[0];
                    newRoot->children[0] = currNode;
                    newRoot->children[1] = newNode;
                    newRoot->num = 1;
                    root = newRoot;
                }

                //hard coding outputs for testing purposes
                //cout << "In overflow leaf node case, inserting: " << newNode->zipCode[0].zipCode << endl;
                //cout << "Parent of new node: " << root->children[0]->zipCode[0].zipCode << " 1: " << root->children[1]->zipCode[0].zipCode << endl;
            }
        }
    }

    void nodeTraversal(Node *inNode){
        if(inNode != nullptr){
            for(int i = 0; i < inNode->num; i++){
                cout << inNode->zipCode[i].zipCode << " ";
            }
        }
    }

    void treeTraversal(Node *currNode){
        if(currNode != nullptr){
            for(int i = 0; i < currNode->num; i++){
                cout << currNode->zipCode[i].zipCode << " ";
            }
            cout << endl;
            if(!currNode->isLeaf){
                for(int i = 0; i < currNode->num + 1; i++){
                    treeTraversal(currNode->children[i]);
                }
            }
        }
    }

    void findState(Node *currNode, string inState, vector<vector<int>> &inVec){
        if(currNode != nullptr){
            for(int i = 0; i < currNode->num; i++){
                if(currNode->zipCode[i].state == inState){
                    for(int j = 0; j < currNode->zipCode[i].grossIncomeVec.size(); j++){
                        inVec[0].push_back(currNode->zipCode[i].grossIncomeVec[j]);
                    }
                    for(int j = 0; j < currNode->zipCode[i].taxesPaidVec.size(); j++){
                        inVec[1].push_back(currNode->zipCode[i].taxesPaidVec[j]);
                    }
                    for(int j = 0; j < currNode->zipCode[i].taxAmountVec.size(); j++){
                        inVec[2].push_back(currNode->zipCode[i].taxAmountVec[j]);
                    }
                }
            }
            cout << endl;
            if(!currNode->isLeaf){
                for(int i = 0; i < currNode->num + 1; i++){
                    treeTraversal(currNode->children[i]);
                }
            }
        }
    }

    bool searchBool(int inZip){
        //tree traversal returning node with matching zip code
        Node *currNode = this->root;
        bool found = false;
        if(this->root == nullptr){
            //cout << "Tree empty!" << endl;
        } else {
            while(!currNode->isLeaf){
                for(int i = 0; i < currNode->num; i++){
                    if(inZip < currNode->zipCode[i].zipCode){
                        currNode = currNode->children[i];
                        //need to exit for loop
                        break;
                    }
                    //if at end of
                    if(i == currNode->num - 1){
                        currNode = currNode->children[i + 1];
                        //need to exit for loop
                        break;
                    }
                }
            }
            for(int i = 0; i < currNode->num; i++){
                if(inZip == currNode->zipCode[i].zipCode){
                    found = true;
                }
            }
        }
        return found;
    }

    //function for access into proper zipNode to access data vectors
    zipNode searchZip(int inZip){
        //tree traversal returning node with matching zip code
        zipNode returnNode;
        Node *currNode = this->root;
        if(this->root == nullptr){
            //cout << "Tree empty!" << endl;
        } else {
            while(!currNode->isLeaf){
                for(int i = 0; i < currNode->num; i++){
                    if(inZip < currNode->zipCode[i].zipCode){
                        currNode = currNode->children[i];
                        //need to exit for loop
                        break;
                    }
                    //if at end of currNode, go to last child
                    if(i == currNode->num - 1){
                        currNode = currNode->children[i + 1];
                        //need to exit for loop
                        break;
                    }
                }
            }
            for(int i = 0; i < currNode->num; i++){
                if(inZip == currNode->zipCode[i].zipCode){
                    returnNode = currNode->zipCode[i];
                }
            }
        }
        return returnNode;
    }

    Node *searchParent(Node *child, Node *currNode){
        //basically a tree traversal
        Node *returnNode = nullptr;
        for(int i = 0; i < currNode->num + 1; i++){
            if(currNode->children[i] == child){
                //found parent node
                returnNode = currNode;
                return returnNode;
            } else {
                //recursively call function again
                returnNode = searchParent(child, currNode->children[i]);
                return returnNode;
            }
        }
        return returnNode;
    }

    void splitNonLeaf(int key, Node *currNode, Node *child){
        //very similar to above node splitting technique
        if(currNode->num < size - 1){
            //cout << "No leaf overflow" << endl;
            int count = 0;
            //check overflow, as well as finding correct index of new key
            while(count < currNode->size && key > currNode->zipCode[count].zipCode && currNode->zipCode[count].zipCode != 0){
                count++;
            }

            //this will move all indexes up one until we reach correct index, count
            for(int i = currNode->num; i > count; i--){
                currNode->zipCode[i] = currNode->zipCode[i - 1];
            }
            //cout << "count: " << count << endl;

            for(int i = currNode->num; i > count + 1; i--){
                currNode->children[i] = currNode->children[i - 1];
            }

            currNode->zipCode[count].zipCode = key;
            currNode->num++;
            currNode->children[count + 1] = child;
            //cout << "currNode->num: " << currNode->num << endl;
        } else {
            Node *newNode = new Node(this->size);
            vector<zipNode> tempKeys(this->size + 1);
            //zipNode tempKeys[this->size + 1];
            vector<Node*> tempChildren(this->size + 2);
            //Node *tempChildren[this->size + 2];
            for(int i = 0; i < this->size; i++){
                tempKeys[i] = currNode->zipCode[i];
                //cout << "tempKeys[i]: " << tempKeys[i] << endl;
            }
            for(int i = 0; i < this->size + 1; i++){
                tempChildren[i] = currNode->children[i];
                //cout << "tempChildren[i]: " << tempChildren[i] << endl;
            }
            //cout << "No leaf overflow" << endl;
            int count = 0;
            //check overflow, as well as finding correct index of new key
            while(count < currNode->size && key > currNode->zipCode[count].zipCode && currNode->zipCode[count].zipCode != 0){
                count++;
            }
            //this will move all indexes up one until we reach correct index, count
            for(int i = this->size; i > count; i--){
                tempKeys[count] = tempKeys[count - 1];
            }
            tempKeys[count].zipCode = key;
            int newCount = 0;
            while(newCount < currNode->size && key > currNode->zipCode[newCount].zipCode && currNode->zipCode[newCount].zipCode != 0){
                newCount++;
            }
            for(int i = this->size + 2; i > newCount; i--){
                tempChildren[count] = tempChildren[count - 1];
            }
            tempChildren[newCount + 1] = child;
            currNode->num = (size + 1) / 2;
            newNode->num = size - ((size + 1) / 2);
            int temp = currNode->num + 1;
            for(int i = 0; i < newNode->num; i++){
                newNode->zipCode[i] = tempKeys[temp];
                temp++;
            }
            temp = currNode->num + 1;
            for(int i = 0; i < newNode->num + 1; i++){
                newNode->children[i] = tempChildren[temp];
                temp++;
            }
            if(currNode != root){
                //call to add intermediary node again
                splitNonLeaf(currNode->zipCode[currNode->num].zipCode, searchParent(root, currNode), newNode);
            } else {
                Node *newRoot = new Node(size);
                newRoot->zipCode[0] = newNode->zipCode[currNode->num];
                newRoot->children[0] = currNode;
                newRoot->children[1] = newNode;
                newRoot->num = 1;
                root = newRoot;
            }
        }
    }
};
