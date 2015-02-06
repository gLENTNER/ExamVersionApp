//
// setup.cc
// ExamVersionApp
//
// This C++ script sets up the environment for making
// a new exam
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

// function for replacing strings within other strings
void replaceALL( string, string, string& );

int main( int argc, const char *argv[] ){

	// get the name of the exam
	string examNAME;
	cout << "\n Name of new exam: ";
	getline( cin, examNAME );

	// get the version name
	string examVERSION;
	cout << "\n Version (number/letter): ";
	getline( cin, examVERSION );

	// ask about shuffling
	string shuffle;
	cout << "\n Shuffle questions or questions+answers ([both]/questions): ";
	getline( cin, shuffle );
	if ( shuffle == "" || shuffle == "both" ){

		shuffle = "both";

	} else if ( shuffle != "questions" ){

		cerr << "That response was not understood!" << endl;
		return -1;
	}

	// how many questions to keep
	string keep_str; int keep_int = 0;
	cout << "\n How many questions would you like to include";
	cout << " ([" << argc - 1 << "]): ";
	getline( cin, keep_str );
	if ( keep_str == "" ){

		keep_int = argc - 1;

	} else {

		stringstream convert( keep_str );
		convert >> keep_int;

		if ( keep_int > argc - 1 || keep_int < 1 ){

			cerr << "The number of questions to keep must be";
			cerr << " a positive integer and less than the total";
			cerr << " number of exam question files contained in";
			cerr << " the que/ directory!" << endl;

			return -1;
		}
	}

	// write exam configuration to file
	ofstream configFILE( "etc/exam.config" );

	if ( configFILE.is_open( ) ){

		configFILE << shuffle  << endl;
		configFILE << keep_int << endl;
		configFILE.close( );

	} else {

		cerr << "Failed to open etc/exam.config!" << endl;
	}

	// file name is exam name without spaces
	string examFILE = examNAME + "_" + examVERSION;
	replaceALL( " ", "_", examFILE );
	replaceALL( "#", "_", examFILE );

	// read in makefile and replace exam name
	ifstream makefileIN( "etc/makefile" );
	string   make_file = "", newline;

	// get contents of makefile
	if ( makefileIN.is_open( ) ){

		while ( ( getline( makefileIN, newline ) ) )
			make_file += newline + "\n";

		makefileIN.close( );

	} else {

		cerr << "etc/makefile could not be opened!" << endl;
		return -1;
	}

	// replace exam name
	replaceALL( "<exam name>", examFILE, make_file );

	// write makefile
	ofstream makefileOUT( "makefile" );

	if ( makefileOUT.is_open( ) ){

		makefileOUT << make_file;
		makefileOUT.close( );

	} else {

		cerr << "Failed to open makefile!" << endl;
	}

	// latex doesn't recognize "#" alone
	replaceALL( "#", "\\#", examNAME );
	replaceALL( "#", "\\#", examVERSION );

	// search latex preamble for keywords
	ifstream preambleIN( "../tex/source/preamble.tex" );
	string   preamble_file;

	// get contents of preamble
	if ( preambleIN.is_open( ) ){

		while ( ( getline( preambleIN, newline ) ) )
			preamble_file += newline + "\n";

		preambleIN.close( );

	} else {

		cerr << "Failed to open tex/source/preamble.tex!" << endl;
	}

	// replace keywords
	replaceALL( "<exam name>",    examNAME,    preamble_file );
	replaceALL( "<exam version>", examVERSION, preamble_file );

	// write out preamble
	ofstream preambleOUT( "../tex/preamble.tex" );
	
	if ( preambleOUT.is_open( ) ){

		preambleOUT << preamble_file;
		preambleOUT.close( );

	} else {

		cerr << "Failed to open tex/preamble.tex!" << endl;
	}

	// search latex header for keywords
	ifstream headerIN( "../tex/source/header.tex" );
	string   header_file;

	// get contents of header
	if ( headerIN.is_open( ) ){

		while ( ( getline( headerIN, newline ) ) )
			header_file += newline + "\n";

		headerIN.close( );

	} else {

		cerr << "Failed to open tex/source/header.tex!" << endl;
	}

	// replace keywords
	replaceALL( "<exam name>",    examNAME,    header_file );
	replaceALL( "<exam version>", examVERSION, header_file );

	// write out header
	ofstream headerOUT( "../tex/header.tex" );
	
	if ( headerOUT.is_open( ) ){

		headerOUT << header_file;
		headerOUT.close( );

	} else {

		cerr << "Failed to open tex/header.tex!" << endl;
	}

	// get footer
	ifstream footerIN( "../tex/source/footer.tex" );
	string   footer_file = "";

	if ( footerIN.is_open( ) ){

		while ( ( getline( footerIN, newline ) ) )
			footer_file += newline + "\n";

		footerIN.close( );

	} else {

		cerr << "IOError: Failed to open tex/source/footer.tex!\n\n";
	}

	// replace keywords
	replaceALL( "<exam name>",    examNAME,    footer_file );
	replaceALL( "<exam version>", examVERSION, footer_file );

	// write out to footer
	ofstream footerOUT( "../tex/footer.tex" );

	if ( footerOUT.is_open( ) ){

		footerOUT << footer_file;
		footerOUT.close( );

	} else {

		cerr << "IOError: Failed to open tex/footer.tex!\n\n";
	}

	// instructions
	cout << "\n Type \"make exam\" to generate the document.\n\n";

	return 0;
}

// replace all instances of one string in another with alternative
void replaceALL( string search_str, string replace_str, string& input_str ){

	size_t pos = 0; 

	while ( ( pos = input_str.find(search_str, pos) ) != string::npos ){

		input_str.replace( pos, search_str.length( ), replace_str );
		pos += replace_str.length( );
	}
}
