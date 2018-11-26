#include <Eigen/Core>
#include <Eigen/Dense>
#include <iostream>

using namespace std;

int main() {
  Eigen::setNbThreads(8);
  Eigen::initParallel();
  Eigen::ArrayXf v(6);
  v << 1, 2, 3, 4, 5, 6;
  cout << "v.head(3) =" << endl << v.head(3) << endl << endl;
  cout << "v.tail<3>() = " << endl << v.tail<3>() << endl << endl;
  v.segment(1, 4) *= 2;
  cout << "after 'v.segment(1,4) *= 2', v =" << endl << v << endl;
  v << 1, 2, 3, 4, 5, 6;
  cout << "after re-init, v =" << endl << v << endl;
  v *= 1.1;
  cout << "after v * 1.1, v =" << endl << v << endl;
  cout << endl;
  cout << "(v > 0).all()   = " << (v > 0).all() << endl;
  cout << "(v > 0).any()   = " << (v > 0).any() << endl;
  cout << "(v > 0).count() = " << (v > 0).count() << endl;
  cout << endl;
  cout << "(v > 2.2).all()   = " << (v > 2.2).all() << endl;
  cout << "(v > 2.2).any()   = " << (v > 2.2).any() << endl;
  cout << "(v > 2.2).count() = " << (v > 2.2).count() << endl;
  cout << "num threads used: " << Eigen::nbThreads() << endl;

  return 0;
}
