template<class Converter, typename T, typename ...Args>
T PAT::GetValue(const T& defaultValue, Args& ...args, Converter convert) const
{
	T result;
	return !children.empty() && convert(*children.rbegin(), result, args...) ? result : defaultValue;
}

template<class Converter, typename T, typename ...Args>
bool PAT::TryGetValue(T& output, Args& ...args, Converter convert) const
{
	return !children.empty() && convert(*children.rbegin(), output, args...);
}

template<class Converter, typename T, typename ...Args>
T PAT::Convert(const T& defaultValue, Args& ...args, Converter convert) const
{
	T result;
	return convert(*this, result, args...) ? result : defaultValue;
}

template<class Converter, typename T, typename ...Args>
bool PAT::TryConvert(T& output, Args& ...args, Converter convert) const
{
	return convert(*this, output, args...);
}
