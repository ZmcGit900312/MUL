///////////////////////////////////////////////////////////
//  Current.h
//  Implementation of the Class ArrayCurrent
//  Created on:      26-9月-2019 17:41:03
//  Original author: ZhengMX
///////////////////////////////////////////////////////////

#ifndef CURRENT_H
#define CURRENT_H
#include "IImpedance.h"
namespace Core
{
	namespace Solution
	{	

		class ElementCurrent
		{
		public:
			virtual ~ElementCurrent() { _data.clear();_data.shrink_to_fit(); }

			virtual void SaveBinary(ofstream & ofs);

			virtual void ReadBinary(ifstream &ifs);

			virtual dcomplex GetCurrent(size_t id)const { return _data[id]; }

			string ConfigName;
			size_t _unknowns;
			double _frequency;
			vector<dcomplex> _data;
		};

		//求解数据类，参加md文档
		class ArrayCurrent:public ElementCurrent
		{

		public:

			ArrayCurrent(): _elementUnknowns(0), _numberOfElement(0), _arrayBiasX(0), _arrayBiasY(0){}

			~ArrayCurrent() { _array.clear();_array.shrink_to_fit(); }
			
			size_t _elementUnknowns;
			size_t _numberOfElement;
			double _arrayBiasX, _arrayBiasY;
			vector<Vector2i> _array;

			void SaveBinary(ofstream & ofs)override;			

			void ReadBinary(ifstream &ifs)override;
			
			dcomplex GetCurrent(size_t id, int x, int y)const;
			
		};
		
		
		class CurrentInfo
		{
		public:
			static CurrentInfo* GetInstance() { return &_instance; }
			~CurrentInfo() { this->Reformat(); }

			int _numberOfConfig=0;
			EImpedance category=MoM;
			vector<ElementCurrent*> Current;

			bool ReadCurrent(const char* fileName);
			bool SaveCurrent(const char* fileName);
			void Reformat(EImpedance ty=MoM);
		private:
			CurrentInfo(){ this->Reformat(); }
			CurrentInfo operator=(CurrentInfo&) = delete;
			CurrentInfo(CurrentInfo&) = delete;

			static CurrentInfo _instance;
			
		};
	}

}
#endif
