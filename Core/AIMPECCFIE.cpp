#include "stdafx.h"
#include "AIMPECCFIE.h"

Core::AIMPECCFIE::AIMPECCFIE(const ImpConfiguration & configuration, IImpService * impedance, IEConfiguration ie) : 
IMatrixFiller(configuration, impedance,ie), 
_imp(static_cast<ImpAIM*>(impedance))
{
	size_t layer[3] = { 2 * configuration.xNumber - 1,2 * configuration.yNumber - 1 ,2 * configuration.zNumber - 1 };
	MultiExpTool.Reset(configuration, layer, W7);
	_gridNum = MultiExpTool.GetGridNum();

	_localGreen.resize(_gridNum, _gridNum);
	_localGreenX.resize(_gridNum, _gridNum);
	_localGreenY.resize(_gridNum, _gridNum);
	_localGreenZ.resize(_gridNum, _gridNum);

	_layerElementSize = 2 * _layerNum.array() - 1;
	for (int i = 0; i < _dim; i++)
	{
		_layerElementSizeAcu(i) = _layerElementSize.head(i + 1).prod();
	}	

	Console->info("Take the PECCFIEFiller");
	RuntimeLog->info("Take the PECCFIEFiller");
	ResultLog->info("Take the PECCFIEFiller");
}

Core::AIMPECCFIE::~AIMPECCFIE()
{
	_gama.clear();
	_gama.shrink_to_fit();
	_imp = nullptr;
}

void Core::AIMPECCFIE::MultipoleExpansion(vector<IBasicFunction*>& bf)
{
	Console->debug("Mulitpole Expansion");
	_gama.clear();
	const size_t estimatedSize = _unknowns * _gridNum;
	//Gama Reservation
	vector<T> tripletsGamaX, tripletsGamaY, tripletsGamaZ, tripletsGamaD;

	tripletsGamaX.reserve(estimatedSize);
	tripletsGamaY.reserve(estimatedSize);
	tripletsGamaZ.reserve(estimatedSize);
	tripletsGamaD.reserve(estimatedSize);
	_imp->GetGammaX().resize(_layerElementSizeAcu(_dim - 1), _unknowns);
	_imp->GetGammaX().reserve(estimatedSize);
	_imp->GetGammaY().resize(_layerElementSizeAcu(_dim - 1), _unknowns);
	_imp->GetGammaY().reserve(estimatedSize);
	_imp->GetGammaZ().resize(_layerElementSizeAcu(_dim - 1), _unknowns);
	_imp->GetGammaZ().reserve(estimatedSize);
	_imp->GetGammaD().resize(_layerElementSizeAcu(_dim - 1), _unknowns);
	_imp->GetGammaD().reserve(estimatedSize);
	_gama.reserve(estimatedSize);


	//MultiExpansion
	Console->debug("Push all the Gmama Matrix Into Triplets");
	const clock_t start = clock();
	for (int i = 0; i < _unknowns; ++i)
	{
		MatrixXcd coef = MultiExpTool(static_cast<RWG*>(bf[i]));

		gama temp{ MultiExpTool.Index,coef.col(0),coef.col(1),coef.col(2),coef.col(3) };
		const auto gridnum = temp.index.size();
		for (auto j = 0;j < gridnum; ++j)
		{
			//Push
			auto const pos = temp.index[j].w();
			tripletsGamaX.push_back(T(pos, i, temp.gamax(j)));
			tripletsGamaY.push_back(T(pos, i, temp.gamay(j)));
			tripletsGamaZ.push_back(T(pos, i, temp.gamaz(j)));
			tripletsGamaD.push_back(T(pos, i, temp.gamad(j)));
		}
		_gama.push_back(temp);
	}
	_gama.shrink_to_fit();
	tripletsGamaX.shrink_to_fit();
	tripletsGamaY.shrink_to_fit();
	tripletsGamaZ.shrink_to_fit();
	tripletsGamaD.shrink_to_fit();
	Console->debug("Finish Multipole Expansion for Triplets...OK");

	//Construct the SparesMatrix and makeCompressed
	_imp->GetGammaX().setFromTriplets(tripletsGamaX.begin(), tripletsGamaX.end());
	_imp->GetGammaY().setFromTriplets(tripletsGamaY.begin(), tripletsGamaY.end());
	_imp->GetGammaZ().setFromTriplets(tripletsGamaZ.begin(), tripletsGamaZ.end());
	_imp->GetGammaD().setFromTriplets(tripletsGamaD.begin(), tripletsGamaD.end());

	_imp->GetGammaX().makeCompressed();
	_imp->GetGammaY().makeCompressed();
	_imp->GetGammaZ().makeCompressed();
	_imp->GetGammaD().makeCompressed();

	const clock_t end = clock();
	const double timecost = static_cast<double>(end - start) / CLOCKS_PER_SEC;
	Console->info("Multipole Expansion costs time:\t{}s", timecost);
	RuntimeLog->info("Multipole Expansion costs time:\t{}s", timecost);
	ResultLog->info("Multipole Expansion costs time:\t{}s", timecost);

	tripletsGamaX.clear(); tripletsGamaX.shrink_to_fit();
	tripletsGamaY.clear(); tripletsGamaY.shrink_to_fit();
	tripletsGamaZ.clear(); tripletsGamaZ.shrink_to_fit();
	tripletsGamaD.clear(); tripletsGamaD.shrink_to_fit();
}

void Core::AIMPECCFIE::GreenMatrixSet(IGreen * green)
{
	AIMAssist::MulFFTMultiplicator *tools = new AIMAssist::MulFFTMultiplicator;
	MKL_LONG layer[3] = { _layerElementSize.z() ,_layerElementSize.y(), _layerElementSize.x() };

	tools->Reset(_dim, layer);

	_imp->_fftTools = tools;

	unsigned bias = 0;
	GenerateGreenBase(green);

	switch (_equationType)
	{
	case EFIE:

		_imp->GetGreen().resize(tools->Length());
		_imp->GetGreen() = construct(0);

		tools->fwd(_imp->GetGreen());
		return;
	case MFIE:throw spd::spdlog_ex("Not Develop");
	case CFIE:
		_imp->GetGreen().resize(tools->Length());
		_imp->GetGreenX().resize(tools->Length());
		_imp->GetGreenY().resize(tools->Length());
		_imp->GetGreenZ().resize(tools->Length());	
		
		_imp->GetGreen() = construct(0);
		_imp->GetGreenX() = construct(1);
		_imp->GetGreenY() = construct(2);
		_imp->GetGreenZ() = construct(3);

		tools->fwd(_imp->GetGreen());
		tools->fwd(_imp->GetGreenX());
		tools->fwd(_imp->GetGreenY());
		tools->fwd(_imp->GetGreenZ());
		return;//Unfinish
	case IBCEFIE: throw spd::spdlog_ex("Not Develop");
	case IBCMFIE: throw spd::spdlog_ex("Not Develop");
	case IBCCFIE: throw spd::spdlog_ex("Not Develop");
	default: throw spd::spdlog_ex("Set Green Error");
	}


}

void Core::AIMPECCFIE::TriangleFillingStrategy(Mesh & mesh, vector<IBasicFunction*>& bf)
{
	Console->info("IBC Near Field Set under TFS");
	if (_gama.size() < 1 || _layerElementSizeAcu(_dim - 1) < 1)
		throw spd::spdlog_ex("Error in NearCorrection: Please Call MultipoleExpansion first");

	const size_t estimatedSize = _unknowns * _unknowns / 25;
	const size_t triangleNum = static_cast<size_t>(mesh.TriangleSize());
	size_t currentProgress = 0;
	//cout << "Reserve the NearPart of Impedance...\n";
	//Reserve
	vector<T> tripletsNearPart;
	tripletsNearPart.reserve(estimatedSize);
	//Set Near Field Triplets
	vector<element> Z;
	Z.reserve(9);
	clock_t start = clock();


	for (auto r = mesh.TriangleVector.begin(), ed = mesh.TriangleVector.end();r != ed;++r, ++currentProgress)
	{
		//list<element> Z;
		RWGTriangle* row = dynamic_cast<RWGTriangle*>(*r);
		for (auto c = r + 1;c != ed;++c)
		{
			RWGTriangle* col = dynamic_cast<RWGTriangle*>(*c);
			Vector3d distance = row->Centre() - col->Centre();
			if (distance.norm() > _threshold*1.3)continue;

			//Calculate Triangle integration
			for (short i = 0; i < 3; i++)
			{
				if (!row->RWGSign[i])continue;
				const auto field = static_cast<RWG*>(bf[row->RWGID(i)]);
				for (short j = 0; j < 3; j++)
				{
					if (!col->RWGSign[j])continue;
					const auto source = static_cast<RWG*>(bf[col->RWGID(j)]);
					Vector3d edgeDistance = field->Centre() - source->Centre();
					if (edgeDistance.norm() > _threshold)continue;
					Z.push_back({ i, j, row->RWGSign[i] * col->RWGSign[j] });
				}
			}
			if (Z.size() == 0)continue;
			_compute.OperatorCPEC(row, col, Z,_alpha,_eta);
			//Set LowerTriangle
			for (auto i = Z.cbegin();i != Z.cend();++i)
			{
				auto source = std::get<0>(*i), field = std::get<1>(*i);
				const auto value = std::get<2>(*i);
				//For Precondition
				tripletsNearPart.push_back(T(source, field, value));
				tripletsNearPart.push_back(T(field, source, value));
				//Non Precondition
				/*if(source == field)
				{
					tripletsNearPart.push_back(T(source, field, value*2.0));
					continue;
				}
				if(source> field)
				{
					tripletsNearPart.push_back(T(source, field, value));
				}
				else
				{
					tripletsNearPart.push_back(T(field, source, value));
				}*/
			}
			Z.clear();
		}
		//SelfSet
		{
			auto selfList = _compute.OperatorCPEC(row, _alpha, _eta);
			//Calculate Triangle integration
			for (auto zmc = selfList.begin(); zmc != selfList.end(); ++zmc)
			{
				auto source = std::get<0>(*zmc), field = std::get<1>(*zmc);
				const auto value = std::get<2>(*zmc);
				tripletsNearPart.push_back(T(source, field, value));
			}			
		}
		cout << "Progress:" << setw(10) << 100 * static_cast<double>(currentProgress) / triangleNum << "%\r";
	}
	tripletsNearPart.shrink_to_fit();
	_imp->GetNearFieldMatrix().reserve(tripletsNearPart.size());
	_imp->GetNearFieldMatrix().setFromTriplets(tripletsNearPart.begin(), tripletsNearPart.end());
	_imp->GetNearFieldMatrix().makeCompressed();

	clock_t end = clock();
	double timecost = double(end - start) / CLOCKS_PER_SEC;
	cout << "\r";
	Console->info("Near-Matrix Setting by TFS Time is:\t{}s", timecost);
	Console->info("Triplet and NearField have {0} and {1} elements.", tripletsNearPart.size(), _imp->GetNearFieldMatrix().nonZeros());
	RuntimeLog->info("Near-Matrix Setting by TFS Time is:\t{}s", timecost);
	RuntimeLog->info("Triplet and NearField have {0} and {1} elements.", tripletsNearPart.size(), _imp->GetNearFieldMatrix().nonZeros());
	ResultLog->info("Near-Matrix Setting by TFS Time is:\t{}s", timecost);
	ResultLog->info("Triplet and NearField have {0} and {1} elements.", tripletsNearPart.size(), _imp->GetNearFieldMatrix().nonZeros());

	tripletsNearPart.clear();

	//PreCondition
	Console->info("Calculate Preconditioning...");
	Solver->Precondition(_imp);
	Console->info("Precondition cost:\t{}s", Solver->GetPreconditionTime());
	ResultLog->info("Precondition cost:\t{}s", Solver->GetPreconditionTime());
	RuntimeLog->info("Precondition cost:\t{}s", Solver->GetPreconditionTime());

	//Near Correction
	Console->debug("Begin to Correction");
	start = clock();
	for (int col = 0; col < _imp->GetNearFieldMatrix().outerSize(); ++col)
	{
		for (SparseMatrix<dcomplex>::InnerIterator it(_imp->GetNearFieldMatrix(), col); it; ++it)
		{
			//if (it.row() < it.col())continue;
			const dcomplex comp = GetFarFieldImpedacneAIM(it.row(), it.col());
			const dcomplex difvalue = it.value() - comp;
			if (it.row() == it.col())
			{
				tripletsNearPart.push_back(T(it.row(), it.col(), difvalue));
				continue;
			}
			if (abs(difvalue) / abs(it.value()) > _eps)
			{
				tripletsNearPart.push_back(T(it.row(), it.col(), difvalue));
				//tripletsNearPart.push_back(T(it.col(), it.row(), difvalue));
			}

		}
		cout << "Progress:" << setw(10) << 100 * static_cast<double>(col) / _imp->GetNearFieldMatrix().outerSize() << "%\r";
	}
	tripletsNearPart.shrink_to_fit();
	_imp->GetNearFieldMatrix().reserve(tripletsNearPart.size());
	_imp->GetNearFieldMatrix().setFromTriplets(tripletsNearPart.cbegin(), tripletsNearPart.cend());
	_imp->GetNearFieldMatrix().makeCompressed();

	end = clock();
	timecost += double(end - start) / CLOCKS_PER_SEC;
	cout << "\r";
	const double sparsity = static_cast<double>(_imp->GetNearFieldMatrix().nonZeros()) / _unknowns / _unknowns;
	Console->info("Near Field by TFS TotalTime is:\t{}s", timecost);
	RuntimeLog->info("Near Field by TFS TotalTime is:\t{}s", timecost);
	ResultLog->info("Near Field by TFS TotalTime is:\t{}s", timecost);
	Console->debug("Nonzeros have {0} and take {1}%.", _imp->GetNearFieldMatrix().nonZeros(), 100 * sparsity);
}

void Core::AIMPECCFIE::NearCorrection(vector<IBasicFunction*>& bf)
{

}

void Core::AIMPECCFIE::GenerateGreenBase(IGreen * green)
{
	Console->debug("Generate Green Base");

	switch (_equationType)
	{
	case EFIE:
		_greenBase.resize(_layerNum.prod());
		for (size_t zzmc = 0, count = 0;zzmc < _layerNum[2];zzmc++)
		{
			for (size_t yzmc = 0;yzmc < _layerNum[1];yzmc++)
			{
				for (size_t xzmc = 0;xzmc < _layerNum[0];xzmc++)
				{
					if (count != 0)_greenBase(count++) = green->Scalar(Vector3d::Zero(), _interval*Vector3d{ xzmc ,yzmc,zzmc });
					else _greenBase(count++) = 0;

				}
			}
		}
		return;
	case MFIE: throw spd::spdlog_ex("Not Develop");
	case CFIE:
		_greenBase.resize(_layerNum.prod());
		_greenGradientBaseX.resize(_layerNum.prod());
		_greenGradientBaseY.resize(_layerNum.prod());
		_greenGradientBaseZ.resize(_layerNum.prod());
		for (size_t zzmc = 0, count = 0;zzmc < _layerNum[2];zzmc++)
		{
			for (size_t yzmc = 0;yzmc < _layerNum[1];yzmc++)
			{
				for (size_t xzmc = 0;xzmc < _layerNum[0];xzmc++)
				{
					if (count != 0)
					{
						_greenBase(count) = green->Scalar(_interval*Vector3d{ xzmc ,yzmc,zzmc }, Vector3d::Zero());
						Vector3cd grad = green->Gradient(_interval*Vector3d{ xzmc ,yzmc,zzmc }, Vector3d::Zero());
						_greenGradientBaseX(count) = grad.x();
						_greenGradientBaseY(count) = grad.y();
						_greenGradientBaseZ(count) = grad.z();
						count++;
					}
					else
					{
						_greenBase(count) = 0;
						_greenGradientBaseX(count) = 0;
						_greenGradientBaseY(count) = 0;
						_greenGradientBaseZ(count) = 0;
						count++;
					}


				}
			}
		}
		return;
	case IBCEFIE: throw spd::spdlog_ex("Not Develop");
	case IBCMFIE: throw spd::spdlog_ex("Not Develop");
	case IBCCFIE: throw spd::spdlog_ex("Not Develop");
	default: throw spd::spdlog_ex("Set Green Error");
	}


}

dcomplex Core::AIMPECCFIE::GetFarFieldImpedacneAIM(const size_t row, const size_t col)
{
	gama& field = _gama[row], source = _gama[col];
	//MatrixXcd green{ _gridNum,_gridNum };
	for (int i = 0; i < _gridNum; ++i)
	{
		for (int j = 0; j < _gridNum; ++j)
		{
			Vector4i fp(field.index[i]), sp(source.index[j]);
			size_t location = gridGreenlocation(fp, sp);
			Vector3i diff(fp.head<3>() - sp.head<3>());

			_localGreen(i, j) = _greenBase[location];
			_localGreenX(i, j) = diff(0) < 0 ? -_greenGradientBaseX[location] : _greenGradientBaseX[location];
			_localGreenY(i, j) = diff(1) < 0 ? -_greenGradientBaseY[location] : _greenGradientBaseY[location];
			_localGreenZ(i, j) = diff(2) < 0 ? -_greenGradientBaseZ[location] : _greenGradientBaseZ[location];
		}
	}
	//L part
	const dcomplex Lx = field.gamax.transpose()*(_localGreen*source.gamax);
	const dcomplex Ly = field.gamay.transpose()*(_localGreen*source.gamay);
	const dcomplex Lz = field.gamaz.transpose()*(_localGreen*source.gamaz);
	const dcomplex Ld = field.gamad.transpose()*(_localGreen*source.gamad);

	dcomplex L = (Lx + Ly + Lz - Ld / (k*k)) * 1i*k*_eta;

	if (_equationType != CFIE)return L;


	//K part
	dcomplex Kx = field.gamax.transpose()*(_localGreenY*source.gamaz - _localGreenZ * source.gamay);
	dcomplex Ky = field.gamay.transpose()*(_localGreenZ*source.gamax - _localGreenX * source.gamaz);
	dcomplex Kz = field.gamaz.transpose()*(_localGreenX*source.gamay - _localGreenY * source.gamax);

	dcomplex K = (Kx + Ky + Kz);

	//#ifdef _DEBUG
	//	cout << "GreenGradientX:\n"<<_localGreenX.block(0,0,7,7) << endl;
	//	cout << "GreenGradientY:\n" << _localGreenY.block(0, 0, 7, 7) << endl;
	//	cout << "GreenGradientZ:\n" << _localGreenZ.block(0, 0, 7, 7) << endl;
	//	return K;//TEST
	//#endif

	return L * _alpha + (1 - _alpha)*_eta*K;
}

VectorXcd Core::AIMPECCFIE::constructIterated(unsigned & bias, const unsigned level)
{
	const unsigned N = _layerNum(level);
	VectorXcd data{ _layerElementSizeAcu(level) };
	const int length = data.size();
	if (level)
	{
		const int ESA = _layerElementSizeAcu(level - 1);
		data.head(ESA) = constructIterated(bias, level - 1);
		int i = 1;
		for (i = 1;i < N - 1;i++)
		{
			data.segment(i * ESA, ESA) = constructIterated(bias, level - 1);
			data.segment(length - i * ESA, ESA) = data.segment(i *ESA, ESA);
		}
		data.segment(i * ESA, ESA) = constructIterated(bias, level - 1);
	}
	else
	{
		data.head(N) = _greenBase.segment(bias, N);
		bias += N;
		data.tail(N - 1) = data.segment(1, N - 1).reverse();
	}
	return data;
}

VectorXcd Core::AIMPECCFIE::construct(int component)
{
	VectorXcd data{ _layerElementSize.head(_dim).prod() };
	const Index length = data.size();
	Index count = 0;
	if (_dim == 2)
	{
		switch (component)
		{
		case 1:
			//x
			for (Index indexy = 0; indexy < _layerNum(1); ++indexy)
			{
				Index biasy = indexy * _layerElementSize.x();
				Index biasye = biasy + _layerElementSize.x();
				for (Index indexx = 0; indexx < _layerNum(0); ++indexx)
				{
					Index biasx = biasy + indexx;
					data(biasx) = _greenGradientBaseX(count++);
					if (indexx == 0)continue;
					data(biasye - indexx) = -data(biasx);
				}

				if (indexy == 0)continue;
				data.segment(length - biasy, _layerElementSize.x()) = data.segment(biasy, _layerElementSize.x());
			}
			break;
		case 2:
			//y
			for (Index indexy = 0; indexy < _layerNum(1); ++indexy)
			{
				Index biasy = indexy * _layerElementSize.x();
				Index biasye = biasy + _layerElementSize.x();
				for (Index indexx = 0; indexx < _layerNum(0); ++indexx)
				{
					Index biasx = biasy + indexx;
					data(biasx) = _greenGradientBaseY(count++);
					if (indexx == 0)continue;
					data(biasye - indexx) = data(biasx);
				}

				if (indexy == 0)continue;
				data.segment(length - biasy, _layerElementSize.x()) = -data.segment(biasy, _layerElementSize.x());
			}
			break;
		default:
			//scalar
			for (Index indexy = 0; indexy < _layerNum(1); ++indexy)
			{
				Index biasy = indexy * _layerElementSize.x();
				Index biasye = biasy + _layerElementSize.x();
				for (Index indexx = 0; indexx < _layerNum(0); ++indexx)
				{
					Index biasx = biasy + indexx;
					data(biasx) = _greenBase(count++);
					if (indexx == 0)continue;
					data(biasye - indexx) = data(biasx);
				}

				if (indexy == 0)continue;
				data.segment(length - biasy, _layerElementSize.x()) = data.segment(biasy, _layerElementSize.x());
			}
			break;

		}

	}
	else
	{	
		switch (component)
		{
		case 1://x
			for (Index indexz = 0; indexz < _layerNum(2); ++indexz)
			{
				Index biasz = indexz * _layerElementSize.x()*_layerElementSize.y();
				Index biasze = biasz + _layerElementSize.x()*_layerElementSize.y();
				for (Index indexy = 0; indexy < _layerNum(1); ++indexy)
				{
					Index biasy = biasz + indexy * _layerElementSize.x();
					Index biasye = biasy + _layerElementSize.x();
					for (Index indexx = 0; indexx < _layerNum(0); ++indexx)
					{
						Index biasx = biasy + indexx;
						data(biasx) = _greenGradientBaseX(count++);
						if (indexx == 0)continue;
						data(biasye - indexx) = -data(biasx);
					}

					if (indexy == 0)continue;
					data.segment(biasze - indexy * _layerElementSize.x(), _layerElementSize.x()) = data.segment(biasy, _layerElementSize.x());


				}

				if (indexz == 0)continue;
				data.segment(length - biasz, _layerElementSize.x()*_layerElementSize.y()) = data.segment(biasz, _layerElementSize.x()*_layerElementSize.y());
			}
			break;
		case 2://y
			for (Index indexz = 0; indexz < _layerNum(2); ++indexz)
			{
				Index biasz = indexz * _layerElementSize.x()*_layerElementSize.y();
				Index biasze = biasz + _layerElementSize.x()*_layerElementSize.y();
				for (Index indexy = 0; indexy < _layerNum(1); ++indexy)
				{
					Index biasy = biasz + indexy * _layerElementSize.x();
					Index biasye = biasy + _layerElementSize.x();
					for (Index indexx = 0; indexx < _layerNum(0); ++indexx)
					{
						Index biasx = biasy + indexx;
						data(biasx) = _greenGradientBaseY(count++);
						if (indexx == 0)continue;
						data(biasye - indexx) = data(biasx);
					}

					if (indexy == 0)continue;
					data.segment(biasze - indexy * _layerElementSize.x(), _layerElementSize.x()) = -data.segment(biasy, _layerElementSize.x());


				}

				if (indexz == 0)continue;
				data.segment(length - biasz, _layerElementSize.x()*_layerElementSize.y()) = data.segment(biasz, _layerElementSize.x()*_layerElementSize.y());
			}
			break;
		case 3://z
			for (Index indexz = 0; indexz < _layerNum(2); ++indexz)
			{
				Index biasz = indexz * _layerElementSize.x()*_layerElementSize.y();
				Index biasze = biasz + _layerElementSize.x()*_layerElementSize.y();
				for (Index indexy = 0; indexy < _layerNum(1); ++indexy)
				{
					Index biasy = biasz + indexy * _layerElementSize.x();
					Index biasye = biasy + _layerElementSize.x();
					for (Index indexx = 0; indexx < _layerNum(0); ++indexx)
					{
						Index biasx = biasy + indexx;
						data(biasx) = _greenGradientBaseZ(count++);
						if (indexx == 0)continue;
						data(biasye - indexx) = data(biasx);
					}

					if (indexy == 0)continue;
					data.segment(biasze - indexy * _layerElementSize.x(), _layerElementSize.x()) = data.segment(biasy, _layerElementSize.x());


				}

				if (indexz == 0)continue;
				data.segment(length - biasz, _layerElementSize.x()*_layerElementSize.y()) = -data.segment(biasz, _layerElementSize.x()*_layerElementSize.y());
			}
			break;
		default:
			//Scalar
			for (Index indexz = 0; indexz < _layerNum(2); ++indexz)
			{
				Index biasz = indexz * _layerElementSize.x()*_layerElementSize.y();
				Index biasze = biasz + _layerElementSize.x()*_layerElementSize.y();
				for (Index indexy = 0; indexy < _layerNum(1); ++indexy)
				{
					Index biasy = biasz + indexy * _layerElementSize.x();
					Index biasye = biasy + _layerElementSize.x();
					for (Index indexx = 0; indexx < _layerNum(0); ++indexx)
					{
						Index biasx = biasy + indexx;
						data(biasx) = _greenBase(count++);
						if (indexx == 0)continue;
						data(biasye - indexx) = data(biasx);
					}

					if (indexy == 0)continue;
					data.segment(biasze - indexy * _layerElementSize.x(), _layerElementSize.x()) = data.segment(biasy, _layerElementSize.x());


				}

				if (indexz == 0)continue;
				data.segment(length - biasz, _layerElementSize.x()*_layerElementSize.y()) = data.segment(biasz, _layerElementSize.x()*_layerElementSize.y());
			}
			break;
		}
	}
	return data;
}

size_t Core::AIMPECCFIE::gridGreenlocation(const Vector4i & p1, const Vector4i & p2) const
{
	const Vector3i w{ 1,_layerNum[0] ,_layerNum[0] * _layerNum[1] };
	return w.dot((p1.head<3>() - p2.head<3>()).cwiseAbs());
}
