#include "pipecsfunctions.h"
#include "logger.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stack>
#include <queue>
using namespace std;

const std::vector<int> ALLOWED_DIAMETERS = { 500, 700, 1000, 1400 };

void PipelineSystem::addPipe() {
    Pipe pipe;
    cin >> pipe;
    pipes.emplace(pipe.getId(), pipe);
    cout << "New pipe added successfully! ID: " << pipe.getId() << "\n\n";
}

void PipelineSystem::addCS() {
    CS cs;
    cin >> cs;
    stations.emplace(cs.getId(), cs);
    cout << "New compressor station added successfully! ID: " << cs.getId() << "\n\n";
}

void PipelineSystem::viewAllObjects() {
    if (pipes.empty()) {
        cout << "No pipes available.\n\n";
    }
    else {
        cout << "Pipes\n";
        for (const auto& pipe_pair : pipes) {
            cout << pipe_pair.second;
        }
    }

    if (stations.empty()) {
        cout << "No compressor stations available.\n\n";
    }
    else {
        cout << "Compressor Stations\n";
        for (const auto& cs_pair : stations) {
            cout << cs_pair.second;
        }
    }

    if (!connections.empty()) {
        cout << "Connections in Gas Network\n";
        for (const auto& conn : connections) {
            cout << conn;
        }
    }
}

void PipelineSystem::editPipe() {
    int id;
    cout << "Enter pipe ID to edit: ";
    id = GetCorrectNumber<int>(0, INT_MAX);
    if (pipes.count(id)) {
        pipes[id].editPipe();
        cout << "Pipe edited successfully!\n\n";
    }
    else {
        cout << "Pipe with ID " << id << " not found.\n\n";
    }
}

void PipelineSystem::editCS() {
    int id;
    cout << "Enter compressor station ID to edit: ";
    id = GetCorrectNumber<int>(0, INT_MAX);
    if (stations.count(id)) {
        stations[id].editCS();
        cout << "Compressor station edited successfully!\n\n";
    }
    else {
        cout << "Compressor station with ID " << id << " not found.\n\n";
    }
}

void PipelineSystem::deletePipe() {
    int id;
    cout << "Enter pipe ID to delete: ";
    id = GetCorrectNumber<int>(0, INT_MAX);
    if (pipes.count(id)) {
        if (pipes[id].getInUse()) {
            cout << "Cannot delete pipe ID " << id << " because it is used in a connection.\n";
            cout << "Please delete the connection first.\n\n";
            return;
        }
        pipes.erase(id);
        cout << "Pipe deleted successfully!\n\n";
    }
    else {
        cout << "Pipe with ID " << id << " not found.\n\n";
    }
}

void PipelineSystem::deleteCS() {
    int id;
    cout << "Enter compressor station ID to delete: ";
    id = GetCorrectNumber<int>(0, INT_MAX);
    if (stations.count(id)) {
        bool isUsed = false;
        for (const auto& conn : connections) {
            if (conn.getStartCS() == id || conn.getEndCS() == id) {
                isUsed = true;
                break;
            }
        }

        if (isUsed) {
            cout << "Cannot delete CS ID " << id << " because it is used in connections.\n";
            cout << "Please delete related connections first.\n\n";
            return;
        }

        stations.erase(id);
        cout << "Compressor station deleted successfully!\n\n";
    }
    else {
        cout << "Compressor station with ID " << id << " not found.\n\n";
    }
}

unordered_set<int> PipelineSystem::findPipesByName(const string& name) {
    unordered_set<int> result;
    for (const auto& pipe_pair : pipes) {
        if (pipe_pair.second.getName().find(name) != string::npos) {
            result.insert(pipe_pair.first);
        }
    }
    return result;
}

unordered_set<int> PipelineSystem::findPipesByFixing(bool fixing) {
    unordered_set<int> result;
    for (const auto& pipe_pair : pipes) {
        if (pipe_pair.second.getFixing() == fixing) {
            result.insert(pipe_pair.first);
        }
    }
    return result;
}

unordered_set<int> PipelineSystem::findCSByName(const string& name) {
    unordered_set<int> result;
    for (const auto& cs_pair : stations) {
        if (cs_pair.second.getName().find(name) != string::npos) {
            result.insert(cs_pair.first);
        }
    }
    return result;
}

unordered_set<int> PipelineSystem::findCSByUnusedPercentage(float minPercent, float maxPercent) {
    unordered_set<int> result;
    for (const auto& cs_pair : stations) {
        float unusedPercent = cs_pair.second.getUnusedPercentage();
        if (unusedPercent >= minPercent && unusedPercent <= maxPercent) {
            result.insert(cs_pair.first);
        }
    }
    return result;
}

void PipelineSystem::searchPipesMenu() {
    cout << "Search pipes:\n";
    cout << "1. By name\n";
    cout << "2. By repair status\n";
    cout << "3. By usage status\n";
    cout << "Choice: ";
    int choice = GetCorrectNumber<int>(1, 3);
    unordered_set<int> results;
    if (choice == 1) {
        string name;
        cout << "Enter name to search: ";
        INPUT_LINE(cin, name);
        results = findPipesByName(name);
    }
    else if (choice == 2) {
        cout << "Enter status (0 - in repair, 1 - working): ";
        int status = GetCorrectNumber<int>(0, 1);
        results = findPipesByFixing(status == 1);
    }
    else if (choice == 3) {
        cout << "Enter usage status (0 - free, 1 - in use): ";
        int usage = GetCorrectNumber<int>(0, 1);
        for (const auto& pipe_pair : pipes) {
            if (pipe_pair.second.getInUse() == (usage == 1)) {
                results.insert(pipe_pair.first);
            }
        }
    }
    if (results.empty()) {
        cout << "No pipes found.\n\n";
    }
    else {
        cout << "Found " << results.size() << " pipes:\n";
        for (int id : results) {
            if (pipes.count(id)) {
                cout << "ID: " << id << " - " << pipes[id].getName()
                    << " (Status: " << (pipes[id].getFixing() ? "Working" : "In repair")
                    << ", In use: " << (pipes[id].getInUse() ? "Yes" : "No") << ")\n";
            }
        }
        cout << endl;
    }
}

void PipelineSystem::searchCSMenu() {
    cout << "Search compressor stations:\n";
    cout << "1. By name\n";
    cout << "2. By unused workshops percentage\n";
    cout << "Choice: ";
    int choice = GetCorrectNumber<int>(1, 2);
    unordered_set<int> results;
    if (choice == 1) {
        string name;
        cout << "Enter name to search: ";
        INPUT_LINE(cin, name);
        results = findCSByName(name);
    }
    else if (choice == 2) {
        float minPercent, maxPercent;
        cout << "Enter minimum unused percentage: ";
        minPercent = GetCorrectNumber<float>(0, 100);
        cout << "Enter maximum unused percentage: ";
        maxPercent = GetCorrectNumber<float>(minPercent, 100);
        results = findCSByUnusedPercentage(minPercent, maxPercent);
    }
    if (results.empty()) {
        cout << "No compressor stations found.\n\n";
    }
    else {
        cout << "Found " << results.size() << " compressor stations:\n";
        for (int id : results) {
            if (stations.count(id)) {
                float unusedPercent = stations[id].getUnusedPercentage();
                cout << "ID: " << id << " - " << stations[id].getName()
                    << " (Unused: " << unusedPercent << "%)\n";
            }
        }
        cout << endl;
    }
}

void PipelineSystem::batchEditPipes(const unordered_set<int>& pipeIds) {
    if (pipeIds.empty()) {
        cout << "No pipes to edit.\n\n";
        return;
    }
    cout << "Selected pipes for batch editing:\n";
    for (int id : pipeIds) {
        if (pipes.count(id)) {
            cout << "ID: " << id << " - " << pipes[id].getName() << endl;
        }
    }
    cout << "Edit all pipes? (1 - yes, 0 - select specific): ";
    int choice = GetCorrectNumber<int>(0, 1);
    unordered_set<int> pipesToEdit;
    if (choice == 1) {
        pipesToEdit = pipeIds;
    }
    else {
        cout << "Enter pipe IDs to edit (one per line, -1 to finish): ";
        int id;
        while (true) {
            id = GetCorrectNumber<int>(-1, INT_MAX);
            if (id == -1) break;
            if (pipeIds.find(id) != pipeIds.end()) {
                pipesToEdit.insert(id);
            }
            else {
                cout << "ID " << id << " not in search results.\n";
            }
        }
    }
    if (pipesToEdit.empty()) {
        cout << "No pipes selected for editing.\n\n";
        return;
    }
    unordered_map<int, bool> pipeStatuses;
    for (int id : pipesToEdit) {
        if (pipes.count(id)) {
            cout << "Enter status for pipe ID " << id << " (0 - in repair, 1 - working): ";
            bool newStatus = GetCorrectBool();
            pipeStatuses[id] = newStatus;
        }
    }
    for (const auto& pipe_pair : pipeStatuses) {
        int id = pipe_pair.first;
        bool newStatus = pipe_pair.second;
        if (pipes.count(id)) {
            bool oldStatus = pipes[id].getFixing();
            pipes[id].setFixing(newStatus);
            cout << "Pipe ID " << id << " changed from "
                << (oldStatus ? "working" : "repair") << " to "
                << (newStatus ? "working" : "repair") << endl;
        }
    }
    cout << "Batch editing completed!\n\n";
}

int PipelineSystem::findFreePipeByDiameter(int diameter) {
    for (auto& pipe_pair : pipes) {
        Pipe& pipe = pipe_pair.second;
        if (!pipe.getInUse() && pipe.getDiameter() == diameter && pipe.getFixing()) {
            return pipe_pair.first;
        }
    }
    return -1;
}

unordered_map<int, vector<int>> PipelineSystem::buildAdjacencyList() const {
    unordered_map<int, vector<int>> adjList;

    for (const auto& conn : connections) {
        int start = conn.getStartCS();
        int end = conn.getEndCS();


        if (stations.count(start) && stations.count(end)) {
            adjList[start].push_back(end);
        }
    }

    return adjList;
}

void PipelineSystem::connectCS() {
    if (stations.size() < 2) {
        cout << "Need at least 2 compressor stations to create a connection.\n\n";
        return;
    }

    cout << "Available compressor stations:\n";
    for (const auto& cs_pair : stations) {
        cout << "ID: " << cs_pair.first << " - " << cs_pair.second.getName() << endl;
    }

    int startCS, endCS;
    cout << "\nEnter ID of starting CS (source): ";
    startCS = GetCorrectNumber<int>(0, INT_MAX);

    if (!stations.count(startCS)) {
        cout << "CS with ID " << startCS << " not found.\n\n";
        return;
    }

    cout << "Enter ID of ending CS (destination): ";
    endCS = GetCorrectNumber<int>(0, INT_MAX);

    if (!stations.count(endCS)) {
        cout << "CS with ID " << endCS << " not found.\n\n";
        return;
    }

    if (startCS == endCS) {
        cout << "Cannot connect CS to itself.\n\n";
        return;
    }

    cout << "\nAllowed diameters: ";
    for (int diam : ALLOWED_DIAMETERS) {
        cout << diam << " ";
    }
    cout << "mm\n";

    cout << "Enter required pipe diameter: ";
    int diameter = GetCorrectNumber<int>(0, 2000);

    bool validDiameter = false;
    for (int allowed : ALLOWED_DIAMETERS) {
        if (diameter == allowed) {
            validDiameter = true;
            break;
        }
    }

    if (!validDiameter) {
        cout << "Invalid diameter. Please use one of the allowed values.\n\n";
        return;
    }

    int pipeId = findFreePipeByDiameter(diameter);

    if (pipeId == -1) {
        cout << "No free pipe with diameter " << diameter << " mm found.\n";
        cout << "You need to create a new pipe with this exact diameter.\n\n";

        cout << "Would you like to create a new pipe? (1 - yes, 0 - no): ";
        int createNew = GetCorrectNumber<int>(0, 1);

        if (createNew == 1) {
            Pipe newPipe;

            cout << "\nCreating new pipe for connection:\n";
            cout << "Important: Pipe diameter must be exactly " << diameter << " mm\n";
            cout << "           Pipe status must be 'Working' (1)\n\n";

            cin >> newPipe;

            if (newPipe.getDiameter() != diameter) {
                cout << "Error: Pipe diameter is " << newPipe.getDiameter()
                    << " mm, but required " << diameter << " mm.\n";
                cout << "Connection cancelled.\n\n";
                Pipe::decrementNextId();
                return;
            }

            if (!newPipe.getFixing()) {
                cout << "Error: Pipe is not in working condition.\n";
                cout << "Connection cancelled.\n\n";
                Pipe::decrementNextId();
                return;
            }

            pipes.emplace(newPipe.getId(), newPipe);
            pipeId = newPipe.getId();
            cout << "New pipe created with ID: " << pipeId << "\n";
        }
        else {
            cout << "Connection cancelled.\n\n";
            return;
        }
    }

    Connection conn(startCS, endCS, pipeId);
    connections.push_back(conn);

    pipes[pipeId].setInUse(true);

    cout << "\nConnection created successfully!\n";
    cout << "Connection ID: " << conn.getId() << "\n";
    cout << "From CS " << startCS << " to CS " << endCS << "\n";
    cout << "Using pipe ID " << pipeId << " (Diameter: " << diameter << " mm)\n\n";
}

bool PipelineSystem::removeConnection(int startCS, int endCS) {
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        if (it->getStartCS() == startCS && it->getEndCS() == endCS) {
            int pipeId = it->getPipeId();

            if (pipes.count(pipeId)) {
                pipes[pipeId].setInUse(false);
            }

            connections.erase(it);
            return true;
        }
    }
    return false;
}

void PipelineSystem::removeAllConnectionsWithCS(int csId) {
    connections.erase(
        std::remove_if(connections.begin(), connections.end(),
            [csId](const Connection& conn) {
                return conn.getStartCS() == csId || conn.getEndCS() == csId;
            }),
        connections.end()
    );

    for (auto& pipe_pair : pipes) {
        if (pipe_pair.second.getInUse()) {
            bool stillUsed = false;
            for (const auto& conn : connections) {
                if (conn.getPipeId() == pipe_pair.first) {
                    stillUsed = true;
                    break;
                }
            }
            if (!stillUsed) {
                pipe_pair.second.setInUse(false);
            }
        }
    }
}

void PipelineSystem::removeConnectionsByPipe(int pipeId) {
    connections.erase(
        std::remove_if(connections.begin(), connections.end(),
            [pipeId](const Connection& conn) {
                return conn.getPipeId() == pipeId;
            }),
        connections.end()
    );

    if (pipes.count(pipeId)) {
        pipes[pipeId].setInUse(false);
    }
}

void PipelineSystem::disconnectCS() {
    if (stations.empty()) {
        cout << "No compressor stations available.\n\n";
        return;
    }

    cout << "Available compressor stations:\n";
    for (const auto& cs_pair : stations) {
        cout << "ID: " << cs_pair.first << " - " << cs_pair.second.getName() << endl;
    }

    cout << "\nEnter CS ID to disconnect: ";
    int csId = GetCorrectNumber<int>(0, INT_MAX);

    if (!stations.count(csId)) {
        cout << "CS with ID " << csId << " not found.\n\n";
        return;
    }

    bool hasConnections = false;
    for (const auto& conn : connections) {
        if (conn.getStartCS() == csId || conn.getEndCS() == csId) {
            hasConnections = true;
            break;
        }
    }

    if (!hasConnections) {
        cout << "CS " << csId << " has no connections.\n\n";
        return;
    }

    cout << "Disconnect all connections with CS " << csId << "? (1 - yes, 0 - no): ";
    int confirm = GetCorrectNumber<int>(0, 1);

    if (confirm == 1) {
        removeAllConnectionsWithCS(csId);
        cout << "All connections with CS " << csId << " have been removed.\n\n";
    }
    else {
        cout << "Operation cancelled.\n\n";
    }
}

void PipelineSystem::disconnectConnection() {
    if (connections.empty()) {
        cout << "No connections available.\n\n";
        return;
    }

    cout << "Current connections:\n";
    for (const auto& conn : connections) {
        cout << "Connection: CS " << conn.getStartCS() << " -> CS " << conn.getEndCS()
            << " (Pipe ID: " << conn.getPipeId() << ")\n";
    }

    cout << "\nEnter start CS ID: ";
    int startCS = GetCorrectNumber<int>(0, INT_MAX);
    cout << "Enter end CS ID: ";
    int endCS = GetCorrectNumber<int>(0, INT_MAX);

    if (removeConnection(startCS, endCS)) {
        cout << "Connection CS " << startCS << " -> CS " << endCS
            << " has been removed.\n\n";
    }
    else {
        cout << "Connection not found.\n\n";
    }
}

void PipelineSystem::disconnectByPipe() {
    if (pipes.empty()) {
        cout << "No pipes available.\n\n";
        return;
    }

    cout << "Available pipes:\n";
    for (const auto& pipe_pair : pipes) {
        if (pipe_pair.second.getInUse()) {
            cout << "ID: " << pipe_pair.first << " - " << pipe_pair.second.getName()
                << " (Diameter: " << pipe_pair.second.getDiameter() << " mm, In use: Yes)\n";
        }
    }

    cout << "\nEnter pipe ID to disconnect: ";
    int pipeId = GetCorrectNumber<int>(0, INT_MAX);

    if (!pipes.count(pipeId)) {
        cout << "Pipe with ID " << pipeId << " not found.\n\n";
        return;
    }

    if (!pipes[pipeId].getInUse()) {
        cout << "Pipe ID " << pipeId << " is not in use.\n\n";
        return;
    }

    int count = 0;
    for (const auto& conn : connections) {
        if (conn.getPipeId() == pipeId) {
            count++;
        }
    }

    if (count == 0) {
        cout << "No connections found using pipe ID " << pipeId << ".\n\n";
        return;
    }

    cout << "Found " << count << " connection(s) using pipe ID " << pipeId << ".\n";
    cout << "Disconnect all? (1 - yes, 0 - no): ";
    int confirm = GetCorrectNumber<int>(0, 1);

    if (confirm == 1) {
        removeConnectionsByPipe(pipeId);
        cout << "All connections using pipe ID " << pipeId << " have been removed.\n\n";
    }
    else {
        cout << "Operation cancelled.\n\n";
    }
}   

void PipelineSystem::viewNetwork() {
    if (connections.empty()) {
        cout << "No connections in the gas network.\n\n";
        return;
    }

    cout << "Gas Transportation Network\n\n";

    cout << "Connections:\n";
    for (const auto& conn : connections) {
        cout << "CS " << conn.getStartCS() << " -> CS " << conn.getEndCS()
            << " (Pipe ID: " << conn.getPipeId();

        int pipeId = conn.getPipeId();
        if (pipes.count(pipeId)) {
            cout << ", Diameter: " << pipes.at(pipeId).getDiameter() << " mm";
        }
        cout << ")\n";
    }

    auto adjList = buildAdjacencyList();

    cout << "\nAdjacency list:\n";
    if (adjList.empty()) {
        cout << "No valid connections (some CS might be deleted)\n";
    }
    else {
        for (const auto& adj_pair : adjList) {
            cout << "CS " << adj_pair.first << " -> ";
            for (int neighbor : adj_pair.second) {
                cout << "CS " << neighbor << " ";
            }
            cout << endl;
        }
    }

    cout << "\nNetwork statistics:\n";
    cout << "Total connections: " << connections.size() << "\n";
    cout << "Active CS in network: " << adjList.size() << "\n";

    if (hasCycle(adjList)) {
        cout << "Warning: Network contains cycles\n";
    }
    else {
        cout << "Network is a Directed Acyclic Graph\n";
    }
    cout << endl;
}
bool PipelineSystem::hasCycleUtil(int v, unordered_map<int, bool>& visited,
    unordered_map<int, bool>& recStack,
    const unordered_map<int, vector<int>>& adjList) const {
    if (!visited[v]) {
        visited[v] = true;
        recStack[v] = true;

        auto it = adjList.find(v);
        if (it != adjList.end()) {
            for (int neighbor : it->second) {
                if (!visited[neighbor] && hasCycleUtil(neighbor, visited, recStack, adjList)) {
                    return true;
                }
                else if (recStack[neighbor]) {
                    return true;
                }
            }
        }
    }
    recStack[v] = false;
    return false;
}

bool PipelineSystem::hasCycle(const unordered_map<int, vector<int>>& adjList) const {
    if (adjList.empty()) return false;

    unordered_map<int, bool> visited;
    unordered_map<int, bool> recStack;

    for (const auto& adj_pair : adjList) {
        visited[adj_pair.first] = false;
        recStack[adj_pair.first] = false;
    }

    for (const auto& adj_pair : adjList) {
        int v = adj_pair.first;
        if (!visited[v] && hasCycleUtil(v, visited, recStack, adjList)) {
            return true;
        }
    }

    return false;
}


void PipelineSystem::topologicalSortUtil(int v, unordered_map<int, bool>& visited,
    list<int>& stack,
    const unordered_map<int, vector<int>>& adjList) const {
    visited[v] = true;

    auto it = adjList.find(v);
    if (it != adjList.end()) {
        for (int neighbor : it->second) {
            if (!visited[neighbor]) {
                topologicalSortUtil(neighbor, visited, stack, adjList);
            }
        }
    }

    stack.push_front(v);
}

void PipelineSystem::topologicalSort() {
    auto adjList = buildAdjacencyList();

    if (adjList.empty()) {
        cout << "Network is empty.\n\n";
        return;
    }

    if (hasCycle(adjList)) {
        cout << "Cannot perform topological sort: network contains cycles!\n";
        cout << "Topological sort is only possible for Directed Acyclic Graphs.\n\n";
        return;
    }

    unordered_map<int, bool> visited;
    list<int> stack;
    for (const auto& adj_pair : adjList) {
        visited[adj_pair.first] = false;
    }

    for (const auto& adj_pair : adjList) {
        int v = adj_pair.first;
        if (!visited[v]) {
            topologicalSortUtil(v, visited, stack, adjList);
        }
    }

    cout << "Topological Sort Order (processing order):\n\n";

    int order = 1;
    for (int csId : stack) {
        cout << order << ". CS " << csId;
        if (stations.count(csId)) {
            cout << " - " << stations.at(csId).getName();
        }
        cout << endl;
        order++;
    }

    cout << "\nInterpretation:\n";
    cout << "- Stations should be processed in this order\n";
    cout << "- Each station depends only on stations before it\n";
    cout << "- No station depends on stations after it\n\n";
}

void PipelineSystem::saveToFile(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file << pipes.size() << "\n";
        for (const auto& pipe_pair : pipes) {
            const Pipe& pipe = pipe_pair.second;
            file << pipe.getId() << "\n" << pipe.getName() << "\n"
                << pipe.getLength() << "\n" << pipe.getDiameter() << "\n"
                << pipe.getFixing() << "\n" << pipe.getInUse() << "\n";
        }

        file << stations.size() << "\n";
        for (const auto& cs_pair : stations) {
            const CS& cs = cs_pair.second;
            file << cs.getId() << "\n" << cs.getName() << "\n"
                << cs.getWorkshop() << "\n" << cs.getActiveWorkshop() << "\n"
                << cs.getScore() << "\n";
        }

        file << connections.size() << "\n";
        for (const auto& conn : connections) {
            file << conn.getStartCS() << "\n"
                << conn.getEndCS() << "\n"
                << conn.getPipeId() << "\n";
        }

        file.close();
        cout << "Data saved to " << filename << " successfully!\n\n";
    }
    else {
        cout << "Error opening file for writing.\n\n";
    }
}

void PipelineSystem::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        pipes.clear();
        stations.clear();
        connections.clear();

        int pipeCount;
        file >> pipeCount;
        for (int i = 0; i < pipeCount; i++) {
            Pipe pipe;
            loadPipeFromFile(file, pipe);

            bool inUse;
            file >> inUse;
            pipe.setInUse(inUse);

            pipes.emplace(pipe.getId(), pipe);
        }

        int csCount;
        file >> csCount;
        for (int i = 0; i < csCount; i++) {
            CS cs;
            loadCSFromFile(file, cs);
            stations.emplace(cs.getId(), cs);
        }

        int connCount;
        file >> connCount;
        for (int i = 0; i < connCount; i++) {
            int startCS, endCS, pipeId;
            file >> startCS >> endCS >> pipeId;

            Connection conn(startCS, endCS, pipeId);
            connections.push_back(conn);

            if (pipes.count(pipeId)) {
                pipes[pipeId].setInUse(true);
            }
        }

        file.close();
        cout << "Data loaded from " << filename << " successfully!\n\n";
    }
    else {
        cout << "Error opening file for reading.\n\n";
    }
}