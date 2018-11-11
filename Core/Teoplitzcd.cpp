#include "stdafx.h"
#include "Teoplitzcd.h"

Core::AIMAssist::Teoplitzcd::Teoplitzcd(const VectorXi layerNum)
{
	if (layerNum.x()) reset(layerNum);
	else
	{
		_layerNum = layerNum;
		_layer = 0;
		_row = 0;
		_col = 0;
		_length = 0;
		_offside = 0;
	}
}

void Core::AIMAssist::Teoplitzcd::reset(const VectorXi layerNum)
{
	_layerNum = layerNum;
	_layer = static_cast<unsigned>(layerNum.size());
	_row = layerNum.prod();
	_col = _row;
	_vecN = 2 * layerNum.array() - 1;
	_length = _vecN.prod();
	_vecES = VectorXi::Ones(_layer);
	_ES = _vecES;
	for (unsigned i = 1; i < _layer; ++i)
	{
		_vecES(i) = _vecES(i - 1)*_vecN(i - 1);
		_ES(i) = _ES(i - 1)*layerNum(i - 1);
	}
	_offside = _length - _vecES.dot(layerNum-VectorXi::Ones(_layer));
	//construct index table
	Index.clear();
	for (unsigned i = 0; i < _length; ++i)
	{
		Index.push_back(id_pos(i).dot(_vecES));
	}
	//allocate memory
	_data.resize(_length);
	_data.fill(0);
}


void Core::AIMAssist::Teoplitzcd::constructIterated(const VectorXcd & row, const unsigned level, const unsigned bias)
{
	const unsigned elementSize = _vecES(level);
	const unsigned N = _layerNum(level);
	const unsigned mid = elementSize*(N-1)+ bias ;
	if(level)
	{
		constructIterated(row.head(_ES(level)), level - 1, mid);
		for (unsigned i = 1; i < N; ++i)
		{
			constructIterated(row.segment(i*_ES(level),_ES(level)),level-1, mid + i*elementSize);
			_data.segment(mid - i*elementSize, elementSize) = _data.segment(mid + i*elementSize, elementSize);
		}
	}
	else
	{
		_data.segment(mid, N) = row;
		for (unsigned i = 1; i < N; ++i)
		{
			_data(mid - i) = row(i);
		}
	}
}


void Core::AIMAssist::Teoplitzcd::padding(const VectorXcd & input, VectorXcd & output) const
{
	output.fill(0);
	for (unsigned i = 0; i < input.size(); ++i)
	{
		output(Index[i]) = input(i);
	}
}

void Core::AIMAssist::Teoplitzcd::unpadding(const VectorXcd & input, VectorXcd & output) const
{
	const unsigned bias = (unsigned)input.size() - _offside;
	for (unsigned i = 0; i < output.size(); ++i)
	{
		//const unsigned outpos = id_pos(i).dot(_vecES) +bias;
		output(i) = input(Index[i] +bias);
	}
}

dcomplex Core::AIMAssist::Teoplitzcd::at(const VectorXi&row, const VectorXi&col)
{
	VectorXi offside= _layerNum.array()-row.array()+col.array()-1;
	const unsigned position = offside.dot(_vecES);

	return _data(position);
}

VectorXi Core::AIMAssist::Teoplitzcd::id_pos(const unsigned val) const
{
	VectorXi pos(VectorXi::Zero(_layer));
	auto rest = val;
	for (int i = _layer-1; i >=0 ; --i)
	{
		pos(i) = rest / _ES(i);
		rest -= pos(i)*_ES(i);
	}

	return pos;
}

