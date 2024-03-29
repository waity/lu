#ifndef ALGORITHM
#include <vector>
#include <iostream>
#include <string>

#define ALGORITHM
class Algorithm {
  public:

    virtual ~Algorithm() {}
    virtual std::string name() = 0;
    virtual int run(bool debug) = 0;

    void setup(std::vector<int> request_sequence, int list_length) {
      Algorithm::request_length = request_sequence.size();
      Algorithm::request_sequence = request_sequence;
      Algorithm::list_length = list_length;

      list.clear();
      for ( int i = 0; i < list_length; i++ ) {
        list.push_back(i);
      }
    }

  protected:
    int request_length;
    std::vector<int> request_sequence;
    int list_length;
    std::vector<int> list;

    void print_list_ints(std::vector<int> l) {
      std::cout << "[";
      for (uint i=0; i < l.size(); i++){
        std::cout << l[i];
        if (i != l.size() - 1) {
          std::cout << ", ";
        }
      }
      std::cout << "]\n";
    }
};
#endif