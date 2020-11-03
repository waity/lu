#ifndef ALGORITHM
#include <vector>
#include <iostream>

#define ALGORITHM
class Algorithm {
  public:
    void setup(std::vector<int> request_sequence, int list_length) {
      Algorithm::request_length = request_sequence.size();
      Algorithm::request_sequence = request_sequence;
      Algorithm::list_length = list_length;

      list.clear();
      for ( int i = 0; i < list_length; i++ ) {
        list.push_back(i);
      }
    }

    virtual int run() = 0;

  protected:
    int request_length;
    std::vector<int> request_sequence;
    int list_length;
    std::vector<int> list;

    void print_list() {
      //vector<int> v;
      std::cout << "[";
      for (int i=0; i < list_length; i++){
        std::cout << list[i];
        if (i != list_length - 1) {
          std::cout << ", ";
        }
      }
      std::cout << "]\n";
    }
};
#endif