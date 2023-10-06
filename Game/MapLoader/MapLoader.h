#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include "../Debug/ParameterMgr.h"
#include <DirectXMath.h>
#include <list>

//木や岩などの配置
class MapObject
{
public:
	std::string m_objetName;
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotition;
	DirectX::XMFLOAT3 m_scale;
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
	MapManager(){};
	//既にjsonデータがある場合
	MapManager(ParameterMgr f_parm){};
	MapManager(int a) {};
	static void Init();
	static void Update();
	//現在有効なステージ番号
	static void Draw(int f_stageNumber);
};
