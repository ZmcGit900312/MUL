#include "stdafx.h"
#include "RWG.h"
#include "Green.h"
#define RWGMemory 20

using namespace Core;
using namespace std;


#pragma region RWG

RWG::RWG(const size_t id, const pair<int, Vector3d> node[4], Triangle* tplus, Triangle* tminus):
_id(id), _tplus(dynamic_cast<RWGTriangle*>(tplus)),_tminus(dynamic_cast<RWGTriangle*>(tminus))
{
	
	_node[0] = node[0];
	_node[1] = node[1];
	_node[2] = node[2];
	_node[3] = node[3];
	{
		int bindId = _tplus->NodeID(0) == node[0].first ? 0 : _tplus->NodeID(1) == node[0].first ? 1 : 2;
		//_tplus->RWGId[bindId] = id;
		_tplus->RWGSign[bindId] = 1;
		_tplus->Rn[bindId] = this;
		bindId = _tminus->NodeID(0) == node[1].first ? 0 : _tminus->NodeID(1) == node[1].first ? 1 : 2;
		//_tminus->RWGId[bindId] = id;
		_tminus->RWGSign[bindId] = -1;
		_tminus->Rn[bindId] = this;
	}
	
	_centre = (_node[2].second + _node[3].second) / 2.0;
	_edgeLength = (_node[2].second - _node[3].second).norm();
}

Vector3d RWG::CurrentPlus(const Vector3d val) const
{
	return _edgeLength/2/_tplus->Area()*(val-_node[0].second);
}

Vector3d RWG::CurrentMinus(const Vector3d val) const
{
	return _edgeLength / 2 / _tminus->Area()*(_node[1].second-val);
}

/**
 * \brief 以二进制形式储存当下的RWG基函数信息
 * \param ofs 写入的文件流
 */
void RWG::SaveBinary(ofstream & ofs)
{
	int info[7] = { static_cast<int>(_id),_tplus->ID() , _tminus->ID() ,_node[0].first ,
		_node[1].first ,_node[2].first ,_node[3].first };
	ofs.write(reinterpret_cast<char*>(info),sizeof(int)*7);
	ofs.write(reinterpret_cast<char*>(&_current), sizeof(dcomplex));
}

/**
 * \brief 静态方法：建立RWG基函数
 * \param mesh 剖分数据
 * \param RWGList 基函数序列
 * \return 基函数个数
 */
size_t RWG::CreatRWGBasicFunctionList(Mesh* mesh, vector<IBasicFunction*>* RWGList)
{
	int T1[3] = { 0 }, T2[3] = { 0 };
	pair<int, Vector3d> tempNode[4];
	
	size_t unknowns = 0;
	size_t currentProgress = 0;
	const size_t triangleNum =mesh->GetTriangle();
	RWGList->clear();
	RWGList->reserve(static_cast<size_t>(triangleNum)*1.5);

	for (auto plus = mesh->TriangleMock.begin(), ed = mesh->TriangleMock.end(); plus != ed; ++plus)
	{
		for (auto minus = plus + 1; minus != ed; ++minus)
		{
			short commonEdgeCount = 0;
			//Find the common edge
			for (int p1 = 0; p1 < 3 && commonEdgeCount < 2; p1++)
			{
				for (int p2 = 0; p2 < 3; p2++)
				{
					if ((*plus)->NodeID(p1) == (*minus)->NodeID(p2))
					{
						T1[commonEdgeCount] = p1;
						T2[commonEdgeCount] = p2;
						++commonEdgeCount; break;
					}
				}
			}

			if (commonEdgeCount != 2)continue;
			//Set BasicFunction
			T1[2] = 3 - T1[0] - T1[1];
			T2[2] = 3 - T2[0] - T2[1];
			//node plus id;
			const int npid = (*plus)->NodeID(T1[2]);
			const int nmid = (*minus)->NodeID(T2[2]);
			const int nlid = (*plus)->NodeID(T1[0]);
			const int nrid = (*plus)->NodeID(T1[1]);

			tempNode[0] = { npid,mesh->GetNode(npid) };
			tempNode[1] = { nmid,mesh->GetNode(nmid) };
			tempNode[2] = { nlid,mesh->GetNode(nlid) };
			tempNode[3] = { nrid,mesh->GetNode(nrid) };

			RWGList->push_back(new RWG(unknowns++, tempNode, *plus, *minus));
		}
		cout << "Progress:" << setw(10) << 100 * static_cast<double>(currentProgress++) / triangleNum << "%\r";
	}
	return static_cast<size_t>(RWGList->size());
}

#pragma endregion 