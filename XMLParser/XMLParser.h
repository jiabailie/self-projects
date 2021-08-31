//
//  XMLParser.h
//  Lab
//
//  Created by yangruiguo on 2021/8/30.
//  Copyright © 2021 yangruiguo. All rights reserved.
//

#ifndef XMLParser_h
#define XMLParser_h

#include "XML.h"

class XMLParser {
private:
    XMLDocument document;
private:
    // Split one string by specific splitor
    vector<string> splitStr(string& str, char splitor);
    
    // Find the index of correspond close xml node
    int findCloseIndex(int startIndex, XMLNode& headNode, vector<string>& xmlParts);
    
    // Parse xml attribute
    XMLAttribute parseAttribute(string& str);
    
    // Parse xml node, this type of node has no close node
    XMLNode parseNode(string& xmlPart);
    
    // Parse xml node, this type of node has start/close nodes
    bool parseNode(int start, int end, XMLNode& headNode, vector<string>& xmlParts);
    
    bool parseRawXml(string& strXml);

public:
    XMLDocument parseXML(string& strXml);
};

vector<string> XMLParser::splitStr(string& str, char splitor) {
    str.push_back(splitor);

    vector<string> parts;
    string part;
    for(int i = 0; i < str.size(); ++i) {
        if(str[i] == splitor) {
            if(part.size() > 0) {
                parts.push_back(part);
                part.clear();
            }
            continue;
        }
        part.push_back(str[i]);
    }
    return parts;
}

int XMLParser::findCloseIndex(int startIndex, XMLNode &headNode, vector<string> &xmlParts) {
    int closeIndex = -1;
    for(int i = startIndex + 1; i < xmlParts.size() && closeIndex == -1; ++i) {
        if(xmlParts[i][0] == '<' && xmlParts[i][1] == '/') {
            string tagName = xmlParts[i].substr(2);
            tagName.pop_back();

            if(tagName.compare(headNode.name) == 0) {
                closeIndex = i;
            }
        }
    }
    
    if(closeIndex == -1) {
        throw "INVALID XML: No corresponding close node!";
    }
    
    return closeIndex;
}

XMLAttribute XMLParser::parseAttribute(string& str) {
    XMLAttribute xmlAttr;
    for(int i = 0; i < str.size(); ++i) {
        if(str[i] == '=') {
            xmlAttr.value = str.substr(i + 1);
            break;;
        }
        xmlAttr.name.push_back(str[i]);
    }
    return xmlAttr;
}

XMLNode XMLParser::parseNode(string& xmlPart) {
    string copy(xmlPart);
    while(copy.back() == '>' || copy.back() == '/') {
        copy.pop_back();
    }
    copy = copy.substr(1);
    vector<string> parts = splitStr(copy, ' ');

    XMLNode node;
    node.name = parts[0];

    if(xmlPart.size() > 1) {
        for(int i = 1; i < parts.size(); ++i) {
            XMLAttribute attribute = parseAttribute(parts[i]);
            node.attributes.push_back(attribute);
        }
    }

    return node;
}

bool XMLParser::parseNode(int start, int end, XMLNode& headNode, vector<string>& xmlParts) {
    if(start + 1 == end) {
        return true;
    }

    if(xmlParts[start + 1][0] != '<') {
        headNode.value = xmlParts[start + 1];
        return true;
    }

    for(int i = start + 1; i < end; ++i) {
        string xmlPart = xmlParts[i];

        // single node
        if(xmlPart[xmlPart.size() - 2] == '/') {
            XMLNode node = parseNode(xmlPart);
            i += 1;
            headNode.nodes.push_back(node);
            continue;
        }

        XMLNode subHead = parseNode(xmlPart);
        int closeTagIndex = findCloseIndex(i + 1, subHead, xmlParts);
        parseNode(i, closeTagIndex, subHead, xmlParts);
        headNode.nodes.push_back(subHead);
        i = closeTagIndex;
    }

    return true;
}

// Parse raw xml string to xml document
bool XMLParser::parseRawXml(string& strXml) {
    if(strXml.size() == 0) {
        return false;
    }

    // strXML is one pure text
    if(strXml[0] != '<') {
        XMLNode node;
        node.value = strXml;
        document.nodes.push_back(node);

        return true;
    }

    // Split strXml to smaller pieces
    vector<string> parts;
    string part;
    for(int i = 0; i < strXml.size(); ++i) {
        if(strXml[i] == '<' && part.size() > 0) {
            parts.push_back(part);
            part.clear();
        }
        part.push_back(strXml[i]);
        if(strXml[i] == '>') {
            parts.push_back(part);
            part.clear();
        }
    }

    int cursor = 0;
    while(cursor != parts.size()) {
        string p = parts[cursor];

        // Encounter one single xml node
        // eg. <hoppy action="coding"/>
        if(p[p.size() - 2] == '/') {
            XMLNode node = parseNode(p);
            cursor += 1;
            document.nodes.push_back(node);
            continue;
        }

        XMLNode head = parseNode(p);

        // Find correspond close node
        int closeIndex = findCloseIndex(cursor + 1, head, parts);
        parseNode(cursor, closeIndex, head, parts);
        document.nodes.push_back(head);
        cursor = closeIndex + 1;
    }

    return true;
}

XMLDocument XMLParser::parseXML(string& str) {
    bool valid = parseRawXml(str);
    if(!valid) {
        throw "INVALID XML";
    }
    return document;
}

#endif /* XMLParser_h */
