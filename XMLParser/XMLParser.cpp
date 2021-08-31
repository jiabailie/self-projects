//
//  XMLParser.cpp
//  Lab
//
//  Created by yangruiguo on 2021/8/19.
//  Copyright © 2021 yangruiguo. All rights reserved.
//
#include "XML.h"
#include "XMLParser.h"
#include "XMLDisplayer.h"

int main() {
    XMLParser parser;
    string strXml = "<person a1=\"2\" a2=\"2\"><name sex=\"male\">hello, world</name><age>12</age><hoppy action=\"coding\"/></person>";
    XMLDocument document = parser.parseXML(strXml);
    
    XMLDisplayer displayer;
    displayer.displayXMLDocument(document);
    
    return 0;
}
