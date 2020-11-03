#include <iostream>
#include <cstring>
#include <functional>
#include <random>
#include <memory>
#include "Algorithm.h"
#include "MoveToFront.h"


typedef std::mt19937 engine_type;
engine_type engine;

/*
 * Arguments:
 * -r request length
 * -l list length
 * -n number of trials
 * -A algorithms
 * -f file 
 * -s Seed
 */
int main(int argc, char* argv[]) {
  int request_length, list_length, number_of_trials, seed;
  std::string file_name;
  // std::vector<Algorithm*> algorithms;
  std::vector<std::unique_ptr<Algorithm>> algorithms;
  bool valid = true;

  for ( int i = 1; i < argc; i++ ) {
    std::string c = argv[i];
    if ( c.compare("-r") == 0 ) {
      request_length = std::stoi(argv[++i]);
    }
    else if ( c.compare("-l") == 0 ) {
      list_length = std::stoi(argv[++i]);
    }
    else if ( c.compare("-n") == 0 ) {
      number_of_trials = std::stoi(argv[++i]);
    }
    else if ( c.compare("-A") == 0 ) {
      algorithms.push_back(std::make_unique<MoveToFront>());
    }
    else if ( c.compare("-f") == 0 ) {
      file_name = argv[++i];
    }
    else if ( c.compare("-s") == 0 ) {
      seed = std::stoi(argv[++i]);
    }
  }

  // validate command line
  if ( algorithms.size() < 1 ) {
    valid = false;
  }
  else if ( (request_length == 0 || number_of_trials == 0) && file_name.length() < 1 ) {
    valid = false;
  }

  if ( !valid ) {
    std::cout << "Invalid command line arguments\n";
    exit(-1);
  }

  if ( seed ) {
    engine_type::result_type const seedval = seed;
    engine.seed(seedval);
  }

  std::uniform_int_distribution<engine_type::result_type> udist(0, list_length - 1);
  auto rng = std::bind(udist, engine);

  for ( int t = 0; t < number_of_trials; t++ ) {
    std::vector<int> request_sequence;
    for ( int i = 0; i < request_length; i++ ) {
      request_sequence.push_back(rng());
    }

    for ( uint i = 0; i < algorithms.size(); i++ ) {
      algorithms.at(i)->setup(request_sequence, list_length);
      std::cout << algorithms.at(i)->run() << "\n";
    }
  }
}