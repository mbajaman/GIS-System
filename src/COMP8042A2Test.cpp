#include <iostream>
#include <CuckooHashTable.h>
#include <Graph.h>
#include <SequenceAlignment.h>

using namespace std;

int main() {
    cout << "***** Part 1: Modified Cuckoo Hashing *****" << endl;
    HashTable<string, StringHashFamily<2>> h1(5);
    HashTable<string, StringHashFamily<2>> h2(11);
    h1.insert("A");
    h1.insert("B");
    h1.insert("C");
    h1.insert("D");
    h1.insert("E");
    h1.insert("F");
    h1.insert("G");
    cout << h1.print();
    int keys_1[] = { 20,50,53,75,100,67,105,3,36,39 };
    for (auto k:keys_1)
        h2.insert(std::to_string(k));
    cout << h2.print();
    /*
     The output of my implementation:
        Contents of Cuckoo hash table:
         | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | [] | A | B | C | D | E | F | G | [] | []
        Contents of Cuckoo hash table:
         | [] | [] | [] | [] | 50 | [] | 36 | 53 | [] | 39 | [] | [] | [] | 75 | [] | [] | [] | [] | [] | 67 | [] | 100 | 3 | [] | [] | [] | 105 | 20 | []
     */
    cout << "***** Part 2: Topological Sorting *****" << endl;
    int adjacencyMatrix[4][4] = {0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0};
    int testAdjacencyMatrix[5][5] = {0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0};
    Graph graph;
    graph.CreateGraph(testAdjacencyMatrix);
    cout << "Result of topological sort: ";
    cout << graph.TopologicalSort() << endl;
    /*
     The output of my implementation:
        Result of topological sorting: 0 1 2 3
     */
    cout << "***** Part 3: Sequence Alignment *****" << endl;
    string s1 = "HELLO";
    string s2 = "HeLO";
    int penalty_mismatch = 2;
    int penalty_gap = 1;
    cout << s1 << endl;
    cout << s2 << endl;
    int cost = alignSequenceAndGetMinCost(s1, s2, penalty_mismatch, penalty_gap);
    cout << s1 << endl;
    cout << s2 << endl;
    cout << cost << endl;
    /*
    The output of my implementation:
     HELLO
     HeLO
     HEL_LO
     H__eLO
     3
    */
    return 0;
}
