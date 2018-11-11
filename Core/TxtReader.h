#pragma once
#include<list>
using namespace std;

namespace Core
{
	/**
	 * \brief 一个用于读写文本的读取器
	 * \n 它将带有分隔符的文本逐个字符串写入List链表
	 * \n 利用GetContext可以获取该链表进行操作
	 */
	static class TxtReader{
	public:
		TxtReader(char*filename = nullptr){ Read(filename); }
		~TxtReader() { _context.clear(); }

		bool Read(char*filename);
		bool Write(char*filename) const;
		char*& Name() { return _filename; }
		list<string>& GetContext() { return _context; }
	private:
		char* _filename = nullptr;
		list<string> _context;
	}TReader;
}