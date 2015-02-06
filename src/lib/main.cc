//
// main.cc
// ExamVersionApp
//
// main.cc is the main source file for the "exam" application.
// This main program takes the arguments past to it and creates
// the new "exam" object
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
#include "../inc/question.hh"
#include "../inc/exam.hh"

using namespace std;

int main( int argc, const char *argv[] ){

	// create exam
	exam *this_exam = new exam( argc, argv );

	// if exam was built properly, write exam
	if ( this_exam -> failed ){

		cerr << "Error: Exam was not constructed";
		cerr << " properly ... \n\n";

	} else this_exam -> write( );
	
	// delete exam
	delete this_exam;

	return 0;
}
