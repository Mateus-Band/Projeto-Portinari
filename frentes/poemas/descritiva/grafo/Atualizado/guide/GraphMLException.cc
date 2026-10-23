/**********************************************************
 * this code is an adaption of a programm of              *
 * Juergen Lerner (lerner@fmi.uni-konstanz.de)            *
 *        University of Konstanz, Germany                 *
 *                                                        *
 **********************************************************/
#include "GraphML.h"

GraphMLException::GraphMLException() {
}


GraphMLException::GraphMLException(const unsigned int t, unsigned int l=0,
 unsigned int c=0){
  type = t;
  switch(type){
  case PARSE_ERROR:
    description = PARSE_ERROR_DESC;
    break;
  case FATAL_PARSE_ERROR:
    description = FATAL_PARSE_ERROR_DESC;
    break;
  case PARSE_WARNING:
    description = PARSE_WARNING_DESC;
    break;
  case INVALID_NAMESPACE:
    description = INVALID_NAMESPACE_DESC;
    break;
  case INVALID_ROOTELEMENT:
    description = INVALID_ROOTELEMENT_DESC;
    break;
  case DUPLICATE_PORT_NAME:
    description = DUPLICATE_PORT_NAME_DESC;
    break;
  case INVALID_KEY:
    description = INVALID_KEY_DESC;
    break;
  case INVALID_SOURCE:
    description = INVALID_SOURCE_DESC;
    break;
  case INVALID_TARGET:
    description = INVALID_TARGET_DESC;
    break;
  case INVALID_NODE:
    description = INVALID_NODE_DESC;
    break;
  case INVALID_PORT:
    description = INVALID_PORT_DESC;
    break;
  case SOURCE_NOT_IN_GRAPH:
    description = SOURCE_NOT_IN_GRAPH_DESC;
    break;
  case TARGET_NOT_IN_GRAPH:
    description = TARGET_NOT_IN_GRAPH_DESC;
    break;
  case NO_GRAPH_IN_FILE:
    description = NO_GRAPH_IN_FILE_DESC;
    break;
  }
  line = l;
  column = c;
}


GraphMLException::~GraphMLException()
{
}

void GraphMLException::setMessage(DOMString m){
  user_message = m;
}

void GraphMLException::setSystemId(DOMString uri){
  systemId = uri;
}

DOMString GraphMLException::getMessage(){
  return user_message;
}
DOMString GraphMLException::getSystemId(){
  return systemId;
}
DOMString GraphMLException::getDescription(){
  return description;
}
int GraphMLException::getLine(){
  return line;
}
int GraphMLException::getColumn(){
  return column;
}

bool GraphMLException::isParseException(){
  return type == PARSE_ERROR || type == FATAL_PARSE_ERROR || type ==
 PARSE_WARNING;
}
