# Solver for Frank Longo's "Spelling Bee" puzzles

Longo's "Spelling Bee" puzzle comes out in the NYT weekend variety puzzles. It presents a circle of 7 letters, with six of them arranged in a hexagon, and the seventh at the center; for example:

```
  A C
 T M I
  N L
```
The instructions read:
> How many common words of 5 or more letters can you spell using the letters in the hive? Every answer must use the center letter at least once. Letters may be reused in a word. At least one word will use all 7 letters. Proper names and hyphenated words are not allowed. Score 1 point for each answer, and 3 points for a word that uses all 7 letters.

The code in this repo gives a very simple solver. Run `make` (with any modern C++ compiler) and you will get a binary named `spbee`. Give it the string of letters in the puzzle, with the required (central) letter capitalized. For example, for the above puzzle, you would run
```
./spbee actMinl
```
or any permutation thereof (as long as the capitalization doesn't change).  Words that use all 7 letters will have an asterisk (*) to the left in the output.

`spbee` assumes that the dictionary file is located at `/usr/share/dict/words`. You can point to a different dictionary file with `-d` or `--dict`.

`spbee` will also allow you to solve generalizations of Longo's puzzle.  You can provide more or less than 7 letters; you can deman more or less than 1 mandatory letter by changing the capitalization.  You can change the minimum allowed solution length by using `-n` or `--nmin` (default is 5).

Finally, if you provide more than 1 string (delimited by spaces) on the command line, `spbee` will solve all of the puzzles at once, reporting all of the results (inclusive or).
