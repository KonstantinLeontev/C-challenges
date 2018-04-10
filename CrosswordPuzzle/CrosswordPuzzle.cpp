#include <iostream>
#include <vector>
#include <string>

enum DIRECTION { NONE, VERT, HORIZ };

void getWordsFromString(std::vector<std::string> &words, const std::string &hints) {
	int size = hints.size();
	std::string temp{};
	int firstChar{};
	for(int i = 0; i < size; i++) {
		if (hints[i] == ';') {
			// Copy substring to the temp string.
			temp = hints.substr(firstChar, i - firstChar);
			// Add temp string to the vector.
			words.push_back(temp);
			// Change first position to i value.
			firstChar = i + 1;
		}
	}
	temp = hints.substr(firstChar, hints.back());
	words.push_back(temp);
}

void puzzleSolution(
	std::vector<std::string> &crossword, const std::vector<std::string> &words, int &wordQuant, int wordCnt) {
	int charCnt{};
	DIRECTION pos = VERT;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (crossword[i][j] != '+') {
				// If there is a free place put char from word here.
				crossword[i][j] = words[wordCnt][charCnt];
				charCnt++;
				// If next place in the row is free:
				if (crossword[i][j + 1] != '+') {
					// Try to put next symbol there.
					continue;
				}
				// If next place in the column is free;
				if (crossword[i + 1][j] != '+') {
					// Try to put next symbol there:
					j++;
					break;
				}
			}
		}
	}
}

void recPuzzleSolution(
	std::vector<std::string> &crossword, // Main grid.
	const std::vector<std::string> &words, // Words to place into grid.
	int wordCnt, // Words counter.
	int wordCharCnt, // Word character counter.
	int i, // Row's number.
	int j, // Column's number.
	DIRECTION dir // direction - none, vert or horizontal.
) {
	// Function works until there are some place in grid and words to place in.
	if (i < 10 && j < 10 && wordCnt < words.size() && wordCharCnt < words[wordCnt].size()) {

		// Tere's no free place here.
		if (crossword[i][j] == '+') {
			// If current word haven't started yet.
			if (!wordCharCnt) {
				// If it's no last place in the row and current word isn't vertical.
				if (j < 9) {
					recPuzzleSolution(crossword, words, wordCnt, wordCharCnt, i, j + 1, dir);
				}
				// Else if it's no last row jump to the start of the next row.
				else if (i < 9) {
					recPuzzleSolution(crossword, words, wordCnt, wordCharCnt, i + 1, 0, dir);
				}
				//// Else try next word from beginning.
				//else {
				//	recPuzzleSolution(crossword, words, wordCnt + 1, 0, 0, 0, NONE);
				//}
			}
			// Else try next word from grid's beginning.
			else {
				recPuzzleSolution(crossword, words, wordCnt + 1, 0, 0, 0, NONE);
			}
		}

		// It's a free place to put char here.
		else if (crossword[i][j] == '-') {
			// Put char in that place.
			crossword[i][j] = words[wordCnt][wordCharCnt];

			// Check for next place on the right.
			if (j < 9 && crossword[i][j + 1] != '+' && (dir == NONE || dir == HORIZ)) {
				recPuzzleSolution(crossword, words, wordCnt, wordCharCnt + 1, i, j + 1, HORIZ);
			}
			// Check for the next place down.
			if (i < 9 && crossword[i + 1][j] != '+' && (dir == NONE || dir == VERT)) {
				recPuzzleSolution(crossword, words, wordCnt, wordCharCnt + 1, i + 1, j, VERT);
			}
			// Call the function again for the next word.
			recPuzzleSolution(crossword, words, wordCnt + 1, 0, 0, 0, NONE);
		}

		// Else we have to check for crossing with already placed words.
		else {

			// If current carachters are the same.
			if (crossword[i][j] == words[wordCnt][wordCharCnt]) {
				// If it's a first char in the current word.
				if (!wordCharCnt) {
					// Check for free positions to the right.
					if (j < 9 && crossword[i][j + 1] == '-') {
						// Go to the right to put next char there. Set direction flag to HORIZ.
						recPuzzleSolution(crossword, words, wordCnt, wordCharCnt + 1, i, j + 1, HORIZ);
					}
					// Check for position to the down.
					else if (i < 9 && crossword[i + 1][j] == '-') {
						// Go down and place next char there. Set direction flag to VERT.
						recPuzzleSolution(crossword, words, wordCnt, wordCharCnt + 1, i + 1, j, VERT);
					}
				}
				// Else if it's a last char in the current word.
				else if (wordCharCnt == words[wordCnt].size() - 1) {
					// Current word is complete and we can call next word.
					recPuzzleSolution(crossword, words, wordCnt + 1, 0, 0, 0, NONE);
				}

				// Else we have to check next position in the current direction.
				else {
					// If the word is horizontal and we have free place on the right.
					if (dir == HORIZ && j < 9 && crossword[i][j + 1] == '-') {
						recPuzzleSolution(crossword, words, wordCnt, wordCharCnt + 1, i, j + 1, dir);
					}
					// If word is vertical and we have free place down.
					else if (dir == VERT && i < 9 && crossword[i + 1][j] == '-') {
						recPuzzleSolution(crossword, words, wordCnt, wordCharCnt + 1, i + 1, j, dir);
					}
				}
			}

			// Else if this is a first char in the given word.
			else if (!wordCharCnt) {
				// Just go to the next.
				if (j < 9) {
					recPuzzleSolution(crossword, words, wordCnt, wordCharCnt, i, j + 1, dir);
				}
				else if (i < 9) {
					recPuzzleSolution(crossword, words, wordCnt, wordCharCnt, i + 1, 0, dir);
				}
			}

			// Else current word doesn't fit and we have to try next word.
			else {
				recPuzzleSolution(crossword, words, wordCnt + 1, 0, 0, 0, NONE);
			}
		}
	}
}

void crosswordPuzzle(std::vector<std::string> &crossword, std::string &hints) {
	// Make the vector with words in it.
	std::vector<std::string> words(0);
	getWordsFromString(words, hints);

	// Find the solution for this puzzle.
	int wordQuant = words.size();
	puzzleSolution(crossword, words, wordQuant, 0);
}

int main() {
	// Read crossword grid from the buffer.
	std::vector<std::string> crossword(10);
	std::vector<std::string> words;

	for (int i = 0; i < 10; i++) {
		std::cin >> crossword[i];
	}
	// Read words from the buffer.
	std::string hints;
	std::cin >> hints;

	// Fill crossword with given words.
	// crosswordPuzzle(crossword, hints);

	// Get words from hints input.
	getWordsFromString(words, hints);

	// Fill crossword with given words.
	recPuzzleSolution(crossword, words, 0, 0, 0, 0, NONE);

	// Output crossword onto the screen.
	std::cout << crossword[0]; // Just first line.
	for (int i = 1; i < 10; i++) {
		std::cout << '\n' << crossword[i];
	}
}