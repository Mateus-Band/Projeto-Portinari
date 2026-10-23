/**********************************************************
 * this code is an adaption of a programm of              *
 * Juergen Lerner (lerner@fmi.uni-konstanz.de)            *
 *        University of Konstanz, Germany                 *
 *                                                        *
 **********************************************************/
#include <xercesc/dom/DOM.hpp>

#include "GraphML.h"

//supported GraphML elements



GraphMLNames::GraphMLNames(){
  prefixed = false;
  //set the default values for the prefix and the namespace uri
  NS_PREFIX = DOMString("");
}
/**
 *set the namespace prefix used by GraphML 
 */
void GraphMLNames::setNSPrefix(const DOMString &prefix){
  NS_PREFIX = prefix;
  if(prefix.length() > 0){
    prefixed = true;
  }
  else{
    prefixed = false;
  }
}

bool GraphMLNames::isPrefixed() const{
  return prefixed;
}

DOMString GraphMLNames::nsPrefix(){
  return NS_PREFIX;
}

DOMString GraphMLNames::nsUri(){
  return GRAPHML_URI;
}

const DOMString GraphMLNames::graphmlLN(){
  return DOMString(GRAPHML_GRAPHML);
}
const DOMString GraphMLNames::graphmlQN(){
  if(prefixed){
    return NS_PREFIX + ":" + DOMString(GRAPHML_GRAPHML);
  }
  else{
    return DOMString(GRAPHML_GRAPHML);
  }
}
const DOMString GraphMLNames::externalLN(){
  return DOMString(GRAPHML_EXTERNAL);
}
const DOMString GraphMLNames::externalQN(){
  if(prefixed){
    return NS_PREFIX + ":" + DOMString(GRAPHML_EXTERNAL);
  }
  else{
    return DOMString(GRAPHML_EXTERNAL);
  }
}
const DOMString GraphMLNames::descLN(){
  return DOMString(GRAPHML_DESC);
}
const DOMString GraphMLNames::descQN(){
  if(prefixed){
    return NS_PREFIX + ":" + DOMString(GRAPHML_DESC);
  }
  else{
    return DOMString(GRAPHML_DESC);
  }
}
const DOMString GraphMLNames::graphLN(){
  return DOMString(GRAPHML_GRAPH);
}
const DOMString GraphMLNames::graphQN(){
  if(prefixed){
    return NS_PREFIX + ":" + DOMString(GRAPHML_GRAPH);
  }
  else{
    return DOMString(GRAPHML_GRAPH);
  }
}
const DOMString GraphMLNames::nodeLN(){
  return DOMString(GRAPHML_NODE);
}
const DOMString GraphMLNames::nodeQN(){
  if(prefixed){
    return NS_PREFIX + ":" + DOMString(GRAPHML_NODE);
  }
  else{
    return DOMString(GRAPHML_NODE);
  }
}
const DOMString GraphMLNames::portLN(){
  return DOMString(GRAPHML_PORT);
}
const DOMString GraphMLNames::portQN(){
  if(prefixed){
    return NS_PREFIX + ":" + DOMString(GRAPHML_PORT);
  }
  else{
    return DOMString(GRAPHML_PORT);
  }
}
const DOMString GraphMLNames::edgeLN(){
  return DOMString(GRAPHML_EDGE);
}
const DOMString GraphMLNames::edgeQN(){
  if(prefixed){
    return NS_PREFIX + ":" + DOMString(GRAPHML_EDGE);
  }
  else{
    return DOMString(GRAPHML_EDGE);
  }
}
const DOMString GraphMLNames::hyperedgeLN(){
  return DOMString(GRAPHML_HYPEREDGE);
}
const DOMString GraphMLNames::hyperedgeQN(){
  if(prefixed){
    return NS_PREFIX + ":" + DOMString(GRAPHML_HYPEREDGE);
  }
  else{
    return DOMString(GRAPHML_HYPEREDGE);
  }
}
const DOMString GraphMLNames::endpointLN(){
  return DOMString(GRAPHML_ENDPOINT);
}
const DOMString GraphMLNames::endpointQN(){
  if(prefixed){
    return NS_PREFIX + ":" + DOMString(GRAPHML_ENDPOINT);
  }
  else{
    return DOMString(GRAPHML_ENDPOINT);
  }
}
const DOMString GraphMLNames::keyLN(){
  return DOMString(GRAPHML_KEY);
}
const DOMString GraphMLNames::keyQN(){
  if(prefixed){
    return NS_PREFIX + ":" + DOMString(GRAPHML_KEY);
  }
  else{
    return DOMString(GRAPHML_KEY);
  }
}
const DOMString GraphMLNames::dataLN(){
  return DOMString(GRAPHML_DATA);
}
const DOMString GraphMLNames::dataQN(){
  if(prefixed){
    return NS_PREFIX + ":" + DOMString(GRAPHML_DATA);
  }
  else{
    return DOMString(GRAPHML_DATA);
  }
}
//==========================

const DOMString GraphMLNames::graph_id(){
  return DOMString(GRAPH_ID);
}

const DOMString GraphMLNames::node_id(){
  return DOMString(NODE_ID);
}
const DOMString GraphMLNames::port_name(){
  return DOMString(PORT_NAME);
}
const DOMString GraphMLNames::data_key(){
  return DOMString(DATA_KEY);
}
const DOMString GraphMLNames::data_id(){
  return DOMString(DATA_ID);
}
const DOMString GraphMLNames::key_id(){
  return DOMString(KEY_ID);
}
const DOMString GraphMLNames::key_for(){
  return DOMString(KEY_FOR);
}
const DOMString GraphMLNames::edge_source(){
  return DOMString(EDGE_SOURCE);
}
const DOMString GraphMLNames::edge_target(){
  return DOMString(EDGE_TARGET);
}
const DOMString GraphMLNames::edge_id(){
  return DOMString(EDGE_ID);
}
const DOMString GraphMLNames::edge_sourceport(){
  return DOMString(EDGE_SOURCEPORT);
}
const DOMString GraphMLNames::edge_targetport(){
  return DOMString(EDGE_TARGETPORT);
}
const DOMString GraphMLNames::edge_dir(){
  return DOMString(EDGE_DIR);
}
const DOMString GraphMLNames::graph_edgedef(){
  return DOMString(GRAPH_EDGEDEFAULT);
}
const DOMString GraphMLNames::hyperedge_id(){
  return DOMString(HYPEREDGE_ID);
}
const DOMString GraphMLNames::endpoint_id(){
  return DOMString(ENDPOINT_ID);
}
const DOMString GraphMLNames::endpoint_node(){
  return DOMString(ENDPOINT_NODE);
}
const DOMString GraphMLNames::endpoint_port(){
  return DOMString(ENDPOINT_PORT);
}
const DOMString GraphMLNames::endpoint_type(){
  return DOMString(ENDPOINT_TYPE);
}
const DOMString GraphMLNames::key_for_all(){
  return DOMString(KEY_FOR_ALL);
}
const DOMString GraphMLNames::graph_edgedefault_directed(){
  return DOMString(GRAPH_EDGEDEFAULT_DIRECTED);
}
const DOMString GraphMLNames::graph_edgedefault_undirected(){
  return DOMString(GRAPH_EDGEDEFAULT_UNDIRECTED);
}
const DOMString GraphMLNames::edge_dir_true(){
  return DOMString(EDGE_DIR_TRUE);
}
const DOMString GraphMLNames::edge_dir_false(){
  return DOMString(EDGE_DIR_FALSE);
}
const DOMString GraphMLNames::node_id_prefix(){
  return DOMString(NODE_ID_PREFIX);
}
const DOMString GraphMLNames::key_default(){
  return DOMString(KEY_DEFAULT);
}


