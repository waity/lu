#ifndef OPTALGORITHM
#include "Algorithm.h"
#include <bits/stdc++.h> 
#define OPTALGORITHM
class OptAlgorithm : public Algorithm {
  public:
    virtual std::string name() = 0;
    int run() {
      populate_states();
      int table[request_length + 1][possible_state_count];
      // fill first row; cost to shift from initial state to each state.
      for ( int i = 0; i < possible_state_count; i++ ) {
        table[0][i] = shift(list, possible_states.at(i), -1);
      }
      for ( int request_index = 0; request_index < request_length; request_index++ ) {
        int table_index = request_index + 1;
        for ( int state_index = 0; state_index < possible_state_count; state_index++ ) {
          int min;
          bool first_pass = true;

          for ( int previous_state_index = 0; previous_state_index < possible_state_count; previous_state_index++ ) {

            int previous_cost = table[table_index - 1][previous_state_index];
            int shift_cost = shift(possible_states.at(previous_state_index), possible_states.at(state_index), request_index);
            int potential_cost = INT_MAX;

            if ( shift_cost != INT_MAX && previous_cost != INT_MAX ) {
              potential_cost = previous_cost + shift_cost + access_cost(possible_states.at(previous_state_index), request_index);
            }

            if ( first_pass ) {
              min = potential_cost;
              first_pass = false;
            }
            else if ( potential_cost < min ) {
              min = potential_cost;
            }
            
          }
          table[table_index][state_index] = min;
        }
      }

      possible_states.clear();
      return *std::min_element(table[request_length], table[request_length] + possible_state_count);
    };
  protected:
    int possible_state_count = 0;
    std::vector<std::vector<int>> possible_states;
    void populate_states() {
      do {
        possible_states.push_back(list);
      } while (std::next_permutation(list.begin(), list.end()));
      possible_state_count = possible_states.size();
    }
    int access_cost(std::vector<int> state, int request_index) {
      int request = request_sequence.at(request_index);
      std::vector<int>::iterator list_request_it = std::find(state.begin(), state.end(), request);
      int list_request_index = list_request_it - state.begin();
      int cost = list_request_index + 1;
      return cost;
    }
  private:
    virtual int shift(std::vector<int> from, std::vector<int> to, int request_index) = 0;
};
#endif