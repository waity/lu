#include "DeadOrAlive.h"
#include <algorithm>

int DeadOrAlive::move(int request, int request_index) {
  std::vector<int>::iterator list_request_it = std::find(list.begin(), list.end(), request);
  int list_request_index = list_request_it - list.begin();

  int cost = list_request_index + 1;

  std::vector<int>::iterator seq_next_request_it = std::find(request_sequence.begin() + request_index + 1, request_sequence.end(), request);

  // If we do not request this item again. return lookup cost.
  if ( seq_next_request_it == request_sequence.end() ) {
    return cost;
  }

  double doa_weight = 0;

  for ( int i = list_request_index - 1; i >= 0; i-- ) {
    std::vector<int>::iterator dead_or_alive = std::find(request_sequence.begin() + request_index + 1, seq_next_request_it, list[i]);
    if (*dead_or_alive == list[i]) {
      doa_weight += alive_weight;
    }
    else {
      doa_weight -= dead_weight;
    }
  }

  if ( doa_weight <= 0 ) {
    list.erase(list_request_it);
    list.insert(list.begin(), request);
  }

  return cost;
}