#include "PATConverters.h"

using std::stoi;
using std::stof;
using std::stoul;

bool PATConverters::ToInt::operator()(const PAT& tree, int& output) const
{
	try { output = stoi(tree.token); return true; }
	catch (...) {}

	return false;
}
bool PATConverters::ToInt::operator()(const PAT& tree, unsigned int& output) const
{
	try { output = stoul(tree.token); return true; }
	catch (...) {}

	return false;
}
bool PATConverters::ToFloat::operator()(const PAT& tree, float& output) const
{
	try { output = stof(tree.token); return true; }
	catch (...) {}

	return false;
}