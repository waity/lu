#ifndef OPTFREE
#include "_OptAlgorithm.h"
#define OPTFREE
  class OptFree : public OptAlgorithm {
    public:
      std::string name() {
        return "opt free";
      };
    private:
      int shift(std::vector<int> from, std::vector<int> to, int request_index);
  };
#endif