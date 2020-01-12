CPPFLAGS=-std=c++1z -O2

all: spbee spbee-scores spbee-multi-scores pangram-words

enable-1-pangrams-without-s.txt: pangram-words
	./pangram-words -d ./enable1.txt | sed -e '/s/d' | cut -d' ' -f2 | sort | uniq > enable-1-pangrams-without-s.txt

