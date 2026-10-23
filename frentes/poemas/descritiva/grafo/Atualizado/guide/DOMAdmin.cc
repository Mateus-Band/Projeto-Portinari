#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <iostream.h>
#include <strstream.h>
#include "DOMAdmin.h"


DOMAdmin::DOMAdmin()
{
  // Initialize the XML4C system
  // encoding of chars,... etc
  //do this always before performing any operations with GraphMLDocument

  try {
    cout << "initializing platform utils ... " << flush;
    XMLPlatformUtils::Initialize();
    cout << "ok" << endl;
  }
  catch (const XMLException& toCatch){
    cerr << "Error during initialization! :\n" << endl;
    throw toCatch;
  }
}

DOMAdmin::~DOMAdmin()
{
  // terminate the XML4C system

  try{
    cout << "terminating the platform utils ... " << flush;
    XMLPlatformUtils::Terminate();
    cout << "ok" << endl;
  }
  catch (const XMLException& toCatch){
    cerr << "Error during termination! :\n" << endl;
    throw toCatch;
  }
}

//===============================================
  //begin helper methods
  //===============================================
  double DOMAdmin::string2double(const string& lstr)
  {
    int n = lstr.length();
    char cstr[n+1];

    for (int i=0; i < n; i++) cstr[i] = lstr[i];
    cstr[n] = '\0';

    double t = atof(cstr);
    return t;
  }

  DOMString DOMAdmin::double2string(double d)
  {
    //  string_ostream os;

    std::ostrstream os;
    os << d << ends;
    char* cs = os.str();
    DOMString retVal(cs);
    delete[] cs;
    return retVal;
  }

  //creates a DOMString from an int value
  DOMString DOMAdmin::int2string(int m) {

    if (m < 0) return (DOMString("-") += int2string((-1)*m) );
    else if (m < 10)
      switch(m) {
      case 0: return "0";
      case 1: return "1";
      case 2: return "2";
      case 3: return "3";
      case 4: return "4";
      case 5: return "5";
      case 6: return "6";
      case 7: return "7";
      case 8: return "8";
      case 9: return "9";
      }
    else {
      int n = m % 10;
      switch(n) {
      case 0: return (int2string( m/10 ) += "0");
      case 1: return (int2string( (m-n)/10) += "1");
      case 2: return (int2string( (m-n)/10) += "2");
      case 3: return (int2string( (m-n)/10) += "3");
      case 4: return (int2string( (m-n)/10) += "4");
      case 5: return (int2string( (m-n)/10) += "5");
      case 6: return (int2string( (m-n)/10) += "6");
      case 7: return (int2string( (m-n)/10) += "7");
      case 8: return (int2string( (m-n)/10) += "8");
      case 9: return (int2string( (m-n)/10) += "9");
      }
    }
    return DOMString("ERROR while converting int to DOMString");
  } //end int2string

  int DOMAdmin::string2int(const string& str)
  {
    int n = str.length();
    char cstr[n+1];

    for (int i=0; i < n; i++) cstr[i] = str[i];
    cstr[n] = '\0';

    return atoi(cstr);
  }
