/**********************************************************
 * this code is an adaption of a programm of              *
 * Juergen Lerner (lerner@fmi.uni-konstanz.de)            *
 *        University of Konstanz, Germany                 *
 *                                                        *
 **********************************************************/
#include "GraphML.h"

#include <xercesc/sax/SAXException.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/parsers/DOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <ostream.h>
#include <fstream.h>
#include <string>
#include <set>

//============================================
//Constructors
//============================================
GraphMLDocument::GraphMLDocument():  
  INDENTATION(" "){  
  DOM_DOMImplementation impl;
  DOM_DocumentType docType = impl.createDocumentType(names.graphmlQN(), 0, "");
  fDoc = impl.createDocument(names.nsUri(), names.graphmlQN(), docType);
}

GraphMLDocument::GraphMLDocument(const char *filename):  
  INDENTATION(" "){
  try{
    //parse the file
    read(filename);
  }
  catch(GraphMLException &e){
    e.setMessage(e.getMessage() + "\n Exception thrown during parsing. \n Syntactic error.");
    throw e;
  }
  try{
    //check the semantic constraints
    processDocument();
  }
  catch(GraphMLException &e){
    e.setMessage(e.getMessage() + "\n Exception thrown during processing. \n Semantic error.");
    throw e;
  }
}
GraphMLDocument::GraphMLDocument(DOM_Document &doc):  
  fDoc(doc), INDENTATION(" "){

}

//destructor 
GraphMLDocument::~GraphMLDocument(){ }


//============================================
//Getter methods
//============================================
DOM_Document& GraphMLDocument::getDocument(){
  return fDoc;
}
void GraphMLDocument::setDocument(DOM_Document& doc){
  fDoc = doc;
}
GraphMLNames& GraphMLDocument::getNames(){
  return names;
}
//============================================
//Parameter setting
//============================================
//void GraphMLDocument::setDTDSystemId(const DOMString &dtd){
//  DTD = dtd;
//}
//DOMString GraphMLDocument::getDTDSystemId(){
//  return DTD;
//}

void GraphMLDocument::setIndentation(const DOMString &indent){
  INDENTATION = indent;
}

void GraphMLDocument::setPrefix(const DOMString &prefix){
  names.setNSPrefix(prefix);
}

//============================================
//File reading
//============================================
void GraphMLDocument::read(const char *filename){
  // Instantiate the DOM parser.
  DOMParser parser;
  //we validate ...
  parser.setValidationScheme(DOMParser::Val_Always);
  //... according to the xml namespace specification
  parser.setDoNamespaces(true);
  // accept schema
  parser.setDoSchema(true);

  //install the error handler
  GraphMLErrorHandler handler;
  parser.setErrorHandler(&handler);
  //try the parsing
  try
    {
      cout << "parsing the file..." << endl;
      parser.parse(filename);
    }

  catch (const XMLException& toCatch)
    {
      cout << "\nError (xml_exception): during parsing'" << filename << "'\n"
	   << "Exception message is:  \n"
	   << DOMString(toCatch.getMessage()).transcode() << "\n" << endl;
    }
  catch (const DOM_DOMException& toCatch)
    {
      cout << "\nError (dom_exception) during parsing: '" << filename << "'\n"
	   << "Exception message is:  \n"
	   << toCatch.msg.transcode() << "\n" << endl;
    }
  catch (GraphMLException& toCatch)
    {
      cout << "\nError (GraphMLException) during parsing: '" << toCatch.getSystemId().transcode() << "'\n"
	   << "Exception message is:  \n"
	   << toCatch.getMessage().transcode() << "\n" << endl;
    }
  catch (...)
    {
      cerr << "\nUnexpected exception during parsing: '" << filename << "'\n";
    }
  //get the document
  if (handler.getSawErrors())
    {
      cout << "\nErrors occured, no output available\n" << endl;
      throw handler.getException();
    }
  else
    {
      cout << "parsing succeded. Got the document" << endl;
      fDoc = parser.getDocument();
    }
}



//============================================
//File writing
//============================================
void GraphMLDocument::print(const char *filename, const DOMString &internalSubset){
  ofstream out(filename);
  print(out, internalSubset);
}
void GraphMLDocument::print(ostream &target, const DOMString &internalSubset){
  cout << "writeToStream called" << endl;

  writeProlog(target, internalSubset);

  DOM_Element root = fDoc.getDocumentElement();
  root.normalize();
  writeDOMNode(target, root, 0);
}

//**********************************************
//implementations of the private methods
//**********************************************
//============================================
//Methods for reading, constraint checking 
//============================================
void GraphMLDocument::processDocument(){
  DOM_Element root = fDoc.getDocumentElement();
  //check the namespace
  DOMString str = root.getNamespaceURI();
  if(str == 0 || !str.equals(names.nsUri())){
    GraphMLException e(GraphMLException::INVALID_NAMESPACE);
    throw e;
  }
  //if the element has been created with a DOM Level 1 method
  //then there is no local name. (shouldn't happen)
  str = root.getLocalName();
  if(str == 0){
    cout << "no localname part of the root element." << endl;
  }
  else{
    //check the root elements name
    if(!str.equals(names.graphmlLN())){
      GraphMLException e(GraphMLException::INVALID_ROOTELEMENT);
      throw e;
    }
  }
  //set the prefix used for GraphMl
  str = root.getPrefix();
  names.setNSPrefix(str);

  //check the other constraints
  checkUniquenessOfPorts(root);
  checkIDREFConstraints(root);
}


void GraphMLDocument::checkUniquenessOfPorts(DOM_Element &root){
  DOM_NodeList nodes = root.getElementsByTagNameNS(names.nsUri(), names.nodeLN());

  //for each node n in this document
  for(unsigned int i = 0; i < nodes.getLength(); ++i){
    DOM_Node n = nodes.item(i);

    //port names within n have to be unique 
    set<string> portNames;

    DOM_NodeList children = n.getChildNodes();
    for(unsigned int j = 0; j < children.getLength(); ++j){
      DOM_Node c = children.item(j);

      if(c.getNodeType() == DOM_Node::ELEMENT_NODE && c.getLocalName().equals(names.portLN())){
	DOM_Element port = (DOM_Element &) c;

	if(portNames.find(port.getAttribute(names.port_name()).transcode()) != portNames.end()){
	  GraphMLException e(GraphMLException::DUPLICATE_PORT_NAME);

	  DOMString msg("Duplicate port name: ");
	  msg = msg + port.getAttribute(names.port_name());
	  msg = msg + "\n in node: ";
	  DOM_Element elem = (DOM_Element &) n;
	  msg = msg + elem.getAttribute(names.node_id());

	  e.setMessage(msg);

	  throw e;
	}
	portNames.insert(port.getAttribute(names.port_name()).transcode());

	//port might contain other ports
	DOM_NodeList ports = port.getElementsByTagNameNS(names.nsUri(), names.portLN());
	for(unsigned int k = 0; k < ports.getLength(); ++k){
	  c = ports.item(k);
	  port = (DOM_Element &) c;

	  if(portNames.find(port.getAttribute(names.port_name()).transcode()) != portNames.end()){
	    GraphMLException e(GraphMLException::DUPLICATE_PORT_NAME);

	    DOMString msg("Duplicate port name: ");
	    msg = msg + port.getAttribute(names.port_name());
	    msg = msg + "\n in node: ";
	    DOM_Element elem = (DOM_Element &) n;
	    msg = msg + elem.getAttribute(names.node_id());

	    e.setMessage(msg);

	    throw e;
	  }
	}
      }
    }
  }
}

void GraphMLDocument::checkIDREFConstraints(DOM_Element &root){
  //check the edges
  DOM_NodeList nList = root.getElementsByTagNameNS(names.nsUri(), names.edgeLN());
  for(unsigned int i = 0; i < nList.getLength(); ++i){
    DOM_Node n = nList.item(i);
    DOM_Element e = (DOM_Element &) n;

    isNodeAndHasPort(e.getAttribute(names.edge_source()), e.getAttribute(names.edge_sourceport()), GraphMLException::INVALID_SOURCE);

    isNodeAndHasPort(e.getAttribute(names.edge_target()), e.getAttribute(names.edge_targetport()), GraphMLException::INVALID_TARGET);

  }

  //check the endpoints
  nList = root.getElementsByTagNameNS(names.nsUri(), names.endpointLN());
  for(unsigned int i = 0; i < nList.getLength(); ++i){
    DOM_Node n = nList.item(i);
    DOM_Element e = (DOM_Element &) n;

    isNodeAndHasPort(e.getAttribute(names.endpoint_node()), e.getAttribute(names.endpoint_port()), GraphMLException::INVALID_NODE);

  }
  //check the data elements
  nList = root.getElementsByTagNameNS(names.nsUri(), names.dataLN());
  for(unsigned int i = 0; i < nList.getLength(); ++i){
    DOM_Node n = nList.item(i);
    DOM_Element e = (DOM_Element &) n;

    DOM_Element key = fDoc.getElementById(e.getAttribute(names.data_key()));
    if(!key.getLocalName().equals(names.keyLN())){
      GraphMLException e(GraphMLException::INVALID_KEY);
      throw e;
    }

  }
}

void GraphMLDocument::isNodeAndHasPort(DOMString nodeId, DOMString portName, const unsigned int type){
  DOM_Element node = fDoc.getElementById(nodeId);

  if(!names.nodeLN().equals(node.getLocalName())){
    GraphMLException e(type);
    e.setMessage(nodeId + " is not the id of any node");

    throw e;
  }

  if(portName.length() > 0){
    //look for the port
    bool found = false;
    DOM_NodeList nList = node.getChildNodes();
    for(unsigned int i = 0; i < nList.getLength(); ++i){
      DOM_Node n = nList.item(i);

      if(n.getNodeType() == DOM_Node::ELEMENT_NODE && n.getLocalName().equals(names.portLN())){
	DOM_Element port = (DOM_Element &) n;

	if(port.getAttribute(names.port_name()).equals(portName)){
	  found = true;
	}
	//look for ports within this port
	DOM_NodeList ports = port.getElementsByTagNameNS(names.nsUri(), names.portLN());

	for(unsigned int j = 0; j < ports.getLength(); ++j){
	  DOM_Node pn = ports.item(j);
	  DOM_Element p = (DOM_Element &) pn;

	  if(p.getAttribute(names.port_name()).equals(portName)){
	    found = true;
	  }
	}
      }
    }

    if(!found){
      GraphMLException e(GraphMLException::INVALID_PORT);
      e.setMessage(portName + " is not the name of any port within the node: " + nodeId);
      throw e;
    }
  }
}


//============================================
//Methods for writing
//============================================

//writes the attributes of elem like name1="val1" name2="val2" ..
void GraphMLDocument::writeAttributes(ostream &target, DOM_Node &elem){
    DOM_NamedNodeMap attributes = elem.getAttributes();
    //number of attributes
    int attNum = attributes.getLength();
    for (int i = 0; i < attNum; i++){
      //get the i'th attribute
      DOM_Node attribute = attributes.item(i);
      //write the attribute name
      target << " " << (attribute.getNodeName()).transcode();
      //write ="
      target << "=\""; 
      //write the attribute value
      target << (attribute.getNodeValue()).transcode();
      //write "
      target << "\"";
    }
}//end of writeAttributes

//writes recursively a domnode
//expects node being an element node
void GraphMLDocument::writeDOMNode(ostream &target, DOM_Node &node, int treedepth){

  writeIndent(target, treedepth);
  target << "<" << node.getNodeName().transcode();
  writeAttributes(target, node);

  if(node.hasChildNodes()){


    //================================
    //can be removed; first part (begin)
    //================================
    //we handle the case of node being a <bend> in a different fashion
    if(node.getLocalName().equals("bend")){ //*&%^%*&^(%$^&
      //in this case node is a <bend> element and it has childnode(s)
      //==>it has exactly one child node and this is <pos>
      target << ">";//no endl
      //write the child elements
      DOM_NodeList dl = node.getChildNodes();
      unsigned int num = dl.getLength();
      for(unsigned int i = 0; i < num; i++){
        DOM_Node n = dl.item(i);
        if(n.getNodeType() == DOM_Node::ELEMENT_NODE){
          target << "<" << n.getNodeName().transcode();
          writeAttributes(target, n);
        //<pos> has no children
          target << "/>";//still no endl
        }
      }
    }
    else
    {
      //it is not a <bend> 
      //================================
      //can be removed; first part (end) DONT FORGET THE SECOND PART
      //================================

      target << ">" << endl;
      //write the child elements

      DOM_NodeList dl = node.getChildNodes();
      char* str;
      unsigned int num = dl.getLength();
      for(unsigned int i = 0; i < num; i++)
      {
        DOM_Node n = dl.item(i);

        switch(n.getNodeType())
        {
           case DOM_Node::ELEMENT_NODE:
              //recursive call
              writeDOMNode(target, n, treedepth+1);
              break;
           case DOM_Node::TEXT_NODE:
              writeIndent(target, treedepth+1);
              str = n.getNodeValue().transcode();
               if(strcspn(str,"\r\n\t")!=0)
                target << str << endl;
              break;
           default: break;//do nothing
        }


      }
      //write the closing tag
      writeIndent(target, treedepth);



      //================================
      //can be removed; second part (begin)
      //================================
    }
    //================================
    //can be removed; second part (end)
    //================================



    target << "</" << node.getNodeName().transcode() << ">" << endl;
  }
  else{
    target << "/>" << endl;
  }
}

//write a sequence of whitespaces
void GraphMLDocument::writeIndent(ostream &target, unsigned int treeDepth){
  for(unsigned int i = 0; i < treeDepth; i++){
    target << INDENTATION.transcode();
  }
}

void GraphMLDocument::writeProlog(ostream &target, const DOMString &internalSubset){
  target << "<?xml version=\"1.0\" ?>" << endl;
    
//  target << "<!DOCTYPE ";
//  cout << "dtd = " << DTD.transcode() << endl;
//  target << names.graphmlQN().transcode() << " SYSTEM \"" << DTD.transcode() << "\" ";

  //check whether there is need for an internal subset
//  if(names.isPrefixed() || internalSubset.length() > 0){
//    target << "[" << endl;
//    if(names.isPrefixed()){
//      target << "<!ENTITY % GRAPHML.prefixed \"INCLUDE\">" << endl;
//      target << "<!ENTITY % GRAPHML.prefix \"" << names.nsPrefix().transcode() << "\">" << endl;
//    }
//    target << internalSubset.transcode() << endl;
//    target << "]";
//  }
//
//  target << ">" << endl;
    
}
