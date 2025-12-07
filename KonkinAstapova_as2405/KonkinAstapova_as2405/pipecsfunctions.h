#pragma once
#include "pipe.h"
#include "cs.h"
#include "connection.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
#include <list>
using namespace std;

class PipelineSystem {
private:
    unordered_map<int, Pipe> pipes;
    unordered_map<int, CS> stations;
    vector<Connection> connections;
    unordered_map<int, list<int>> adjacencyList;

public:
    PipelineSystem() {}

    void addPipe();
    void addCS();
    void viewAllObjects();
    void editPipe();
    void editCS();
    void deletePipe();
    void deleteCS();
    void searchPipesMenu();
    void searchCSMenu();
    unordered_set<int> findPipesByName(const string& name);
    unordered_set<int> findPipesByFixing(bool fixing);
    unordered_set<int> findCSByName(const string& name);
    unordered_set<int> findCSByUnusedPercentage(float minPercent, float maxPercent);

    void batchEditPipes(const unordered_set<int>& pipeIds);
    void saveToFile(const string& filename);
    void loadFromFile(const string& filename);

    void connectCS();
    void viewNetwork();
    void topologicalSort();
    bool hasCycleUtil(int v, unordered_map<int, bool>& visited, unordered_map<int, bool>& recStack);
    bool hasCycle();
    void topologicalSortUtil(int v, unordered_map<int, bool>& visited, list<int>& stack);

    int findFreePipeByDiameter(int diameter);

};

void loadPipeFromFile(istream& in, Pipe& pipe);
void loadCSFromFile(istream& in, CS& cs);