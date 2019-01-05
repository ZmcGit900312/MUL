#include "stdafx.h"
#include "MatrixSetting.h"
#include "Const.h"

using namespace std;
using namespace AIMAssist;


MatrixSetting::MatrixSetting(const ImpConfiguration& configuration,IImpService*impedance):
VectorTp(Vector3i{ configuration.xNumber,configuration.yNumber,configuration.zNumber }),
_layerNum(Vector3i{ configuration.xNumber,configuration.yNumber,configuration.zNumber }), 
_compute(k, W4, W7,eta)
{
	MultiExpTool.Reset(configuration, W7);
	_interval = configuration.Interval;
	_unknowns = configuration.ImpSize;
	_threshold = configuration.Threshold*Lambda;
	_eps = configuration.NearCorrectionEps;
	_gridNum = MultiExpTool.GetGridNum();
	_localGreen.resize(_gridNum, _gridNum);
	_imp = static_cast<ImpAIM*>(impedance);
	Tools::TeoplitzMultiplicator = new TeoplitzAssist(VectorTp);
}

MatrixSetting::~MatrixSetting()
{
	_gama.clear(); 
	_gama.shrink_to_fit();
	_imp = nullptr;
}

void MatrixSetting::MultipoleExpansion(vector<IBasicFunction*>&bf)
{
	Console->debug("Mulitpole Expansion");
	_gama.clear();
	const size_t estimatedSize = _unknowns*_gridNum;
	//Gama Reservation
	vector<T> tripletsGamaX, tripletsGamaY, tripletsGamaZ, tripletsGamaD;

	tripletsGamaX.reserve(estimatedSize);
	tripletsGamaY.reserve(estimatedSize);
	tripletsGamaZ.reserve(estimatedSize);
	tripletsGamaD.reserve(estimatedSize);
	_imp->GetGammaX().reserve(estimatedSize);
	_imp->GetGammaY().reserve(estimatedSize);
	_imp->GetGammaZ().reserve(estimatedSize);
	_imp->GetGammaD().reserve(estimatedSize);
	_gama.reserve(estimatedSize);
	//MultiExpansion
	Console->debug("Push all the Gmama Matrix Into Triplets");
	const clock_t start = clock();
	for (int i = 0; i<_unknowns; ++i)
	{
		MatrixXcd coef = MultiExpTool(static_cast<RWG*>(bf[i]));

		gama temp{ MultiExpTool.GetIndex(),coef.col(0),coef.col(1),coef.col(2),coef.col(3) };
		const auto gridnum = temp.index.size();
		for (auto j = 0;j <gridnum; ++j)
		{
			//Push
			tripletsGamaX.push_back(T(temp.index(j), i, temp.gamax(j)));
			tripletsGamaY.push_back(T(temp.index(j), i, temp.gamay(j)));
			tripletsGamaZ.push_back(T(temp.index(j), i, temp.gamaz(j)));
			tripletsGamaD.push_back(T(temp.index(j), i, temp.gamad(j)));
		}
		_gama.push_back(temp);
	}
	_gama.shrink_to_fit();
	tripletsGamaX.shrink_to_fit();
	tripletsGamaY.shrink_to_fit();
	tripletsGamaZ.shrink_to_fit();
	tripletsGamaD.shrink_to_fit();
	//cout << "Finish Multipole Expansion for Triplets...OK\n";
		
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
	const double timecost =static_cast<double>(end - start) / CLOCKS_PER_SEC;
	Console->info("Multipole Expansion costs time:\t{}s", timecost);
	ResultL->info("Multipole Expansion costs time:\t{}s", timecost);

	tripletsGamaX.clear(); tripletsGamaX.shrink_to_fit();
	tripletsGamaY.clear(); tripletsGamaY.shrink_to_fit();
	tripletsGamaZ.clear(); tripletsGamaZ.shrink_to_fit();
	tripletsGamaD.clear(); tripletsGamaD.shrink_to_fit();
}

void MatrixSetting::NearCorrection(vector< IBasicFunction*>&bf)
{
	Console->info("Near Field Set under Traditional Way");
	if (_gama.size() < 1 || VectorTp.Length() < 1)
		throw spd::spdlog_ex("Error in NearCorrection: Please Call MultipoleExpansion first");

	const size_t estimatedSize = _unknowns*_unknowns/100;
	const size_t Sum = (_unknowns + 1)*_unknowns;
	size_t currentProgress = 0;
	//cout << "Reserve the NearPart of Impedance...\n";
	//Reserve
	vector<T> tripletsNearPart;
	tripletsNearPart.reserve(estimatedSize);

	//Set Near Field Triplets
	const clock_t start = clock();

	for (int row = 0; row<_unknowns; ++row)
	{
		const auto field = static_cast<RWG*>(bf[row]);
		for (int col = row + 1; col<_unknowns; ++col)
		{
			const auto source = static_cast<RWG*>(bf[col]);
			Vector3d distance = field->Centre() - source->Centre();
			if (distance.norm()>_threshold)continue;
			const dcomplex comp = GetFarFieldImpedacneAIM(row, col);
			const dcomplex ref = _compute.SetImpedance(field, source);
			const dcomplex difvalue = ref - comp;
			if (norm(difvalue) / norm(ref)<_eps)continue;
			tripletsNearPart.push_back(T(row, col, difvalue));
			tripletsNearPart.push_back(T(col, row, difvalue));
			/*_imp->GetNearFieldMatrix().insert(row, col) = difvalue;
			_imp->GetNearFieldMatrix().insert(col, row) = difvalue;*/
		}
		const dcomplex comp = GetFarFieldImpedacneAIM(row, row);
		const dcomplex ref = _compute.SetImpedance(field, field);
		tripletsNearPart.push_back(T(row, row, ref - comp));
		//_imp->GetNearFieldMatrix().insert(row, row) = ref - comp;
		currentProgress += 2*(_unknowns-row);
		cout <<"Progress:"<< setw(10) << 100 * static_cast<double>(currentProgress) / Sum << "%\r";
	}
	tripletsNearPart.shrink_to_fit();
	_imp->GetNearFieldMatrix().reserve(tripletsNearPart.size());
	_imp->GetNearFieldMatrix().setFromTriplets(tripletsNearPart.begin(), tripletsNearPart.end());
	_imp->GetNearFieldMatrix().makeCompressed();
	tripletsNearPart.clear();

	const clock_t end = clock();
	const double timecost = double (end - start) / CLOCKS_PER_SEC;
	cout << "\r";
	Console->info("Near Field FillingTime is:\t{}s", timecost);
	ResultL->info("Near Field FillingTime is:\t{}s", timecost);
	Console->info("Triplet and NearField have {0} and {1} elements.", tripletsNearPart.size(), _imp->GetNearFieldMatrix().nonZeros());
	Console->info("Nonzeros have {0} and take {1}%.", _imp->GetNearFieldMatrix().nonZeros(),
		100 * (double)_imp->GetNearFieldMatrix().nonZeros() / (_unknowns*_unknowns));

	//Precondition
	Solver->Precondition(_imp);
	
}

void Core::MatrixSetting::TriangleFillingStrategy(Mesh & mesh, vector<IBasicFunction*>&bf)
{
	Console->info("Near Field Set under TFS");
	if (_gama.size() < 1 || VectorTp.Length() < 1)
		throw spd::spdlog_ex("Error in NearCorrection: Please Call MultipoleExpansion first");

	const size_t estimatedSize = _unknowns*_unknowns / 25;
	const size_t triangleNum = static_cast<size_t>(mesh.GetTriangle());
	size_t currentProgress = 0;
	//cout << "Reserve the NearPart of Impedance...\n";
	//Reserve
	vector<T> tripletsNearPart;
	tripletsNearPart.reserve(estimatedSize);
	//Set Near Field Triplets
	clock_t start = clock();
	
	for(auto r=mesh.TriangleMock.begin(),ed=mesh.TriangleMock.end();r!=ed;++r,++currentProgress)
	{		
		list<element> Z;
		RWGTriangle* row = dynamic_cast<RWGTriangle*>(*r);
		for(auto c=r+1;c!=ed;++c)
		{
			RWGTriangle* col = dynamic_cast<RWGTriangle*>(*c);
			Vector3d distance = row->Centre() - col->Centre();
			if (distance.norm()>_threshold*1.3)continue;
			
			//Calculate Triangle integration
			for (short i = 0; i < 3; i++)
			{
				if(!row->RWGSign[i])continue;
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
			_compute.SetImpedance(row, col,Z);
			//Set LowerTriangle
			for(auto i=Z.cbegin();i!=Z.cend();++i)
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
			//Calculate Triangle integration
			for (short i = 0; i < 3; i++)
			{
				if (!row->RWGSign[i])continue;
				const auto source = row->RWGID(i);
				tripletsNearPart.push_back(T(source, source, row->Z(source, source)));
				for (short j = i+1; j < 3; j++)
				{
					if (!row->RWGSign[j])continue;
					const auto field = row->RWGID(j);
					const auto vaule = row->Z(field, source);
					//For Precondition
					tripletsNearPart.push_back(T(source, field, vaule));
					tripletsNearPart.push_back(T(field, source, vaule));
					//Non Precondition
					/*if(source<field)
					{
						tripletsNearPart.push_back(T(field, source, vaule));
					}
					else { tripletsNearPart.push_back(T(source, field, vaule)); }*/
				}
			}
		}
		cout << "Progress:" << setw(10) << 100*static_cast<double>(currentProgress) / triangleNum << "%\r";
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
	ResultL->info("Near-Matrix Setting by TFS Time is:\t{}s", timecost);
	ResultL->info("Triplet and NearField have {0} and {1} elements.", tripletsNearPart.size(), _imp->GetNearFieldMatrix().nonZeros());

	tripletsNearPart.clear();
	//PreCondition

	Solver->Precondition(_imp);
	//Near Correction
	Console->debug("Begin to Correction");
	start = clock();
	for (int col = 0; col < _imp->GetNearFieldMatrix().outerSize(); ++col)
	{
		for (SparseMatrix<dcomplex>::InnerIterator it(_imp->GetNearFieldMatrix(), col); it; ++it)
		{
			if (it.row() < it.col())continue;
			const dcomplex comp = GetFarFieldImpedacneAIM(it.row(), it.col());
			const dcomplex difvalue = it.value() - comp;
			if (it.row() == it.col())
			{
				tripletsNearPart.push_back(T(it.row(), it.col(), difvalue));
				continue;
			}
			if (norm(difvalue) / norm(it.value()) > _eps)
			{
				tripletsNearPart.push_back(T(it.row(), it.col(), difvalue));
				tripletsNearPart.push_back(T(it.col(), it.row(), difvalue));
			}

		}
	}
	tripletsNearPart.shrink_to_fit();
	_imp->GetNearFieldMatrix().reserve(tripletsNearPart.size());
	_imp->GetNearFieldMatrix().setFromTriplets(tripletsNearPart.cbegin(),tripletsNearPart.cend());
	_imp->GetNearFieldMatrix().makeCompressed();

	end = clock();
	timecost += double(end - start) / CLOCKS_PER_SEC;
	cout << "\r";
	const double sparsity = static_cast<double>(_imp->GetNearFieldMatrix().nonZeros()) / _unknowns / _unknowns;
	Console->info("Near Field by TFS TotalTime is:\t{}s", timecost);
	ResultL->info("Near Field by TFS TotalTime is:\t{}s", timecost);
	Console->debug("Nonzeros have {0} and take {1}%.", _imp->GetNearFieldMatrix().nonZeros(), 100* sparsity);
}

void MatrixSetting::GreenMatrixSet()
{
	_imp->GetGreen().resize(Tools::TeoplitzMultiplicator->Length());
	_imp->GetGreen() = Tools::TeoplitzMultiplicator->TeoplitzFFT(VectorTp);
}


VectorXcd MatrixSetting::generateGreenVector(IGreen* green) const
{
	Console->debug("Generate Green Base");
	const unsigned totalNum = VectorTp.rows();
	VectorXcd greenBase{ totalNum };
	greenBase(0) = 0;
	for (unsigned i =1; i < totalNum; ++i)
	{
		const auto pos = VectorTp.id_pos(i);
		const Vector3d temp{ _interval*pos.cast<double>() };
		greenBase(i) = green->Scalar(Vector3d::Zero(), temp);
	}

	return greenBase;
}
//Need k 1i eta from const
dcomplex MatrixSetting::GetFarFieldImpedacneAIM(const size_t row, const size_t col)
{
	gama& source = _gama[row],field = _gama[col];
	for (int i = 0; i < _gridNum; ++i)
	{
		for (int j = 0; j < _gridNum; ++j)
		{	
			_localGreen(i, j) = VectorTp.at(field.index(i),source.index(j));
		}
	}

	const dcomplex tempx = field.gamax.transpose()*(_localGreen*source.gamax);
	const dcomplex tempy = field.gamay.transpose()*(_localGreen*source.gamay);
	const dcomplex tempz= field.gamaz.transpose()*(_localGreen*source.gamaz);
	const dcomplex tempd = field.gamad.transpose()*(_localGreen*source.gamad);
	
	return 1i*k*eta*(tempx + tempy + tempz- tempd/ (k*k));
}


