// UTRC_Calculate_Friends.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
using namespace std;

typedef pair<string, int> FRIEND_DISTANCE;
typedef vector<FRIEND_DISTANCE> LIST_FRIENDS;
typedef set<string> SET_FRIENDS;
typedef map<string, SET_FRIENDS> MAP_FRIENDS;


int ReadInputData(const char* szFileName, MAP_FRIENDS& mapFriends)
{
	ifstream ifs(szFileName);
	if (!ifs.is_open())
	{
		cout << "open file failed!" << endl;
		return -1;
	}

	string sLine;
	while (getline(ifs, sLine))
	{
		int nPos = sLine.find_first_of(",");
		if (nPos == string::npos)
		{
			// format not valid??
			continue;
		}

		string sLName = sLine.substr(0, nPos);
		string sRName = sLine.substr(nPos + 1);

		// bidirection
		mapFriends[sLName].insert(sRName);
		mapFriends[sRName].insert(sLName);
	}

	return 0;
}

int CalcFriendDistance(const MAP_FRIENDS& mapFriends, const string& A, const string& B)
{
	SET_FRIENDS uniqFriends;
	LIST_FRIENDS listFriends;
	listFriends.push_back(make_pair(A, 0));

	// traverse A's friends of different level (BFS)
	for (int i = 0; i < listFriends.size(); ++i)
	{
		const string& sName = listFriends[i].first;
		int iDistance = listFriends[i].second;

		if (sName == B)
		{
			return iDistance;
		}
		else
		{
			// avoid loop and needlessly check
			if (uniqFriends.end() != uniqFriends.find(sName))
			{
				continue;
			}
			uniqFriends.insert(sName);

			MAP_FRIENDS::const_iterator itName = mapFriends.find(sName);
			if (itName != mapFriends.end())
			{
				const SET_FRIENDS& setFriends = itName->second;
				for (SET_FRIENDS::const_iterator itor = setFriends.begin(); itor != setFriends.end(); ++itor)
				{
					listFriends.push_back(make_pair(*itor, iDistance + 1));
				}
			}
		}
	}

	return -1;
}

int main(int argc, char* argv[])
{
	string sFirstName;
	string sSecondName;
	string sTrueorFalse;
	MAP_FRIENDS mapFriends;
	// 1. read data from file
	int iRet = ReadInputData("SocialNetwork.txt", mapFriends);
	if (iRet != 0)
	{
		return 1;
	}

	// 2. statistic num of people
	cout << "total number: " << mapFriends.size() << endl;

	// 3. calc distance between A and B 
	while (true)
	{
		cout << "Please Input the First Name:" << endl;
		cin >> sFirstName;
		cout << "Please Input the Second Name:" << endl;
		cin >> sSecondName;
		cout << "distance between " + sFirstName + " and " + sSecondName + " is " << CalcFriendDistance(mapFriends, sFirstName, sSecondName) << endl;
		cout << "Contiune or Not(Yes/Not)?" << endl;
		cin >> sTrueorFalse;
		if ("Not" == sTrueorFalse)
		{
			break;
		}
	}
	return 0;
}


