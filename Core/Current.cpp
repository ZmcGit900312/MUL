///////////////////////////////////////////////////////////
//  Current.cpp
//  Implementation of the Class ArrayCurrent
//  Created on:      26-9ÔÂ-2019 17:41:03
//  Original author: ZhengMX
///////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Current.h"
#include "Const.h"

using Core::Solution::ArrayCurrent;
using Core::Solution::ElementCurrent;
using namespace Core;
using namespace Eigen;


ArrayCurrent::ArrayCurrent(size_t elementUnknowns, double fre, string tag,  size_t numberOfElement, double biasX, double biasY) : ElementCurrent(elementUnknowns*numberOfElement,fre,tag),
_elementUnknowns(elementUnknowns), 
_numberOfElement(numberOfElement), 
_arrayBiasX(biasX), 
_arrayBiasY(biasY)
{
	_arrayLocation.clear();
	_arrayLocation.reserve(numberOfElement);
}

void ArrayCurrent::SaveBinary(ofstream & ofs)
{
	double infoDouble[3] = { _frequency,_arrayBiasX,_arrayBiasY };
	size_t infoInt[3] = { _numberOfElement,_elementUnknowns,_unknowns };

	ofs.write(reinterpret_cast<char*>(infoDouble), sizeof(double) * 3);
	ofs.write(reinterpret_cast<char*>(infoInt), sizeof(size_t) * 3);

	for (size_t zmc = 0; zmc < _numberOfElement; ++zmc)
	{
		size_t arrayloc[2] = { _arrayLocation[zmc].x(), _arrayLocation[zmc].y() };
		ofs.write(reinterpret_cast<char*>(arrayloc), sizeof(size_t) * 2);
		for (size_t num = 0;num < _elementUnknowns;num++)
		{
			ofs.write(reinterpret_cast<char*>(&(_data[zmc*_elementUnknowns + num])), sizeof(dcomplex));
		}
	}

}

void ArrayCurrent::ReadBinary(ifstream & ifs)
{
	_arrayLocation.clear();
	_data.clear();

	ifs.read(reinterpret_cast<char*>(&_frequency), sizeof(double));
	ifs.read(reinterpret_cast<char*>(&_arrayBiasX), sizeof(double));
	ifs.read(reinterpret_cast<char*>(&_arrayBiasY), sizeof(double));

	ifs.read(reinterpret_cast<char*>(&_numberOfElement), sizeof(size_t));
	ifs.read(reinterpret_cast<char*>(&_elementUnknowns), sizeof(size_t));
	ifs.read(reinterpret_cast<char*>(&_unknowns), sizeof(size_t));

	for (size_t zmc = 0; zmc < _numberOfElement; ++zmc)
	{
		size_t loc[2];
		ifs.read(reinterpret_cast<char*>(loc), sizeof(size_t) * 2);
		_arrayLocation.push_back(Vector2i{ loc[0],loc[1] });
		for (size_t num = 0;num < _elementUnknowns;num++)
		{
			dcomplex val = 0;
			ifs.read(reinterpret_cast<char*>(&val), sizeof(dcomplex));
			_data.push_back(val);
		}
	}
}

dcomplex ArrayCurrent::GetCurrent(size_t id, int x, int y) const
{
	for (int i = 0; i < _numberOfElement; ++i)
	{
		if (_arrayLocation[i].x() == x && _arrayLocation[i].y()==y)return _data[id + i * _elementUnknowns];
	}
	return dcomplex(0);
}

ElementCurrent::ElementCurrent(size_t unknowns, double fre, string tag):
Tag(tag),_unknowns(unknowns),_frequency(fre)
{
	_data.clear();
	_data.reserve(unknowns);
	ElementCurrent::EMCParameterUpdate();
}

void Core::Solution::ElementCurrent::SaveBinary(ofstream & ofs)
{

	ofs.write(reinterpret_cast<char*>(&_frequency), sizeof(double) );
	ofs.write(reinterpret_cast<char*>(&_unknowns), sizeof(double));

	for (dcomplex val : _data)
	{
		ofs.write(reinterpret_cast<char*>(&val), sizeof(dcomplex));
	}
	
}

void Core::Solution::ElementCurrent::ReadBinary(ifstream & ifs)
{
	_data.clear();

	ifs.read(reinterpret_cast<char*>(&_frequency), sizeof(double));
	ifs.read(reinterpret_cast<char*>(&_unknowns), sizeof(double));

	for (size_t num=0;num<_unknowns;num++)
	{
		dcomplex val(0);
		ifs.read(reinterpret_cast<char*>(&val), sizeof(dcomplex));
		_data.push_back(val);
	}

}

void Core::Solution::ElementCurrent::EMCParameterUpdate() const
{
	Frequency = _frequency;
	Omega = 2 * M_PI*Frequency;
	k = Omega / c0;
	Lambda = c0 / Frequency;
}



//CurrentInfo Initial

Solution::CurrentInfo Solution::CurrentInfo::_instance;

bool Solution::CurrentInfo::ReadCurrent(const char* fileName)
{
	ifstream ifs(fileName, ios::in | ios::binary);


	if (ifs.is_open())
	{
		
		ifs.read(reinterpret_cast<char*>(&category), sizeof(int));
		ifs.read(reinterpret_cast<char*>(&_numberOfConfig), sizeof(int));

		const clock_t start = clock();
		ElementCurrent* current = nullptr;
		for (int zmc = 0; zmc < _numberOfConfig; ++zmc)
		{
			if (category == Array)current = new ArrayCurrent;
			else current = new ElementCurrent;
				
			current->Tag = std::to_string(zmc);
			current->ReadBinary(ifs);
			Current.push_back(current);
		}


		const clock_t end = clock();
		const double _time = double(end - start) / CLOCKS_PER_SEC;
		Console->info("Read Current costs:\t{0:10.9} s", _time);
		ifs.clear();
		ifs.close();
	}
	else return false;

	return true;

}

bool Solution::CurrentInfo::SaveCurrent(const char* fileName)
{
	ofstream ofs(fileName, ios::out | ios::binary | ios::trunc);


	if (ofs.is_open())
	{
		Console->info("The Current File Path is\t" + string(fileName));
		ResultLog->info("The Current File Path is\t" + string(fileName));
		//RWG
		ofs.write(reinterpret_cast<char*>(&category), sizeof(int));
	
		ofs.write(reinterpret_cast<char*>(&_numberOfConfig), sizeof(int));

		const clock_t start = clock();
		for (auto current : Current)
		{
			current->SaveBinary(ofs);
		}

		const clock_t end = clock();
		const double _time = double(end - start) / CLOCKS_PER_SEC;
		Console->info("Write Current costs:\t{0:10.9} s",_time);

		ofs.flush();
		ofs.close();

	}
	else return false;


	return true;
}

void Solution::CurrentInfo::Reformat(EImpedance ty)
{
	if (Current.size() > 0)
	{
		for (auto current : Current)
		{
			delete current;
		}
	}
	Current.clear();
	Current.shrink_to_fit();
	_numberOfConfig = 0;
	category = ty;
}

