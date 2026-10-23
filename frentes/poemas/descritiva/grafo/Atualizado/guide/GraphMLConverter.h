#ifndef GRAPHML_CONVERTER_H
#define GRAPHML_CONVERTER_H

#include <map>
#include <string>
#include <xercesc/dom/DOM.hpp>


#include "Graph.h"
#include "GraphML.h"
#include "DOMAdmin.h"

typedef map<string, size_tt> StrMap;

extern "C" NodeSplitMap *nodeMap;
extern "C" void * pGraphMLConverter;

class GraphMLConverter
{
 public:

    GraphMLConverter();

    //constructor; starts the parsing of file 'filename'
    GraphMLConverter(const char *filename);

    ~GraphMLConverter();

    //parses the file 'filename' and converts the xml
    //document into the guide graph 
    void parse(const char *filename);
		void saveDoc(const char *filename);
		void setVerticesCoordinates( int dim);

    
 private:
    //pointer to the graph
    Graph *m_pGraph;

 public:
    Graph *getGraph() { return m_pGraph; };
    void setGraph(Graph* pGraph, int numComponents,int dimension);

 private:
    bool getNextNode(DOM_Element &node,unsigned int &i,const DOMString &type,
                                                              const DOM_NodeList &nl);
		void setVertexCoord(DOM_Element &vert,int dim,float * coord);
    DOM_Element createData(const DOMString &key);
		DOM_Element createVertexElement(size_tt num);
    DOM_Element createEdgeElement(size_tt source, size_tt target);
		DOM_Element createKey(const DOMString &id, const DOMString &forVal,
                                        const DOMString &name, const DOMString &type);
    DOM_Text createText(DOMString data);



    //DOM-Initialisierung
    DOMAdmin m_domAdmin;
		//
    GraphMLDocument m_gDoc;

    //classes for the (extended) GraphML
    XGMLNames m_graphml;

    coord_t m_weighted; // 0 means graph is not weighted, a nonzero value represents the standard weight


    int getSourceNode(DOM_Element edgeElem, StrMap &nodeById);
    int getTargetNode(DOM_Element edgeElem, StrMap &nodeById);
    int exists_edge(size_tt **adjList, size_tt source, size_tt target);

    //methods used by parse
    void parseGraph(DOM_Element &gr);
/*      node parseNode(DOM_Element &nodeElem); */
    void parseEdge(DOM_Element &edgeElem, coord_t &weight);
    coord_t parseWeight(DOM_Element &g);
    void parseKeys(DOM_Element &g);

};

#endif




