#include <iostream>
#include <fstream>

#include <cctype>
#include <algorithm>

#include "cxxopts.hpp"

#include "puzzle.hpp"

int main(int argc, char* argv[]) {
  try {

    int nmin;
    std::string dict;
    std::string puzPath;
    std::ifstream dictFile;
    std::ifstream puzFile;

    std::vector<puzzleTester> puzzleTesters;

    cxxopts::Options options(argv[0], "Solver for Frank Longo's \"Spelling Bee\" puzzle, with scoring for many puzzles");

    options.add_options()
      ("d,dict", "Dictionary file path", cxxopts::value<std::string>(dict)
       ->default_value("./enable1.txt"), "FILE")
      ("p,puzfile", "Puzzles file path", cxxopts::value<std::string>(puzPath)
       ->default_value("./enable-1-pangrams-without-s.txt"), "FILE")
      ("n,nmin", "Minimum word length", cxxopts::value<int>(nmin)
       ->default_value("4"), "N")
      ("help", "Print this help message")
      ;

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

    // Open puzzles file
    puzFile.open(puzPath);
    if (! puzFile.good() ) {
      std::cerr << "Can't read from puzzles file '"
                << dict << "'" << std::endl;
      exit(1);
    };

    // Go through the puzzles file
    std::string puzLine;

    while (std::getline(puzFile, puzLine)) {
      if (validInput(puzLine)) {
        puzzleTesters.push_back(puzzleTester(puzLine));
      } else {
        std::cerr << "Ignoring invalid puzzle input: " << puzLine << std::endl;
      }
    }

    // All done with puzzles file
    puzFile.close();

    // set all the nmin's
    for (auto& tester : puzzleTesters) {
      tester.nmin = nmin;
    }

    // Open dictionary file

    dictFile.open(dict);
    if (! dictFile.good() ) {
      std::cerr << "Can't read from dictionary file '"
                << dict << "'" << std::endl;
      exit(1);
    };

    // Go through the dictionary file in one pass

    std::string line;

    while (std::getline(dictFile, line)) {
      for (auto& tester : puzzleTesters) {
        tester.scoreAndAccumulate(line);
      }
    }

    // All done!
    dictFile.close();

    // Print results
    for (auto& tester : puzzleTesters) {
      std::cout << tester.getTotalScore() << " " << tester.getInput() << std::endl;
    }
    
  } catch (const cxxopts::OptionException& e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

  return 0;
}
