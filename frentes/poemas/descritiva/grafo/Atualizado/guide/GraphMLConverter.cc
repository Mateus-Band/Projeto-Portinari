
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ostream.h>
#include <fstream.h>


#include <xercesc/dom/DOM.hpp>

#include "GraphML.h"
#include "xgmlnames.h"
#include "Point.h"
#include "GraphMLConverter.h"

//======================================
// procedures accessed by the c-module
//======================================

extern "C" Graph* readGraphMLFile(const char * filename)
{
  try
    {
      GraphMLConverter* pGMLConv = new GraphMLConverter();
      pGMLConv->parse(filename);
      pGraphMLConverter = (void *)pGMLConv;
      return pGMLConv->getGraph();
    }
  catch(GraphMLException &e)
    {
      cout << "Exception GraphMLException. \n"
	   << "Description: " << e.getDescription().transcode()
	   << "\n SystemId: " << e.getSystemId().transcode() 
	   << "\n Message: " << e.getMessage().transcode() 
	   << "\n Line: " << e.getLine()
	   << "\n Column: " << e.getColumn() << endl;
      if(e.isParseException())
        cout << "ParseException" << endl;
      else 
        cout << "Non-ParseException" << endl;
      return 0;
    }
}

extern "C" void writeGraphMLFile(Graph *pGraph,
    int numComponents, int dimension, const char * filename)
{
  try
    {
    	GraphMLConverter* pGMLConv;
    	if(pGraphMLConverter == NULL)
    	{
    		pGMLConv= new GraphMLConverter();
    		pGMLConv->setGraph(pGraph,numComponents,dimension);
			}
      else
      	pGMLConv =(GraphMLConverter*)pGraphMLConverter;
      pGMLConv->setVerticesCoordinates(dimension);
			pGMLConv->saveDoc(filename);
    }
  catch(GraphMLException &e)
    {
      cout << "Exception GraphMLException. \n"
	   << "Description: " << e.getDescription().transcode()
	   << "\n SystemId: " << e.getSystemId().transcode() 
	   << "\n Message: " << e.getMessage().transcode() 
	   << "\n Line: " << e.getLine()
	   << "\n Column: " << e.getColumn() << endl;
      if(e.isParseException())
        cout << "ParseException" << endl;
      else 
      	cout << "Non-ParseException" << endl;
    }
}


extern "C" void getVertexCoord(size_tt num, float *coord, int component);

//======================================
//GraphMLConverter
//======================================
GraphMLConverter::GraphMLConverter(): m_domAdmin(), m_gDoc(),m_graphml()
{
}

GraphMLConverter::GraphMLConverter(const char *filename): m_domAdmin(), m_gDoc(),m_graphml()
{
  parse(filename);
}
GraphMLConverter::~GraphMLConverter()
{
// the memory of m_pGraph will be freed elsewhere
}

void GraphMLConverter::setGraph(Graph* pGraph, int numComponents,int dimension)
{
  m_pGraph=pGraph;
  //create a dom document
  DOM_DOMImplementation impl;
  DOM_DocumentType docType = impl.createDocumentType(m_graphml.graphmlQN(), 0, "");
  DOM_Document domDoc = impl.createDocument(m_graphml.nsUri(), m_graphml.graphmlQN(), docType);
  m_gDoc.setDocument(domDoc);
  m_graphml.setNSPrefix("");
  DOM_Element g = domDoc.getDocumentElement();
  //========================================
  //set the default point
  DOM_Element key;
  for(int i = 1; i<=dimension; i++)
  {
    key = createKey(m_graphml.key_coord_id(i), m_graphml.nodeLN(),
                        m_graphml.key_coord_name(i), m_graphml.key_coord_type());
    g.appendChild(key);
  }
  DOM_Element graphElem = domDoc.createElementNS(m_graphml.nsUri(), m_graphml.graphQN());
  graphElem.setAttribute(m_graphml.graph_id(), m_graphml.graph_id_value());
  graphElem.setAttribute(m_graphml.graph_edgedef(), m_graphml.graph_edgedefault_undirected());
  g.appendChild(graphElem);

  size_tt vertNumOffset = 0; // used to distinct the vertex-id between different
                              // components of the graph
  // create vertex-elements
	for(int comp = 0; comp < numComponents; comp++)
	{
		for(size_tt vert=0; vert < pGraph[comp].numOfVert; vert++)
			graphElem.appendChild(createVertexElement(vert+vertNumOffset));
    vertNumOffset+=pGraph[comp].numOfVert;
	}
  vertNumOffset=0;
  // create edge-elements
  for(int comp = 0; comp < numComponents; comp++)
	{
 		for(size_tt i = 0; i <  pGraph[comp].numOfVert; i++)
			for(size_tt j = 0; j < pGraph[comp].adjList[0][i]; j++)
			{
			size_tt target,source;
				target = pGraph[comp].adjList[i+1][j]+vertNumOffset;
				source =  i+vertNumOffset;
				if(target > source) // no double edges
					graphElem.appendChild(createEdgeElement( source,target));
			}
    vertNumOffset+=pGraph[comp].numOfVert;
  }

}

void GraphMLConverter::saveDoc(const char * filename)
{
ofstream out(filename);
DOM_Element root = m_gDoc.getDocument().getDocumentElement();
  root.setAttribute(m_graphml.xml_xmlns_name(),m_graphml.xml_xmlns_value());
  root.setAttribute(m_graphml.xml_xsi_name(),m_graphml.xml_xsi_value());
  root.setAttribute(m_graphml.xml_xlink_name(),m_graphml.xml_xlink_value());
  root.setAttribute(m_graphml.xml_schemaLocation_name(),m_graphml.xml_schemaLocation_value());

  m_gDoc.print(out, m_graphml.emptystring());
}

void GraphMLConverter::parse(const char *filename){
  //use the GraphMLDocument class to read the document,
  //check the syntax and the semantic constraints
  m_gDoc.read(filename);

  DOM_Document domDoc = m_gDoc.getDocument();

  //get the root element
  DOM_Element g = domDoc.getDocumentElement();

	//find out the namespace prefix for the GD module
  DOM_NodeList gdElements = g.getElementsByTagNameNS(m_graphml.nsUri(), "*");
  if(gdElements.getLength() > 0)
  {
  	DOMString str(gdElements.item(0).getPrefix());
  	str.println();
     m_graphml.setNSPrefix(str);
  }//otherwise there is no gd element

  //parsing the keys will set the default weight for edges
  parseKeys(g); 

  //get the childnodes of graphml and parse the graphs
  //only top level graphs will be parsed (and put into the gw-graph)
  DOM_NodeList children = g.getChildNodes();
  unsigned int i = 0; 
  DOM_Element gr; 
  if(getNextNode(gr,i,m_graphml.graphLN(),children))
 		parseGraph(gr);
	else
	{
  GraphMLException e(GraphMLException::NO_GRAPH_IN_FILE);
    throw e;
	}
	if(getNextNode(gr,i,m_graphml.graphLN(),children))
		cout << "several graphs found in file - not supported, only the first one will be handled.";
	//TODO: handle several graphs - but this is not an important feature
}

bool GraphMLConverter::getNextNode(DOM_Element &elem,unsigned int &i,const DOMString &type,const DOM_NodeList &nl)
{
   for(unsigned int j = i; j < nl.getLength(); j++)
     { 
      DOM_Node n = nl.item(j);
    //test whether it's a node of the right type
      if(n.getNodeType() ==DOM_Node::ELEMENT_NODE && n.getLocalName().equals(type))
	 		{
	   		elem=(DOM_Element &)n;
	   		i=j+1;
	   		return true;
	 		}
   }
   return false;
}

void GraphMLConverter::setVerticesCoordinates(int dim)
{
// search for vertices-entries in the DOM-tree: document->graphml->graph->node=vertex
DOM_Document domDoc = m_gDoc.getDocument();
DOM_Element elem = domDoc.getDocumentElement();
DOM_NodeList children = elem.getChildNodes();
unsigned int i = 0;
float coord[4];
  getNextNode(elem,i,m_graphml.graphLN(),children);
	children=elem.getChildNodes();
	i=0;
	size_tt j = 0;
  while(getNextNode(elem,i,m_graphml.nodeLN(),children))
	{
    if(nodeMap != NULL)  //graph splits in several components
      getVertexCoord(nodeMap[j].vert,coord,nodeMap[j].comp); // gets the coordinates out of the c-module
    else
       getVertexCoord(j,coord,0); 
		setVertexCoord(elem,dim,coord);  // puts it in DOM-Tree
    j++;
	}
}

void GraphMLConverter::setVertexCoord(DOM_Element &vert,int dim, float* coord)
{
// search for entries for coordinates, if exists, overwrite, else create
// entry in the DOM-tree: vertex->data->...
// IMPORTANT: the vertices in the DOMTree are in the same order
// as they are passed through here
  DOM_NodeList children = vert.getChildNodes();
  unsigned int i = 0;
  DOM_Element data,oldData;
  DOM_Text coorChar;
  DOMString coorStr;
  bool dataNodeFound;
  for(int j=1; j<= dim; j++)
  {
    dataNodeFound = getNextNode(oldData,i,m_graphml.dataLN(),children);
    while(dataNodeFound &&
        (!oldData.getAttribute(m_graphml.data_key()).equals(m_graphml.key_coord_id(j))))
    {
      dataNodeFound = getNextNode(oldData,i,m_graphml.dataLN(),children);
    }
    if(j <= dim)
    {
      data=createData(m_graphml.key_coord_id(j));
      coorStr = m_domAdmin.double2string(coord[j-1]);
      coorChar = createText(coorStr);
      data.appendChild(coorChar);
      if(dataNodeFound)
        vert.replaceChild(data,oldData);
      else
        vert.appendChild(data);
    }
    else if(dataNodeFound)
      vert.removeChild(oldData);
  }
}

DOM_Element GraphMLConverter::createData(const DOMString &key)
{
DOM_Document domDoc= m_gDoc.getDocument();
DOM_Element data = domDoc.createElementNS(m_graphml.nsUri(), m_graphml.dataQN());
	data.setAttribute(m_graphml.data_key(), key);
  return data;
}

DOM_Element GraphMLConverter::createKey(const DOMString &id, const DOMString &forVal,
   const DOMString &name, const DOMString &type)
{
    DOM_Element key = m_gDoc.getDocument().createElementNS(m_graphml.nsUri(), m_graphml.keyQN());

    key.setAttribute(m_graphml.key_id(), id);
    key.setAttribute(m_graphml.key_for(), forVal);
    key.setAttribute(m_graphml.key_attr_name(), name);
    key.setAttribute(m_graphml.key_attr_type(), type);

    return key;
  }

DOM_Text GraphMLConverter::createText(DOMString data)
{
    return m_gDoc.getDocument().createTextNode(data);
}

DOM_Element GraphMLConverter::createVertexElement(size_tt num)
{
DOM_Element vertElem = m_gDoc.getDocument().createElementNS(m_graphml.nsUri(), m_graphml.nodeQN());

DOMString id(m_graphml.node_id_prefix());
    id += m_domAdmin.int2string(num);
    vertElem.setAttribute(m_graphml.node_id(), id);
    //any coordinates of the vertex will be ste elsewhere
		return vertElem;
}

DOM_Element GraphMLConverter::createEdgeElement(size_tt source, size_tt target)
{
	DOM_Element edgeElem = m_gDoc.getDocument().createElementNS(m_graphml.nsUri(), m_graphml.edgeQN());

  //set the source
  DOMString att(m_graphml.node_id_prefix());
  att += m_domAdmin.int2string(source);
  edgeElem.setAttribute(m_graphml.edge_source(), att);

  //set the target
  att = DOMString(m_graphml.node_id_prefix());
  att += m_domAdmin.int2string(target);
  edgeElem.setAttribute(m_graphml.edge_target(), att);

  return edgeElem;
}//end createEdgeElement

void GraphMLConverter::parseGraph(DOM_Element &gr)
{
size_tt numOfVert=0;
size_tt ** adjList;
coord_t ** weightList = 0;
size_tt *degree;
unsigned int i;
DOM_Element nodeElem;
bool isDirected;
StrMap nodeById; //save a mapping from the node-id to the (guide) node
DOM_NodeList nl = gr.getChildNodes(); //get the children of this graph

    m_pGraph = (Graph *)malloc(sizeof(Graph));

//look for the edgedefault attribute - it says whether the graph is directed or not
    if(gr.getAttribute(m_graphml.graph_edgedef()).equals(
                                           m_graphml.graph_edgedefault_directed()))
    {
      cout << "graph is directed and will be converted.\n" <<
        "WARNING: directed property will be lost when writing!\n";
      isDirected = true;
    }
    else
      isDirected = false;
      // TODO: save originally directed edges as directed edges - might be quite expensiv

//get the vertices
  i = 0;
  while(getNextNode(nodeElem,i,m_graphml.nodeLN(),nl))
  {
    //get information about node: perhaps needed to get coordinates in future extensions
    //  node v = parseNode(nodeElem);

    //insert the pair <vertex-id, vertex> into the dictionary
    nodeById.insert(make_pair(nodeElem.getAttribute(m_graphml.node_id()).transcode(),
                                                                            numOfVert));
    numOfVert++;
  }

//initialize the degree of each vertex
  degree = (size_tt *)malloc(numOfVert  * sizeof(size_tt));
  for(i=0;i<numOfVert;i++)
    degree[i]=0;

  adjList = (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
  adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));
  if(m_weighted != 0)
    weightList =  (coord_t **)malloc((numOfVert + 1) * sizeof(coord_t *));
  m_pGraph->adjList=adjList;
  m_pGraph->weightList=weightList;
  m_pGraph->numOfVert=numOfVert;

//get the edges and count the degree of each vertex (node)
  i = 0;
  DOM_Element edgeElem; 
  while(getNextNode(edgeElem,i,m_graphml.edgeLN(),nl))
  {
    int source = getSourceNode(edgeElem,nodeById);
    degree[source] ++;
    // in directed graphs we might count edges twice which occure in both directions
    // so we allocate too much memory --- who cares?

    //since the graph is undirected, we need to add an edge target->source
    int target = getTargetNode(edgeElem,nodeById);
    degree[target]++;
  }

//setting degrees for all vertices and allocating memory
  for(i = 0; i < numOfVert; i ++)
  {
    adjList[0][i]=0;      //this counter will be incremented every time an edge is added
    adjList[i + 1 ] = (size_tt *)malloc(degree[i] * sizeof(size_tt));
    if(m_weighted !=0)
      weightList[i+1] = (coord_t *)malloc(degree[i] * sizeof(coord_t));
  }

//Add edges into the adjacency and weight list
  i=0;
  while(getNextNode(edgeElem,i,m_graphml.edgeLN(),nl))
  {
  	int source = getSourceNode(edgeElem,nodeById);
    int target = getTargetNode(edgeElem,nodeById);
    coord_t weight;
    if(m_weighted != 0)
      parseEdge(edgeElem,weight); // get information about the edge

    //Ensure that a directed edge is not added twice and that self loops are not added
  	if (source == target)
			cout << "WARNING: loop found (vertice " << source
                                    << ") - edge ignored!\n";
	  else if (exists_edge(adjList,source,target) )
    {
       if(!isDirected)
  	  	cout << "WARNING: edge found twice (source " << target << ", target " << source
	       << " ) - edge ignored! (the graph is undirected)\n";
       else // this edge is ignored
       {    // nothing to do
       }
       assert(exists_edge(adjList,target,source));
		}
  	else
		{
  		//add new edge to adjacency list
	    int curr_degree = adjList[0][source];
	  	adjList[source + 1][curr_degree] = target;
      if(m_weighted!=0)
        weightList[source + 1][curr_degree] = weight;
	  	adjList[0][source] ++;
   	  //add the reverse edge to adjacency list
      curr_degree = adjList[0][target];
    	adjList[target+ 1][curr_degree] = source;
      if(m_weighted!=0)
         weightList[target+ 1][curr_degree] = weight;
     	adjList[0][target] ++;

// IMPORTANT: attributes of edges concerning direction are ignored
  	  DOMString dir = edgeElem.getAttribute(m_graphml.edge_dir());
  	  if(dir.length() > 0)
        cout << "direction-attribute of an edge found - attribute ignored\n";
		}
  }
// release memory
  free(degree);
#ifdef DEBUG
  debug("adjList: \n");
  for(int v=0; v< numOfNodes; v++)
  {
    debug4("%d:(adjList[0][%d]=%d) : ",v, v, adjList[0][v]);
    for(int j=0; j < adjList[0][v]; j++){
      debug2("%d ",adjList[v+1][j]);
    }
    debug("\n");
  }
#endif
}

//source and target getter method for an edge with a check whether they are in the same graph

int GraphMLConverter::getSourceNode(DOM_Element edgeElem, StrMap &nodeById)
{
  StrMap::const_iterator it = nodeById.find(edgeElem.getAttribute(m_graphml.edge_source()).transcode());
	if (it == nodeById.end())
	{
    GraphMLException e(GraphMLException::SOURCE_NOT_IN_GRAPH);

    DOMString msg("Source attribute value = ");
    msg = msg + edgeElem.getAttribute(m_graphml.edge_source());
    e.setMessage(msg);

    throw e;

  }
  else
    return it->second;
}

int GraphMLConverter::getTargetNode(DOM_Element edgeElem, StrMap &nodeById)
{
  StrMap::const_iterator it = nodeById.find(edgeElem.getAttribute(m_graphml.edge_target()).transcode());
  if(it == nodeById.end())
  {
    GraphMLException e(GraphMLException::TARGET_NOT_IN_GRAPH);

    DOMString msg("Target attribute value = ");
    msg = msg + edgeElem.getAttribute(m_graphml.edge_target());
    e.setMessage(msg);

    throw e;

  }
  else
    return it->second;
}

// Checks for existence of an edge in the adjacency list
int GraphMLConverter::exists_edge(size_tt **adjList, size_tt source, size_tt target)
{
  for (size_tt i = 0; i < adjList[0][source]; i ++)
    if(adjList[source+1][i] == target)
      return i + 1;
  return 0;
}

void GraphMLConverter::parseEdge(DOM_Element &edgeElem, coord_t &weight)
{
  //look for data elements
  DOM_NodeList nl = edgeElem.getChildNodes();
  unsigned int size = nl.getLength();
  weight=0;
  for(unsigned int i = 0; i < size; i++)
  {
    DOM_Node n = nl.item(i);

   //test whether it is data
    if(n.getNodeType() == DOM_Node::ELEMENT_NODE &&
                          n.getLocalName().equals(m_graphml.dataLN()))
    {
      DOM_Element data = (DOM_Element &) n;
      if(data.getAttribute(m_graphml.data_key()).equals(m_graphml.key_weight_id())
                                       && (weight == 0)) // no double initialization
        weight=parseWeight(data);
    }
  }
  if(weight==0)
    weight=m_weighted;
}

coord_t GraphMLConverter::parseWeight(DOM_Element &g)
{
DOM_NodeList nl = g.getChildNodes();
coord_t weight = 0;
unsigned int size = nl.getLength();
  for(unsigned int i= 0; i < size; i++)
   {
     DOM_Node nd = nl.item(0);
     if(nd.getNodeType() == DOM_Node::TEXT_NODE)
     {
       DOM_CharacterData &weightChrD = (DOM_CharacterData &) nd;
       DOMString weightDStr = weightChrD.getData();
       char * weightCStr =weightDStr.transcode();
       if(strchr(weightCStr,'.')==NULL)
         weight =m_domAdmin.string2int(weightCStr);
       else
         weight = m_domAdmin.string2double(weightCStr);
       break;
     }
   }
  if(weight==0.0)
    return m_weighted; // no initialization took place, so use standard value
  else
    return weight; // use given value
}

void GraphMLConverter::parseKeys(DOM_Element &g)
{
  //  DOM_NodeList keys = g.getElementsByTagNameNS(m_graphml.nsUri(), m_graphml.keyLN());
  DOM_NodeList keys = g.getElementsByTagName(m_graphml.keyLN());
  unsigned int size = keys.getLength();
  m_weighted = 0;
  for(unsigned int i = 0; i < size; ++i)
  {
    DOM_Node n = keys.item(i);
    DOM_Element key = (DOM_Element &) n;
    if(key.getAttribute(m_graphml.key_attr_name()).equals(m_graphml.key_weight_name()))
    {
      m_graphml.set_key_weight_id(key.getAttribute(m_graphml.key_id()));
      DOM_NodeList nl = key.getElementsByTagName(m_graphml.key_default());
      if(m_weighted == 0 && nl.getLength() > 0) // no double initialization
      {
      DOM_Node n = nl.item(0);
      DOM_Element weightStr = (DOM_Element&)n;
        m_weighted = 32; // if no standard is given in file, use 32
        m_weighted = parseWeight(weightStr);
      }
      //      g.removeChild(key); // uncomment, if weight should not be saved
    }
  }
  if(m_weighted == 0)
    cout << "graph is unweighted\n";
  else
    cout << "graph is weighted\n";
} 









