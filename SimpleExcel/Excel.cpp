//
//  Excel.cpp
//
//  Created by yangruiguo on 2021/8/6.
//  Copyright © 2021 yangruiguo. All rights reserved.
//

// cell id [A-Z][0-9]*
// cell value 12, =A2, =A2+5, =A3+B3

#include <cstdio>
#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <set>
#include <queue>

using namespace std;

struct Cell {
    int value;
    string formula;
    vector<int> subValues;
    vector<string> subCellIds;
    Cell() {
        value = 0;
        subValues = vector<int>();
        subCellIds = vector<string>();
    }
};

class Excel {
private:
    map<string, Cell> data;
    
    // this map is used to save the reference information
    // such as if A3, B2 are all using V1, reference["V1"]={"A3", "B2"}
    map<string, set<string>> upperReferCellIds;

private:
    int calculeValue(string& cellId);
    Cell parseFormula(string& formula);
public:
    void insert(string cellId, string formula);
    void update(string cellId, string formula);
    int getValue(string cellId);
};

int Excel::calculeValue(string& cellId) {
    if(data.find(cellId) == data.end()) {
        throw "Encounter cell which hasn't initialized!";
    }
    
    int value = 0;
    for(int v: data[cellId].subValues) {
        value += v;
    }
    for(string id: data[cellId].subCellIds) {
        value += getValue(id);
    }
    return value;
}

Cell Excel::parseFormula(string& formula) {
    Cell cell;
    
    cell.formula = formula;
    
    // formula format {12, =A2, =A2+5, =A3+B3}
    // if formula[0] is not '=', which means it's one integer
    if(formula[0] != '=') {
        cell.value = stoi(formula);
        cell.subValues.push_back(cell.value);
        
        return cell;
    }
    
    string part;
    for(int i = 1; i < formula.size(); ++i) {
        if(formula[i] == '+') {
            if(part[0] >= '0' && part[0] <= '9') {
                cell.subValues.push_back(stoi(part));
            } else {
                cell.subCellIds.push_back(part);
            }
            part.clear();
        } else {
            part.push_back(formula[i]);
        }
    }
    
    if(part[0] >= '0' && part[0] <= '9') {
        cell.subValues.push_back(stoi(part));
    } else {
        cell.subCellIds.push_back(part);
    }
    
    for(int v: cell.subValues) {
        cell.value += v;
    }
    for(string id: cell.subCellIds) {
        cell.value += getValue(id);
    }
    
    return cell;
}

void Excel::insert(string cellId, string formula){
    // if already set value in this cell
    if(data.find(cellId) != data.end()) {
        throw "Encounter cell which has been initialized, please call update to change its value!";
    }
    
    Cell cell = parseFormula(formula);
    data[cellId] = cell;
    
    // update the reference data
    for(string id: cell.subCellIds) {
        if(upperReferCellIds.find(id) == upperReferCellIds.end()) {
            upperReferCellIds[id] = set<string>();
        }
        upperReferCellIds[id].insert(cellId);
    }
}

void Excel::update(string cellId, string formula){
    // if haven't set value for this cell
    if(data.find(cellId) == data.end()) {
        throw "Encounter cell which hasn't been initialized, please call insert at first!";
    }
    
    // remove the reference relationship at first
    for(string id: data[cellId].subCellIds) {
        upperReferCellIds[id].erase(cellId);
    }
    
    Cell cell = parseFormula(formula);
    
    // update the reference data
    for(string id: cell.subCellIds) {
        if(upperReferCellIds.find(id) == upperReferCellIds.end()) {
            upperReferCellIds[id] = set<string>();
        }
        upperReferCellIds[id].insert(cellId);
    }
    
    data[cellId] = cell;
    
    // update the upper reference cell
    queue<string> upperReference;
    for(string id: upperReferCellIds[cellId]) {
        upperReference.push(id);
    }
    
    while(!upperReference.empty()) {
        string id = upperReference.front();
        upperReference.pop();
        
        data[id].value = calculeValue(id);
        for(string subId: upperReferCellIds[id]) {
            upperReference.push(subId);
        }
    }
}

int Excel::getValue(string cellId) {
    if(data.find(cellId) == data.end()) {
        throw "Encounter cell which hasn't initialized!";
    }
    
    return data[cellId].value;
}

int main() {
    Excel excel;
    
    excel.insert("A1", "1");
    excel.insert("A2", "2");
    excel.insert("A3", "=3");
    excel.insert("A4", "=A1");
    excel.insert("A5", "=A1+2");
    excel.insert("A6", "=A1+A2");
    excel.insert("A7", "=A1+A4");
    
    cout << (excel.getValue("A1") == 1 ? "Passed" : "Failed") << endl;
    cout << (excel.getValue("A2") == 2 ? "Passed" : "Failed") << endl;
    cout << (excel.getValue("A3") == 3 ? "Passed" : "Failed") << endl;
    cout << (excel.getValue("A4") == 1 ? "Passed" : "Failed") << endl;
    cout << (excel.getValue("A5") == 3 ? "Passed" : "Failed") << endl;
    cout << (excel.getValue("A6") == 3 ? "Passed" : "Failed") << endl;
    cout << (excel.getValue("A7") == 2 ? "Passed" : "Failed") << endl;
    
    excel.update("A1", "2");
    cout << (excel.getValue("A1") == 2 ? "Passed" : "Failed") << endl;
    cout << (excel.getValue("A4") == 2 ? "Passed" : "Failed") << endl;
    cout << (excel.getValue("A5") == 4 ? "Passed" : "Failed") << endl;
    cout << (excel.getValue("A6") == 4 ? "Passed" : "Failed") << endl;
    cout << (excel.getValue("A7") == 4 ? "Passed" : "Failed") << endl;
    
    excel.update("A4", "=A2+A3");
    cout << (excel.getValue("A4") == 5 ? "Passed" : "Failed") << endl;
    cout << (excel.getValue("A7") == 7 ? "Passed" : "Failed") << endl;
    return 0;
}
