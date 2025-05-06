#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

// as per instructions: 
#include <ctime>
#include <cstdlib> 

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
			// largest string will be 28 letters - only has letters & 0-9 
			// upper case is same as lower case letters 

			// approach notes from instructions: 


			// convert upper case to lower case 
			std::string lowered = k; 
			// variable to store result 
			for (size_t i = 0; i < lowered.length(); i++){
				// loop through all chars in k 
				if (lowered[i] >= 'A' && lowered[i] <= 'Z'){
					// if upper case then lower it through ascii math 
					lowered[i] = lowered[i] + ('a' - 'A');
				}
			}


			// translate letter into val btwn a=0 and b=35, '0' = 26, '9' = 35
			std::vector<unsigned long long> converted(lowered.length()); 
			// use vector to store the converted string so we can access elements 
			for (size_t i = 0; i < lowered.length(); i++){
				converted[i] = letterDigitToNumber(lowered[i]);
				// loop trhough the length and call helper function 
			}


			// translate substring of 6 letters into unsigned long long with w[i]
			// put 0s in leading pos if substring shorter than 6 letters 
			// use the base conversion approach taught in class to avoid repeat pow() calls
			// indexing: digit at "end" is assigned to power 36^0, 2nd to last = 36^1, etc
			
			// if input longer than 6 letters: 
			// do above approach for LAST 6 letters then repeat up 
			// this process should result in sequence of no more than 5 integers: w[0]-w[4]

			unsigned long long w[5] = {0}; 
			// initialize array to store vals 
			//size_t length = converted.length(); 
			// determine length of word (must be less than 29)
			size_t ind = lowered.length(); 
			// ind variable to help iterate in batches of 6 
			size_t w_place = 4; 
			// keeps track of how many of the w[index] places have been filled 

			// general procces idea: 
			// must start from back of word 
			// but each 6 are filled in backwards 
			// so iterate through first 6, then reverse those 6 to fill in 
			//for (size_t i = 0; i < 29; i++){
				while (ind > 0){
					// loop while the word is not done 
				// 	unsigned long long one = converted[ind];
				// 	unsigned long long two = converted[ind-1];
				// 	unsigned long long three = converted[ind-2];
				// 	unsigned long long four = converted[ind-3];
				// 	unsigned long long five = converted[ind-4];
				// 	unsigned long long six = converted[ind-5];
				// 	w[w_place] = one + 36 * (two + 36 * (three + 36 * (four + 36 * (five + 36 * six))));
				// 	std::cout << w[w_place] << std::endl;
				// 	w_place--;
				// 	ind -= 6; 
				// }
				// if (ind > 0){
					unsigned long long val = 0; 
					// variable to track the value during calculation 
					unsigned long long power = 1; 
					// variable to track power 
					// WANT TO USE BASE CONVERSION APPROACH so this variable is needed 
					// AVOID CALLING POW 
					for (size_t j = 0; j < 6 && ind > 0; j++){
						// loop 6 times and while there is still letters left 
						val += converted[ind - 1] * power;
						// base conversion formula - to avoid pow & a little more efficient
						ind--; 
						power *= 36; 
						// update variables - power update so it increases as we move forward in the word - as mimiced in formula 
					}
					w[w_place--] = val; 
					// assign the value to the w array 
				}
			//} 
			// store those vals in array (unsigned long long)
			// place leading zeros in pos of w[i] if not enough chars to make use of all those vals
			// if (w_place != 0){
			// 	for (size_t k = w_place; k < 5; k++){
			// 		w[k] = 0; 
			// 	}
			// }


			// then hash the string: h(k) - conditions for what val 
			// constructor of hash takes a debug parameter 
			// --> if true, set above r vals for debugging, false = rand vals 
			HASH_INDEX_T final_hash = 0; 
			// variable for the final value we are returning 
			for (size_t a = 0; a < 5; a++){
				// loop through all indexes in w 
				final_hash += rValues[a] * w[a];
				// update variable 
				std::cout << w[a] << std::endl;
				// debugging comment lol
			}
			return final_hash; 
			// return the value 
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
			if (letter >= 'a' && letter <= 'z'){
				// if the letter is a lowercase 
				return letter - 'a'; 
				// ascii conversion formula 
			}
			else if (letter >= '0' && letter <= '9'){
				// if letter is a number 
				return letter - '0' + 26; 
				// conversion formula + 26 since after all the letters 
			}
			return 0; 
			// to avoid error 
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
