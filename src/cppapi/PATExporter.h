#ifndef PATEXPORTER_H
#define PATEXPORTER_H

#include "PAT.h"

struct PATExporter
{
	PATExporter(const char* whitespace = "\t");

	void operator()(PAT& tree, const std::string& filename);
	void operator()(PAT& tree, std::ostream& stream);

	private:
		std::string whitespace;
};

#endif // PATEXPORTER_H
