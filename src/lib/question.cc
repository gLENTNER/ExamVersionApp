//
// question.cc
// ExamVersionApp
//
// This is the source file for the "question" object used in the 
// "exam" application. This object takes a file path to a 
// "question" file and reads in the data and stores the raw
// and shuffled form.
//
// Copyright (C) Geoffrey Lentner 2014. All rights reserved.
// See LICENCE file. (GPL v2.0)
//
// contact: Geoffrey Lentner, B.S.
//          Graduate Student / Researcher
//          LL15 Natural Science Building
//          Department of Physics & Astronomy
//          University of Louisville
//          Louisville, KY 40292 USA
//
// email:   geoffrey.lentner@louisville.edu
//
// updated: 2015-02-06 10:27:04 EST
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <list>

#include "../inc/question.hh"
#include "../inc/mt19937.hh"

using namespace std;

// constructor takes path to "exam question" file
question::question( string fileNAME, bool shuffled ){

	// initialization flag set to false, thrown if otherwise
	read_failed = false;

	// open the file
	ifstream input( fileNAME.c_str( ) );

	if ( input.is_open( ) ){

		// string variable for getting a new line
		string newline; 
		
		// get the first line from the input file stream
		getline( input, newline );

		// seed stringstream with new line
		stringstream interpret( newline );

		// variable for the answer number
		string answer_str;

		// variables for including a figure
		figure_included = false; figure_path = "";

		// check first line for answer
		if ( !( interpret >> answer_str ) ||
				!( interpret >> answer ) ){

			cerr << "InputError: Trouble reading first line";
			cerr << " in file [" << fileNAME << "]!" << endl;

			read_failed = true;

		// check formatting
		} else if ( answer_str != "answer:" ||
				answer < 1 ){

			cerr << "InputError: Formatting trouble in file";
			cerr << " [" << fileNAME << "]. See the README file";
			cerr << " for information about constructing question";
			cerr << " files." << endl;

			read_failed = true;
		
		// look for figure path if included
		} else if ( ( interpret >> figure_path ) ){

			figure_included = true;

			if ( !( interpret >> figure_width ) ||
					!( interpret >> figure_label ) ){

				cerr << "InputError: In file [" << fileNAME;
				cerr << "], [" << figure_path << "] was";
				cerr << " provided as a file path to a figure";
				cerr << " but a figure width and/or figure";
				cerr << " label was not provided. See the README";
				cerr << " file for more information about";
				cerr << " constructing your .exam files.\n\n";
			}
		}

		// initialize question prompt string
		prompt = "\t\\item ";

		// get the question prompt
		while ( ( getline( input, newline ) ) ){

			string blank;
			stringstream checkif( newline );

			// exit when first blank line
			if ( !( checkif >> blank ) ) break;
			else prompt += newline + "\n\t";
		}

		// create initial list of answer options
		list<string> option_list; string this_option = "\t\t\\item ";

		// add to current response option or push back
		while ( ( getline( input, newline ) ) ){

			// variables for checking if we have a blank line
			string blank; stringstream checkif( newline );

			if ( !( checkif >> blank ) ){

				option_list.push_back( this_option );
				this_option = "\t\t\\item ";
				continue;

			} else this_option += newline + "\n\t\t";
		}

		// ensure the last item gets added to the list
		if ( this_option != "\t\t\\item " )
			option_list.push_back( this_option );

		// close the input file
		input.close( );

		// shuffle response options if requested
		if ( shuffled ){

			// create map with random numbers as keys
			map<double, string> option_map;

			// PRNG for keys
			mt19937 generator( (unsigned long long)( time(NULL) ) );

			// array of random numbers
			double order[ option_list.size( ) ];

			// likelihood of getting two identical number is small
			for ( int i = 0; i < option_list.size( ); i++ )
				order[i] = generator.random_real( );

			// iterator for random number array
			int iter = 0;

			// transfor to map shuffles questions
			for ( list<string>::iterator this_answer = option_list.begin( );
					this_answer != option_list.end( ); this_answer++ ){

				option_map[ order[ iter++ ] ] = *this_answer;
			}

			// clear list (will be re-made)
			option_list.clear( );

			// reset iterator, index from one ( not zero )
			iter = 1;

			// location of answer
			double location = order[ answer - 1];

			// now iterate over map to find new number for answer, and remake list
			for ( map<double,string>::iterator this_answer = option_map.begin( );
					this_answer != option_map.end( ); this_answer++ ){

				// check if PRN matches PRN of correct answer
				if ( this_answer -> first == location )
					answer = iter;

				// add back to list
				option_list.push_back( this_answer -> second );

				// increment iterator
				iter++;
			}
		}

		// build latex source for this exam question
		source = prompt;
		source += "\n\t\\begin{enumerate}[a)]\n\n";

		for ( list<string>::iterator this_answer = option_list.begin( );
				this_answer != option_list.end( ); this_answer++ ){

			source += *this_answer + "\n";
		}

		source += "\t\\end{enumerate}\n\n";

	// failed to open the file
	} else {

		cerr << "Failed to open file [" << fileNAME;
		cerr << "]!" << endl;

		read_failed = true;
	}
}

// retrieve latex source for this exam question
string question::get_tex( ){

	return source;
}

// retrieve answer
int question::get_answer( ){

	return answer;
}
