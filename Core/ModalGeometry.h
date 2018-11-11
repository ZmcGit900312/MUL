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
			ModalGeometry(){}
			~ModalGeometry(){}
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
			/**
			 * \brief 0~7 from low to upper limitatioin
			 */
			Vector3d _boundaryLimitaion[8];
		};
	}
}

#endif