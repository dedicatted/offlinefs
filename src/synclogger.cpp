/***************************************************************************
 *   Copyright (C) 2007 by                                                 *
 *                 Frank Gsellmann, Tobias Jaehnel, Carsten Kolassa        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "synclogger.h"

#include "synclogentry.h"
#include "confuse.h"
#include "ofsexception.h"
#include "ofsenvironment.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <assert.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define FILE_PATH_VARNAME "filePath"
#define MOD_TIME_VARNAME "modTime"
#define MOD_TYPE_VARNAME "modType"
#define MOD_NUMBER_VARNAME "modNumber"

#define FILE_PATH_DEFAULT "C:\\WINDOWS\\SYSTEM\\win.ini"
#define MOD_TIME_DEFAULT "0000/00/00 25:00:00"
#define MOD_TYPE_DEFAULT "e"


// Initializes the class attributes.
std::auto_ptr<SyncLogger> SyncLogger::theSyncLoggerInstance;
Mutex SyncLogger::m_mutex;

//////////////////////////////////////////////////////////////////////////////
// CONSTRUCTION/ DESTRUCTION
//////////////////////////////////////////////////////////////////////////////

SyncLogger::SyncLogger()
{
//    m_pFile = fopen("C:\\Sync.log", "a");
	m_pCFG = NULL;
}

SyncLogger::~SyncLogger()
{
//    fclose(m_pFile);
}

SyncLogger& SyncLogger::Instance()
{
    MutexLocker obtainLock(m_mutex);
    if (theSyncLoggerInstance.get() == 0)
    {
        theSyncLoggerInstance.reset(new SyncLogger);
    }

    return *theSyncLoggerInstance;
}

bool SyncLogger::AddEntry(const char* pszHash,
						  const char* pszFilePath,
						  const char chType)
{
    // modNumber 123
	// {
	//     filePath = ABC/XYZ.xyz
	//     modTime = 00/00/00 25:00:00
	//     modType = ?
	// }

    //oreiche
    // every file needs only ONE syncentry
    // depending on earlier entries
    string strFilePath = pszFilePath;
    char newType = getModDependingOnOtherEntries(pszHash, strFilePath, chType);
    if (newType == 'x') //nothing to do
	return false;

    // Creates the new entry.
    char szEntry[MAX_PATH + 1024];
    char *szIndex = itoa(m_nNewIndex, 10);
    char strtype[2];
    strtype[0] = newType;//chType;
    strtype[1] = '\0';

    // Writes the new entry.
    strcpy(szEntry, MOD_NUMBER_VARNAME);
    strcat(szEntry, " ");
    strcat(szEntry, szIndex);
    strcat(szEntry, "\n\{\n\t");
    strcat(szEntry, FILE_PATH_VARNAME);
    strcat(szEntry, " = \"");
    strcat(szEntry, pszFilePath);
    strcat(szEntry, "\"\n\t");
    strcat(szEntry, MOD_TIME_VARNAME);
    strcat(szEntry, " = ");
    strcat(szEntry, itoa(time(NULL),10));
    strcat(szEntry, "\n\t");
    strcat(szEntry, MOD_TYPE_VARNAME);
    strcat(szEntry, " = ");
    strcat(szEntry, strtype);
    strcat(szEntry, "\n}\n\n");
//    strncpy(&szEntry[8], (char*)&szTime, sizeof(time_t)); // sizeof(FILETIME));
/*
	// Creates the file name of the sync log.
	char szLogName[MAX_PATH];
	strcpy(szLogName, "Sync_");
	strcat(szLogName, pszHash);
	strcat(szLogName, ".log");

	// Opens the sync log file.
	FILE* pFile = fopen(szLogName, "a");
	if (pFile == 0)
		return false;
*/
	// Opens the sync log file.
	FILE* pFile = OpenLogFile(pszHash, "a");
	if (pFile == NULL)
		return false;

	// Writes the new entry into the sync log.
//    fwrite(szEntry, sizeof(char), 32 + strlen(szEntry), pFile);
    fwrite(szEntry, sizeof(char), strlen(szEntry), pFile);
    fclose(pFile);

    m_nNewIndex++;

    return true;
}

//bool SyncLogger::ReadFirstEntry(const char* pszHash, char* pszFilePath, char& chType)
SyncLogEntry SyncLogger::ReadFirstEntry(const char* pszHash)
{
	ParseFile(pszHash);

	// Assures the correct parsing of the file.
	assert(m_pCFG != NULL);

//	cfg_t* pEntryCFG = cfg_getnsec(m_pCFG, MOD_NUMBER_VARNAME, 0);
	return ReadEntry(cfg_getnsec(m_pCFG, MOD_NUMBER_VARNAME, 0));
/*
	// Assures that the section has been found, which means there is at least one section.
    assert(pEntryCFG != NULL);

	// TODO: Copy values
	SyncLogEntry sle;
	sle.m_strFilePath = cfg_getstr(pEntryCFG, FILE_PATH_VARNAME);
	sle.m_strModTime = cfg_getstr(pEntryCFG, MOD_TIME_VARNAME);
	string strModType = cfg_getstr(pEntryCFG, MOD_TYPE_VARNAME);
	if (strModType.length() != 1)
	{
		// The log entry does not meet the standard.
		return NULL;
	}
	sle.m_chModType = strModType.c_str()[0];

    return sle;
*/
}

SyncLogEntry SyncLogger::ReadEntry(cfg_t* pEntryCFG)
{
	// Assures that the section has been found, which means there is at least one section.
    assert(pEntryCFG != NULL);

	// TODO: Copy values
	string strModType = cfg_getstr(pEntryCFG, MOD_TYPE_VARNAME);
	if (strModType.length() != 1)
	{
		// The log entry does not meet the standard.
		throw OFSException("The log entry does not meet the standard", 1);
	}
	int nModNumber = atoi(cfg_title(pEntryCFG));
	SyncLogEntry sle(cfg_getstr(pEntryCFG, FILE_PATH_VARNAME),
		cfg_getstr(pEntryCFG, MOD_TIME_VARNAME), strModType.c_str()[0],
		nModNumber);

    return sle;
}

bool SyncLogger::ParseFile(const char* pszHash)
{
	if (strcmp(m_szCurShare, pszHash) == 0)
	{
		// File is currently parsed, there is no need to parse it again.
		return true;
	}

    cfg_opt_t modEntry[] =
    {
        // Parses within the group.
        CFG_STR(FILE_PATH_VARNAME, FILE_PATH_DEFAULT, CFGF_NONE),
        CFG_STR(MOD_TIME_VARNAME, MOD_TIME_DEFAULT, CFGF_NONE),
        CFG_STR(MOD_TYPE_VARNAME, MOD_TYPE_DEFAULT, CFGF_NONE),
        CFG_END()
    };
    cfg_opt_t entries[] =
    {
        // Parses the single groups.
        CFG_SEC(MOD_NUMBER_VARNAME, modEntry, CFGF_TITLE | CFGF_MULTI),
        CFG_END()
    };

    // Initializes the parser.
//    cfg_t* pCFG = cfg_init(entries, CFGF_NONE);
    m_pCFG = cfg_init(entries, CFGF_NONE);

    // Parses the file.
	char szLogName[MAX_PATH];
	CalcLogFileName(pszHash, szLogName);
//    if (cfg_parse(pCFG, szLogName) == CFG_PARSE_ERROR)
    if (cfg_parse(m_pCFG, szLogName) == CFG_PARSE_ERROR)
        return false;
    return true;
}

void SyncLogger::CalcLogFileName(const char* pszHash, char* pszLogName)
{
    strcpy(pszLogName, OFSEnvironment::Instance().getOfsDir().c_str());
    strcat(pszLogName, "/Sync_");
    strcat(pszLogName, pszHash);
    strcat(pszLogName, ".log");
}

list<SyncLogEntry> SyncLogger::GetEntries(const char* pszHash, const string strFilePath)
{
	if(!ParseFile(pszHash)) 
	   throw OFSException("Synclogger parse error", 0,true);

	// Assures the correct parsing of the file.
	assert(m_pCFG != NULL);

	list<SyncLogEntry> listOfEntries;
	const int nCount = cfg_size(m_pCFG, MOD_NUMBER_VARNAME);
	for (int i = 0; i < nCount; i++)
	{
		SyncLogEntry sle = ReadEntry(cfg_getnsec(m_pCFG, MOD_NUMBER_VARNAME, i));
		if (strFilePath == "" || sle == strFilePath)
			listOfEntries.push_back(sle);
	}
	return listOfEntries;
}

bool SyncLogger::RemoveEntry(const char* pszHash, SyncLogEntry& sle)
{
	struct stat fileinfo;
	char szLogName[MAX_PATH];
	CalcLogFileName(pszHash, szLogName);
	char szSubstring[20];
	int nRet = lstat(szLogName, &fileinfo);
	if (nRet < 0 && errno == ENOENT)
		throw OFSException(strerror(errno), errno);

	char* pszBuffer = new char[fileinfo.st_size];

	// Opens the sync log file for reading.
	FILE* pFile = OpenLogFile(pszHash, "r");
	if (pFile == NULL)
		return false;

	ssize_t nBytesRead;
	if ((nBytesRead = fread(pszBuffer, sizeof(char), fileinfo.st_size, pFile)) 
	   < 0)
	{
		throw new OFSException(strerror(errno), errno);
	}
	fclose(pFile);

	// Looks for the begin of the entry.
	char *szIndex = itoa(sle.GetNumber(), 10);
	strcpy(szSubstring, MOD_NUMBER_VARNAME);
	strcat(szSubstring, " ");
	strcat(szSubstring, szIndex);
	char* pszEntry = strstr(pszBuffer, szSubstring);

	if (pszEntry == NULL)
	{
		// Entry already removed, nothing to do
		return true;
	}
	int nEntryBegin = pszEntry - pszBuffer;

	// Looks for the end of the entry.
	//strcpy(szSubstring, "\n}\n\n");
	//strcat(szSubstring, MOD_NUMBER_VARNAME);
	pszEntry = strstr(pszEntry, "}");

	// Opens the sync log file for writing.
	pFile = OpenLogFile(pszHash, "w");
	if (pFile == NULL)
		return false;

	fwrite(pszBuffer, sizeof(char), nEntryBegin, pFile);

	if (pszEntry != NULL)
	{
		// Entry already removed, nothing to do
		int nEntryEnd = pszEntry - pszBuffer;
		fwrite(pszEntry+1, sizeof(char), fileinfo.st_size - nEntryEnd-1, pFile);
	}

	fclose(pFile);

	delete[] pszBuffer;

	return true;
}

char SyncLogger::getModDependingOnOtherEntries(const char* pszHash, const string strFilePath, const char chType) {
	/* get the whole list with iterator */
	list<SyncLogEntry> entrylist = GetEntries(pszHash, strFilePath);
	list<SyncLogEntry>::iterator iter;
	char modType = (char)0;

	/* search for entries on given path and delete ALL... */
	for (iter = entrylist.begin(); iter != entrylist.end(); iter++) {
		SyncLogEntry sle = (SyncLogEntry)*iter;
		if (sle == strFilePath) {
			/* ... but store FIRST modification type */
			if ((int)modType == 0)
				modType = sle.GetModType();
			RemoveEntry(pszHash, sle);
		}
	}
	
	/* determine the correct modtype of the entry */
	switch (modType) {
	    case 'c': //Not Exist
		if (chType == 'c')
			return 'c'; //c + c => c
		else if (chType == 'm')
			return 'c'; //c + m => c
		else if (chType == 'd')
			return 'x'; //c + d => none (nothing to do)
		break;

	    case 'm': //Exist
		if (chType == 'c')
			return 'm'; //m + c => m
		else if (chType == 'm')
			return 'm'; //m + m => m
		else if (chType == 'd')
			return 'd'; //m + d => d
		break;

	    case 'd': //Exist
		if (chType == 'c')
			return 'm'; //d + c => m
		else if (chType == 'm')
			return 'm'; //d + m => m
		else if (chType == 'd')
			return 'd'; //d + d => d
		break;

	    default: //no earlier entry found
		return chType; //return new entry type
	}
	
	return chType; //return unknown change type
}

bool SyncLogger::deleteOtherEntries(const char* pszHash) {
	return false;
}

