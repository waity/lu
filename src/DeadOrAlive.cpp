#include "DeadOrAlive.h"
#include <algorithm>

int DeadOrAlive::move(int request, int request_index) {

  // find position of request in list
  std::vector<int>::iterator list_request_it = std::find(list.begin(), list.end(), request);
  int list_request_index = list_request_it - list.begin();

  // cost is access cost
  int cost = list_request_index + 1;

  // next request lies between request_index and end of request_sequence
  std::vector<int>::iterator seq_next_request_it = std::find(request_sequence.begin() + request_index + 1, request_sequence.end(), request);

  // If we do not request this item again. return access cost.
  if ( seq_next_request_it == request_sequence.end() ) {
    return cost;
  }

  double doa_weight = 0;
  double min_doa = 0;
  int discrete_pos = list_request_index;

  // for each element ahead of i in list, check if it is dead or alive.
  for ( int i = list_request_index - 1; i >= 0; i-- ) {
    std::vector<int>::iterator dead_or_alive = std::find(request_sequence.begin() + request_index + 1, seq_next_request_it, list[i]);
    if ( *dead_or_alive == list[i] ) {
      doa_weight += alive_weight;
    }
    else {
      doa_weight -= dead_weight;
      // if continuous, check most dead update position (TYPE TOGGLE)
      if ( continuous && doa_weight <= min_doa ) {
        min_doa = doa_weight;
        discrete_pos = i;
      }
    }
  }

  if ( continuous && min_doa <= 0 ) {
    list.erase(list_request_it);
    list.insert(list.begin() + discrete_pos, request);
  }
  else if ( doa_weight <= 0 ) {
    list.erase(list_request_it);
    list.insert(list.begin(), request);
  }

  return cost;
}