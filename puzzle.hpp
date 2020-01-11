#pragma once

// validate that string s is a valid pattern for the puzzle
// It is only allowed to consist of [a-zA-Z]
bool validInput(const std::string & s) {
  return all_of(s.begin(), s.end(), isalpha);
}

void downSortUniq(std::string & s) {
  // downcase all the characters in s
  std::transform(s.begin(), s.end(),
                 s.begin(), tolower);

  // sort them all and erase duplicates
  std::sort(s.begin(), s.end());
  s.erase(std::unique(s.begin(), s.end()),
          s.end());
};

void upSortUniq(std::string & s) {
  // upcase all the characters in s
  std::transform(s.begin(), s.end(),
                 s.begin(), toupper);

  // sort them all and erase duplicates
  std::sort(s.begin(), s.end());
  s.erase(std::unique(s.begin(), s.end()),
          s.end());
};

class puzzleTester {
public:
  puzzleTester(const std::string &input) :
    input(input), allLower(input) {

    // downcase, sort, and unique all the characters in allLower
    downSortUniq(allLower);

    // copy the uppercase (required) characters into req
    reqLower.resize(std::count_if(input.begin(), input.end(), isupper));
    std::copy_if(input.begin(), input.end(), reqLower.begin(), isupper);

    // downcase, sort, and unique all the characters in reqLower
    downSortUniq(reqLower);
  }

  // Test whether or not a given word 'test' can be made out of the given puzzle.
  // Assume the input 'test' has already been converted to lower case
  // and sorted
  bool pass(const std::string &test) const {
    return std::includes(    test.begin(),     test.end(),
                         reqLower.begin(), reqLower.end()) &&
           std::includes(allLower.begin(), allLower.end(),
                             test.begin(),     test.end());
  }

  unsigned int nmin = 4;

  // Compute the score for a word based on the NYT scoring rules
  unsigned int score(const std::string &word) const {
    if (word.length() < nmin)
      return 0;

    std::string sortedDownWord = word;
    // downcase, sort, and remove dups
    downSortUniq(sortedDownWord);

    if (!pass(sortedDownWord)) {
      return 0;
    };

    unsigned int score = (nmin == word.length()) ? 1 : word.length();

    if (all(sortedDownWord))
      score += allLower.length();

    return score;
  }
  
  // Functor interface
  bool operator()(const std::string &test) const {
    return pass(test);
  }

  // Test whether all letters appear in the word
  bool all(const std::string &test) const {
    return std::includes(    test.begin(),     test.end(),
                         allLower.begin(), allLower.end());
  }

  void debug() const {
    std::cout << "; input=" << input
              << "; allLower=" << allLower
              << "; reqLower=" << reqLower
              << std::endl;
  };

private:
  const std::string input; // the original input
  std::string allLower; // all the letters, lowercase, unique
  std::string reqLower; // all the required letters, lowercase, unique
};
