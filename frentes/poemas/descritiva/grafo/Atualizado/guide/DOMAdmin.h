
#include <string>

class DOMAdmin
{
 public:
  DOMAdmin();
  ~DOMAdmin();
  double string2double(const string& lstr);
  DOMString double2string(double d);
  DOMString int2string(int m);
  int string2int(const string& str);
};

