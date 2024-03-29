#ifndef MTFALGORITHM
#include "_Algorithm.h"
#define MTFALGORITHM
class MTFAlgorithm : public Algorithm {
  public:
    virtual std::string name() = 0;
    int run(bool debug) {
      int cost = 0;
      
      if ( debug ) { 
        std::cout<< name() << "\n";
        Algorithm::print_list_ints(list);
      }
      
      for (int i = 0; i < request_length; i++ ) {
        int request = request_sequence.at(i);
        int step_cost = move(request, i);
        cost += step_cost;
        if ( debug ) {
          // std::cout << "request " << i << " value: " << request << " cost: " << step_cost << " ";
          Algorithm::print_list_ints(list);
        }
      }
      if ( debug ) { std::cout << std::endl;}
      return cost;
    }
  private:
    virtual int move(int request, int request_index) = 0;
};
#endif