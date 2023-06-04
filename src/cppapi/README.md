# PAT C++ API
The PAT C++ API provides a few simple but useful classes for managing PAT objects.

## `class PAT`
This class contains all the data for a single node, which includes leading whitespace, token value, and any child nodes. Included are various const methods for retrieving children, token values of children, and parsing/converting token values into primitives or objects.

Note: Unicode is not yet supported by the API.

----
### Members
#### `string whitespace`
The leading whitespace of the node. Only used by the `==` operator method and set by the `Clear()` method. Populated by the `PATImporter`.

#### `string token`
The token value of the node.

#### `vector<PAT> children`
A vector object of any child nodes.

----
### Methods
All methods are strickly helpers. Any operation that the helpers perform can also be performed manually on the members themselves as they are public.

#### `const string& GetValue(const string& defaultValue = string()) const`
Returns the string token of the last child of the current node. If no children exist, the `defaultValue` is returned instead.

#### `bool TryGetValue(string& output) const`
Assigns to `output` the value of the last child's token. Returns `true` if any children exist on the node, otherwise `false`.

#### `const PAT& operator[](const string& token) const`
Returns a reference to the last child whose token matches the `token` parameter. If no match is found, a reference to an empty PAT object is returned.

#### `const PAT& operator[](const unsigned int i) const`
Returns a reference to the n-th child as specified by the `i` parameter. If `i` is out of bounds, a reference to an empty PAT object is returned.

#### `const PAT& Find(const string& token, const PAT& start) const`
#### `const PAT& Find(const string& token) const`
Returns a reference to the first child whose token matches the `token` parameter. A PAT reference may be passed as the second parameter `start` to specify the child at which to begin searching. If no child is found, a reference to an empty PAT object is returned.

#### `const PAT& Select(const string& token, const unsigned int n) const`
Returns a reference to the n-th child out of the collection of children whose tokens match the `token` parameter. If no child is found, a reference to the empty PAT object is returned.

#### `T GetValue(const T& defaultValue, Args& ...args, Converter convert = Converter()) const`
Passes the last child to a converter function and returns the result. If no children exist or the conversion fails, the `defaultValue` parameter is returned.

#### `bool TryGetValue(T& output, Args& ...args, Converter convert = Converter()) const`
Passes the last child to a converter function and assigns the result to the `output` parameter. If there are no children or the conversion fails, `output` will remain unchanged and `false` is returned, otherwise `output` is assigned the result of the conversion and `true` is returned.

#### `T Convert(const T& defaultValue, Args& ...args, Converter convert = Converter()) const`
Passes the current node to a converter function and returns the result. If the conversion fails, the `defaultValue` parameter is returned.

#### `bool TryConvert(T& output, Args& ...args, Converter convert = Converter()) const`
Passes the current node to a converter function and assigns the result to the `output` parameter. If the conversion fails, `output` will remain unchanged and `false` is returned, otherwise `output` is assigned the result of the conversion and `true` is returned.

#### `void Clear()`
Clears the `whitespace`, `token`, and `children` members.

#### `bool operator==(const PAT& rhs) const`
Returns true if the `whitespace`, `token`, and `children` members match between the current node and `rhs`.

----
### Examples
The following example loads a plaintext file named `user.pat` and then parses from it a name, age, and list of children's names, similar to the following structure:
```
name
	John Smith
age
	27
children
	Alice
	Bob
	Charlie
```
```cpp
PAT pat;
PATImporter()(pat, "./user.pat");
User user;

for (auto&& node : pat) {
	if (node.token == "name") user.name = node.GetValue();
	else if (node.token == "age") user.age = node.GetValue<ToInt>(-1);
	else if (node.token == "children")
		for (auto&& child : node.children)
			user.children.emplace_back(child.token);

cout << "Name: " << user.name << '\n'
	<< "Age: " << user.age << '\n'
	<< "Children:\n";
for (auto&& name : user.children)
	cout << '\t' << name << '\n';
```
For parsing commonly used structures, see the Converter examples below.

----

## `struct PATImporter`
This is a helper class used for loading a PAT formatted file and populating a `PAT` object with its contents. There are two overloads for loading a PAT, one that takes a filename from which to load a PAT, and one that takes an `std::istream` object containing the PAT data to load. Internally the overload that takes a filename calls the method that takes the `istream` object.

Example usage:
```cpp
PAT config;
PATImporter()(config, "./config.pat");
```
```cpp
PAT config;
ifstream configFile("./config.pat");
PATImporter()(config, configFile);
```

----

## Converters
Converters are function objects which are useful for the `GetValue`, `TryGetValue`, `Convert`, and `TryConvert` template methods on PAT objects. A converter function is required to have at least two parameters, the first being a const reference to a PAT object and the second being a reference to a variable to where the result of the conversion should be assigned. A conversion function may have any number of additional parameters that may be passed in through the `GetValue`, `TryGetValue`, `Convert`, and `TryConvert` methods.

There are a couple Converter function objects included in the PATConverters namespace-- `ToInt` and `ToFloat`.

### Example usage of a converter function:
```cpp
PAT numbersList;
PATImporter()(numbersList, "./numbers.txt");

int i = numbersList.GetValue<ToInt>(0);
int j = numbersList[0].Convert<ToInt>(0);
float f, g;
if (numbersList.TryGetValue<ToFloat>(f))
	cout << "Converted float " << f '\n';
if (!numbersList[0].TryConvert<ToFloat>(g))
	cout << "Converted float " << g << '\n';
```

### Example of defining a custom converter function and usage.
Assuming that the node should have the following structure,
```
name
	John Smith
age
	27
children
	Alice
	Bob
	Charlie
```
then the data can be parsed into a `User` object with the following customer converter.
```cpp
struct ToUser {
	bool operator()(const PAT& node, User& user) const {
		User loadedUser;
		bool nameExists = false;
		bool ageValid = false;
		for (auto&& node : pat) {
			if (node.token == "name") nameExists = node.TryGetValue(loadedUser.name);
			else if (node.token == "age") ageValid = node.TryGetValue<ToInt>(loadedUser.age);
			else if (node.token == "children")
				for (auto&& childName : node.children)
					loadedUser.children.emplace_back(childName.token);
		}
		if (nameExists && ageValid) {
			user = std::move(loadedUser);
			return true;
		} else {
			return false;
		}
	}
};
```
This converter can then be used in the following way.
```cpp
PAT userConfig;
PATImporter()(userConfig, "user.pat");

User user;
if (userConfig.TryConvert<ToUser>(user))
	cout << "Name: " << user.name << '\n'
		<< "Age: " << user.age << '\n'
		<< "Children: " << user.children.size() << '\n';
else
	cout << "Could not parse user.\n";
```

### Example of a custom converter for comma delimited values
Sometimes it's useful to nest multiple values within a single node, in which case custom converters can be really handy for parsing such nodes into more complicated structures. For example, suppose a node contains a list of 3D vectors represented by three comma separated numbers, i.e. `2.45,4.8,1`. This is what a custom converter to parse this token may look like:
```cpp
struct ToVec3
{
	bool operator()(const PAT &node, vec3 &output) const
	{
		auto split1 = node.token.find(',');
		auto split2 = node.token.find(',', split1 + 1);
		if (split1 != string::npos && split2 != string::npos)
		{
			try
			{
				output = {
					stof(node.token.substr(0, split1)),
					stof(node.token.substr(split1 + 1, split2)),
					stof(node.token.substr(split2 + 1))
				};

				return true;
			}
			catch (...)
			{
				clog << "Failed to converte node to vec3\n";
			}
		}
		return false;
	}
}
```
