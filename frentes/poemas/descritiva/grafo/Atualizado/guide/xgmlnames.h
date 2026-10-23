/**********************************************************
 * this code is an adaption of a programm of              *
 * Juergen Lerner (lerner@fmi.uni-konstanz.de)            *
 *        University of Konstanz, Germany                 *
 *                                                        *
 **********************************************************/
#ifndef GD_HEADER_GUARD
#define GD_HEADER_GUARD

#include <xercesc/dom/DOM.hpp>

#include "GraphML.h"

#include <string.h>

/**
 *provides the names for the GD module
 *Acronyms: LN stands for local name
 *          QN stands for qualified name
 *          NS or ns stands for namespace
 */
class XGMLNames : public GraphMLNames{
 private:
  bool prefixed;   // actually we dont work with prefixes
  // - this is an holdover from the previous version
  // in future we might use it again, so I leave it in the source code
  static const char *XGML_URI("http://www.graphdrawing.org/xmlns/graphml");
  DOMString NS_PREFIX;

  static char *KEY_WEIGHT_ID("we"); // not a const value - the value is not strictly defined
  static const char *KEY_COORDX1_ID("x1");
  static const char *KEY_COORDX2_ID("x2");
  static const char *KEY_COORDX3_ID("x3");
  static const char *KEY_COORDX4_ID("x4");

  static const char *KEY_WEIGHT_NAME("length");
  static const char *KEY_COORDX1_NAME("coord1");
  static const char *KEY_COORDX2_NAME("coord2");
  static const char *KEY_COORDX3_NAME("coord3");
  static const char *KEY_COORDX4_NAME("coord4");

  static const char *KEY_WEIGHT_TYPE("double");
  static const char *KEY_COORD_TYPE("double");

  //gd attributes for graphml elements
  static const char *KEY_ATTR_TYPE("attr.type");
  static const char *KEY_ATTR_NAME("attr.name");

  static const char *GRAPH_ID_VALUE("G");


  static const char *XML_XMLNS_NAME("xmlns");
  static const char *XML_XSI_NAME("xmlns:xsi");
  static const char *XML_XLINK_NAME("xmlns:xlink");
  static const char *XML_SCHEMALOCATION_NAME("xsi:schemaLocation");

  static const char *XML_XMLNS_VALUE("http://www.graphdrawing.org/xmlns/graphml");
  static const char *XML_XSI_VALUE("http://www.w3.org/2001/XMLSchema-instance");
  static const char *XML_XLINK_VALUE("http://www.w3.org/1999/xlink");
  static const char *XML_SCHEMALOCATION_VALUE("\n\
http://www.graphdrawing.org/xmlns/graphml\ngraphml-structure-1.0rc.xsd\n\n \
http://www.graphdrawing.org/xmlns/graphml\ngraphml-attributes-1.0rc.xsd");

  static const char *EMPTYSTRING("");

 public:
  XGMLNames();

  bool isPrefixed() const;  // unused (see above)

  void setNSPrefix(const DOMString &prefix);  // unused (see above)

  DOMString prolog();

  DOMString nsPrefix(); // unused (see above)
  DOMString nsUri();  // unused (see above)

  /**
   *getter methods for the attribute names
   */
  const DOMString key_coord_type();
  const DOMString key_weight_type();

  void set_key_weight_id(DOMString id);
  const DOMString key_weight_id();
  const DOMString key_coord_id(int dimNum);

  const DOMString key_weight_name();
  const DOMString key_coord_name(int dimNum);

  const DOMString key_attr_name();
  const DOMString key_attr_type();

  const DOMString graph_id_value();

  const DOMString xml_xmlns_name();
  const DOMString xml_xmlns_value();
  const DOMString xml_xsi_name();
  const DOMString xml_xsi_value();
  const DOMString xml_xlink_name();
  const DOMString xml_xlink_value();
  const DOMString xml_schemaLocation_name();
  const DOMString xml_schemaLocation_value();

  const DOMString emptystring();
};

#endif





