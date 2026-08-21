#include "pairtuple.hpp"
namespace m {
std::pair<int,int> order(int a, int b){ return a<b ? std::pair<int,int>{a,b} : std::pair<int,int>{b,a}; }
std::pair<int,double> stats(int n){ return { n, n*0.5 }; }
}
