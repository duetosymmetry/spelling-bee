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

    cxxopts::Options options(argv[0], "Filter for pangram words for Frank Longo's \"Spelling Bee\" puzzles");
    options
      .show_positional_help();

    options.add_options()
      ("d,dict", "Dictionary file path", cxxopts::value<std::string>(dict)
       ->default_value("/usr/share/dict/words"), "FILE")
      ("help", "Print this help message")
      ;

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
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
      std::string sortedUpLine = line;
      // upcase, sort, and remove dups
      upSortUniq(sortedUpLine);

      if (7 == sortedUpLine.length()) {
        std::cout << line << ' ' << sortedUpLine << std::endl;
      };
    }

    // All done!
    dictFile.close();

  } catch (const cxxopts::OptionException& e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

  return 0;
}
