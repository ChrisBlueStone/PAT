// Plaintext Abstract Tree
// 2022 Christopher Salcido
#ifndef PAT_H
#define PAT_H

#include <string>
#include <vector>

// Plaintext Abstract Tree
struct PAT
{
	using string = std::string;

	string whitespace;
	string token;
	std::vector<PAT> children;

	/// \brief Assigns to \a output the value of the last child's token.
	/// \param output A reference to  the string to which to assign the last child's token value.
	/// \return Returns true if the node has any children, otherwise false.
	bool TryGetValue(string& output) const;

	/// \brief Returns the value of last child's token.
	/// If no children exist, a default string is returned.
	/// \param defaultValue The default string value to return if no children exist.
	/// \return The token value of the last child, or \a defaultValue if no children exist.
	const string& GetValue(const string& defaultValue = string()) const;

	/// \brief Returns a reference to the last child whose token matches \a token.
	/// If none match, a reference to an empty \see PAT object is returned.
	const PAT& operator[](const string& token) const;
	/// \brief Returns the n-th child as specified by \a i.
	/// If \a i is out of bounds, an empty \see PAT object is returned.
	const PAT& operator[](const unsigned int i) const;

	/// \brief Finds and returns the first child whose token matches \a token.
	/// If no child is found, an empty \see PAT object is returned.
	/// \param token The token for which to search in the child tokens.
	/// \param start A reference to the \see PAT object at which to begin the search.
	const PAT& Find(const string& token, const PAT& start) const;
	const PAT& Find(const string& token) const;

	/// \brief Finds and returns the Nth child out of all with a matching token. \a token.
	/// If no child is found, an empty \see PAT object is returned.
	/// \param token The token for which to search in the child tokens.
	/// \param n The Nth child of all matching children to return.
	const PAT& Select(const string& token, const unsigned int n) const;

	/// \brief Attempts to run the last child through a converter function and return the result.
	/// \tparam Converter The type of function object to use for conversion.
	/// \tparam T The expected type of the converter function.
	/// \tparam ...Args The types of any additional parameters required by the converter function.
	/// \param defaultValue The default value to return if no children exist or the conversion operation doesn't succeed.
	/// \param args... Additional arguments to pass to the converter function.
	/// \param convert The converter function to use to parse the child PAT node.
	/// \return If there are any children and the conversion succeeds, the result of the conversion is returned,
	/// otherwise `defaultValue` is returned
	/// 
	/// Example usage:
	/// \code{.cpp}
	/// int finalValue = pat.GetValue<ToInt>(0);
	/// \endcode
	/// \see TryGetValue
	/// \see Convert
	/// \see PATConverters::ToInt
	template<class Converter, typename T, typename ...Args>
	T GetValue(const T& defaultValue, Args& ...args, Converter convert = Converter()) const;

	/// \brief Attempts to run the last child through a converter function and returns true if successful.
	/// \tparam Converter The type of function object to use for conversion.
	/// \tparam T The expected type of the converter function.
	/// \tparam ...Args The types of any additional parameters required by the converter function.
	/// \param output A reference to the variable to be populated by the converter function..
	/// \param args... Additional arguments to pass to the converter function.
	/// \param convert The converter function to use to parse the child PAT node.
	/// \return If there are any children, returns the result of the convert function, otherwise false.
	/// 
	/// Example usage:
	/// \code{.cpp}
	/// int finalValue = 0;
	/// if (pat.TryGetValue<ToInt>(finalValue)) {
	/// 	cout << "Successfully parsed the value " << finalValue << '\n';
	/// } else {
	/// 	cout << "Failed to parse an int.\n";
	/// }
	/// \endcode
	/// \see GetValue
	/// \see TryConvert
	/// \see PATConverters::ToInt
	template<class Converter, typename T, typename ...Args>
	bool TryGetValue(T& output, Args& ...args, Converter convert = Converter()) const;

	/// \brief Passes the current node to a converter function and returns the converted type
	/// or a default value if it could not be converted.
	/// \tparam Converter The type of function object to use for conversion.
	/// \tparam T The expected type of the converter function.
	/// \tparam ...Args The types of any additional parameters required by the converter function.
	/// \param defaultValue The default value to return if the converter function returns false.
	/// \param args... Additional arguments to pass to the converter function.
	/// \param convert The converter function to use to parse the child PAT node.
	/// \return If there are any children and the conversion succeeds, the result of the conversion is returned,
	/// otherwise `defaultValue` is returned
	/// 
	/// Example usage:
	/// \code{.cpp}
	/// int value = pat.Convert<ToInt>(0);
	/// \endcode
	/// \see TryGetValue
	/// \see Convert
	/// \see PATConverters::ToInt
	template<class Converter, typename T, typename ...Args>
	T Convert(const T& defaultValue, Args& ...args, Converter convert = Converter()) const;

	/// \brief Passes the current node to a converter function and returns true if the conversion was successful.
	/// \tparam Converter The type of function object to use for conversion.
	/// \tparam T The expected type of the converter function.
	/// \tparam ...Args The types of any additional parameters required by the converter function.
	/// \param output A reference to the variable to be populated by the converter function..
	/// \param args... Additional arguments to pass to the converter function.
	/// \param convert The converter function to use to parse the child PAT node.
	/// \return Returns the result of the convert function.
	/// 
	/// Example usage:
	/// \code{.cpp}
	/// int value = 0;
	/// if (pat.TryConvert<ToInt>(value)) {
	/// 	cout << "Successfully parsed the value " << value << '\n';
	/// } else {
	/// 	cout << "Failed to parse an int.\n";
	/// }
	/// \endcode
	/// \see Convert
	/// \see TryGetValue
	/// \see PATConverters::ToInt
	template<class Converter, typename T, typename ...Args>
	bool TryConvert(T& output, Args& ...args, Converter convert = Converter()) const;

	/// \brief Clears the whitespace, token, and children.
	void Clear();

	bool operator==(const PAT& rhs) const;
};


/// \class PAT
/// \brief A PAT structure. Consists of whitespace, a token, and child PAT objects.
/// PATs or Plaintext Abstract Tree are a human-readable data format that is designed
/// to be as simple as can be, using no formatting syntax such as braces, brackets,
/// colons, etc. save only for leading whitespace to signify parent-child and
/// sibling relationships. This makes them simple and easy for humans to read, and
/// very simple and fast for computers to parse.
///
/// PAT files are text-only data files with data organized as a tree.
/// Each line of the file denotes a node of the tree. Any whitespace at the beginning
/// of each line is ignored except for the purpose of determining if the node is a child
/// of a previous node or not. If a node is indented further than any previous line, then
/// it is a child node of the latest line that has less indentation. This simple format
/// creates an emphasis on the data by not requiring any extraneous characters to signify
/// syntax or formatting. If leading whitespace is desired to be part of the token, then
/// it is recommended that the token starts with a leading character that is to be ignored
/// during parsing. Because the format is text-based and relies on newline and whitespace
/// characters, binary data cannot be stored directly. Instead it is recommended that
/// binary data is either stored in a separate file and a reference to it is kept in a
/// token so that the binary data may be fetched during parsing, or the binary data should
/// be base64 encoded.
///
/// Here is an example of a PAT file.
/// \code{.pat}
/// name
/// 	John Smith
/// age
/// 	27
/// children
/// 	Alice
/// 	Bob
/// 	Charlie
/// \endcode
/// Such a file may be parsed into a PAT structure via the \a PATImporter class.
/// Once a PAT object has been loaded, the data can easily be accessed via the various
/// helper methods defined in the PAT class.
///
/// Example of loading a PAT file and populating an object with the data from it.
/// \code{.cpp}
/// PAT pat;
/// PATImporter()(pat, "./user.pat");
/// for (auto&& node : pat) {
/// 	if (node.token == "name") user.name = node.GetValue();
/// 	else if (node.token == "age") user.age = node.GetValue<ToInt>(-1);
/// 	else if (node.token == "children")
/// 		for (auto&& child : node.children)
/// 			user.children.emplace_back(child.token);
/// }
/// \endcode
///
/// Example of trying to find a specific piece of data from a PAT object.
/// \code{.cpp}
/// PAT& specificNode = pat["children"][1];
/// if (specificNode.token.empty()) cout << "User doesn't have a child named Dan.\n";
/// else cout << "User's second child is " << specificNode.token << '\n';
/// \endcode
///
/// Sometimes the same sort of data has to be parsed many times. Instead of parsing it manually,
/// a customer converter can instead be written and used repititious structures.
/// Example of a custom converter that can parse a PAT into an object directly.
/// \code{.cpp}
/// struct ToUser {
/// 	bool operator()(const PAT& node, User& user) const {
/// 		User loadedUser; bool nameExists = false, ageValid = false;
/// 		for (auto&& node : pat) {
/// 			if (node.token == "name") nameExists = node.TryGetValue(loadedUser.name);
/// 			else if (node.token == "age") ageValid = node.TryGetValue<ToInt>(loadedUser.age);
/// 			else if (node.token == "children")
/// 				for (auto&& childName : node.children)
/// 					loadedUser.children.emplace_back(childName.token);
/// 		}
///
/// 		if (nameExists && ageValid) {
/// 			user = std::move(loadedUser);
/// 			return true;
/// 		} else {
/// 			return false;
/// 		}
/// 	}
/// };
/// \endcode
/// And to use it--
/// \code{.cpp}
/// User user;
/// if (!pat.TryConvert<ToUser>(user)) cout << "Failed to parse user.\n";
/// \endcode
///

#include "PAT.inl"

#endif // PAT_H
