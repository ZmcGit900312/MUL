#pragma once

#include <Eigen/Core>
#include "Green.h"
#include "IBasicFunction.h"
//Not use temporary
namespace Core
{
	class IE 
	{

	public:

		typedef dcomplex return_type1;
		typedef dcomplex return_type2;
		IE(IGreen* green) :_green(green) {}
		void ReSetBasicFunction(IBasicFunction* source, IBasicFunction* field=nullptr)
		{
			_source = source; _field = field;
		}
		virtual return_type1 operator()(const Vector3d pt) = 0;
		virtual return_type2 operator()(const Vector3d pt1,const Vector3d pt2) = 0;
		virtual ~IE() { _green = nullptr; _source = nullptr; _field = nullptr; }
	protected:
		
		IBasicFunction *_source=nullptr, *_field=nullptr;
		IGreen* _green = nullptr;
	};

#pragma region EFIE
	class EFIEpp:public IE
	{
	public:
		EFIEpp(IGreen*green):IE(green){}
		return_type1 operator()(const Vector3d pt)override;
		return_type2 operator()(const Vector3d pt1,const Vector3d pt2)override;
	};

	class EFIEpm :public IE
	{
	public:
		EFIEpm(IGreen*green) :IE(green) {}
		return_type1 operator()(const Vector3d pt)override;
		return_type2 operator()(const Vector3d pt1, const Vector3d pt2)override;
	};

	class EFIEmp :public IE
	{
	public:
		EFIEmp(IGreen*green) :IE(green) {}
		return_type1 operator()(const Vector3d pt)override;
		return_type2 operator()(const Vector3d pt1, const Vector3d pt2)override;
	};

	class EFIEmm :public IE
	{
	public:
		EFIEmm(IGreen*green) :IE(green) {}
		return_type1 operator()(const Vector3d pt)override;
		return_type2 operator()(const Vector3d pt1, const Vector3d pt2)override;
	};
#pragma endregion EFIE

#pragma region MFIE
	class MFIEpp :public IE
	{
	public:
		MFIEpp(IGreen*green) :IE(green) {}
		return_type1 operator()(const Vector3d pt)override;
		return_type2 operator()(const Vector3d pt1, const Vector3d pt2)override;
	};

	class MFIEpm :public IE
	{
	public:
		MFIEpm(IGreen*green) :IE(green) {}
		return_type1 operator()(const Vector3d pt)override;
		return_type2 operator()(const Vector3d pt1, const Vector3d pt2)override;
	};

	class MFIEmp :public IE
	{
	public:
		MFIEmp(IGreen*green) :IE(green) {}
		return_type1 operator()(const Vector3d pt)override;
		return_type2 operator()(const Vector3d pt1, const Vector3d pt2)override;
	};

	class MFIEmm :public IE
	{
	public:
		MFIEmm(IGreen*green) :IE(green) {}
		return_type1 operator()(const Vector3d pt)override;
		return_type2 operator()(const Vector3d pt1, const Vector3d pt2)override;
	};
#pragma endregion EFIE
}