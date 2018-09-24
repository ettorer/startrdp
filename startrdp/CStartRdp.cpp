#include "stdafx.h"
#include "CStartRdp.h"
#include <wincred.h>

CStartRdp::CStartRdp()
{
}

CStartRdp::~CStartRdp()
{
}

BOOL CStartRdp::ParseCommandLine(int argc, char *argv[])
{
	char*p;
	for (int i = 1; i < argc; i++) {
		p = argv[i];
		if (!p || strlen(p) < strlen("/v:x") || p[0] != '/' || p[2] != ':')
			return FALSE;
		p++;
		switch (*p) {
		case 'u':
			m_user = getString(p);
			break;
		case 'd':
			m_domain = getString(p);
			break;
		case 'p':
			m_password = getString(p);
			break;
		case 'v':
			m_host = getString(p);
			break;
		default:
			return FALSE;
		}
	}
	if(!m_host.size() || !m_user.size() || !m_password.size())
		return FALSE;
	return TRUE;
}

BOOL CStartRdp::AddCredentials()
{
	char TargetName[MAX_PATH];
	sprintf_s(TargetName, "TERMSRV/%s", m_host.c_str());
	DWORD cbCreds = sizeof(WCHAR)*(1 + m_password.size());
	WCHAR szPassword[MAX_PATH];
	size_t ReturnValue;
	mbstowcs_s(&ReturnValue, szPassword, m_password.c_str(), sizeof(szPassword));
	CREDENTIALA cred;
	memset(&cred, 0, sizeof(cred));
	cred.Type = CRED_TYPE_GENERIC;
	cred.TargetName = (LPSTR)TargetName;
	cred.CredentialBlobSize = cbCreds;
	cred.CredentialBlob = (LPBYTE)szPassword;
	cred.Persist = CRED_PERSIST_LOCAL_MACHINE;
	cred.UserName = (LPSTR)m_user.c_str();

	return ::CredWriteA(&cred, 0);
}

BOOL CStartRdp::StartAndWait()
{
	char szCmdLine[MAX_PATH];
	sprintf_s(szCmdLine, MAX_PATH, "mstsc /v:%s", m_host.c_str());
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	memset(&pi, 0, sizeof(pi));
	if (CreateProcessA(NULL, szCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		if (WAIT_OBJECT_0 == WaitForSingleObject(pi.hProcess, INFINITE)) {
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CStartRdp::RemoveCredentials()
{
	char TargetName[MAX_PATH];
	sprintf_s(TargetName, "TERMSRV/%s", m_host.c_str());
	return CredDeleteA(TargetName, CRED_TYPE_GENERIC, 0);
}

char*CStartRdp::getString(char*str)
{
	static char sz[MAX_PATH];
	int i = 0;
	char*p = str+2;
	memset(sz, 0, sizeof(sz));
	while (*p && *p != ' ') {
		sz[i++] = *p++;
	}
	return sz;
}