#include "PATImporter.h"

#include <fstream>
#include <iostream>

using std::clog;
using std::getline;
using std::ifstream;
using std::isblank;
using std::istream;
using std::string;

void PATImporter::operator()(PAT& tree, const string& filename)
{
	tree.Clear();
	ifstream stream(filename);

	if (stream.is_open())
		operator()(tree, stream);
	else
		clog << "Could not open PAT file: " << filename << '\n';
}

void PATImporter::operator()(PAT& tree, istream& stream)
{
	while (stream)
	{
		// Read indentation
		string whitespace;
		while (isblank(stream.peek()))
			whitespace += stream.get();

		// Read token
		string token;
		getline(stream, token);

		if (whitespace.length() || token.length())
		{
			PAT* parent = &tree;
			PAT* target = nullptr;
			while (target == nullptr)
			{
				if (parent->children.size() > 0)
				{
					auto&& lastChild = *parent->children.rbegin();
					if (whitespace.size() > lastChild.whitespace.size()
						&& whitespace.substr(0, lastChild.whitespace.size()) == lastChild.whitespace)
					{
						parent = &lastChild;
					}
					else
					{
						target = parent;
					}
				}
				else if (parent != &tree || whitespace.size() == 0)
				{
					target = parent;
				}
				else
					break;
			}

			if (target)
				target->children.emplace_back(PAT{ whitespace, token, {} });
		}
	}
}
