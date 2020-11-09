#ifndef OPT
#include "OptAlgorithm.h"
#define OPT
  class Opt : public OptAlgorithm {
    public:
      std::string name() {
        return "opt";
      };
    private:
      int shift(std::vector<int> from, std::vector<int> to, int request_index);
  };
#endif