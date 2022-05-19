#ifndef MOVETOFRONT
#include "_MTFAlgorithm.h"
#define MOVETOFRONT
  class MoveToFront : public MTFAlgorithm {
    public:
      std::string name() {
        return "move to front";
      };
    private:
      int move(int request, int request_index);
    };
#endif