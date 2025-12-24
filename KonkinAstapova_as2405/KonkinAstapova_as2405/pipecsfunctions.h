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
    unordered_map<int, vector<int>> buildAdjacencyList() const;
    bool hasCycleUtil(int v, unordered_map<int, bool>& visited, unordered_map<int, bool>& recStack,const unordered_map<int, vector<int>>& adjList) const;
    bool hasCycle(const unordered_map<int, vector<int>>& adjList) const;
    void topologicalSortUtil(int v, unordered_map<int, bool>& visited,list<int>& stack,const unordered_map<int, vector<int>>& adjList) const;

    int findFreePipeByDiameter(int diameter);

    void disconnectCS();
    void disconnectConnection();
    void disconnectByPipe();

    bool removeConnection(int startCS, int endCS);
    void removeAllConnectionsWithCS(int csId);
    void removeConnectionsByPipe(int pipeId);

};

void loadPipeFromFile(istream& in, Pipe& pipe);
void loadCSFromFile(istream& in, CS& cs);

extern const std::vector<int> ALLOWED_DIAMETERS;