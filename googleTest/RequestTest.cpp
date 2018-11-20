#include "stdafx.h"
#ifdef GTEST
#include "Data.h"
#include "IntegrationRWG.h"
#include "FarField.h"
#include <iomanip>
#include "gtest/gtest.h"
#include "CoreAPI.h"
#include "Const.h"
using namespace Core;
using namespace Eigen;

class RequestTest :public testing::Test
{
public:
	static void SetUpTestCase()
	{
		try
		{
			ASSERT_EQ(0, Core::CreatMesh()) << "Error in Creat Mesh";
			ASSERT_EQ(0, Core::CreatBasicFunction(false)) << "Error in Load BasicFunction";
			ASSERT_EQ(0, Core::SetGreenFunction()) << "Error in set Green Function";
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
		if (ComponentList::MeshService) {
			delete ComponentList::MeshService; ComponentList::MeshService = nullptr;
			Console->debug("Release Mesh");
		}
		if (ComponentList::BFvector.size() > 0)
		{
			for (auto element : ComponentList::BFvector) { delete element; }
			ComponentList::BFvector.clear();
			ComponentList::BFvector.shrink_to_fit();
			Console->debug("Release BasicFunction");
		}
		if (ComponentList::Green)
		{
			delete ComponentList::Green;
			ComponentList::Green = nullptr;
			Console->debug("Release GreenFunction");
		}
	}
};

TEST_F(RequestTest, BaseTest)
{
	Request::FF post(ComponentList::Green, &ComponentList::BFvector, ComponentList::MeshService);
	const double coef = post.Radius*post.Radius * 4 * M_PI / (1 * 1);
	const int thetaNum = 181, phiNum = 1;
	const clock_t start = clock();
	for (int ph = 0; ph < phiNum; ++ph)
	{
		for (int th = 1;th < thetaNum; ++th)
		{
			const double theta = 0 + th * 1;
			const double phi = 0 + ph * 1;
			Vector3cd temp = post.EFieldModify(theta*M_PI_180, phi*M_PI_180);
			Console->debug("({0},{1}) = {2:12.7e} m^2", theta,phi,coef * temp.squaredNorm());
		}
	}

	const clock_t end = clock();
	double timecost = double(end - start) / CLOCKS_PER_SEC;
	Console->info("FarField Calculate cost {}s", timecost);

}

TEST_F(RequestTest, FarFieldTest)
{
	Request::FF post(ComponentList::Green, &ComponentList::BFvector, ComponentList::MeshService);
	const double coef = post.Radius*post.Radius * 4 * M_PI / (1 * 1);
	const int thetaNum = 181, phiNum = 1;
	const clock_t start = clock();
	ofstream ofs("E:/ZMC/Code/C_program/MUL/SourceData/RCSTest.dat",ios_base::out);
	ofs.flags(ios::left);
	if (!ofs.is_open())GTEST_FAIL()<<"Can't construct RCSTest.dat file!";
	ofs << setw(7) << "Theta" << setw(7) << "Phi" << setw(12) << "RCS(m2)" << endl;
	for (int ph = 0; ph < phiNum; ++ph)
	{
		for (int th = 0;th < thetaNum; ++th)
		{
			const double theta = 0 + th * 1;
			const double phi = 0 + ph * 1;
			Vector3cd efield(post.EField(theta*M_PI_180, phi*M_PI_180));
			Vector3cd efieldRef(post.EFieldModify(theta*M_PI_180, phi*M_PI_180));
			EXPECT_NEAR(efieldRef.norm(), efield.norm(), 1.0e-5) << "error in FarField Request (th,phi)=" << theta << phi;
			//cout << "Ref:\t" << efieldRef.transpose() << '\n' << "Cal:\t" << efield.transpose() << endl;
			Console->debug("({0},{1}) = {2:12.7e} m^2,{3:12.7e} m^2", theta, phi, 
				coef * efieldRef.squaredNorm(),coef * efield.squaredNorm());
			ofs << setw(7) << theta << setw(7) << phi << setw(12) << coef * efield.squaredNorm() << '\n';
		}
	}
	ofs.flush();
	ofs.close();
	const clock_t end = clock();
	double timecost = double(end - start) / CLOCKS_PER_SEC;
	Console->info("FarField Calculate cost {}s", timecost);

}

TEST_F(RequestTest, FarFieldTimeTest)
{
	Request::FF post(ComponentList::Green, &ComponentList::BFvector, ComponentList::MeshService);
	const double coef = post.Radius*post.Radius * 4 * M_PI / (1 * 1);
	const int thetaNum = 181, phiNum = 1;
	Console->info("Vertical RCS Node={0}*{1}={2}",thetaNum,phiNum,thetaNum*phiNum);
	const clock_t start = clock();
	for (int ph = 0; ph < phiNum; ++ph)
	{
		for (int th = 0;th < thetaNum; ++th)
		{
			const double theta = 0 + th * 1;
			const double phi = 0 + ph * 1;
			Vector3cd efield(post.EField(theta*M_PI_180, phi*M_PI_180));
			double RCS = coef * efield.squaredNorm();
		}
	}
	const clock_t end = clock();
	double timecost = double(end - start) / CLOCKS_PER_SEC;
	Console->info("FarField Calculate cost {}s", timecost);

}
#endif