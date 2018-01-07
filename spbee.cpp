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

class puzzleTester {
public:
  puzzleTester(int nmin, const std::string &input) :
    nmin(nmin), input(input), allLower(input) {

    // downcase all the characters in allLower
    std::transform(allLower.begin(), allLower.end(),
                   allLower.begin(), tolower);

    // sort them all and erase duplicates
    std::sort(allLower.begin(), allLower.end());
    allLower.erase(std::unique(allLower.begin(), allLower.end()),
                   allLower.end());

    // copy the uppercase (required) characters into req
    int nreq = std::count_if(input.begin(), input.end(), isupper);
    reqLower.assign(nreq, 'A');
    std::copy_if(input.begin(), input.end(), reqLower.begin(), isupper);

    // downcase them
    std::transform(reqLower.begin(), reqLower.end(),
                   reqLower.begin(), tolower);

    // sort them all and erase duplicates
    std::sort(reqLower.begin(), reqLower.end());
    reqLower.erase(std::unique(reqLower.begin(), reqLower.end()),
                   reqLower.end());

  }

  bool pass(const std::string &test) const {
    if (test.length() < nmin)
      return false;



    return true;
  }

  void report() const {
    std::cout << "nmin=" << nmin << "; input=" << input
              << "; allLower=" << allLower
              << "; reqLower=" << reqLower
              << std::endl;
  };

private:
  const int nmin; // the minimum length of a word to pass the test
  const std::string input; // the original input
  std::string allLower; // all the letters, lowercase, unique
  std::string reqLower; // all the required letters, lowercase, unique
};

int main(int argc, char* argv[]) {
  try {

    int nmin;
    std::string dict;
    std::ifstream dictFile;
    std::vector<std::string> words;
    std::vector<puzzleTester> puzzleTesters;

    cxxopts::Options options(argv[0], "Spelling Bee puzzle solver");
    options
      .positional_help("[optional args]")
      .show_positional_help();

    options.add_options()
      ("d,dict", "File", cxxopts::value<std::string>()
       ->default_value("/usr/share/dict/words"), "FILE")
      ("n,nmin", "An integer", cxxopts::value<int>(nmin)
       ->default_value("5"), "N")
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

      for (const auto& word : words) {
        puzzleTesters.push_back(puzzleTester(nmin, word));
        puzzleTesters.back().report();
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
