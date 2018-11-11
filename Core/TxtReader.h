#pragma once
#include<list>
using namespace std;

namespace Core
{
	/**
	 * \brief һ�����ڶ�д�ı��Ķ�ȡ��
	 * \n �������зָ������ı�����ַ���д��List����
	 * \n ����GetContext���Ի�ȡ��������в���
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