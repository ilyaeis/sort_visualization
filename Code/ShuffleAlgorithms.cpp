#include "ShuffleAlgorithms.hpp"

Shuffle generate_shuffle(bool start) {
	Shuffle shuffler;
	shuffler.type_of_shuffle = rand() % (shuffle_count - 2); // dont want to
	while (start && shuffler.type_of_shuffle == 2) { shuffler.type_of_shuffle = rand() % (shuffle_count - 2); }

	if (shuffler.type_of_shuffle <= shuffle_count - 2) { return shuffler; }
	return shuffler;
}
#include <iostream>
void start_shuffle(std::vector<int>* array, Shuffle shuffler) {
	switch (shuffler.type_of_shuffle) {
	case 0:
		random_shuffle_vector(array, true);
		break;
	case 1:
		reverse_random_shuffle_vector(array);
		break;
	case 2:
		vector_reverse(array);
		break;
	case 3:
		exit(-1);
	case 4:
		exit(-1);
	default:
		exit(-1);
	}
}

// --- edit vector

void random_shuffle_vector(std::vector<int>* array, bool draw) {
	for (int i = 0; i < (*array).size(); i++) {
		int first = i;
		int second = rand() % (*array).size();

		if (draw) {
			drawing(array, i, first, second);
		}
		swap(&(*array)[first], &(*array)[second]);
	}
}

void reverse_random_shuffle_vector(std::vector<int>* array) {
	for (int i = (*array).size() - 1; i >= 0; i--) {

		int first = i;
		int second = rand() % (*array).size();

		drawing(array, i, first, second);

		swap(&(*array)[first], &(*array)[second]);
	}
}

void vector_reverse(std::vector<int>* array) {
	for (int i = 0; i < (*array).size() / 2; i++) {

		drawing(array, i, i, (*array).size() - 1 - i);

		swap(&(*array)[i], &(*array)[(*array).size() - 1 - i]);
	}
}

// - extra edit vector
void custom_swap_shuffle_vector(std::vector<int>* array, int start, int end, int step, int x, int plus) {
	if (step > 0) {
		for (int i = start; i < end; i += step) {
			int first = i % (*array).size();
			int second = int(i * x + plus) % (*array).size();

			drawing(array, i, first, second);

			swap(&(*array)[first], &(*array)[second]);
		}
	}
	else {
		for (int i = start; i > end; i += step) {
			int first = i % (*array).size();
			int second = int(i * x) % (*array).size();

			drawing(array, i, first, second);

			swap(&(*array)[first], &(*array)[second]);
		}
	}
}

void custom_vector_reverse(std::vector<int>* array, int from, int to, int how_much, int step) {
	if (how_much > 0) {
		for (int i = 0; i < how_much; i += step) {

			drawing(array, i, from + i, to + i);

			swap(&(*array)[from + i], &(*array)[to + i]);
		}
	}
	else {
		how_much *= -1;
		for (int i = how_much; i > 0; i--) {

			drawing(array, i, from + i, to - i);

			swap(&(*array)[from + i], &(*array)[to - i]);
		}
	}
}
