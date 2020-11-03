#ifndef MTFALGORITHM
#include "Algorithm.h"
#define MTFALGORITHM
class MTFAlgorithm : public Algorithm {
  public:
    virtual std::string name() = 0;
    int run() {
      int cost = 0;
      for (int i = 0; i < request_length; i++ ) {
        int request = request_sequence.at(i);
        cost += move(request, i);
      }
      return cost;
    }
  private:
    virtual int move(int request, int request_index) = 0;
};
#endif