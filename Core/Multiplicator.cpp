#include "stdafx.h"
#include "Multiplicator.h"

void Core::AIMAssist::MulFFTMultiplicator::MVP(const VectorXcd & FFTMatrix, VectorXcd & res)
{
	_computer.fwd(res);
	res.array() *= FFTMatrix.array();
	_computer.inv(res);

}

Core::AIMAssist::SingleFFTMultiplicator::~SingleFFTMultiplicator()
{
	Index.clear();
	Index.shrink_to_fit();
}

void Core::AIMAssist::SingleFFTMultiplicator::MVP(const VectorXcd& FFTMatrix, VectorXcd& res)
{
	Padding(res, _workspace);
	_computer.fwd(_workspace);
	_workspace.array() *= FFTMatrix.array();
	_computer.inv(_workspace);
	Unpadding(_workspace, res);
}

int Core::AIMAssist::SingleFFTMultiplicator::Reset(Teoplitzcd & tp)
{
	long _length = 2;
	while (_length < static_cast<long>(tp.Length()))_length <<= 1;
	_workspace.resize(_length);
	_bias = _length - tp.Offside();
	Index.clear();
	Index = tp.Index;

	return _computer.Reset(1, &_length);
}

VectorXcd Core::AIMAssist::SingleFFTMultiplicator::TpFFT(Teoplitzcd & tp) const
{
	VectorXcd result(VectorXcd::Zero(_computer.Length()));
	result.tail(tp.Length()) = tp.data();
	_computer.fwd(result);
	return result;
}

void Core::AIMAssist::SingleFFTMultiplicator::Padding(const VectorXcd & input, VectorXcd & output) const
{
	output.fill(0);
	for (unsigned i = 0; i < input.size(); ++i)output(Index[i]) = input(i);
}

void Core::AIMAssist::SingleFFTMultiplicator::Unpadding(const VectorXcd & input, VectorXcd & output) const
{
	for (unsigned i = 0; i < output.size(); ++i)output(i) = input(Index[i] + _bias);
}
