#ifndef PATCONVERTERS_H
#define PATCONVERTERS_H

#include "PAT.h"

namespace PATConverters
{
	/// \brief A simple converter to help convert PAT tokens to integers.
	struct ToInt
	{
		bool operator()(const PAT& tree, int& output) const;
		bool operator()(const PAT& tree, unsigned int& output) const;
	};

	/// \brief A simple converter to help convert PAT tokens to floats.
	struct ToFloat
	{
		bool operator()(const PAT& tree, float& output) const;
	};
}

#endif // PATCONVERTERS_H
