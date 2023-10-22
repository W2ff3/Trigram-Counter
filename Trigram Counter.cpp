// Wilson Lopez
// December 6, 2021

// Sources:
//https://blog.xrds.acm.org/2017/10/introduction-n-grams-need/
//https://www.w3schools.com/cpp/cpp_files.asp
//https://www.geeksforgeeks.org/cpp-program-read-file-word-word/
//https://www.javatpoint.com/cpp-string-c-str-function
//https://code4coding.com/c-check-whether-the-given-alphabet-is-upper-case-or-lowercase-2/
//https://www.asciitable.com/
//https://www.cplusplus.com/reference/string/string/erase/
//https://www.cplusplus.com/reference/vector/vector/erase/
//https://stackoverflow.com/questions/5289597/how-to-use-vectorpush-back-with-a-struct
//https://www.geeksforgeeks.org/csv-file-management-using-c/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

// Contains a string that contains the trigram (sequence of 3 words),
// and the number of appearances that, that specific trigram has in the file.
struct TriCount
{
	std::string trigram;
	unsigned int count = 0;
};

// This function stores the name of a .txt file that is in the same directory as the .cpp file (written by Cyril Harris).
void Compute_1_Filename(std::string& filename)
{
	WIN32_FIND_DATAA findfiledata;
	HANDLE hfind;
	std::string file_extension = "*.txt";
	std::string directory_file_ext = file_extension;
	hfind = FindFirstFileA(directory_file_ext.c_str(), &findfiledata);

	if (hfind != INVALID_HANDLE_VALUE)
		filename = findfiledata.cFileName;
	else
	{
		std::cout << "No file was present." << std::endl;
		exit(0);
	}
}


// Verifies that the parameter is a word, and modifies the string's characters according to its attributes.
std::string VerifyWord(std::string t)
{
	for (unsigned int i = 0; i < t.size(); i++)
	{
		// If the character in the word is UPPERCASE:
		if (t[i] >= 'A' and t[i] <= 'Z')
		{
			t[i] += 32;	// Add 32 to get the lowercase equivalent.
		}
		// If the character in the word is lowercase or a number from 0 to 9:
		else if ((t[i] >= 'a' and t[i] <= 'z') or (t[i] >= '0' and t[i] <= '9'))
		{
			t[i];	// Do nothing.
		}
		// Else, if it is not a valid letter or number:
		else
		{
			t.erase(t.begin() + i);	// Remove the character from the string
			i--;					// and decrement the loop's iterator.
		}
	}
	return t;
}

// Populates file_words vector with verified words from .txt file input.
void StoreFileWords(std::vector<std::string>& file_words, const std::string& foldername_filename)
{
	std::ifstream file_input;
	std::string temp;
	
	file_input.open(foldername_filename);

	while (file_input >> temp)
		file_words.push_back(VerifyWord(temp));
}

// Searches for trigrams inside file_words vector and pushes trigrams into the vector of TriCounts.
void SearchFileTrigrams(const std::vector<std::string>& words, std::vector<TriCount>& tri)
{
	const unsigned int w_num = words.size();

	if (w_num < 3) 
		std::cout << "There are no trigrams.";

	for (unsigned int i = 0; i < w_num; i++)
	{
		// Break loop if at the last trigram.
		if ((i + 1 == w_num - 1) and (i + 2 == w_num))
			break;

		// Push back new found trigrams (with a starting count of 1) to file_trigrams vector.
		tri.push_back({ words[i] + " " + words[i + 1] + " " + words[i + 2], 1 });

		// Look for more instances of already found trigrams, and increment their respective count variables.
		for (unsigned int j = i + 1; j < w_num; j++)
		{
			// Break if at the last trigram.
			if ((j + 1 == w_num - 1) and (j + 2 == w_num))
				break;
			// Increment count is another instance of ith trigram is found at another location.
			if (tri[i].trigram == words[j] + " " + words[j + 1] + " " + words[j + 2])
				tri[i].count++;
		}
	}
}

// Formats and outputs a .csv files with trigrams results.
void CreateCSV(const std::string foldername_filename, const std::vector<TriCount>& file_trigrams)
{
	std::ofstream file_output;

	file_output.open("Trigram Counts - " + foldername_filename + " Results.csv");
	file_output << "Trigram" << ", " << "Counts" << std::endl;

	for (unsigned int i = 0; i < file_trigrams.size(); i++)
	{
		file_output << file_trigrams[i].trigram << ", " << file_trigrams[i].count << std::endl;
	}

	file_output.close();
}

int main()
{
	std::string foldername_filename;
	std::vector<std::string> file_words;
	struct std::vector<TriCount> file_trigrams;

	Compute_1_Filename(foldername_filename);

	StoreFileWords(file_words, foldername_filename);

	SearchFileTrigrams(file_words, file_trigrams);

	CreateCSV(foldername_filename, file_trigrams);

	std::cout << "Counting Trigrams from: " << foldername_filename << std::endl;

	return 0;
}