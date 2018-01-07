#include <iostream>
#include <fstream>

#include <cctype>
#include <algorithm>

#include "cxxopts.hpp"

// validate that string s is a valid pattern for the puzzle
// It is only allowed to consist of [a-zA-Z]
bool validInput(const std::string & s) {
  return all_of(s.begin(), s.end(), isalpha);
}

// Check if a word passes the criteria:
// 1. longer than nmin
// 2. consists of only the letters in s
// 3. contains all the required letters in s (capitalized ones)
// Note, s should be valid and sorted, so all the capitalized letters are adjacent
bool pass(int nmin, const std::string & s, const std::string & test) {
  std::string all = s;
  std::transform(all.begin(), all.end(), all.begin(), tolower);

  int nreq = std::count_if(s.begin(), s.end(), isupper);
  auto firstReq = std::find_if(s.begin(), s.end(), isupper);

  std::string reqd(firstReq, firstReq+nreq);

  std::cout << s << " " << all << " " << reqd << std::endl;

  return true;

}

int main(int argc, char* argv[]) {
  try {

    int nmin;
    std::string dict;
    std::ifstream dictFile;
    std::vector<std::string> words;

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

    // Open dictionary file

    dict = result["d"].as<std::string>();
    dictFile.open(dict);
    if (! dictFile.good() ) {
      std::cerr << "Can't read from dictionary file '"
                << dict << "'" << std::endl;
      exit(1);
    };

    // Validate input words

    if (result.count("positional")) {
      words = result["positional"].as<std::vector<std::string>>();
      for (const auto& word : words) {
        if (!validInput(word))
          std::cout << "Ignoring invalid input: " << word << std::endl;
      }

      words.erase(std::remove_if(words.begin(), words.end(),
                    [](const std::string & word){return !validInput(word);} ),
                  words.end());

      for (auto& word : words) {
        std::sort(word.begin(), word.end());
        word.erase(std::unique(word.begin(), word.end()),
                   word.end());
      }

      for (const auto& word : words) {
        pass(nmin, word, "");
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
