// Direct Cache F06.cpp contains the following:
//
// - a set of #define's representing the constants used for this code
// - the declaration of the memory array
// - the definition of the class DirectCache including the constructor
// - powerOfTwo() function which returns the power of two to an exponent
// - main() with the initialization of the memory array and the class

#include "stdafx.h"
#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

// The next two defines represent the size of the cache.  The first one
// is simply the number of words the cache can hold.  Dividing this by
// the number of words per block should give us the number of lines in 
// the cache.  The second one, MAXIMUM_NUMBER_OF_LINE_BITS, is really
// a fudge.  It is actually equal to log2(CACHE_SIZE), i.e., the number
// of bits that would be used for a line id if the block size were 
// equal to 1.  Every time you double the size of the block, you take
// away one bit from the number of line bits.


#define CACHE_SIZE	4096
#define MAXIMUM_NUMBER_OF_LINE_BITS	12
#define EIGHT_BIT_MASK 0xff

// This allows us to define the memory array.
#define NUMBER_OF_BITS_IN_ADDRESS	16


// pow() function, to calculate powers.

int powerOfTwo(int);

// We also need our memory.  Since this is declared dynamically, we need
// to initialize it as a pointer to an array of char.


char *memory;

/////////////////////////// class DirectCache ////////////////////////////
// This class defines a simulated direct cache of variable block size.  //
// It includes the constructor and prototypes for the functions         //
// requestMemoryAddress() and getPercentageOfHits().                    //
//////////////////////////////////////////////////////////////////////////


class DirectCache
{
public: 
    DirectCache(int number_of_word_bits);
    bool requestMemoryAddress(unsigned int address);
    unsigned int getPercentageOfHits(void);

private:
// Since the number of lines varies depending on the declared
// number of words per block, we need to dynamically declare an
// array to hold the tags and the block of words. The arrays 
// "tags" and "blocks" are declared in the constructor.
	int *tags;
	int *blocks;
	int number_of_hits;
	int number_of_memory_requests;
	int number_of_lines_in_cache;
	int number_of_words_in_block;
	unsigned int tag_mask;
	unsigned int line_mask;
	unsigned int word_mask;
};

DirectCache::DirectCache(int number_of_word_bits)
{
	int number_of_tag_bits, number_of_line_bits;

// With no memory accesses yet, initialize both the access and hit counters.
	number_of_memory_requests=0;
	number_of_hits=0;

// Initialize the cache arrays containing the tags and the blocks
	number_of_words_in_block = powerOfTwo(number_of_word_bits);
	number_of_lines_in_cache = CACHE_SIZE / number_of_words_in_block;
	tags = new (int[number_of_lines_in_cache]);
	blocks = new (int[number_of_lines_in_cache, number_of_words_in_block]);

// Now figure out how many bits there are for each component of the address, i.e.,
// how many bits for the tag and how many bits for the line id.
	number_of_line_bits = MAXIMUM_NUMBER_OF_LINE_BITS - number_of_word_bits;
	number_of_tag_bits = NUMBER_OF_BITS_IN_ADDRESS - number_of_line_bits - number_of_word_bits;

// From the number of bits in each address component, we can now create the 
// bit masks that will be used to pull apart an address.
	tag_mask=0;
	line_mask=0;
	word_mask=0;
	for(int i=0; i<number_of_tag_bits; i++)
		tag_mask=(tag_mask<<1)+1;

	for(int i=0; i<number_of_line_bits; i++)
	{
		tag_mask=tag_mask<<1;
		line_mask=(line_mask<<1)+1;
	}

	for(int i=0; i<number_of_word_bits; i++)
	{
		tag_mask=tag_mask<<1;
		line_mask=line_mask<<1;
		word_mask=(word_mask<<1)+1;
	}

	
// Initialize the tag arrays to ensure that we don't accidentally record
// a hit if the block has not actually been loaded yet.
	for(int i=0; i<number_of_lines_in_cache; i++)
		tags[i]=-1;
}


////////////////////////////// powerOfTwo() //////////////////////////////
// powerOfTwo() returns the value of two raised to the power exponent.  //
// Only allows for positive exponents.  If exponent is less than zero,  //
// an error condition of -1 is returned.                                //
//////////////////////////////////////////////////////////////////////////


int powerOfTwo(int exponent)
{
	int return_value;
	if(exponent < 0)
		return_value=-1;
	else if(exponent == 0)
		return_value = 1;
	else
	{
		return_value = 1;
		for (int i = 0; i < exponent; i++)
			return_value*=2;
	}
	return(return_value);
}

int _tmain(int argc, _TCHAR* argv[])
{
// Declare the array of memory.
	memory = new (char[powerOfTwo(NUMBER_OF_BITS_IN_ADDRESS)]);

// Ask user for the number of bits for the word id
	int number_of_bits_defining_block_size;
	cout << "Enter number of word id bits (1, 2, 3, 4, or 5): ";
	cin >> number_of_bits_defining_block_size;

// The int should truncate, but we need to limit the number of bits
// defining a block.
	if ((number_of_bits_defining_block_size > 0) && 
		(number_of_bits_defining_block_size < 6))
	{
// Declare the cache
		DirectCache* cache;
		cache = new DirectCache(number_of_bits_defining_block_size);

// This is a simple loop to initialize the memory array.  We're just
// putting computed garbage in here so that we can have something to
// examine/check when it comes time to verifying the operation of the
// cache.
		for (int i = 0; i < powerOfTwo(NUMBER_OF_BITS_IN_ADDRESS); i++)
			memory[i] = (65535 - i) & EIGHT_BIT_MASK;
	}
	else
	{
		cout << "Error: The number of bits entered must be 1, 2, 3, 4, or 5.";
	}
	return 0;
}