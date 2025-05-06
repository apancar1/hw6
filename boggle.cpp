#ifndef RECCHECK
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <random>
#include <iomanip>
#include <fstream>
#include <exception>
#endif

#include "boggle.h"

std::vector<std::vector<char> > genBoard(unsigned int n, int seed)
{
	//random number generator
	std::mt19937 r(seed);

	//scrabble letter frequencies
	//A-9, B-2, C-2, D-4, E-12, F-2, G-3, H-2, I-9, J-1, K-1, L-4, M-2, 
	//N-6, O-8, P-2, Q-1, R-6, S-4, T-6, U-4, V-2, W-2, X-1, Y-2, Z-1
	int freq[26] = {9,2,2,4,12,2,3,2,9,1,1,4,2,6,8,2,1,6,4,6,4,2,2,1,2,1};
	std::vector<char> letters;
	for(char c='A'; c<='Z';c++)
	{
		for(int i=0;i<freq[c-'A'];i++)
		{
			letters.push_back(c);
		}
	}
	std::vector<std::vector<char> > board(n);
	for(unsigned int i=0;i<n;i++)
	{
		board[i].resize(n);
		for(unsigned  int j=0;j<n;j++)
		{
			board[i][j] = letters[(r() % letters.size())];
		}
	}
	return board;
}

void printBoard(const std::vector<std::vector<char> >& board)
{
	unsigned int n = board.size();
	for(unsigned int i=0;i<n;i++)
	{
		for(unsigned int j=0;j<n;j++)
		{
			std::cout << std::setw(2) << board[i][j];
		}
		std::cout << std::endl;
	}
}

std::pair<std::set<std::string>, std::set<std::string> > parseDict(std::string fname)
{
	std::ifstream dictfs(fname.c_str());
	if(dictfs.fail())
	{
		throw std::invalid_argument("unable to open dictionary file");
	} 
	std::set<std::string> dict;
	std::set<std::string> prefix;
	std::string word;
	while(dictfs >> word)
	{
		dict.insert(word);
		for(unsigned int i=word.size()-1;i>=1;i--)
		{
			prefix.insert(word.substr(0,i));
		}
	}
	prefix.insert("");
	return make_pair(dict, prefix);
}

std::set<std::string> boggle(const std::set<std::string>& dict, const std::set<std::string>& prefix, const std::vector<std::vector<char> >& board)
{
	std::set<std::string> result;
	for(unsigned int i=0;i<board.size();i++)
	{
		for(unsigned int j=0;j<board.size();j++)
		{
			boggleHelper(dict, prefix, board, "", result, i, j, 0, 1);
			boggleHelper(dict, prefix, board, "", result, i, j, 1, 0);
			boggleHelper(dict, prefix, board, "", result, i, j, 1, 1);
		}
	}
	
	return result;
}

bool boggleHelper(const std::set<std::string>& dict, const std::set<std::string>& prefix, const std::vector<std::vector<char> >& board, 
								   std::string word, std::set<std::string>& result, unsigned int r, unsigned int c, int dr, int dc)
{
//add your solution here!
	// recursively search for words in the given board 
	// r and c arguments are the current position in the board 
	// dr and dc input rgument set the search direction 
	// don't change dr or dc while you search, use them to set r and c as you recurse to next pos 
	// recursivelt build up a word along the direction of search 
	// insert ONLY the longest word found in result set 
	// return type of bool should help you insert only the longest word found 

	// sol should be backtracking 
	// if adding a new letter along search direction does not make the word longer, 
	// then stop recursion 
	// you mayyy need to add a word to the result set at this point 
	// use prefix to see if a string is a prefix of a longer word(s)

	// board is like 2D array of chars 
	// access: board[x][y]

	// base case: 
	// reach the end of the board 

	if (r >= board.size() || c >= board.size()){
		// want to check to make sure we are still in bounds 
		// return false bc will reach end of board 
		return false; 
	}

	word += board[r][c]; 
	// update the word (after first if statement so it wont crash lol)

	if (prefix.find(word) == prefix.end() && dict.find(word) == dict.end()){
		// make sure the word is a prefix so it can be longer & exists 
		// return false if it isn't 
		// these are conditions to be met for BACKTRACKING APPROACH
		return false; 
	}

	if (boggleHelper(dict, prefix, board, word, result, r + dr, c + dc, dr, dc)){
		// call recursion 
		return true; 
	}

	// if recursion did not return a word that can find more 
	// so almost termination 
	if (dict.find(word) != dict.end()){
		// if the word is a word 
		result.insert(word);
		// insert it 
		return true; 
		// end 
	}

	return false; 
	// not true --> will go back one step & BACKTRACK 
}