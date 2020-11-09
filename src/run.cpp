#include <iostream>
#include <cstring>
#include <functional>
#include <random>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <thread>
#include "Algorithm.h"
#include "MoveToFront.h"
#include "DeadOrAlive.h"
#include "Opt.h"
#include "OptFree.h"


typedef std::mt19937 engine_type;
engine_type engine;

auto setup_rng(bool seed_given, int seed, int max_value) {
  if ( !seed_given ) {
    srand((unsigned int)time(NULL));
    seed = rand();
  }

  engine_type::result_type const seedval = seed;
  engine.seed(seedval);

  std::uniform_int_distribution<engine_type::result_type> udist(0, max_value);
  auto rng = std::bind(udist, engine);
  return rng;
}

void write_csv_line(std::ofstream &file, std::vector<std::string> &elements) {
  for ( uint i = 0; i < elements.size(); i++ ) {
    file << elements.at(i);
    if ( i != elements.size() - 1 ) {
      file << ",";
    }
  }
  file << "\n";
}

void run_request_sequence(std::vector<std::unique_ptr<Algorithm>> &algorithms, std::vector<int> &request_sequence, int list_length, std::ofstream &file) {
  std::vector<std::string> line;

  std::stringstream ss;
  for ( uint i = 0; i < request_sequence.size(); i++ ) {
    ss << request_sequence.at(i);
  }

  line.push_back(ss.str());
  
  for ( uint i = 0; i < algorithms.size(); i++ ) {
    algorithms.at(i)->setup(request_sequence, list_length);
    int result = algorithms.at(i)->run();
    line.push_back(std::to_string(result));
  }

  write_csv_line(file, line);
}

/*
 * Arguments:
 * -r request length
 * -l list length
 * -n number of trials
 * -M mtf
 * -D dead or alive (discrete)
 * -d dead or alive (continuous)
 * -f file 
 * -s Seed
 */
int main(int argc, char* argv[]) {
  int request_length, list_length, number_of_trials, seed;
  std::string file_name;
  std::vector<std::unique_ptr<Algorithm>> algorithms;
  bool valid = true;
  bool seed_given = false;


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
    else if ( c.compare("-M") == 0 ) {
      algorithms.push_back(std::make_unique<MoveToFront>());
    }
    else if ( c.compare("-O") == 0 ) {
      algorithms.push_back(std::make_unique<Opt>());
    }
    else if ( c.compare("-o") == 0 ) {
      algorithms.push_back(std::make_unique<OptFree>());
    }
    else if ( c.compare("-D") == 0 ) {
      int dead_weight = 1;
      int alive_weight = 1;

      algorithms.push_back(std::make_unique<DeadOrAlive>(dead_weight, alive_weight, false));
    }
    else if ( c.compare("-d") == 0 ) {
      int dead_weight = 1;
      int alive_weight = 1;

      algorithms.push_back(std::make_unique<DeadOrAlive>(dead_weight, alive_weight, true));
    }
    else if ( c.compare("-f") == 0 ) {
      file_name = argv[++i];
    }
    else if ( c.compare("-s") == 0 ) {
      seed = std::stoi(argv[++i]);
      seed_given = true;
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

  // start clock
  auto start = std::chrono::system_clock::now();
  std::time_t start_time = std::chrono::system_clock::to_time_t(start);
  std::cout << "starting, " << algorithms.size() << " algorithms running\n";
  std::cout << "start time: " << std::ctime(&start_time);

  auto rng = setup_rng(seed_given, seed, list_length - 1);

  std::ofstream file;
  file.open("results.csv");

  std::vector<std::string> header;
  header.push_back("sequence");
  for ( uint i = 0; i < algorithms.size(); i++ ) {
    header.push_back(algorithms.at(i)->name());
  }

  write_csv_line(file, header);

  for ( int t = 0; t < number_of_trials; t++ ) {
    // create request sequence
    // std::vector<int> request_sequence = {4, 4, 2, 0, 2};
    std::vector<int> request_sequence;
    for ( int i = 0; i < request_length; i++ ) {
      int request = rng();
      request_sequence.push_back(request);
    }

    // run request sequence on algorithms.
    run_request_sequence(algorithms, request_sequence, list_length, file);
  }

  file.close();

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  std::cout << "finished at " << std::ctime(&end_time);
  std::cout << "execution took: " << elapsed_seconds.count() << "s\n";
}
