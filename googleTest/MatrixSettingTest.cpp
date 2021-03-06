#include "stdafx.h"

#ifdef GTEST
#include "gtest/gtest.h"
#include "ConventionalMethod.h"
#include "CoreAPI.h"
#include "Const.h"
#include "Data.h"

using namespace Core;
using namespace Eigen;
using namespace AIMAssist;

class AIMTest :public testing::Test
{
protected:
	EFRImp _compute;
	static ConventionalMethod* aimComputer;
	AIMTest() :_compute(k, W4, W7, eta) {}
	~AIMTest() {}

	static void SetUpTestCase()
	{
		try
		{
			SystemConfig.ImpConfig.impType = AIM;
			//SystemConfig.SolverConfig.Precond = Solution::ILU;
			if (Mesh::GetInstance()->IsLock())
			{
				ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
				ASSERT_EQ(0, Core::CreatBasicFunction(false)) << "Error in Load BasicFunction";
			}
			if (ComponentList::BFvector.size() < 1)ASSERT_EQ(0, Core::CreatBasicFunction(false)) << "Error in Load BasicFunction";
			if (!Core::IGreen::GetInstance())EXPECT_EQ(0, Core::SetGreenFunction());
			ASSERT_EQ(0, Core::PreCalculateSelfTriangleImpedance()) << "Error in Pre-compute the SelfTriangle Impedance";
			ASSERT_EQ(0, Core::CreatImpedance()) << "Error in Initial the Impedance class";

			cout << "\n";
			Console->debug("Allocate the ConventionalMethod oject");
			//Random initial
			srand(static_cast<unsigned>(time(nullptr)));
			aimComputer = new ConventionalMethod(SystemConfig.ImpConfig, ComponentList::ImpService);
			aimComputer->MultipoleExpansion(ComponentList::BFvector);
			aimComputer->TeoplitzSet(IGreen::GetInstance());
			Console->debug("Teoplitz is ffting");
			aimComputer->GreenMatrixSet();

		}
		catch (spd::spdlog_ex&ex)
		{

			Console->warn(ex.what());
			RuntimeL->warn(ex.what());
			RuntimeL->flush();
		}
	}

	static void TearDownTestCase()
	{
		if (ComponentList::ImpService)
		{
			delete ComponentList::ImpService;
			ComponentList::ImpService = nullptr;
			Console->debug("Release Matrix");
		}
		if (Tools::TeoplitzMultiplicator)
		{
			delete Tools::TeoplitzMultiplicator;
			Tools::TeoplitzMultiplicator = nullptr;
			Console->debug("Release TeoplitzMultiplicator");
		}
		if(Core::Solver)
		{
			delete Core::Solver;
			Core::Solver = nullptr;
		}

		delete aimComputer;
		aimComputer = nullptr;
	}

	void FarFieldCompared(RWG*source, RWG*field) const
	{
		const size_t isource = source->GetID(), ifield = field->GetID();
		Vector3d distance = source->Centre() - field->Centre();
		const auto threshold = distance.norm() / Lambda;
		const dcomplex ref = _compute.SetImpedance(field, source);
		const dcomplex test = aimComputer->GetFarFieldImpedacneAIM(isource, ifield);
		Console->debug("\nImpedance:\t({0},{1})\nDistance:\t{2}��\nreference:\t({3},{4})\nNear:\t\t({5},{6})",
			isource, ifield, threshold, ref.real(), ref.imag(), test.real(), test.imag());
		if (threshold > 0.3)
		{
			const dcomplex difval = ref - test;
			const double eps = 1.0e-3;
			if (abs(difval) / abs(ref) > eps)
				Console->warn("FarField near error is more than {2}% for ({0},{1})", isource, ifield,100*eps);
		}
	}

};


ConventionalMethod* AIMTest::aimComputer = nullptr;


TEST_F(AIMTest, Multiplication)
{
	try
	{
		//throw spd::spdlog_ex("Multiplication is not Testing");
		auto& bf = ComponentList::BFvector;
		ImpAIM* imp = static_cast<ImpAIM*>(ComponentList::ImpService);

		const size_t unknowns = SystemConfig.ImpConfig.ImpSize;
		const double threshold = SystemConfig.ImpConfig.Threshold*Lambda;

		Console->debug("Matrix Far Field Random Setting Test:");
		for (int count = 0; count < 20;)
		{
			const int row = rand() % unknowns;
			const int col = rand() % unknowns;
			const auto source = static_cast<RWG*>(bf[row]);
			const auto field = static_cast<RWG*>(bf[col]);
			Vector3d distance = field->Centre() - source->Centre();
			const double dnorm = distance.norm();
			if (dnorm < threshold)continue;
			dcomplex ref = _compute.SetImpedance(field, source);
			VectorXcd vrowx = imp->GetGammaX().col(row);
			VectorXcd vrowy = imp->GetGammaY().col(row);
			VectorXcd vrowz = imp->GetGammaZ().col(row);
			VectorXcd vrowd = imp->GetGammaD().col(row);
			VectorXcd vcolx = imp->GetGammaX().col(col);
			VectorXcd vcoly = imp->GetGammaY().col(col);
			VectorXcd vcolz = imp->GetGammaZ().col(col);
			VectorXcd vcold = imp->GetGammaD().col(col);
			Tools::TeoplitzMultiplicator->MultiplyTeoplitz(imp->CGetGreen(), vcolx);
			Tools::TeoplitzMultiplicator->MultiplyTeoplitz(imp->CGetGreen(), vcoly);
			Tools::TeoplitzMultiplicator->MultiplyTeoplitz(imp->CGetGreen(), vcolz);
			Tools::TeoplitzMultiplicator->MultiplyTeoplitz(imp->CGetGreen(), vcold);
			dcomplex tempx = vrowx.dot(vcolx);
			dcomplex tempy = vrowy.dot(vcoly);
			dcomplex tempz = vrowz.dot(vcolz);
			dcomplex tempd = vrowd.dot(vcold);
			dcomplex test = 1i*k*eta*(tempx + tempy + tempz - tempd / (k*k));//Don't forget coefficient
			Console->debug("\nNumber:\t{0}\nImpedance:\t({1},{2})\nDistance:\t{3}��\nreference:\t({4},{5})\nNear:\t\t({6},{7})",
				++count, row, col, dnorm / Lambda, ref.real(), ref.imag(), test.real(), test.imag());
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		Console->warn(ex.what());
		RuntimeL->warn(ex.what());
		RuntimeL->flush();
	}

}

TEST_F(AIMTest, Multiplication2)
{
	try
	{
		//throw spd::spdlog_ex("Multiplication2 is not Testing");
		auto& bf = ComponentList::BFvector;
		ImpAIM* imp = static_cast<ImpAIM*>(ComponentList::ImpService);

		const size_t unknowns = SystemConfig.ImpConfig.ImpSize;
		const double threshold = SystemConfig.ImpConfig.Threshold*Lambda;

		Console->debug("Matrix Far Field Random Setting Test:");

		for (int count = 0; count < 20;)
		{
			const int row = rand() % unknowns;
			const int col = rand() % unknowns;
			const auto source = static_cast<RWG*>(bf[row]);
			const auto field = static_cast<RWG*>(bf[col]);
			Vector3d distance = field->Centre() - source->Centre();
			const double dnorm = distance.norm();
			if (dnorm < threshold)continue;

			const dcomplex ref = _compute.SetImpedance(field, source);
			VectorXcd x{ VectorXcd::Zero(unknowns) };
			x(col) = 1.0;
			VectorXcd test = *imp*x;
			/*For x=0,except x(col)=1
			 *The test is the col of all the impedance
			 *test(row) = Impedance(row,col)
			 */
			Console->debug("\nNumber:\t{0}\nImpedance:\t({1},{2})\nDistance:\t{3}��\nreference:\t({4},{5})\nNear:\t\t({6},{7})",
				++count, row, col, dnorm / Lambda, ref.real(), ref.imag(), test(row).real(), test(row).imag());
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		Console->warn(ex.what());
		RuntimeL->warn(ex.what());
		RuntimeL->flush();
	}
}


TEST_F(AIMTest, FarField)
{
	try
	{
		//throw spd::spdlog_ex("FarField is not Testing");
		auto& bf = ComponentList::BFvector;
		Console->debug("Far Field Set Test:");
		{
			Console->debug("Non Diagnol Compared");
			int source[3] = { 18,38,647 }, field[3] = { 654,22,123 };
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					FarFieldCompared(static_cast<RWG*>(bf[source[i]]), static_cast<RWG*>(bf[field[j]]));
				}
			}
			Console->debug("Diagnol Compared");
			FarFieldCompared(static_cast<RWG*>(bf[18]), static_cast<RWG*>(bf[18]));
			FarFieldCompared(static_cast<RWG*>(bf[56]), static_cast<RWG*>(bf[56]));
			FarFieldCompared(static_cast<RWG*>(bf[123]), static_cast<RWG*>(bf[123]));
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		Console->warn(ex.what());
		RuntimeL->warn(ex.what());
		RuntimeL->flush();
	}
}

TEST_F(AIMTest, NearField)
{
	try
	{
		//throw spd::spdlog_ex("NearField is not Testing");
		auto& bf = ComponentList::BFvector;

		const size_t unknowns = SystemConfig.ImpConfig.ImpSize;
		const double threshold = SystemConfig.ImpConfig.Threshold*Lambda;
		Console->debug("Matrix Near Field Random Setting Test:");
		for (int count = 0; count < 20;)
		{
			int row = rand() % unknowns, col = rand() % unknowns;
			const auto source = static_cast<RWG*>(bf[row]);
			const auto field = static_cast<RWG*>(bf[col]);
			Vector3d distance = field->Centre() - source->Centre();
			if (distance.norm() > threshold)continue;
			count++;
			FarFieldCompared(source, field);
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		Console->warn(ex.what());
		RuntimeL->warn(ex.what());
		RuntimeL->flush();
	}

}

TEST_F(AIMTest, TFSNearFieldSet)
{
	try
	{
		//throw spd::spdlog_ex("NearField is not Testing");
		auto& bf = ComponentList::BFvector;

		const size_t unknowns = SystemConfig.ImpConfig.ImpSize;
		const double threshold = SystemConfig.ImpConfig.Threshold*Lambda;
		Console->debug("Matrix Near Field TFS Setting Test:");
		auto mesh = Mesh::GetInstance();

		//Triplet
		typedef Triplet<dcomplex> T;
		int num = unknowns;
		SparseMatrix<dcomplex> imp{ num, num };
		
#pragma region TFS
		{
		Console->info("Set Near Field");

		const size_t estimatedSize = unknowns*unknowns / 25;
		const size_t Sum = (mesh->TriangleSize() + 1)*mesh->TriangleSize();
		size_t currentProgress = 0;
		//cout << "Reserve the NearPart of Impedance...\n";
		//Reserve
		vector<T> tripletsNearPart;
		tripletsNearPart.reserve(estimatedSize);
		//Set Near Field Triplets
		vector<element> Z;
		Z.reserve(9);

		const clock_t start = clock();
		int count = 0;
		for (auto r = mesh->TriangleVector.begin(), ed = mesh->TriangleVector.end();r != ed;++r, ++currentProgress)
		{
			//list<element> Z;
			RWGTriangle* row = dynamic_cast<RWGTriangle*>(*r);
			for (auto c = r + 1;c != ed;++c)
			{
				RWGTriangle* col = dynamic_cast<RWGTriangle*>(*c);
				Vector3d distance = row->Centre() - col->Centre();
				if (distance.norm()>threshold*1.3)continue;

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
						if (edgeDistance.norm() > threshold)
						{
							//RuntimeL->debug("BasisFunction {0} and {1} with distance:{2}", field->GetID(), source->GetID(), edgeDistance.norm());
							continue;
						}
						Z.push_back({ i, j, row->RWGSign[i] * col->RWGSign[j] });
					}
				}
				if (Z.size() == 0)continue;
				_compute.SetImpedance(row, col, Z);
				//Set LowerTriangle
				for (auto i = Z.cbegin(); i != Z.cend(); ++i)
				{
					auto source = std::get<0>(*i), field = std::get<1>(*i);
					const auto value = std::get<2>(*i);
					if (source == field)
					{
						tripletsNearPart.push_back(T(source, field, value*2.0));
						continue;
					}
					if (source> field)
					{
						tripletsNearPart.push_back(T(source, field, value));
					}
					else
					{
						tripletsNearPart.push_back(T(field, source, value));
					}
				}
				Z.clear();
			}
			//SelfSet
			{
				//Calculate Triangle integration
				for (short i = 0; i < 3; i++)
				{
					if (!row->RWGSign[i])continue;
					auto source = row->RWGID(i);
					tripletsNearPart.push_back(T(source, source, row->Z(source)));
					for (short j = i + 1; j < 3; j++)
					{
						if (!row->RWGSign[j])continue;
						auto field = row->RWGID(j);
						if (source<field)
						{
							tripletsNearPart.push_back(T(field, source, row->Z(field, source)));
						}
						else { tripletsNearPart.push_back(T(source, field, row->Z(source, field))); }
					}
				}

			}
			currentProgress += 100 * (mesh->TriangleSize() - count);
			cout << "Progress:" << setw(10) << currentProgress / Sum << "%\r";
		}
		tripletsNearPart.shrink_to_fit();
		imp.reserve(tripletsNearPart.size());
		imp.setFromTriplets(tripletsNearPart.begin(), tripletsNearPart.end());
		imp.makeCompressed();

		const clock_t end = clock();
		const double timecost = double(end - start) / CLOCKS_PER_SEC;
		cout << "\r";
		Console->debug("Near-Matrix Setting by TFS Time is:\t{}s", timecost);
		Console->debug("Triplet and NearField have {0} and {1} elements.", tripletsNearPart.size(), imp.nonZeros()); 
		}
#pragma endregion 
		
		for (int col = 0; col < imp.outerSize(); ++col)
		{
			for (SparseMatrix<dcomplex>::InnerIterator it(imp, col); it; ++it)
			{
				const dcomplex ref = _compute.SetImpedance(static_cast<RWG*>(bf[it.row()]), static_cast<RWG*>(bf[it.col()]));
				//dcomplex diffTest = ref - it.value();
				double eps = norm(ref - it.value()) / norm(it.value());
				if (eps > 1.0e-9)
				{
					RuntimeL->error("Eps of ({0},{1}) in TFSNearCorrection is:{2}", it.row(), it.col(), eps);
					throw spd::spdlog_ex("TFS has some error in MatrixSettingTest.cpp 402"); 
				}
					

			}
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		
		Console->warn(ex.what());
		RuntimeL->warn(ex.what());
		RuntimeL->flush();
	}
}


TEST_F(AIMTest, SolveTest)
{
	try
	{
		//throw spd::spdlog_ex("AIMCalculate is not Testing");

		if(SystemConfig.ImpConfig.FillingStrategy==1)
		aimComputer->TriangleFillingStrategy(*Mesh::GetInstance(), ComponentList::BFvector);
		else aimComputer->NearCorrection(ComponentList::BFvector);

		ASSERT_EQ(0, Core::SetRightHand()) << "Error in Set RightHand";
		auto info = Core::Solve();
		EXPECT_EQ(0, info) << "Error in Solve Matrix with BicgStab";
		if (info == 0)
		{
			EXPECT_EQ(0, Core::SaveBasicFunction(SystemConfig.BasicFunctionFileName.c_str())) << "Error in save BasicFunction";
			EXPECT_EQ(0, Core::CalculateRequest()) << "Error in Calculate the FarField";
		}
	}
	catch (spd::spdlog_ex&ex)
	{
		Console->warn(ex.what());
		RuntimeL->warn(ex.what());
		RuntimeL->flush();
	}
}

#endif