//
// exam.cc
// ExamVersionApp
//
// This is the source file for the "exam" object used in the 
// "exam" application. This object takes the file paths
// to the questions and builds the output exam.
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
#include <list>
#include <map>

#include "../inc/exam.hh"
#include "../inc/question.hh"
#include "../inc/mt19937.hh"

// defined contructor builds question list
exam::exam( int argc, const char *argv[] ){

	// assume not failed unless otherwise flagged
	failed = false;

	// set question list pointer to NULL
	question_list  = NULL;

	// check config file for shuffle status
	// and number of questions to keep
	ifstream config( "etc/exam.config" );

	if ( config.is_open( ) ){

		// temporary variable for shuffle parameter
		string init_shuffle;

		// try to get shuffle parameter
		if ( !( config >> init_shuffle ) ){

			cerr << "InputError: Failed to read shuffle ";
			cerr << "parameter from etc/exam.config!\n";

			failed = true;
		
		// try to get number of questions to keep
		} else if ( !( config >> keepers ) ){

			cerr << "InputError: Failed to read parameter";
			cerr << " for number of questions to keep from";
			cerr << " etc/exam.config!\n";
			
			failed = true;
		}

		// close config file
		config.close( );

		if ( failed ) return;

		// assign shuffle parameter
		if ( init_shuffle == "both" ) shuffle = true;
		else                          shuffle = false;

	// failed to open config file
	} else {

		cerr << "IOError: Failed to open etc/exam.config!" << endl;
	}

	// allocate memory for question list
	question_list = new list<question*>;

	// construct questions list
	for ( int i = 1; i < argc; i++ ){

		string fileNAME = string( argv[i] );

		question *this_question = new question( fileNAME, shuffle );

		if ( this_question -> read_failed ){

			cerr << "Error: From question [" << argv[i];
			cerr << "] ...\n\n";

			failed = true;
			return;

		} else {
		
			// add question to exam
			question_list -> push_back( this_question );
		}
	}

	// map random numbers to questions to shuffle
	mt19937 generator( (unsigned long long)( time(NULL) ) );

	// map for questions
	map<double, question*> question_map;

	// iterate over list and transfer to map using PRNG
	for ( list<question*>::iterator que_iter = question_list -> begin( );
			que_iter != question_list -> end( ); que_iter++ ){

		question *this_question = *que_iter;

		question_map[ generator.random_real( ) ] = this_question;
	}

	// clear list before reverse transfer
	question_list -> clear( );

	// now iterate over map and transfer back
	for ( map<double, question*>::iterator que_iter = question_map.begin( );
			que_iter != question_map.end( ); que_iter++ ){

		question_list -> push_back( que_iter -> second );
	}

	// delete map
	question_map.clear( );
}

// destructor clears memory
exam::~exam( ){

	if ( question_list -> size( ) ){

		for ( list<question*>::iterator que_iter = question_list -> begin( );
				que_iter != question_list -> end( ); que_iter++ ){

			question *this_question = *que_iter;

			if ( this_question ){

				delete this_question;
				this_question = NULL;
			}
		}

		question_list -> clear( );
	}

	if ( question_list ){

		delete question_list;
		question_list = NULL;
	}
}

// write questions to latex file
void exam::write( ){

	// check that we have questions to write
	if ( question_list && question_list -> size( ) ){

		// open the latex file
		ofstream output( "../tex/questions.tex" );

		// check that the file opened
		if ( output.is_open( ) ){

			// begin enumerate environment
			output << "\\begin{enumerate}[1.]\n\n";

			// iterator breaks loop if > keepers
			int iter = 1;

			// write questions
			for ( list<question*>::iterator que_iter = question_list -> begin( );
					que_iter != question_list -> end( ); que_iter++ ){

				if ( keepers < iter++ ) break;

				question *this_question = *que_iter;
				output << "\t\\bigskip\\bigskip\n\n";
				output << "\t\\begin{minipage}{\\linewidth}\n\n";
				output << this_question -> get_tex( );

				// include figure if available
				if ( this_question -> figure_included ){

					output << "\n\t\\begin{figure}[H]\n\n";
					output << "\t\t\\centering\n\n";
					output << "\t\t\\captionsetup{width=";
					output << this_question -> figure_width;
					output << "}\n\n\t\t\\includegraphics[width=";
					output << this_question -> figure_width;
					output << "]{" << this_question -> figure_path;
					output << "}\n\n\t\t\\caption{}";
					output << "\n\n\t\t\\label{";
					output << this_question -> figure_label;
					output << "}\n\n\t\\end{figure}\n\n";
				}

				output << "\t\\end{minipage}\n\n";
			}

			// close the enumerate environment
			output << "\\end{enumerate}";

			// close the output file
			output.close( );

		// file wasn't opened properly
		} else {

			cerr << "IOError: exam::write( ) failed to open";
			cerr << " tex/questions.tex!\n";
		}

	} else {

		cerr << "Error: Call to exam::write( ) failed";
		cerr << " because either the constructor failed";
		cerr << " or there were no question files found!" << endl;

		return;
	}

	// write exam answer key
	ofstream output( "../out/exam.answers" );

	// check that file is open
	if ( output.is_open( ) ){

		// max number of rows in text file
		int max_rows = 30;

		// create rows of text
		string *row = new string [ max_rows ];

		// initialize rows as blank strings
		for ( int i = 0; i < max_rows; i++ )
			row[i] = "";

		// iterator for writing to appropriate row
		int iter = -1;

		// question number
		int que_num = 1;

		// write answers to questions
		for ( list<question*>::iterator que_iter = question_list -> begin( );
				que_iter != question_list -> end( ); que_iter++ ){

			// check row number
			iter++; if ( iter > max_rows - 1 ) iter = 0;

			// get the question object
			question *this_question = *que_iter;

			// answer for this question
			int answer = this_question -> get_answer( );

			// write to row
			row[iter] = row[iter] + alphabetic( que_num++, answer );
		}

		// write header to answer key file
		output << "Answer Key: \n\n";

		// write rows to file
		for ( int i = 0; i < max_rows; i++ )
			output << row[i] << endl;

		// close file
		output.close( );

		// delete pointer to strings
		delete[] row;

	// failed to open file
	} else {

		cerr << "IOError: Failed to open out/<exam>.answers!\n\n";
	}
}

// function maps integers to alphabet
string exam::alphabetic( int number, int answer ){

	// map all the way to z
	map<int, string> alphabet;
	alphabet[1 ] = "a";
	alphabet[2 ] = "b";
	alphabet[3 ] = "c";
	alphabet[4 ] = "d";
	alphabet[5 ] = "e";
	alphabet[6 ] = "f";
	alphabet[7 ] = "g";
	alphabet[8 ] = "h";
	alphabet[9 ] = "i";
	alphabet[10] = "j";
	alphabet[11] = "k";
	alphabet[12] = "l";
	alphabet[13] = "m";
	alphabet[14] = "n";
	alphabet[15] = "o";
	alphabet[16] = "p";
	alphabet[17] = "q";
	alphabet[18] = "r";
	alphabet[19] = "s";
	alphabet[20] = "t";
	alphabet[21] = "u";
	alphabet[22] = "v";
	alphabet[23] = "w";
	alphabet[24] = "x";
	alphabet[25] = "y";
	alphabet[26] = "z";

	// convert number to string
	stringstream alpha_num; alpha_num << number;

	// write answer string
	string answer_str = "\t" + alpha_num.str( ) + ". ";
  
	if ( number < 10 ) {

		answer_str += 	"  ";

	} else if ( number < 100 ) {

		answer_str += " ";
	}	

	return answer_str + alphabet[ answer ];
}
