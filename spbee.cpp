#include <iostream>
#include <fstream>

#include <cctype>
#include <algorithm>

#include "cxxopts.hpp"

// validate that string s is a valid pattern for the puzzle
// It is only allowed to consist of [a-zA-Z]
bool validInput(std::string s) {
  return all_of(s.begin(), s.end(), isalpha);
}

int main(int argc, char* argv[]) {
  try {

    int nmin;
    std::string dict;
    std::ifstream dictFile;

    cxxopts::Options options(argv[0], "Spelling Bee puzzle solver");
    options
      .positional_help("[optional args]")
      .show_positional_help();

    options.add_options()
      ("d,dict", "File", cxxopts::value<std::string>()
       ->default_value("/usr/share/dict/words"), "FILE")
      ("n,nmin", "An integer", cxxopts::value<int>()->default_value("5"), "N")
      ("help", "Print help")
      ("positional",
       "Positional arguments: these are the arguments that are entered "
       "without an option", cxxopts::value<std::vector<std::string>>())
      ;

    options.parse_positional({"positional"});

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

    dict = result["d"].as<std::string>();
    dictFile.open(dict);
    if (! dictFile.good() ) {
      std::cerr << "Can't read from dictionary file '"
                << dict << "'" << std::endl;
      exit(1);
    };

    if (result.count("positional")) {
      auto& words = result["positional"].as<std::vector<std::string>>();
      for (const auto& word : words) {
        std::cout << word << ", " << validInput(word) << std::endl;
      }
    }
    
    std::string line;

    // for(int i=0; (i<10) && std::getline(dictFile, line); i++) {
    //   std::cout << line << std::endl;
    // }

    dictFile.close();
    
  } catch (const cxxopts::OptionException& e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

  return 0;
}
