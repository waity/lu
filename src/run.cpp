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


auto setup_rng(bool seed_given, int seed, int max_value) {
  typedef std::mt19937 engine_type;
  engine_type engine;
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
  ss << "\"";
  for ( uint i = 0; i < request_sequence.size(); i++ ) {
    ss << request_sequence.at(i);
    if ( i != request_sequence.size() - 1 ) {
      ss << ",";
    }
  }
  ss << "\"";

  line.push_back(ss.str());
  
  for ( uint i = 0; i < algorithms.size(); i++ ) {
    algorithms.at(i)->setup(request_sequence, list_length);
    int result = algorithms.at(i)->run();
    line.push_back(std::to_string(result));
  }

  write_csv_line(file, line);
}

/**
 * Generates a sequence based on the sequence number for a particular set of inputs.
 */
std::vector<int> construct_sequence(int sequence_number, int base, int request_length) {
  std::vector<int> ret(request_length);
  for ( int i = 0; i < request_length; i++ ) {
    int place = (int) (sequence_number / pow(base, (request_length - 1) - i)) % base;
    sequence_number -= place  * pow(base, (request_length - 1) - i);
    ret[i] = place;
  }

  return ret;
}

/**
 * Takes a sequence and generates the next sequence in order, given a base value
 */
void next_sequence(std::vector<int> &sequence, int base, int request_length) {
  int value;
  for ( int i = request_length - 1; i >= 0; i-- ) {
    value = sequence[i];
    value += 1;
    if (value >= base ) {
      sequence[i] = 0;
    }
    else {
      sequence[i] = value;
      break;
    }
  }
}


/*
 * Arguments:
 * -request_length [length] request length
 * -list_length [length] list length
 * -num_trials [number] number of trials
 * -M mtf
 * -D dead or alive (discrete)
 * -d dead or alive (continuous)
 * -O Opt
 * -o Free Opt
 * -ALL all algorithms
 * -file [name] file
 * -seed [seed] Seed
 * -initial [number] first trial to run (if provided, num_trials sequential from initial)
 */
int main(int argc, char* argv[]) {
  int request_length, list_length, number_of_trials, seed, initial_sequence_number, base;
  std::string file_name;
  std::vector<std::unique_ptr<Algorithm>> algorithms;

  bool sequential = false;
  bool valid = true;
  bool seed_given = false;
  bool from_file = false;

  for ( int i = 1; i < argc; i++ ) {
    std::string c = argv[i];
    if ( c.compare("-request_length") == 0 ) {
      request_length = std::stoi(argv[++i]);
    }
    else if ( c.compare("-list_length") == 0 ) {
      list_length = std::stoi(argv[++i]);
      base = list_length;
    }
    else if ( c.compare("-num_trials") == 0 ) {
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
      algorithms.push_back(std::make_unique<DeadOrAlive>(1, 1, false));
    }
    else if ( c.compare("-d") == 0 ) {
      algorithms.push_back(std::make_unique<DeadOrAlive>(1, 1, true));
    }
    else if (c.compare("-ALL") == 0 ) {
      algorithms.push_back(std::make_unique<MoveToFront>());
      algorithms.push_back(std::make_unique<Opt>());
      algorithms.push_back(std::make_unique<OptFree>());
      algorithms.push_back(std::make_unique<DeadOrAlive>(1, 1, false));
      algorithms.push_back(std::make_unique<DeadOrAlive>(1, 1, true));
    }
    else if ( c.compare("-file") == 0 ) {
      file_name = argv[++i];
      from_file = true;
    }
    else if ( c.compare("-seed") == 0 ) {
      seed = std::stoi(argv[++i]);
      seed_given = true;
    }
    else if ( c.compare("-initial") == 0 ) {
      sequential = true;
      initial_sequence_number = std::stoi(argv[++i]);
    }
  }

  // validate command line
  if ( algorithms.size() < 1 ) {
    valid = false;
  }
  else if ( (request_length == 0 || number_of_trials == 0) && !from_file ) {
    valid = false;
  }
  else if ( list_length == 0 ) {
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

  std::ofstream out_file;
  out_file.open("results.csv");

  std::vector<std::string> header;
  header.push_back("sequence");
  for ( uint i = 0; i < algorithms.size(); i++ ) {
    header.push_back(algorithms.at(i)->name());
  }

  write_csv_line(out_file, header);

  if ( from_file ) {
    std::ifstream in_file;
    in_file.open(file_name);
    std::string line;
    while ( std::getline(in_file,line) ) {
      std::vector<int> request_sequence;

      for(char c : line) {
        request_sequence.push_back(c - '0');
      }

      run_request_sequence(algorithms, request_sequence, list_length, out_file);
    }
 
    in_file.close();
  }
  else if ( sequential ) {
    std::vector<int> request_sequence = construct_sequence(initial_sequence_number, base, request_length);
    for ( int request = 0; request < number_of_trials; request++ ) {
      run_request_sequence(algorithms, request_sequence, list_length, out_file);
      next_sequence(request_sequence, base, request_length);
    }
  }
  else {
    for ( int t = 0; t < number_of_trials; t++ ) {
      // create request sequence
      // std::vector<int> request_sequence = {4, 4, 2, 0, 2};
      std::vector<int> request_sequence;
      for ( int i = 0; i < request_length; i++ ) {
        int request = rng();
        request_sequence.push_back(request);
      }

      // run request sequence on algorithms.
      run_request_sequence(algorithms, request_sequence, list_length, out_file);
    }
  }

  out_file.close();

  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  std::cout << "finished at " << std::ctime(&end_time);
  std::cout << "execution took: " << elapsed_seconds.count() << "s\n";
}
