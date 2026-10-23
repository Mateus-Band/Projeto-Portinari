/**********************************************************
 * this code is an adaption of a programm of              *
 * Juergen Lerner (lerner@fmi.uni-konstanz.de)            *
 *        University of Konstanz, Germany                 *
 *                                                        *
 **********************************************************/
#include <xercesc/dom/DOM.hpp>

#include <iostream.h>

#include "xgmlnames.h"


XGMLNames::XGMLNames(){
  prefixed = true;
  //set the default value for the prefix 
  NS_PREFIX = DOMString("gd"); // actually we dont work with prefixes
  // - this is an holdover from the previous version
  // in future we might use it again, so I leave it in the source code
}
/**
 *set the namespace prefix used by GraphML   -- unused (see above)
 */
void XGMLNames::setNSPrefix(const DOMString &prefix){
  NS_PREFIX = prefix;
  if(prefix.length() > 0){
    prefixed = true;
  }
  else{
    prefixed = false;
  }
}

bool XGMLNames::isPrefixed() const{
  return prefixed;
}

DOMString XGMLNames::nsPrefix(){
  return NS_PREFIX;
}

DOMString XGMLNames::nsUri(){
  return XGML_URI;
}

//////////////////////////////////////////////////////////////////

const DOMString XGMLNames::key_weight_type(){
  return DOMString(KEY_WEIGHT_TYPE);
}

const DOMString XGMLNames::key_coord_type(){
  return DOMString(KEY_COORD_TYPE);
}

void XGMLNames::set_key_weight_id(DOMString id){
  KEY_WEIGHT_ID=id.transcode();
}

const DOMString XGMLNames::key_weight_id(){
  return DOMString(KEY_WEIGHT_ID);
}
const DOMString XGMLNames::key_coord_id(int dimNum){
  switch(dimNum)
  {
    case 1: return DOMString(KEY_COORDX1_ID);
    case 2: return DOMString(KEY_COORDX2_ID);
    case 3: return DOMString(KEY_COORDX3_ID);
    case 4: return DOMString(KEY_COORDX4_ID);
    default: cout << "fatal index error" << endl; exit(-1);
  }
}


const DOMString XGMLNames::key_weight_name(){
  return DOMString(KEY_WEIGHT_NAME);
}
const DOMString XGMLNames::key_coord_name(int dimNum){
  switch(dimNum)
  {
    case 1: return DOMString(KEY_COORDX1_NAME);
    case 2: return DOMString(KEY_COORDX2_NAME);
    case 3: return DOMString(KEY_COORDX3_NAME);
    case 4: return DOMString(KEY_COORDX4_NAME);
    default: cout << "fatal index error" << endl; exit(-1);
  }
}

const DOMString XGMLNames::key_attr_name(){
  return DOMString(KEY_ATTR_NAME);
}
const DOMString XGMLNames::key_attr_type(){
  return DOMString(KEY_ATTR_TYPE);
}

const DOMString XGMLNames::graph_id_value(){
return DOMString(GRAPH_ID_VALUE);
}

const DOMString XGMLNames::xml_xmlns_name()
{ return DOMString(XML_XMLNS_NAME); }
const DOMString XGMLNames::xml_xmlns_value()
{ return DOMString(XML_XMLNS_VALUE); }
const DOMString XGMLNames::xml_xsi_name()
{ return DOMString(XML_XSI_NAME); }
const DOMString XGMLNames::xml_xsi_value()
{ return DOMString(XML_XSI_VALUE); }
const DOMString XGMLNames::xml_xlink_name()
{ return DOMString(XML_XLINK_NAME); }
const DOMString XGMLNames::xml_xlink_value()
{ return DOMString(XML_XLINK_VALUE); }
const DOMString XGMLNames::xml_schemaLocation_name()
{ return DOMString(XML_SCHEMALOCATION_NAME); }
const DOMString XGMLNames::xml_schemaLocation_value()
{ return DOMString(XML_SCHEMALOCATION_VALUE); }


const DOMString XGMLNames::emptystring(){
return DOMString(EMPTYSTRING);
}