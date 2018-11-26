#include "stdafx.h"
#include "ModalGeometry.h"
#include "Log.h"

Assist::ModalGeometry Assist::ModalGeometry::_instance;

void Assist::ModalGeometry::SetLimitationBoundary(Mesh * mesh)
{
	vector<Vector3d>* nodeVector = mesh->GridVector();
	vector<Vector3d>::iterator nodeptr = nodeVector->begin();
	const vector<Vector3d>::const_iterator ed=nodeVector->cend();

	double lowerX = nodeptr->x(), lowerY = nodeptr->y(), lowerZ = nodeptr->z();
	double upperX = lowerX, upperY = lowerY, upperZ = lowerZ;

	for (++nodeptr; nodeptr!=ed ; ++nodeptr)
	{
		if (nodeptr->x() < lowerX)lowerX = nodeptr->x();
		if (nodeptr->y() < lowerY)lowerY = nodeptr->y();
		if (nodeptr->z() < lowerZ)lowerZ = nodeptr->z();

		if (nodeptr->x() > upperX)upperX = nodeptr->x();
		if (nodeptr->y() > upperY)upperY = nodeptr->y();
		if (nodeptr->z() > upperZ)upperZ = nodeptr->z();
	}

	//lowerX = round(lowerX) - 1;
	//lowerY = round(lowerY) - 1;
	//lowerZ = round(lowerZ) - 1;
	//upperX = round(upperX) + 1;
	//upperY = round(upperY) + 1;
	//upperZ = round(upperZ) + 1;

	_boundaryLimitaion[0] = { lowerX,lowerY,lowerZ };
	_boundaryLimitaion[1]=  { upperX,lowerY,lowerZ };
	_boundaryLimitaion[2] = { lowerX,upperY,lowerZ };
	_boundaryLimitaion[3] = { upperX,upperY,lowerZ };

	_boundaryLimitaion[4] = { lowerX,lowerY,upperZ };
	_boundaryLimitaion[5] = { upperX,lowerY,upperZ };
	_boundaryLimitaion[6] = { lowerX,upperY,upperZ };
	_boundaryLimitaion[7] = { upperX,upperY,upperZ };

	Console->info("Box is from ({0:+5.3f},{1:+5.3f},{2:+5.3f}) to ({3:+5.3f},{4:+5.3f},{5:+5.3f})",
		lowerX, lowerY, lowerZ, upperX, upperY, upperZ);
	ResultL->info("Lower Point of Box is ({0:+5.3f},{1:+5.3f},{2:+5.3f})", lowerX, lowerY, lowerZ);
	ResultL->info("Upper Point of Box is ({0:+5.3f},{1:+5.3f},{2:+5.3f})", upperX, upperY, upperZ);
}
