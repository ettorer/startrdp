#pragma once
#include <string>

class CStartRdp
{
public:
	CStartRdp();
	~CStartRdp();
	BOOL ParseCommandLine(int argc, char *argv[]);
	BOOL AddCredentials();
	BOOL StartAndWait();
	BOOL RemoveCredentials();
private:
	char* getString(char*str);
	std::string m_user;
	std::string m_password;
	std::string m_domain;
	std::string m_host;
};

