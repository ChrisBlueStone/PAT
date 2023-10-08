#include <PAT.h>
#include <PATConverters.h>
#include <PATImporter.h>
#include <assert.h>
#include <sstream>

using PATConverters::ToFloat;
using PATConverters::ToInt;
using std::stringstream;

void TestGetValues(const PAT&);
void TestTryGetValues(const PAT&);
void TestConvert(const PAT&);
void TestTryConvert(const PAT&);

int Converters(int argc, char **argv)
{
	stringstream ss(
		"Integers\n"
		"	5\n"
		"	8\n"
		"	13\n"
		"Negative Integers\n"
		"	-1\n"
		"Floats\n"
		"	3.14159\n"
		"	1.414214\n"
	);

	PAT pat;
	PATImporter()(pat, ss);

	TestGetValues(pat);
	TestTryGetValues(pat);
	TestConvert(pat);
	TestTryConvert(pat);

	return 0;
}

void TestGetValues(const PAT& pat)
{
	bool defaultIntegerRetrieved =
		pat.GetValue<ToInt>(42) == 42
		&& pat.GetValue<ToInt>(5) == 5;
	bool defaultFloatRetrieved =
		pat.GetValue<ToFloat>(3.14159f) == 3.14159f
		&& pat.GetValue<ToFloat>(1.234f) == 1.234f;

	bool finalIntRetrieved =
		pat["Integers"].GetValue<ToInt>(0) != 0
		&& pat["Integers"].GetValue<ToInt>(0) == 13;
	bool finalFloatRetrieved =
		pat["Floats"].GetValue<ToFloat>(0.0f) != 0.0f
		&& pat["Floats"].GetValue<ToFloat>(0.0f) == 1.414214f;

	assert(defaultIntegerRetrieved);
	assert(defaultFloatRetrieved);
	assert(finalIntRetrieved);
	assert(finalFloatRetrieved);
}

void TestTryGetValues(const PAT& pat)
{
	{
		unsigned int i = 42, j = 1;
		bool tryGetToIntFails = !pat.TryGetValue<ToInt>(i) && !pat.TryGetValue<ToInt>(j);
		bool tryGetToIntValueUnchanged = i == 42 && j == 1;
		bool tryGetToIntSuccess = pat["Integers"].TryGetValue<ToInt>(i);
		bool tryGetToIntValueCorrect = i == 13;

		assert(tryGetToIntFails);
		assert(tryGetToIntValueUnchanged);
		assert(tryGetToIntSuccess);
		assert(tryGetToIntValueCorrect);
	}

	{
		int i;
		bool tryGetToUIntSuccess = pat["Negative Integers"].TryGetValue<ToInt>(i);
		bool tryGetToUIntValueCorrect = i == -1;

		assert(tryGetToUIntSuccess);
		assert(tryGetToUIntValueCorrect);
	}

	{
		float f = 123.0f, f2 = 0.0f;
		bool tryGetToFloatFails = !pat.TryGetValue<ToFloat>(f) && !pat.TryGetValue<ToFloat>(f2);
		bool tryGetToFloatValueUnchanged = f == 123.0f && f2 == 0.0f;
		bool tryGetToFloatSuccess = pat["Floats"].TryGetValue<ToFloat>(f);
		bool tryGetToFloatValueCorrect = f == 1.414214f;

		assert(tryGetToFloatFails);
		assert(tryGetToFloatValueUnchanged);
		assert(tryGetToFloatSuccess);
		assert(tryGetToFloatValueCorrect);
	}
}

void TestConvert(const PAT& pat)
{
	bool defaultIntegerConverted =
		pat.Convert<ToInt>(42) == 42
		&& pat.Convert<ToInt>(5) == 5;
	bool defaultFloatConverted =
		pat.Convert<ToFloat>(3.14159f) == 3.14159f
		&& pat.Convert<ToFloat>(1.234f) == 1.234f;

	bool intConverted =
		pat["Integers"][0].Convert<ToInt>(0) != 0
		&& pat["Integers"][0].Convert<ToInt>(0) == 5;
	bool floatConverted =
		pat["Floats"][0].Convert<ToFloat>(0.0f) != 0.0f
		&& pat["Floats"][0].Convert<ToFloat>(0.0f) == 3.14159f;

	assert(defaultIntegerConverted);
	assert(defaultFloatConverted);
	assert(intConverted);
	assert(floatConverted);
}

void TestTryConvert(const PAT& pat)
{
	{
		unsigned int i = 42, j = 1;
		bool tryConvertToIntFails = !pat.TryConvert<ToInt>(i) && !pat.TryConvert<ToInt>(j);
		bool tryConvertToIntValueUnchanged = i == 42 && j == 1;
		bool tryConvertToIntSuccess = pat["Integers"][0].TryConvert<ToInt>(i);
		bool tryConvertToIntValueCorrect = i == 5;

		assert(tryConvertToIntFails);
		assert(tryConvertToIntValueUnchanged);
		assert(tryConvertToIntSuccess);
		assert(tryConvertToIntValueCorrect);
	}

	{
		int i;
		bool tryConvertToUIntSuccess = pat["Negative Integers"][0].TryConvert<ToInt>(i);
		bool tryConvertToUIntValueCorrect = i == -1;

		assert(tryConvertToUIntSuccess);
		assert(tryConvertToUIntValueCorrect);
	}

	{
		float f = -123.0f, f2 = 0.0f;
		bool tryConvertToFloatFails = !pat.TryConvert<ToFloat>(f) && !pat.TryConvert<ToFloat>(f2);
		bool tryConvertToFloatValueUnchanged = f == -123.0f && f2 == 0.0f;
		bool tryConvertToFloatSuccess = pat["Floats"][0].TryConvert<ToFloat>(f);
		bool tryConvertToFloatValueCorrect = f == 3.14159f;

		assert(tryConvertToFloatFails);
		assert(tryConvertToFloatValueUnchanged);
		assert(tryConvertToFloatSuccess);
		assert(tryConvertToFloatValueCorrect);
	}
}
