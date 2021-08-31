//
//  XML.h
//  Lab
//
//  Created by yangruiguo on 2021/8/30.
//  Copyright © 2021 yangruiguo. All rights reserved.
//

#ifndef XML_h
#define XML_h

#include <cstdio>
#include <vector>
#include <cstring>
#include <iostream>
#include <map>
#include <climits>
#include <queue>
#include <regex>

using namespace std;

struct XMLAttribute {
    string name;
    string value;
};

struct XMLNode {
    string name;
    string value;
    vector<XMLNode> nodes;
    vector<XMLAttribute> attributes;
};

struct XMLDocument {
    vector<XMLNode> nodes;
};

#endif /* XML_h */
