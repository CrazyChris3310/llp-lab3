#include <iostream>
#include <boost/asio.hpp>
#include "inner.hxx"
#include <memory>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

  try
  {
    unique_ptr<OuterClass> h (outer(argv[1]));

      cout << h->str() << endl;
      cout << h->inner().field1() << endl;
      cout << h->inner().field2() << endl;


    InnerClass in("I was done programmaticaly", 28);
    OuterClass out("New class", in);

    xml_schema::namespace_infomap map;
    map[""].name = "";
    map[""].schema = "inner.xsd";

    outer(cout, out, map);
  }
  catch (const xml_schema::exception& e)
  {
    cerr << e << endl;
    return 1;
  }

  return 0;
}