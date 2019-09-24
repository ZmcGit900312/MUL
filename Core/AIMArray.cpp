#include "stdafx.h"
#include "AIMArray.h"
#include "Multiplicator.h"

Core::AIMArray::AIMArray(const ImpConfiguration & configuration, IImpService * impedance, const IEConfiguration & ieConfig) :
IMatrixFiller(configuration, impedance, ieConfig),
_imp(static_cast<ImpArrayAIM*>(impedance))
{
	
	size_t layer[3] = { 2 * configuration.xNumber - 1,2 * configuration.yNumber - 1 ,2 * configuration.zNumber - 1 };
	MultiExpTool.Reset(configuration, layer, W7);
	_gridNum = MultiExpTool.GetGridNum();
	_localGreen.resize(_gridNum, _gridNum);
	_layerElementSize = 2 * _layerNum.array() - 1;
	for (int i = 0;i < _dim;i++)
	{
		_layerElementSizeAcu(i) = _layerElementSize.head(i + 1).prod();
	}
	//Array Information
	_numXArray = configuration.numArrayX;
	_numYArray = configuration.numArrayY;
	_distanceBiasX = configuration.distanceBiasX;
	_distanceBiasY = configuration.distanceBiasY;
	_totalUnknowns = _numXArray * _numYArray*_unknowns;

	//Layer Size
	_layerGreenSize.head(3)= 2 * _layerNum.array() - 1;
	_layerGreenSize[3] = 2 * _numXArray - 1;
	_layerGreenSize[4] = 2 * _numYArray - 1;
	for (int i = 0;i < 5;i++)
	{
		_layerGreenSizeAcu(i) = _layerGreenSize.head(i+1).prod();
	}

	Console->info("Take the Array AIM");
	RuntimeLog->info("Take the Array AIM");
	ResultLog->info("Take the Array AIM");
}

Core::AIMArray::~AIMArray()
{
	_gama.clear();
	_gama.shrink_to_fit();
	_imp = nullptr;
}

void Core::AIMArray::MultipoleExpansion(vector<IBasicFunction*>& bf)
{
	Console->debug("Mulitpole Expansion");
	_gama.clear();
	const size_t estimatedSize = _totalUnknowns * _gridNum;
	double unitExpTime = 0.0;
	//Gama Reservation
	vector<T> tripletsGamaX, tripletsGamaY, tripletsGamaZ, tripletsGamaD;

	tripletsGamaX.reserve(estimatedSize);
	tripletsGamaY.reserve(estimatedSize);
	tripletsGamaZ.reserve(estimatedSize);
	tripletsGamaD.reserve(estimatedSize);
	_imp->GetGammaX().resize(_layerGreenSizeAcu[4], _totalUnknowns);
	_imp->GetGammaX().reserve(estimatedSize);
	_imp->GetGammaY().resize(_layerGreenSizeAcu[4], _totalUnknowns);
	_imp->GetGammaY().reserve(estimatedSize);
	_imp->GetGammaZ().resize(_layerGreenSizeAcu[4], _totalUnknowns);
	_imp->GetGammaZ().reserve(estimatedSize);
	_imp->GetGammaD().resize(_layerGreenSizeAcu[4], _totalUnknowns);
	_imp->GetGammaD().reserve(estimatedSize);
	_gama.reserve(estimatedSize);


	//MultiExpansion
	Console->debug("Push all the Gama Matrix Into Triplets");
	const clock_t start = clock();
	for (int i = 0; i < _unknowns; ++i)
	{
		const clock_t startExpTime = clock();
		MatrixXcd coef = MultiExpTool(static_cast<RWG*>(bf[i]));
		const clock_t endExpTime = clock();
		unitExpTime += static_cast<double>(endExpTime - startExpTime) / CLOCKS_PER_SEC;

		gama temp{ MultiExpTool.Index,coef.col(0),coef.col(1),coef.col(2),coef.col(3) };
		const auto gridnum = temp.index.size();
		for (auto j = 0;j < gridnum; ++j)
		{
			//Push
			auto const pos = temp.index[j].w();
			for(int unitY=0;unitY<_numYArray;unitY++)
			{
				for(int unitX=0;unitX<_numXArray;unitX++)
				{
					size_t cols = _unknowns * (unitX+unitY* _numXArray)+i;
					size_t rows = pos + unitY * _layerGreenSizeAcu[3] + unitX * _layerGreenSizeAcu[2];
					tripletsGamaX.push_back(T(rows, cols, temp.gamax(j)));
					tripletsGamaY.push_back(T(rows, cols, temp.gamay(j)));
					tripletsGamaZ.push_back(T(rows, cols, temp.gamaz(j)));
					tripletsGamaD.push_back(T(rows, cols, temp.gamad(j)));
				}
			}
			
		}
#ifdef _DEBUG
		_gama.push_back(temp);//_gama is for debug
#endif
	}
#ifdef _DEBUG
	_gama.shrink_to_fit();
#endif
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
	Console->info("Multipole Expansion costs whole time:\t{:12.7f}s", timecost);
	Console->info("Multipole Expansion for unit time:\t{:12.7}s", unitExpTime);
	RuntimeLog->info("Multipole Expansion costs whole time:\t{:12.7f}s", timecost);
	RuntimeLog->info("Multipole Expansion for unit time:\t{:12.7f}s", unitExpTime);
	ResultLog->info("Multipole Expansion costs whole time:\t{:12.7f}s", timecost);
	ResultLog->info("Multipole Expansion for unit time:\t{:12.7f}s", unitExpTime);

	tripletsGamaX.clear(); tripletsGamaX.shrink_to_fit();
	tripletsGamaY.clear(); tripletsGamaY.shrink_to_fit();
	tripletsGamaZ.clear(); tripletsGamaZ.shrink_to_fit();
	tripletsGamaD.clear(); tripletsGamaD.shrink_to_fit();
}

void Core::AIMArray::GreenMatrixSet(IGreen * green)
{
	//Bind Green Point
	_green = green;

	_tools = new AIMAssist::MulFFTMultiplicator;
	//需要5阶FFT才可以
	MKL_LONG layer[5] = { _layerGreenSize[4] ,_layerGreenSize[3], 
		_layerGreenSize[2] ,_layerGreenSize[1] ,_layerGreenSize[0]  };

	_tools->Reset(5, layer);
	_imp->GetGreen().resize(_tools->Length());

	//Keep the Posization
	VectorXi position{ VectorXi::Zero(5) };//x-y-z-X-Y
	_imp->GetGreen() = ConstructIterated(position, position.size()-1);

	_tools->fwd(_imp->GetGreen());
	_imp->_fftTools = _tools;
}

void Core::AIMArray::TriangleFillingStrategy(Mesh & mesh, vector<IBasicFunction*>& bf)
{
	Console->info("Unit MoM Impedance Under TFS");
	vector<element> coupleList;
	coupleList.reserve(9);

	const auto beginTriangle = Mesh::GetInstance()->TriangleVector.begin();
	const auto endTriangle = Mesh::GetInstance()->TriangleVector.cend();


	const clock_t start = clock();
	//Face-to-Face Set Impedance
	for (auto i = beginTriangle; i != endTriangle; ++i)
	{
		RWGTriangle* izmc = dynamic_cast<RWGTriangle*>(*i);

		//auto selfList=compute.OperatorL(izmc);
		auto selfList = equation->Set(izmc);

		for (auto zmc = selfList.begin(); zmc != selfList.end(); ++zmc)
		{
			const int row = get<0>(*zmc);
			const int col = get<1>(*zmc);
			const dcomplex val = get<2>(*zmc);
			_imp->LocalMatrix()(row, col) += val;
			if (equation->GetType() == EFIE && row != col)_imp->LocalMatrix()(col, row) += val;
			//if (row == col)continue;
		}

		for (auto j = beginTriangle;j != i;++j)
		{
			RWGTriangle* jzmc = dynamic_cast<RWGTriangle*>(*j);

			coupleList.clear();
			for (short ilocal = 0; ilocal < 3; ilocal++)
			{
				for (short jlocal = 0; jlocal < 3; jlocal++)
				{
					coupleList.push_back({ ilocal, jlocal, izmc->RWGSign[ilocal] * jzmc->RWGSign[jlocal] });
				}
			}

			//auto coupleList =compute.SetImpedanceL(izmc, jzmc);
			equation->Set(izmc, jzmc, coupleList);
			for (auto zmc = coupleList.begin(); zmc != coupleList.end(); ++zmc)
			{
				const int row = get<0>(*zmc);
				const int col = get<1>(*zmc);
				const dcomplex val = get<2>(*zmc);
				_imp->LocalMatrix()(row, col) += val;
				_imp->LocalMatrix()(col, row) += val;
			}


		}
		cout << "Progress:" << setw(8) << izmc->ID() + 1 << "of " << Mesh::GetInstance()->TriangleSize() << "\r";
	}
	const clock_t end = clock();
	double timecost = double(end - start) / CLOCKS_PER_SEC;
	cout << "\r";
	Console->info("Near-Matrix Setting by TFS Time is:\t{}s", timecost);
	RuntimeLog->info("Near-Matrix Setting by TFS Time is:\t{}s", timecost);
	ResultLog->info("Near-Matrix Setting by TFS Time is:\t{}s", timecost);
}

void Core::AIMArray::NearCorrection(vector<IBasicFunction*>& bf)
{
}

#ifdef _DEBUG
dcomplex Core::AIMArray::GetFarFieldApproximateImpedacne(const size_t row, const size_t col, Vector3i arrayBias)
{
	gama& source = _gama[row], field = _gama[col];
	
	Vector3d Rb{ _distanceBiasX*arrayBias.x(),_distanceBiasY*arrayBias.y(),0 };


	for (int fieldIndex = 0; fieldIndex < _gridNum; ++fieldIndex)
	{
		Vector3d rf{ field.index[fieldIndex].x(),field.index[fieldIndex].y(),field.index[fieldIndex].z() };
		for (int sourceIndex = 0; sourceIndex < _gridNum; ++sourceIndex)
		{
			Vector3d rs{ source.index[sourceIndex].x(), source.index[sourceIndex].y() ,source.index[sourceIndex].z() };
			
			Vector3d Ru =  (rf - rs)*_interval;
			_localGreen(fieldIndex, sourceIndex) = _green->Scalar(Rb+Ru,Vector3d::Zero(3));
		}
	}

	const dcomplex tempx = field.gamax.transpose()*(_localGreen*source.gamax);
	const dcomplex tempy = field.gamay.transpose()*(_localGreen*source.gamay);
	const dcomplex tempz = field.gamaz.transpose()*(_localGreen*source.gamaz);
	const dcomplex tempd = field.gamad.transpose()*(_localGreen*source.gamad);

	return (tempx + tempy + tempz - tempd / (k*k)) * 1i*k*eta;
}

#endif
VectorXcd Core::AIMArray::ConstructIterated(VectorXi& pos, const unsigned level)
{
	const size_t length = _layerGreenSizeAcu[level];
	VectorXcd data{ VectorXcd::Zero(length) };
	const size_t N = (_layerGreenSize[level]+1)/2;
#ifdef _DEBUG
	cout << "level="<<level<<"  Location=["<<pos.transpose() << "]\r";
#endif

	if(level>0)
	{
		pos[level] = 0;
		const int childLength= _layerGreenSizeAcu[level - 1];
		data.head(childLength)= ConstructIterated(pos, level - 1);
		VectorXi _pos{ pos };
		for (pos[level]=1; pos[level] < N; ++pos[level])
		{
			_pos[level] = -pos[level];			
			data.segment(pos[level] * childLength, childLength) = ConstructIterated(pos, level - 1);
			data.segment(length - pos[level] * childLength, childLength) = ConstructIterated(_pos, level - 1);
			
		}
	}
	else
	{
		if (pos[3]==0&&pos[4] == 0)return data;
		pos[0] = 0;
		Vector3d Rb{ _distanceBiasX * pos[3] ,_distanceBiasY * pos[4] ,0 };
		Vector3d Ru{ _interval * pos[0] ,_interval * pos[1] ,_interval * pos[2] };
		Vector3d _Ru{ Ru };

		//data[0]= pos.sum()?_green->Scalar(Ru + Rb, Vector3d::Zero()):0;
		data[0] =  _green->Scalar(Ru + Rb, Vector3d::Zero()) ;
		//Green Teoplitz Rb+Ru
		for (pos[0] = 1; pos[0] < N; ++pos[0])
		{					
			Ru.x() = _interval * pos[0];
			_Ru.x() = -Ru.x();
			data[pos[0]]= _green->Scalar(Ru + Rb,Vector3d::Zero());
			data[length - pos[0]] = _green->Scalar(Rb+_Ru , Vector3d::Zero());
		}
		
		
	}
	return data;
}

