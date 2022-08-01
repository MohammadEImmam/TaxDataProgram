#include <iostream>
#include <iomanip>
#include <queue>
#include <fstream>
#include <istream>
#include <string>
#include <map>
#include "binaryheap.h"
#include "bplustree.cpp"
using namespace std;

void ReadFile(BTree &data)
{
    ifstream file("2014.csv");

    if(file.is_open())
    {
        cout << "Loading Data..." << endl;

        string zipString;
        string state;
        string grossIncome;
        string paidAmount;
        string incomeTax;

        //The Header
        getline(file, state, ',');
        getline(file, zipString, ',');
        getline(file, grossIncome, ',');
        getline(file, paidAmount, ',');
        getline(file, incomeTax);

        //The Actual data
        int prevZip = -1;
        string prevState = "";

        //For the B+ tree
        vector<int> grossIncomeV;
        vector<int> paidAmountV;
        vector<int> incomeTaxV;

        //NOTE: CHECK IF THE VERY LAST VALUE GETS INSERTED CORRECTLY!!!
        while(file.eof() == false)
        {
            getline(file, state, ',');
            getline(file, zipString, ',');
            getline(file, grossIncome, ',');
            getline(file, paidAmount, ',');
            getline(file, incomeTax);

            int zip = stoi(zipString);

            //If it is not the first zip in the function
            if(prevZip != -1)
            {
                //If the previous zip or previous state is not the same as the current
                if(prevZip != zip || prevState != state)
                {
                    //Place the data into the tree and reset the vector
                    data.insert(prevZip, grossIncomeV, paidAmountV, incomeTaxV, prevState);

                    grossIncomeV.clear();
                    paidAmountV.clear();
                    incomeTaxV.clear();
                }
            }

            //Always add info to the tree
            int gI = stoi(grossIncome);
            int pA = stoi(paidAmount);
            int iT = stoi(incomeTax);

            grossIncomeV.push_back(gI);
            paidAmountV.push_back(pA);
            incomeTaxV.push_back(iT);

            prevZip = zip;
            prevState = state;
        }

        cout << "Loading Data Complete." << endl;
        cout << endl;
    }
    else
    {
        cout << "Error" << endl;
    }

}

double FindMedianHeap(vector<int> info)
{
    //Refrenced from the Heaps module - Heaps.pdf - slides 63-80
    int capacity = info.size();

    //Make a min tree
    BinaryHeap minTree(capacity, false);
    int minSize = 0;
    //Make a max tree
    BinaryHeap maxTree(capacity, true);
    int maxSize = 0;

    for(int i = 0; i < info.size(); i++)
    {
        double infoInt = (double) info.at(i);

        //Adding data into the trees
        if(maxSize == 0 || infoInt < maxTree.GetHead())
        {
            //Insert info into max tree
            maxTree.Insert(infoInt);
            maxSize++;
        }
        else
        {
            //Insert info into min tree
            minTree.Insert(infoInt);
            minSize++;
        }

        //Balancing the trees
        if(maxSize > minSize && (maxSize - minSize) == 2)
        {
            /*If the max tree is greater  then place the
            root of the max tree into the min tree*/
            minTree.Insert(maxTree.Extract());
            maxSize--;
            minSize++;
        }
        else if(minSize > maxSize &&(minSize - maxSize) == 2)
        {
            /*If the min tree is greater  then place the
            root of the min tree into the max tree*/
            maxTree.Insert(minTree.Extract());
            minSize--;
            maxSize++;
        }
    }

    //The Results
    if(minSize == maxSize)
    {
        double higherVal = minTree.GetHead();
        double lowerVal = maxTree.GetHead();
        return (higherVal + lowerVal) / 2;
    }

    if(minSize > maxSize)
    {
        return minTree.GetHead();
    }

    return maxTree.GetHead();
}

double FindMedianB(zipNode z, bool type)
{
    double median;
    vector<int> info;
    if(type == true)
    {
        //Find median for grossIncome
        info = z.getGrossInc();
    }
    else
    {
        //Find median for taxes paid
        info = z.getTaxesPaid();
    }

    sort(info.begin(), info.end());

    int size = info.size();

    if(size % 2 == 0)
    {
        //An even amount of values;
        size /= 2;
        median = (info.at(size) + info.at(size + 1))/ 2;
    }
    else
    {
        //An odd amount of values
        size /= 2;
        median = info.at(size);
    }

    return median;
}

double FindAverageB(zipNode z, bool type)
{
    double sum = 0;
    double amount = 0;
    vector<int> info;
    if(type == true)
    {
        //Find median for grossIncome
        info = z.getGrossInc();
    }
    else
    {
        //Find median for taxes paid
        info = z.getTaxesPaid();
    }

    for(int i = 0; i < info.size(); i++)
    {
        sum += (double) info.at(i);
        amount++;
    }

    return sum/amount;
}

double FindAverageHeap(vector<int> info)
{
    int capacity = info.size();

    //Make a min tree
    BinaryHeap minTree(capacity, false);
    int minSize = 0;

    //Placing info into min heap
    for(int i = 0; i < info.size(); i++)
    {
        minTree.Insert(info.at(i));
    }

    double sum = 0;

    //Place data into sum
    for(int i = 0; i < capacity; i++)
    {
        sum += (double) minTree.Extract();
    }

    sum /= capacity;

    return sum;
}

bool ZipSize(string zip)
{
    if (zip.size() != 5)
    {
        return false;
    }

    return true;
}

bool StateAbrevitaion(string state)
{
    if(state.size() != 2)
    {
        return false;
    }
    return true;
}

int main()
{
    //Create B+ tree
    BTree data(2000);
    ReadFile(data);

    bool continueProgram = true;

    cout << setw(52) << "Welcome to the Tax Data Program!" << endl;
    cout << setw(64) << "*This program will provide you with data of your taxes*" << endl;
    cout << setw(59) << "All that is needed is your zip code and state" << endl;

    while(continueProgram == true)
    {
        cout << endl;
        cout <<  setfill(' ') << setw(73);
        cout << setw(47) << "Tax Data Program Menu" << endl;
        cout << setfill('-') << setw(73) << "-" << endl;

        cout <<  setfill(' ');

        string state = "";
        while(StateAbrevitaion(state) == false || state == "")
        {
            cout << setw(55) << "Please enter your state's abbreviation" << endl;
            cin >> state;

            if(StateAbrevitaion(state) == false)
            {
                cout << setw(60) << "Sorry, but that is an invalid state abreviation" << endl;
                cout << setw(56) << "Please enter a valid state abbriveiation" << endl;
            }
        }

        string zipString = "";
        while(zipString == "" || ZipSize(zipString) == false)
        {
            cout << setw(49) << "Please enter your zip code" << endl;
            cin >> zipString;

            if(ZipSize(zipString) == false)
            {
                cout << setw(55) << "Sorry, but that is an invalid zip code" << endl;
                cout << setw(56) << "Please enter a zip code that has 5 digits" << endl;
            }
        }
        int zip = stoi(zipString);

        //Change these conditions up once the B+ tree is given
        zipNode z;
        if(data.searchBool(zip) == true)
        {
            z = data.searchZip(zip);
        }

        if(data.searchBool(zip) == false)
        {
            cout << setw(62) << "Sorry but that zip code is not within this data base" << endl;
            cout << setw(44) << "Please try agian" << endl;
        }
            //Find the State
        else if(z.getState() != state)
        {
            cout << setw(73) << "Sorry but the zip code and state combination is not within this data base" << endl;
            cout << setw(44) << "Please try agian" << endl;
        }
        else
        {
            int choice;
            cout << setw(65)<< "Choose a data structure that will perform the calculations" << endl;
            cout << setw(43) << "1. Binary Heap" << endl;
            cout << setw(39) << "2. B+ Tree" << endl;
            cin >> choice;


            //First get the needed values -> no need to place all the whole data structure in the funciton
            vector<int> grossIncomeV = z.getGrossInc();
            vector<int> paidAmountV = z.getTaxesPaid();

            double avgGrossIncome;
            double avgTaxesPaid;
            double medianGrossIncome;
            double medianTaxesPaid;

            string dataType;
            if(choice == 1)
            {
                //Heap
                avgGrossIncome = FindAverageHeap(grossIncomeV);
                avgTaxesPaid = FindAverageHeap(paidAmountV);
                medianGrossIncome = FindMedianHeap(grossIncomeV);
                medianTaxesPaid = FindMedianHeap(paidAmountV);
                dataType = "Binary Heap";
            }
            else if(choice == 2)
            {
                //B+ tree
                avgGrossIncome = FindAverageB(z, true);
                avgTaxesPaid = FindAverageB(z, false);
                medianGrossIncome = FindMedianB(z, true);
                medianTaxesPaid = FindMedianB(z, false);
                dataType = "B+ Tree";
            }

            cout << endl;
            cout << setw(27) << "Tax Data for " << zipString << ", " << state << " using a " << dataType << endl;
            cout << setfill('-') << setw(73) << "-" << endl;
            cout << setfill(' ');
            cout << setw(39) << "Avg Gross Income: $" << avgGrossIncome << endl;
            cout << setw(45) << "Avg Amount of Tax Paid: $" << avgTaxesPaid << endl;
            cout << setw(42) << "Median Gross Income: $" << medianGrossIncome << endl;
            cout << setw(48) << "Median Amount of Tax Paid: $" << medianTaxesPaid << endl;

            //Allows the program to end
            cout << endl;
            cout << setw(71) << "To end the program please enter -1 to continue please enter any value" << endl;
            string input;
            cin >> input;

            if(input == "-1")
            {
                continueProgram = false;
            }
        }
    }
}

