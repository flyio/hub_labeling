#include "../src/labeling.hpp"

int test_make_label(){
  Labeling index(4);
  bool backward = true;
  index.add(0,1,1,!backward);
  index.add(0,2,3,!backward);
  index.add(3,1,1,backward);
  index.add(3,2,1,backward);

  index.sort_all();

  cout << "dist from 0 to 3 = " << index.query(0,3) << endl;

  if(index.query(0,3)!=2) cerr << "[Error] Query result is invalid" << endl;

  index.write(cout);
}

int test_label_io(){
  Labeling index(4);
  bool backward = true;
  index.add(0,1,1,!backward);
  index.add(0,2,3,!backward);
  index.add(3,1,1,backward);
  index.add(3,2,1,backward);

  index.sort_all();

  ofstream f("test.index");

  index.write(f);

  ifstream g("test.index");

  Labeling index2;
  index2.read(g);

  cout << "index (original)" << endl;
  index.write(cout);

  cout << "index (loaded)" << endl;
  index2.write(cout);
}

int main(){
  test_make_label();
  test_label_io();
}