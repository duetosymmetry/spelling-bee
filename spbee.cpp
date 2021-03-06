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
    std::vector<puzzleTester> puzzleTesters;

    cxxopts::Options options(argv[0], "Solver for Frank Longo's \"Spelling Bee\" puzzles");
    options
      .positional_help("[puzzle letter strings (see below)]")
      .show_positional_help();

    options.add_options()
      ("d,dict", "Dictionary file path", cxxopts::value<std::string>(dict)
       ->default_value("/usr/share/dict/words"), "FILE")
      ("n,nmin", "Minimum word length", cxxopts::value<int>(nmin)
       ->default_value("4"), "N")
      ("help", "Print this help message")
      ("positional",
       "puzzle letter strings: For each puzzle, give a string"
       " (without spaces, and only the characters a-z) specifying"
       " the letters that appear in the puzzle.  Mandatory letters"
       " are to appear capitalized.  Example: \"Macilnt\"."
       "  Multiple puzzles can be solved by giving a string for"
       " each one.",
       cxxopts::value<std::vector<std::string>>(words))
      ;

    options.parse_positional({"positional"});

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

    if (!result.count("positional")) { // no input, nothing to do.
      exit(0);
    }

    // Validate input words
    for (const auto& word : words) {
      if (validInput(word))
        puzzleTesters.push_back(puzzleTester(word));
      else
        std::cerr << "Ignoring invalid input: " << word << std::endl;
    }

    if (!puzzleTesters.size()) { // no valid input
      std::cerr << "No valid inputs" << std::endl;
      exit(1);
    }

    // Set each tester's nmin
    for (auto& puzzleTester : puzzleTesters) {
      puzzleTester.nmin = nmin;
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
      std::string sortedDownLine = line;

      if (line.length() < nmin)
        continue;

      // downcase, sort, and remove dups
      downSortUniq(sortedDownLine);

      if (std::any_of(puzzleTesters.begin(), puzzleTesters.end(),
                      [&](const puzzleTester & t){ return t(sortedDownLine); })) {

        if (std::any_of(puzzleTesters.begin(), puzzleTesters.end(),
                        [&](const puzzleTester & t){
                          return t.all(sortedDownLine); })) {
          std::cout << "* ";
        } else { std::cout << "  "; };

        std::cout << line << std::endl;
      }
    }

    // All done!
    dictFile.close();

  } catch (const cxxopts::OptionException& e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

  return 0;
}
