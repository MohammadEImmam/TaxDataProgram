Administrative:
Team Name: Taxes
Team Members: Ibet Gonzalez Viltres, Mohammad Immam, Ryan Kolb
GitHub URL: https://github.com/MohammadEImmam/TaxDataProgram
Link to Video: https://www.youtube.com/watch?v=-9u8hJ8bH1Y&ab_channel=IbetGonzalez

Redefined Proposal:

Problem: 
Different states and zip codes tend to have different income tax. This means that based on where one lives, they can be paying a different amount of tax. This can create confusion and the need for extensive research to find information about income tax.
		
Motivation:
	The problem affects the people that are trying to find a place to move. This tool can help them make a choice based on the data of income tax. Moreover, the problem also affects the general population as a whole in terms of a knowledge gap of not understanding income tax fluctuations.

Features:
		We know that we have solved the problem when our program is able to :
Give income tax information based on area code
Search through an extensive database to find tax information
Get information based on state and/or zip codes.
Use a B+ tree to store all of the zip codes and their associated information
Use a min and max heap to find the median values

Data: 
		Individual Income Tax Statistics :
The dataset contains tax information based on ZIP code. The data is based on individual tax income tax returns that were filed with the IRS in 2014. Data includes :
State
Zip Code
a00100 - Adjusted gross income
a18300 - Taxes paid amount
a06500 - Income tax amount [8]
	
https://www.kaggle.com/datasets/irs/individual-income-tax-statistics?select=2014.csv




Tools:
We will be using C++ as the only programming language for this project and our IDE will be Visual Studio Code. The GUI will be built in the console.

Visuals:


	
Strategy:
We plan to use a B+ tree to store and navigate through the data, using the zip codes as the comparison values for the sorting portion. A B+ tree has a performance of O(log n) for its search, insert, and delete operations.

We also plan on using a min and max heap to find the median adjusted gross income and median amount of taxes paid. This data structure has a performance of (log n) for its insert and extract operations.

Distributions of Responsibility and Roles:

Ibet Gonzalez Viltres
 Mohammad Immam
Ryan Kolb
Putting together the GUI
Creating the min and max heaps
Creating the B+ tree
Implementing both data sets into the program





Analysis:
Any changes the group made after the proposal? The rationale behind the changes.
 We changed the roles for the project to account for all the group members.

Complexity analysis of the major functions/features you implemented in terms of Big O for the worst case
 
B+ Tree: n = Number of keys in the tree
Insert: O(logn)
Since the data is stored only in the leaf nodes, we do not have to worry about the non leaf nodes and only use them for traversal. Thus, insertion traverses to the O(logn) position  
splitNonLeaf: O(logn)
Works the same as the insertion operation but is called for specifically when there is a need to add an intermediary node with a key of the smallest of the larger child 
treeTraversal: O(n)
Tree traversal goes through every key in the tree, thus the complexity is O(n)
searchBool: O(logn)
The search functions are also O(logn) because we do not have to search through all of the intermediary node keys.
searchZip: O(logn)
Traverses logn nodes to find the right key
searchParent: O(logn)
Searches logn keys to find when the children of currNode match up to the child node
Binary Heap:  H = Height
Note: The height of a complete binary tree with N nodes is logN, complexities can also be defined in that form.
Insert: O(H)
Complexity of adding an element to the heap is O(1) because it inserts to the end of the array, and the complexity of swapping the elements is O(H) = O(1+H) = O(H)
Extract: O(H)
Complexity of deleting an element from the heap is also O(1) because it deletes the first index, and the complexity of swapping the elements is O(H) = O(1+H) = O(H)
Decrease: O(H)
Random access of index is O(1), and the swapping of the elements is O(H), thus O(1+H) = O(H)
Delete: O(H)
Complexity of deleting an element is O(1) due to random access, the swapping of elements is O(H), thus O(1+H) = O(H)
Heapify: O(H)
The method swaps elements based on parameters and the complexity depends on the height of the tree thus, O(H)
 
Main Program:
ReadFile: O(n)
The program must iterate through an n amount of information to store it into the B+ tree and as it is iterating it inserts the data into the B+ tree which is O(log n). This makes the time complexity to be O(n + logn) which can be simplified to O(n)
FindAverageB: O(n)
The function iterates through n amount of values in the vector to calculate the average the value.
FindMedianB: O(1)
The function has no iterations and only copies existing data so it is O(1).
FindAverageHeap: O(n)
This function creates a max heap which takes O(H) time and then iterates through the heap which takes O(n) time to find the average. This makes the data O(H + n) time which is simplified to O(n)
FindMedianHeap: O(H*n)
This function iterates through a vector which takes O(n) time and it also inserts data into the heaps which takes O(H) time making it O(H*n) time.
Main: O(n^3 * H)
The ReadFile is called first and it is O(n) time. Then there is a while loop that will execute after n amount of inputs and within it are the FindAverage functions and FindMedian functions. Heap has a worse time complexity than the B+ tree so the worst time complexity for the while loop is O(n^3 *H). This makes the time complexity for the main O(n + n^3 * H) which is simplified to O(n^3 * H)
 
 
 
Reflection:
As a group, how was the overall experience for the project?
The overall experience was great, we faced challenges and helped each other overcome them. Moreover, we assigned individual tasks to each other which lined up at the end to create the final version of the project.
 
Did you have any challenges? If so, describe.
We faced challenges during parts of the project and specific functions of the data structures, we overcame them by helping each other out and researching the issues.
 
Some specific examples were in the B+ tree, mainly the insertion method as that is the driving code behind the tree. The first issue was allocating the wrong number of keys and children nodes, where the entire node structure was redone. Another major issue was the actual data storage. Originally, each leaf node had an array of integers as keys, with 3 vectors that stored the data. However, since each node can only have 3 data vectors but multiple keys, a new zipNode struct was created as the keys instead. 
 
If you were to start once again as a group, any changes you would make to the project and/or workflow?
If we were to start over, one change that we would make would be the planning process. We would draw out the main function and what we want within the program before starting the code.
 
Comment on what each of the members learned through this process : 
 
Mohammad Immam - Deep dived into binary heap and learned the data structure’s implementation in C++. Understood B+ trees better and it’s implementation in C++.
 
Ibet Gonzalez - I learned how to implement binary heaps and B+ trees into a program and how to use them to be able to get the wanted data.
 
Ryan Kolb - I discovered the intricacies of B+ trees and its effectiveness in storing large amounts of data. The insertion into the B+ tree was easily one of the most challenging tasks I had taken on, but after much research I was able to understand it and implement it in C++.
 
 
References
Inspiration for Binary Heap framework : https://www.geeksforgeeks.org/binary-heap/
B+ Tree visualizations: https://iq.opengenus.org/b-tree-search-insert-delete-operations/
https://www.geeksforgeeks.org/introduction-of-b-tree/
https://www.geeksforgeeks.org/insertion-in-a-b-tree/
