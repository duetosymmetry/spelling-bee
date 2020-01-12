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
    std::ifstream dictFile;
    std::vector<std::string> words;

    cxxopts::Options options(argv[0], "Solver for Frank Longo's \"Spelling Bee\" puzzle, with scoring");
    options
      .positional_help("puzzle letter string (see below)")
      .show_positional_help();

    options.add_options()
      ("d,dict", "Dictionary file path", cxxopts::value<std::string>(dict)
       ->default_value("/usr/share/dict/words"), "FILE")
      ("n,nmin", "Minimum word length", cxxopts::value<int>(nmin)
       ->default_value("4"), "N")
      ("t,total", "Report total score")
      ("help", "Print this help message")
      ("positional",
       "puzzle letter string: Give a string"
       " (without spaces, and only the characters a-z) specifying"
       " the letters that appear in the puzzle.  Mandatory letters"
       " are to appear capitalized.  Example: \"Macilnt\".",
       cxxopts::value<std::vector<std::string>>(words))
      ;

    options.parse_positional({"positional"});

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

    if (1 != result.count("positional")) { // no input, nothing to do.
      std::cout << "Need exactly 1 puzzle string" << std::endl;
      exit(0);
    }

    if (! validInput(words[0])) {
      std::cerr << "Invalid input: " << words[0] << std::endl;
      exit(0);
    }

    puzzleTester tester(words[0]);
    tester.nmin = nmin;
    
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
      unsigned long wordScore = tester.scoreAndAccumulate(line);

      if (wordScore == 0)
        continue;

      std::string sortedDownLine = line;
      // downcase, sort, and remove dups
      downSortUniq(sortedDownLine);

      if (tester.all(sortedDownLine))
        std::cout << "* ";
      else
        std::cout << "  ";

      std::cout << line << " " << wordScore << std::endl;

    }

    // All done!
    dictFile.close();

    if (result.count("total")) {
      std::cout << "---------------" << std::endl;
      std::cout << "Total score: " << tester.getTotalScore() << std::endl;
    }

  } catch (const cxxopts::OptionException& e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

  return 0;
}
