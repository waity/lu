#ifndef MTFALGORITHM
#include "Algorithm.h"
#define MTFALGORITHM
class MTFAlgorithm : public Algorithm {
  public:
    int run() {
      int cost = 0;
      for (int i = 0; i < request_length; i++ ) {
        cost += move(request_sequence.at(i));
      }
      return cost;
    }
  private:
    virtual int move(int request) = 0;
};
#endif