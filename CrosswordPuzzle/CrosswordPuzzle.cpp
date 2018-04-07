#include <iostream>
#include <vector>
#include <string>

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
}

void puzzleSolution(
	std::vector<std::string> &crossword, const std::vector<std::string> &words, int &wordQuant, int wordCnt) {
	int charCnt{};
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
	for (int i = 0; i < 10; i++) {
		std::cin >> crossword[i];
	}
	// Read words from the buffer.
	std::string hints;
	std::cin >> hints;

	// Fill crossword with given words.
	crosswordPuzzle(crossword, hints);

	// Output crossword onto the screen.
	std::cout << crossword[0]; // Just first line.
	for (int i = 1; i < 10; i++) {
		std::cout << '\n' << crossword[i];
	}
}