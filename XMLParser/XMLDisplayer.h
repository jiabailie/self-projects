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
    for(int i = 0; i < node.attributes.size(); ++i) {
        string xmlAttr;
        xmlAttr.append(getShift(shift + 1));
        xmlAttr.append("name=");
        xmlAttr.append(node.attributes[i].name);
        xmlAttr.append(";value=");
        xmlAttr.append(node.attributes[i].value);
        cout << xmlAttr << endl;
    }

    for(int i = 0; i < node.nodes.size(); ++i) {
        displayXMLNode(shift + 4, node.nodes[i]);
    }
}

void XMLDisplayer::displayXMLDocument(XMLDocument& document) {
    for(int i = 0; i < document.nodes.size(); ++i) {
        displayXMLNode(0, document.nodes[i]);
    }
}

#endif /* XMLDisplayer_h */
