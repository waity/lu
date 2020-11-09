#include "OptFree.h"

// Opt Free shift returns INT_MAX if impossible to state, and 0 if possible (as it will be a free exchange)
int OptFree::shift(std::vector<int> from, std::vector<int> to, int request_index) {
  std::vector<int> copy = from;
  int request_value = request_index >= 0 ? request_sequence.at(request_index) : -1;

  int position = 0;

  while ( position < list_length ) {
    for ( int i = 0; i < list_length - position; i++ ) {
      if ( copy.at(i) == to.at(position) ) {
        int v = copy.at(i);
        copy.erase(copy.begin() + i);
        if ( i != 0 && v != request_value ) {
          return INT_MAX;
        }
        position++;
        break;
      }
    }
  }
  return 0;
}