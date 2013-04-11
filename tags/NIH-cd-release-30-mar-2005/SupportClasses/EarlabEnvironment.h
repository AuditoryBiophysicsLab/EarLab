#pragma once

class EarlabEnvironment
{
public:
	EarlabEnvironment();
	~EarlabEnvironment();

	void SetInputDirectoryPath(char *NewInputPath);
	const char *GetInputDirectoryPath(void);

	void SetOutputDirectoryPath(char *NewOutputPath);
	const char *GetOutputDirectoryPath(void);

private:
	void CopyStringContents(char **LocalString, const char *NewContents);
	char *InputDirectoryPath;
	char *OutputDirectoryPath;
};
