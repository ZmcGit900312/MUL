#include "stdafx.h"
#include "TxtReader.h"

using namespace std;

Core::TxtReader TReader;

bool Core::TxtReader::Read(char*filename)
{
	if (!filename)return false;
	_filename = filename;
	ifstream _fid(_filename, ios_base::in);
	if (_fid.is_open())
	{
		//Read all context into buffer and close
		stringstream buffer;
		buffer << _fid.rdbuf();
		_fid.close(); 
		//Split the words
		_context.clear();
		string word;
		while (buffer >> word)_context.push_back(word);
		return true;
	}
	return  false;
}

bool Core::TxtReader::Write(char * filename) const
{
	if (!filename)return false;
	if (_context.empty())return false;
	ofstream of(filename, ios_base::out);
	
	for (const auto element : _context)of << element << ' ';
	of.close();
	return true;
}
