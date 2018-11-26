#pragma once
#ifndef MODALGEOMETRY_H
#define MODALGEOMETRY_H
#include "Mesh.h"

namespace Core
{
	namespace Assist
	{
		class ModalGeometry
		{
		public:
			~ModalGeometry(){}
			static ModalGeometry* GetInstance() { return &_instance; }

			/**
			 * \brief Get the limitation Boundary
			 * \param val Index of limitation
			 * \return Cartianse Boundary
			 */
			Vector3d GetLimitationBoundary(const int val)const { return _boundaryLimitaion[val]; }

			/**
			 * \brief According the Node to Calculate the boundary
			 * \param mesh Mesh File
			 */
			void SetLimitationBoundary(Mesh* mesh);
		private:
			ModalGeometry() = default;
			ModalGeometry operator = (ModalGeometry&) = delete;
			ModalGeometry(ModalGeometry&) = delete;
			/**
			 * \brief 0~7 from low to upper limitatioin
			 */
			Vector3d _boundaryLimitaion[8];

			static ModalGeometry _instance;
		};
	}
}

#endif