//
//  XMLDisplayer.h
//  Lab
//
//  Created by yangruiguo on 2021/8/30.
//  Copyright © 2021 yangruiguo. All rights reserved.
//

#ifndef XMLDisplayer_h
#define XMLDisplayer_h

#include "XML.h"

class XMLDisplayer {
private:
    string getShift(int shift);
    void displayXMLNode(int shift, XMLNode& node);
public:
    void displayXMLDocument(XMLDocument& document);
};


string XMLDisplayer::getShift(int shift) {
    string str;
    for(int i = 0; i < shift; ++i) {
        str.append(" ");
    }
    return str;
}

void XMLDisplayer::displayXMLNode(int shift, XMLNode& node) {
    string xmlNode;
    xmlNode.append(getShift(shift));
    xmlNode.append("name=");
    xmlNode.append(node.name);
    xmlNode.append(";value=");
    xmlNode.append(node.value);
    cout << xmlNode << endl;

    cout << getShift(shift) << "XML Attributes:" << endl;
    for(XMLAttribute attr: node.attributes) {
        string xmlAttr;
        xmlAttr.append(getShift(shift + 1));
        xmlAttr.append("name=");
        xmlAttr.append(attr.name);
        xmlAttr.append(";value=");
        xmlAttr.append(attr.value);
        cout << xmlAttr << endl;
    }

    for(XMLNode subNode: node.nodes) {
        displayXMLNode(shift + 4, subNode);
    }
}

void XMLDisplayer::displayXMLDocument(XMLDocument& document) {
    for(XMLNode node: document.nodes) {
        displayXMLNode(0, node);
    }
}

#endif /* XMLDisplayer_h */
