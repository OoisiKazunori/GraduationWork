#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include "../Debug/ParameterMgr.h"
#include <DirectXMath.h>
#include <list>

class MapObject
{
public:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotition;
	DirectX::XMFLOAT3 m_scale;
};

// ���x���f�[�^
class MapData
{
	std::list<MapObject> objects;

public:
	MapData(){};
	//����json�f�[�^������ꍇ
	MapData(ParameterMgr f_parm){};
	MapData(int a) {};
	void Init();
	/*void Update();
	void Draw();*/
};
