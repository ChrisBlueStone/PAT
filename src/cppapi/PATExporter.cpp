#include "PATExporter.h"

#include <fstream>
#include <iostream>

using std::clog;
using std::getline;
using std::isblank;
using std::ofstream;
using std::ostream;
using std::string;

PATExporter::PATExporter(const char* whitespace) : whitespace(whitespace) {}

void PATExporter::operator()(PAT& tree, const string& filename)
{
	tree.Clear();
	ofstream stream(filename);

	if (stream.is_open())
		operator()(tree, stream);
	else
		clog << "Could not open PAT file: " << filename << '\n';
}

void WritePAT(ostream& output, const PAT& tree, const string& whitespace, const string& leadingWhitespace = string())
{
	output << leadingWhitespace << tree.token << '\n';

	for (auto&& child : tree.children)
		WritePAT(output, child, leadingWhitespace + whitespace);
}

void PATExporter::operator()(PAT& tree, ostream& stream)
{
	for (auto&& child : tree.children)
		WritePAT(stream, child, whitespace);
}
