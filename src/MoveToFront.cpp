#include "MoveToFront.h"
#include <algorithm>

int MoveToFront::move(int request, int request_index) {
  std::vector<int>::iterator it = std::find(list.begin(), list.end(), request);
  int cost = it - list.begin() + 1;
  list.erase(it);
  list.insert(list.begin(), request);
  return cost;
}