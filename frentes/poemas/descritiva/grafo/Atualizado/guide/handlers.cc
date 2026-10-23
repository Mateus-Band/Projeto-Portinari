/**********************************************************
 * this code is an adaption of a programm of              *
 * Juergen Lerner (lerner@fmi.uni-konstanz.de)            *
 *        University of Konstanz, Germany                 *
 *                                                        *
 **********************************************************/
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/dom/DOM.hpp>

#include <iostream>

using namespace std;

#include "GraphML.h"
//---------------------------------------------------------
//implementation of the GraphMLErrorHandler
//---------------------------------------------------------

GraphMLErrorHandler::GraphMLErrorHandler() :

    fSawErrors(false)
{
}

GraphMLErrorHandler::~GraphMLErrorHandler()
{
}


// ---------------------------------------------------------------------------
//  GraphMLHandlers: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void GraphMLErrorHandler::error(const SAXParseException& e)
{
    fSawErrors = true;
    fEx = GraphMLException(GraphMLException::PARSE_ERROR, e.getLineNumber(), e.getColumnNumber());
    fEx.setSystemId(e.getSystemId());
    fEx.setMessage(e.getMessage());
    throw fEx;
}

void GraphMLErrorHandler::fatalError(const SAXParseException& e)
{
    fSawErrors = true;
    fEx = GraphMLException(GraphMLException::FATAL_PARSE_ERROR, e.getLineNumber(), e.getColumnNumber());
    fEx.setSystemId(e.getSystemId());
    fEx.setMessage(e.getMessage());
    throw fEx;
}

void GraphMLErrorHandler::warning(const SAXParseException& e)
{
  cout << "warning called" << endl; 
}

void GraphMLErrorHandler::resetErrors()
{
}


