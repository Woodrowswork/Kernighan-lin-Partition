#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <set>
#include <vector>


using namespace std;

int main(/*int argc, char *argv[]*/) {

    std::ifstream inputfile;
    //inputfile.open(argv[1]);
    inputfile.open("C:/Users/Luna Lovegood/ClionProjects/Kernighan-lin/case2.txt");

    if (!inputfile)
    {
        printf("Input file failed to open. Program Terminated!\n");
        return 1;
    }

    int NumNodes, NumEdges, rows, columns, internalcost, externalcost, cutcost,
    bestcase;//change1
    set <int> lockednodes;

    vector < set <int> > PartitionsA;
    vector < set <int> > PartitionsB;

    inputfile >> NumNodes;
    inputfile >> NumEdges;
    int nodecost[NumNodes],
    //totalcost[NumNodes / 2],
    gain[(NumNodes / 2)][(NumNodes / 2)];
    //initializing and creating our matrix of nodes and edges


    int GraphMatrix[NumNodes][NumNodes];
    for (int i = 0; i < NumNodes; i++)
        for (int j = 0; j < NumNodes; j++)
            GraphMatrix[i][j] = 0;

    //while (!inputfile.eof())
    for (int m = 0; m < NumEdges; m++)
        {
        inputfile >> rows >> columns;
        GraphMatrix[rows-1][columns-1] = GraphMatrix[rows-1][columns-1] + 1;
        GraphMatrix[columns-1][rows-1] = GraphMatrix[columns-1][rows-1] + 1;
        }
    inputfile.close();


    //debugging code
    printf("there are %d nodes.\n", NumNodes);
    printf("there are %d edges.\n", NumEdges);
    for (int k = 0; k < NumNodes; k++) {
        for (int l = 0; l < NumNodes; l++)
            printf("%d ", GraphMatrix[k][l]);
        printf(" \n");
    }

    set <int> PartitionA;
    for (int n = 0; n < NumNodes / 2; n++)
        PartitionA.insert(n);
    set <int> PartitionB;
    for (int o = NumNodes / 2; o < NumNodes; o++)
        PartitionB.insert(o);
    set <int> finalPartitionA;
    set <int> finalPartitionB;
    //printing out the partitions
    cout << "PartitionA contains these nodes." << endl;
    for (std::set<int>::iterator itr = PartitionA.begin(); itr != PartitionA.end(); ++itr)
    {
        cout << '\t' << *itr + 1;
    }
    cout << endl;

    cout << "PartitionB contains these nodes." << endl;
    for (std::set<int>::iterator itr = PartitionB.begin(); itr != PartitionB.end(); ++itr)
    {
        cout << '\t' << *itr + 1;
    }
    cout << endl;
    finalPartitionA = PartitionA;
    finalPartitionB = PartitionB;
    //compute the initial cut cost
    cutcost = 0;
    for (int i = 0; i < NumNodes; i++)
    {
        internalcost = 0;
        externalcost = 0;
        for (int j = 0; j < NumNodes; j++) {
                if (PartitionA.count(i) == 1 and PartitionB.count(j) == 1)
                    externalcost = externalcost + GraphMatrix[i][j];
                else if (PartitionA.count(i) == 1 and PartitionA.count(j) == 1)
                    internalcost = internalcost + GraphMatrix[i][j];
                else if (PartitionB.count(i) == 1 and PartitionA.count(j) == 1)
                    externalcost = externalcost + GraphMatrix[i][j];
                else if (PartitionB.count(i) == 1 and PartitionB.count(j) == 1)
                    internalcost = internalcost + GraphMatrix[i][j];
        }
        cutcost = cutcost + externalcost;
        nodecost[i] = externalcost - internalcost;
        cout << "Node " << i+1 << " has " << externalcost << " connected edge to the other partition and " <<
        internalcost << " edges connected inside it's own partition " << "It's total cost is " << nodecost[i] << "." <<
        endl;
    }
    std::cout << "\nThe total cut cost is " << cutcost / 2 << ".\n" << std::endl;
    bestcase = cutcost;
    //loop
    do
    {  cutcost = bestcase;
    int totalcost[NumNodes / 2];
        for (int x = 0; x < NumNodes / 2; x++)
            totalcost[x] = 0;
        for (int u = 0; u < NumNodes / 2; u++) {
            //compute gain
            /*will need to declare gain array down here
             * gain[((NumNodes / 2) - i) * ((NumNodes / 2) - i)];
             */

            for (int x = 0; x < NumNodes / 2; x++)
                cout << totalcost[x] << endl;

            int row_gain_incrementor = 0,
                    column_gain_incrementor = 0,
                    this_gain[NumNodes / 2],
                    swapA = 0,
                    swapB = 0;
            this_gain[u] = -2147483648;

            for (std::set<int>::iterator PartA = PartitionA.begin(); PartA != PartitionA.end(); ++PartA) {
                for (std::set<int>::iterator PartB = PartitionB.begin(); PartB != PartitionB.end(); ++PartB) {
                    gain[row_gain_incrementor][column_gain_incrementor] = nodecost[*PartA] + nodecost[*PartB] -
                                                                          (GraphMatrix[*PartA][*PartB] +
                                                                           GraphMatrix[*PartB][*PartA]);
                    cout << "The gain of swapping " << *PartA + 1 << " and " << *PartB + 1 << " is " <<
                         gain[row_gain_incrementor][column_gain_incrementor] << endl;
                    //this_gain[i] = gain[row_gain_incrementor][column_gain_incrementor];
                    if (gain[row_gain_incrementor][column_gain_incrementor] > this_gain[u] and
                        lockednodes.count(*PartA) == 0
                        and lockednodes.count(*PartB) == 0) {
                        this_gain[u] = gain[row_gain_incrementor][column_gain_incrementor];
                        swapA = *PartA;
                        swapB = *PartB;
                    }
                    column_gain_incrementor++;
                }
                row_gain_incrementor++;
            }
            cout << "i = " << u << endl;
            for (int x = 0; x < NumNodes / 2; x++)
                cout << totalcost[x] << endl;

            if (lockednodes.empty() and this_gain[u] <= 0) {
                cout << "No more good swaps." << endl;
                return 2;
            }
            //finding highest gain
            /*int swapspot = 0,
            localgain = gain[0][0];
                for (int i = 0; i < gain_incrementor; i++) {
                    if (gain[i] > localgain) {
                        localgain = gain[i];
                        swapspot = i;
                    }
                }
            //perform the swap
            PartitionA.insert(swapspot%(NumNodes/2) + NumNodes/2);
            lockednodes.insert(swapspot%(NumNodes/2) + NumNodes/2);
            PartitionA.erase(swapspot/(NumNodes/2));
            PartitionB.insert(swapspot/(NumNodes/2));
            lockednodes.insert(swapspot/(NumNodes/2));
            PartitionB.erase(swapspot%(NumNodes/2) + NumNodes/2);*/
            PartitionA.insert(swapB);
            PartitionA.erase(swapA);
            lockednodes.insert(swapB);
            PartitionsA.push_back(PartitionA);
            PartitionB.insert(swapA);
            PartitionB.erase(swapB);
            PartitionsB.push_back(PartitionB);
            lockednodes.insert(swapA);
            //printing out the partitions
            cout << "PartitionA contains these nodes." << endl;
            for (std::set<int>::iterator itr = PartitionsA[u].begin(); itr != PartitionsA[u].end(); ++itr) {
                cout << '\t' << *itr + 1;
            }
            cout << endl;

            cout << "PartitionB contains these nodes." << endl;
            for (std::set<int>::iterator itr = PartitionsB[u].begin(); itr != PartitionsB[u].end(); ++itr) {
                cout << '\t' << *itr + 1;
            }
            cout << endl;
            cout << "i = " << u << endl;
            //totalcost[i] = 0;
            for (int k = 0; k < NumNodes; k++) {
                internalcost = 0;
                externalcost = 0;
                for (int j = 0; j < NumNodes; j++) {
                    if (PartitionA.count(k) == 1 and PartitionB.count(j) == 1)
                        externalcost = externalcost + GraphMatrix[k][j];
                    else if (PartitionA.count(k) == 1 and PartitionA.count(j) == 1)
                        internalcost = internalcost + GraphMatrix[k][j];
                    else if (PartitionB.count(k) == 1 and PartitionA.count(j) == 1)
                        externalcost = externalcost + GraphMatrix[k][j];
                    else if (PartitionB.count(k) == 1 and PartitionB.count(j) == 1)
                        internalcost = internalcost + GraphMatrix[k][j];
                    else {
                        externalcost = externalcost;
                        internalcost = internalcost;
                    }
                }
                totalcost[u] = totalcost[u] + externalcost;
                nodecost[k] = externalcost - internalcost;
                cout << "Node " << k + 1 << " has " << externalcost << " connected edge to the other partition and " <<
                     internalcost << " edges connected inside it's own partition " << "It's total cost is "
                     << nodecost[k]
                     << "." << endl;
            }
            std::cout << "\nThe total cut cost is " << totalcost[u] / 2 << "." << std::endl;
            std::cout << "The gain is " << this_gain[u] << ".\n" << std::endl;



        }


        cout << "\n LEAVING INNER LOOP!\n" << endl;

        int swapset = 0;

        for (int z = 0; z < NumNodes / 2; z++)
            if (totalcost[z] <= cutcost and totalcost[z] <= bestcase) {
                bestcase = totalcost[z];
                swapset = z;
            }
        for (int x = 0; x < NumNodes / 2; x++)
            cout << totalcost[x] << endl;

        set <int> finalpartionA;

        cout << cutcost << endl;
        cout << bestcase << endl;
        cout << swapset << endl;
        if (bestcase < cutcost){//change 2
            finalPartitionA.swap(PartitionsA[swapset]);
            finalPartitionB.swap(PartitionsB[swapset]);
            }
        lockednodes.clear();
        PartitionsA.clear();
        PartitionsB.clear();


        cout << "PartitionA contains these nodes." << endl;
        for (std::set<int>::iterator itr = finalPartitionA.begin(); itr != finalPartitionA.end(); ++itr)
        {
            cout << '\t' << *itr + 1;
        }
        cout << endl;

        cout << "PartitionB contains these nodes." << endl;
        for (std::set<int>::iterator itr = finalPartitionB.begin(); itr != finalPartitionB.end(); ++itr)
        {
            cout << '\t' << *itr + 1;
        }
        cout << endl;

        for (int k = 0; k < NumNodes; k++)
        {
            for (int j = 0; j < NumNodes; j++) {
                if (finalPartitionA.count(k) == 1 and finalPartitionB.count(j) == 1 and GraphMatrix[k][j] > 0)
                    cout << "The edge Indices are " << k + 1 << " , " << j + 1 << endl;
            }

        }
        cout << "\nLEAVING OUTER LOOP!\n" << endl;
   } while (bestcase < cutcost );


    /*if (bestcase == cutcost)
    {
        cout << "No good swaps!" << endl;
        return 2;
    }*/






    return 0;
}
