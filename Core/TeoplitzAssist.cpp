#include "stdafx.h"
#include "TeoplitzAssist.h"

using namespace Core::AIMAssist;

TeoplitzAssist::~TeoplitzAssist()
{
	Index.clear();
	Index.shrink_to_fit();
}

void TeoplitzAssist::MultiplyTeoplitz(const VectorXcd & tpfft, VectorXcd & x)
{
	Padding(x,_workspace);
	_trans.fwd(_workspace);
	_workspace.array() *= tpfft.array();
	_trans.inv(_workspace);
	Unpadding(_workspace, x);
}

void TeoplitzAssist::Reset(Teoplitzcd & tp)
{
	_length = 2;
	while (_length<static_cast<long>(tp.Length()))_length <<= 1;
	_workspace.resize(_length);
	_trans.ResetFFT(_length);
	_bias = _length - tp.Offside();
	Index.clear();
	Index = tp.Index;
}

//Teoplitz FFT
VectorXcd TeoplitzAssist::TeoplitzFFT(Teoplitzcd&tp) const
{
	VectorXcd result(VectorXcd::Zero(_length));
	result.tail(tp.Length()) = tp.data();
	_trans.fwd(result);
	return result;
}

void TeoplitzAssist::Padding(const VectorXcd & input, VectorXcd & output) const
{
	output.fill(0);
	for (unsigned i = 0; i < input.size(); ++i)
	{
		output(Index[i]) = input(i);
	}
}

void TeoplitzAssist::Unpadding(const VectorXcd & input, VectorXcd & output) const
{
	for (unsigned i = 0; i < output.size(); ++i)
	{
		output(i) = input(Index[i] + _bias);
	}
}
