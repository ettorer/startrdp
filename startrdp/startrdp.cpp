/**
* startrdp: 
*
* Copyright 2018 Ettore Roberto Rizza
*
* This code is licensed under MIT license (see LICENSE.md for details)
*/
// startrdp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CStartRdp.h"

int usage(char *argv[])
{
	printf("startrdp - Command line utility to autologin mstsc connections\r\n");
	printf("See www.flydesktop.com for more information\n");
	printf("\r\n");
	printf("Usage: %s [options] [/v:<server>[:port]]\n", argv[0]);
	printf("\n");
	printf("Syntax:\n");
	printf("    /u:<value> username (just username or username@dmain.com or DOMAIN\\username)\r\n");
	printf("    /p:<value> user password\n\n");
	printf("    /d:<value> optional domain\r\n");
	printf("    /v:<value> server hostname\r\n");
	return 1;
}

int main(int argc, char *argv[])
{
	CStartRdp rdp;

	if (!rdp.ParseCommandLine(argc, argv))
		return usage(argv);

	if (rdp.AddCredentials()) {
		rdp.StartAndWait();			//Start mstsc and wait its exit
		rdp.RemoveCredentials();
	}
    return 0;
}

