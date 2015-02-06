//
// exam.hh
// ExamVersionApp
//
// This is the header file for the "exam" object used in the 
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

#ifndef EXAM_HH
#define EXAM_HH

#include <list>
#include <string>

#include "question.hh"

class exam {

	protected:

		// "question" list for exam
		list<question*>* question_list;
		
		// number of questions to keep
		int keepers;

		// shuffle parameter
		bool shuffle;

	public:

		// default constructor does nothing
		exam( ){ }

		// primary constructor takes file paths
		exam( int argc, const char *argv[] );
		
		// destructor de-allocates memory
		~exam( );

		// write "questions.tex" file
		void write( );

		// convert numerals to alphabetic string
		string alphabetic( int, int );

		// flag for failure of construction
		bool failed;
};

#endif
