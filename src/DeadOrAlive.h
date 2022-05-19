#ifndef DEADORALIVE
#include "_MTFAlgorithm.h"
#define DEADORALIVE
  class DeadOrAlive : public MTFAlgorithm {
    public:
      std::string name() {
        if ( continuous ) {
          return "continuous doa";
        }
        else {
          return "discrete doa";
        }
      };
      DeadOrAlive(double dead_weight, double alive_weight, bool continuous) {
        DeadOrAlive::dead_weight = dead_weight;
        DeadOrAlive::alive_weight = alive_weight;
        DeadOrAlive::continuous = continuous;
      }
    private:
      int move(int request, int request_index);
      double dead_weight;
      double alive_weight;
      bool move_on_stale = true;
      bool continuous = false;
    };
#endif