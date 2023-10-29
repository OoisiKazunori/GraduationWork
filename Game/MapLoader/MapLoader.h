#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include "../Debug/ParameterMgr.h"
#include <DirectXMath.h>
#include <list>
#include"../KazLibrary/Loader/ModelLoader.h"
#include"../KazLibrary/Render/BasicDraw.h"


/*jsonのマップデータから各オブジェクトのトランスフォームをリストに格納していく*/


//木や岩などの配置
class MapObject
{
public:
	std::string m_objetName;
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotition;
	DirectX::XMFLOAT3 m_scale;

	float echoScale = 0.0f;
};

//敵の初期スポーン位置
class EnemySpornData
{
	std::string m_enemyName;
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotition;
	DirectX::XMFLOAT3 m_scale;
};

// マップデータ
class MapManager
{
	//
	static std::list<std::list<MapObject>> m_maps;
	//フォルダ内のファイルネームを取得
	static bool GetFileNames(std::string f_folderPath, std::list<std::string>& f_fileNames);
public:
	//jsonの読み込みをここで行っている
	static void Init();

	//ステージ番号を入れるとトランスフォームのリストが帰ってくる
	static std::list<MapObject> GetStageData(int f_stageNum)
	{
		std::list<std::list<MapObject>>::iterator l_itr = m_maps.begin();
		for (int i = 0; i < f_stageNum; i++)
		{
			l_itr++;
		}
		return *l_itr;
	};
	static KazMath::Transform3D GetPlayerStartPosition(int f_stageNum);
};
