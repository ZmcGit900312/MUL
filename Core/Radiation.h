#pragma once
//辐射场积分核 not used
#include <Eigen/Core>
#include "Green.h"
#include "IBasicFunction.h"
#include "Integration.h"


namespace Core
{
	class RadiationKernel:IntegrationTriangleKernel<Vector3cd,double>
	{		
	public:
		//必须定义return_type1和return_type2
		typedef return_type1 return_type1;
		typedef return_type2 return_type2;
		RadiationKernel(IGreen* green) :_green(green) {}
		virtual ~RadiationKernel() {}
		virtual return_type1 operator()(const Vector3d pt) = 0;
		void SetObservation(const Vector3d val) { _observation = val; }
		void SetBasicFunction(IBasicFunction*val) { _source = val; }
	protected:
		IGreen* _green = nullptr;
		IBasicFunction* _source = nullptr;
		Vector3d _observation;
	};

	//Plus Radiation
	class RadiationKernelp :public RadiationKernel
	{
	public:
		RadiationKernelp(IGreen*green) :RadiationKernel(green) {}
		return_type1 operator()(const Vector3d pt)override;
	};

	//Minus Radiation
	class RadiationKernelm:public RadiationKernel
	{
	public:
		RadiationKernelm(IGreen*green):RadiationKernel(green){}
		return_type1 operator()(const Vector3d pt)override;
	};

	class RadiationTest
	{
	public:
		typedef Vector3cd return_type1;
		typedef dcomplex return_type2;
		RadiationTest(IGreen*green):_green(green){}
		~RadiationTest() { _green = nullptr; }
		return_type1 operator()(IBasicFunction* bf, bool pm, const Vector3d ob, const Vector3d pt) const;
	private:
		IGreen* _green = nullptr;
	};
}
