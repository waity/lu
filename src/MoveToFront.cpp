#include "MoveToFront.h"
#include <algorithm>

int MoveToFront::move(int request) {
  std::vector<int>::iterator i = std::find(list.begin(), list.end(), request);
  int cost = i - list.begin() + 1;
  list.erase(i);
  list.insert(list.begin(), request);
  return cost;
}