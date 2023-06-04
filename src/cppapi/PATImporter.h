#ifndef PATImporter_H
#define PATImporter_H

#include "PAT.h"

struct PATImporter
{
	void operator()(PAT& tree, const std::string& filename);
	void operator()(PAT& tree, std::istream& stream);
};

#endif // PATImporter_H
