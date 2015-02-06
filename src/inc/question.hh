//
// question.hh
// ExamVersionApp
//
// This is the header file for the "question" object used in the 
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

#ifndef QUESTION_HH
#define QUESTION_HH

#include <string>

using namespace std;

class question {

	protected:

		// string for actual "question"
		string prompt;
	
	 	// latex source code for question and itemized answer options	
		string source;

		// integer value for answer to question
		int answer;

	public:

		// default constructor does nothing
		question( ){ }

		// constructor takes input file name and shuffle flag
		question( string fileNAME, bool shuffled = true );

		// destructor does nothing
		~question( ){ }

		// flag for checking valid construction
		bool read_failed;
	   
		// flag for figure
		bool figure_included;

		// file path and variables for figure if exists
		string figure_path, figure_width, figure_label;

		// public function for retrieving source
		string get_tex( );

		int get_answer( );
};

#endif
