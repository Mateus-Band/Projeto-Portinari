/**********************************************************
 * this code is an adaption of a programm of              *
 * Juergen Lerner (lerner@fmi.uni-konstanz.de)            *
 *        University of Konstanz, Germany                 *
 *                                                        *
 **********************************************************/
#ifndef GRAPHML_HEADER_GUARD
#define GRAPHML_HEADER_GUARD

#include <xercesc/dom/DOM.hpp>

#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include <iostream>
#include <ostream>

using namespace std;


//Acronyms: LN stands for local name
//          QN stands for qualified name
//          NS or ns stands for namespace

class GraphMLNames{
 private:
  //flag indicating whether the qualified names of the GraphMLNamespace
  //should contain a prefix
  bool prefixed;
  //the namespace name
  static const char *GRAPHML_URI("http://www.graphdrawing.org/xmlns/graphml");
  
  DOMString NS_PREFIX;

  //the local names of the GraphML elements
  static const char *GRAPHML_GRAPHML("graphml");
  static const char *GRAPHML_EXTERNAL("external");
  static const char *GRAPHML_DESC("desc");
  static const char *GRAPHML_GRAPH("graph");
  static const char *GRAPHML_NODE("node");
  static const char *GRAPHML_PORT("port");
  static const char *GRAPHML_EDGE("edge");
  static const char *GRAPHML_HYPEREDGE("hyperedge");
  static const char *GRAPHML_ENDPOINT("endpoint");
  static const char *GRAPHML_KEY("key");
  static const char *GRAPHML_DATA("data");

  //attributes of GraphML elements
  static const char *GRAPH_ID("id");
  static const char *GRAPH_EDGEDEFAULT("edgedefault");
  static const char *NODE_ID("id");
  static const char *PORT_NAME("name");
  static const char *DATA_KEY("key");
  static const char *DATA_ID("id");
  static const char *KEY_ID("id");
  static const char *KEY_FOR("for");
  static const char *EDGE_SOURCE("source");
  static const char *EDGE_TARGET("target");
  static const char *EDGE_ID("id");
  static const char *EDGE_SOURCEPORT("sourceport");
  static const char *EDGE_TARGETPORT("targetport");
  static const char *EDGE_DIR("directed");
  static const char *HYPEREDGE_ID("id");
  static const char *ENDPOINT_ID("id");
  static const char *ENDPOINT_NODE("node");
  static const char *ENDPOINT_PORT("port");
  static const char *ENDPOINT_TYPE("type");

  //values of selected attributes
  static const char *KEY_FOR_ALL("all");
  static const char *GRAPH_EDGEDEFAULT_DIRECTED("directed");
  static const char *GRAPH_EDGEDEFAULT_UNDIRECTED("undirected");
  static const char *EDGE_DIR_TRUE("true");
  static const char *EDGE_DIR_FALSE("false");
  static const char *NODE_ID_PREFIX("v");
  static const char *KEY_DEFAULT("default");

 public:
  GraphMLNames();
  //method used to set the namespace prefix
  void setNSPrefix(const DOMString &prefix);

  //getter methods for prefixed, the namespace name and prefix
  bool isPrefixed() const;
  DOMString nsPrefix();
  DOMString nsUri();

  //getter methods for the local element names
  const DOMString graphmlLN();
  const DOMString externalLN();
  const DOMString descLN();
  const DOMString graphLN();
  const DOMString nodeLN();
  const DOMString portLN();
  const DOMString edgeLN();
  const DOMString hyperedgeLN();
  const DOMString endpointLN();
  const DOMString keyLN();
  const DOMString dataLN();

  //getter methods for the element qualified names
  const DOMString graphmlQN();
  const DOMString externalQN();
  const DOMString descQN();
  const DOMString graphQN();
  const DOMString nodeQN();
  const DOMString portQN();
  const DOMString edgeQN();
  const DOMString hyperedgeQN();
  const DOMString endpointQN();
  const DOMString keyQN();
  const DOMString dataQN();

  //getter methods for the attributes
  const DOMString graph_id();
  const DOMString node_id();
  const DOMString port_name();
  const DOMString data_key();
  const DOMString data_id();
  const DOMString key_id();
  const DOMString key_for();
  const DOMString edge_source();
  const DOMString edge_target();
  const DOMString edge_id();
  const DOMString edge_sourceport();
  const DOMString edge_targetport();
  const DOMString edge_dir();
  const DOMString graph_edgedef();
  const DOMString hyperedge_id();
  const DOMString endpoint_id();
  const DOMString endpoint_node();
  const DOMString endpoint_port();
  const DOMString endpoint_type();

  //getter methods for attribute values
  const DOMString key_for_all();
  const DOMString graph_edgedefault_directed();
  const DOMString graph_edgedefault_undirected();
  const DOMString edge_dir_true();
  const DOMString edge_dir_false();
  const DOMString node_id_prefix();
  const DOMString key_default();
};


class GraphMLException
{
 public:
  //exception types

  //parse Exceptions
  static const unsigned int PARSE_ERROR = 0;
  static const unsigned int FATAL_PARSE_ERROR = 1;
  static const unsigned int PARSE_WARNING = 2;
  //violations of the semantic constraints
  static const unsigned int INVALID_NAMESPACE = 3;
  static const unsigned int INVALID_ROOTELEMENT = 4;
  static const unsigned int DUPLICATE_PORT_NAME = 5;
  static const unsigned int INVALID_KEY = 6;
  static const unsigned int INVALID_SOURCE = 7;
  static const unsigned int INVALID_TARGET = 8;
  static const unsigned int INVALID_NODE = 9;
  static const unsigned int INVALID_PORT = 10;
  static const unsigned int SOURCE_NOT_IN_GRAPH = 11;
  static const unsigned int TARGET_NOT_IN_GRAPH = 12;
  static const unsigned int NO_GRAPH_IN_FILE = 13;
  //Exception type description
  static const char *PARSE_ERROR_DESC("Error during parsing");
  static const char *FATAL_PARSE_ERROR_DESC("Fatal error during parsing");
  static const char *PARSE_WARNING_DESC("Warning during parsing");
  static const char *INVALID_NAMESPACE_DESC("The namespace of the root-element is unspecified or doesn't match the GraphML namespace");
  static const char *INVALID_ROOTELEMENT_DESC("The localname of the root-element is not 'graphml'");
  static const char *DUPLICATE_PORT_NAME_DESC("Duplicate port name within one node");
  static const char *INVALID_KEY_DESC("Attribute 'key' of <data> doesn't point to a key-id");
  static const char *INVALID_SOURCE_DESC("Attribute 'source' of <edge> doesn't point to a node-id");
  static const char *INVALID_TARGET_DESC("Attribute 'target' of <edge> doesn't point to a node-id");
  static const char *INVALID_NODE_DESC("Attribute 'node' of <endpoint> doesn't point to a node-id");
  static const char *INVALID_PORT_DESC("Attribute 'port' doesn't point to a port-name in the corresponding node");
  static const char *SOURCE_NOT_IN_GRAPH_DESC("The source-node of the current edge was not in the same graph.");
  static const char *TARGET_NOT_IN_GRAPH_DESC("The target-node of the current edge was not in the same graph.");
  static const char *NO_GRAPH_IN_FILE_DESC("No Graph found in the specified xml-file.");

  //empty constructor
  GraphMLException();
  //constructor setting the exception type and 
  //(if the exception is a parse exception) the line and column number
  GraphMLException(const unsigned int type, unsigned int line=0, unsigned int column=0);


  /**
   *Destructor
   */
  ~GraphMLException();

  //set the user message
  void setMessage(DOMString message);
  //the the system id of the document being parsed
  void setSystemId(DOMString uri);

  //getter methods for the information about the exception
  DOMString getMessage();
  DOMString getSystemId();
  DOMString getDescription();
  int getLine();
  int getColumn();
  bool isParseException();



  int line;
  int column;
  DOMString systemId;
  DOMString description;
  DOMString user_message;
  unsigned int type;
};



class SAXParseException;
class GraphMLErrorHandler : public ErrorHandler
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    GraphMLErrorHandler();
    ~GraphMLErrorHandler();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool getSawErrors() const;

    GraphMLException getException() const;

    // -----------------------------------------------------------------------
    //  Implementation of the SAX ErrorHandler interface
    // -----------------------------------------------------------------------
    void warning(const SAXParseException& e);
    void error(const SAXParseException& e);
    void fatalError(const SAXParseException& e);
    void resetErrors();


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    GraphMLErrorHandler(const GraphMLErrorHandler&);
    void operator=(const GraphMLErrorHandler&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fSawErrors
    //      This is set if we get any errors, and is queryable via a getter
    //      method. Its used by the main code to suppress output if there are
    //      errors.
    // -----------------------------------------------------------------------
    GraphMLException fEx;
    bool    fSawErrors;
};

inline bool GraphMLErrorHandler::getSawErrors() const
{
    return fSawErrors;
}

inline GraphMLException GraphMLErrorHandler::getException() const
{
    return fEx;
}

//====================================================
//headerfile for the GraphMLDocument class
//====================================================


class GraphMLDocument {

 public:


  /**
   *Empty constructor.
   */
  GraphMLDocument();

  /**
   *Constructor.
   *Parses the file 'filename' and checks the
   *semantic constraints 
   */
  GraphMLDocument(const char *filename);

  /**
   *Constructor. Sets the underlying DOM Document to doc
   *(without any checking whether it is valid).
   */
  GraphMLDocument(DOM_Document &doc);

  /**
   *destructor
   */
  ~GraphMLDocument();

  /**
   *returns the underlying DOM Document
   */
  DOM_Document& getDocument();

  /**
   *Sets the underlying DOM Document to doc
   *(without any checking whether it is valid).
   */
  void setDocument(DOM_Document& doc);

  /**
   *Returns the GraphMLNames class which can be used to get 
   *the local or qualified names of the GraphML elements,
   *their attributes, namespace name, prefix...
   */
  GraphMLNames& getNames();

  /**
   *Returns the System identifier for the dtd for this document
   */
//  DOMString getDTDSystemId();

//  void setDTDSystemId(const DOMString &dtd);

  /**
   *Sets the string used to indent lines
   *when writing the document into a file. 
   *(a sequence of whitespaces)
   */
  void setIndentation(const DOMString &indent);

  /**
   *Sets the prefix used for GraphML element names
   */
  void setPrefix(const DOMString &prefix);

  /**
   *Parses the file 'filename'.
   */
  void read(const char *filename);

  /**
   *Prints this document to 'target'.  
   */
  void print(ostream &target, const DOMString &internalSubset);

  /**
   *Prints this document to 'filename'.  
   */
  void print(const char *filename, const DOMString &internalSubset);

 private:
  //additional constraints currently unused
  bool noHierarchies;
  bool noHierarchiesInNodes;
  bool noHierarchiesInEdges;
  bool edgesWithinGraph;
  bool edgesWithinLevel;
  bool noHyperedges;
  bool noPorts;
  bool noHierarchicalPorts;
  bool noExternalReferences;

  /**
   *the underlying DOM_Document
   */
  DOM_Document fDoc;

  DOMString INDENTATION;

//  DOMString DTD;

  /**
   *the qualified and local names
   */
  GraphMLNames names;

  /**
   *Checks the semantic constraints
   */
  void processDocument();

  void checkUniquenessOfPorts(DOM_Element &root);

  void checkIDREFConstraints(DOM_Element &root);

  void isNodeAndHasPort(DOMString nodeId, DOMString portName, const unsigned int type);


  /**
   *Writes recursively node to target.
   *This method expects node being of type ELEMENT_NODE.
   *Only the nodes of type ELEMENT_NODE (and their attributes) will be
   *written recursively.
   */
  void writeDOMNode(ostream &target, DOM_Node &node, int treedepth);


  /**
   *Writes the attributes of elem (which must be a node of type
   *ELEMENT_NODE) like att1="value1" att2="value2" ...
   */
  void writeAttributes(ostream &target, DOM_Node &elem);
  /**
   *Used to indent the lines depending of treeDepth.
   */
  void writeIndent(ostream &target, unsigned int treeDepth);
  /**
   *Writes the prolog of the XML document depending on the 
   *DTD System Id, whether a prefix is used for GraphML Elements
   *and the parameter internalSubset
   */
  void writeProlog(ostream &target, const DOMString &internalSubset);

};

#endif
