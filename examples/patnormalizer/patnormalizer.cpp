#include <iostream>
#include <fstream>
#include <PAT.h>
#include <PATImporter.h>
#include <PATExporter.h>

using std::cout;
using std::ofstream;
using std::string;

int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		cout <<
			"Usage: " << argv[0] << " input-file [output-file]\n"
			"Description: Normalizes the whitespace of a PAT file.\n"
			"The input file is overwritten if no output file is specified.";
	}
	else
	{
		PAT pat;
		PATImporter()(pat, argv[1]);
		PATExporter()(pat, argc > 2 ? argv[2] : argv[1]);
	}

	return 0;
}
