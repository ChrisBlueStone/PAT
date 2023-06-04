#include "PAT.h"

using std::string;

static const PAT empty;

bool PAT::TryGetValue(string& output) const
{
	if (children.empty())
		return false;
	else
	{
		output = children.back().token;
		return true;
	}
}

const string& PAT::GetValue(const string& defaultValue) const
{
	return children.empty() ? defaultValue : children.back().token;
}

const PAT& PAT::operator[](const string& token) const
{
	for (auto i = children.crbegin(); i != children.crend(); i++)
		if (i->token == token) return *i;

	return empty;
}

const PAT& PAT::operator[](const unsigned int i) const
{
	return i < children.size() ? children[i] : empty;
}

const PAT& PAT::Find(const string& token) const
{
	for (auto i = children.cbegin(); i != children.cend(); i++)
		if (i->token == token) return *i;
	return empty;
}

const PAT& PAT::Find(const string& token, const PAT& start) const
{
	for (auto i = children.cbegin(); i != children.cend(); i++)
	{
		if (&*i == &start)
		{
			for (; i != children.cend(); i++)
				if (i->token == token)
					return *i;
			return empty;
		}
	}
	return empty;
}

const PAT& PAT::Select(const string& token, const unsigned int i) const
{
	unsigned int count = i;
	for (auto&& child : children)
		if (child.token == token)
		{
			if (count)
				count--;
			else
				return child;
		}

	return empty;
}

void PAT::Clear()
{
	whitespace.clear();
	token.clear();
	children.clear();
}

bool PAT::operator==(const PAT& other) const
{
	return &other == this || (
		whitespace == other.whitespace
		&& token == other.token
		&& children == other.children
	);
}
