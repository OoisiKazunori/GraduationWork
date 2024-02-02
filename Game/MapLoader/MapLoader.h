#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include "../Debug/ParameterMgr.h"
#include <DirectXMath.h>
#include <list>
#include"../KazLibrary/Loader/ModelLoader.h"
#include"../KazLibrary/Render/BasicDraw.h"


/*json�̃}�b�v�f�[�^����e�I�u�W�F�N�g�̃g�����X�t�H�[�������X�g�Ɋi�[���Ă���*/


//�؂��Ȃǂ̔z�u
class MapObject
{
public:
	std::string m_objetName;
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotition;
	DirectX::XMFLOAT3 m_scale;

	float echoScale = 0.0f;
};

//�G�̏����X�|�[���ʒu
class EnemyData
{
public:
	std::list<DirectX::XMINT2> m_position;
};

// �}�b�v�f�[�^
class MapManager
{
	//
	static std::list<std::list<MapObject>> m_maps;

	static std::list<std::list<EnemyData>> m_enemys;
	//�t�H���_���̃t�@�C���l�[�����擾
	static bool GetFileNames(std::string f_folderPath, std::list<std::string>& f_fileNames);
	static std::list<DirectX::XMINT2> m_mapSize;
	static std::list<std::list<std::list<int>>> m_mapChips;
public:
	//json�̓ǂݍ��݂������ōs���Ă���
	static void Init();

	//�X�e�[�W�ԍ�������ƃg�����X�t�H�[���̃��X�g���A���Ă���
	static std::list<MapObject> GetStageData(int f_stageNum)
	{
		std::list<std::list<MapObject>>::iterator l_itr = m_maps.begin();
		for (int i = 0; i < f_stageNum; i++)
		{
			l_itr++;
		}
		return *l_itr;
	};
	////�ő�G��9�ő�`�F�b�N�|�C���g9
	//static std::list<EnemyData> GetEnemyData(int f_stageNum)
	//{
	//	std::list<std::list<EnemyData>>::iterator l_itr = m_enemys.begin();
	//	for (int i = 0; i < f_stageNum; i++)
	//	{
	//		l_itr++;
	//	}
	//	return *l_itr;
	//};
	//static DirectX::XMINT2 GetMapSizeData(int f_stageNum)
	//{
	//	std::list<DirectX::XMINT2>::iterator l_itr = m_mapSize.begin();
	//	for (int i = 0; i < f_stageNum; i++)
	//	{
	//		l_itr++;
	//	}
	//	return *l_itr;
	//};
	//static std::list<std::list<int>> GetMapChips(int f_stageNum)
	//{
	//	auto l_itr = m_mapChips.begin();
	//	for (int i = 0; i < f_stageNum; i++)
	//	{
	//		l_itr++;
	//	}
	//	return *l_itr;
	//};
	//static int GetMapChips(int f_stageNum, int f_x, int f_y)
	//{
	//	auto l_itr = m_mapChips.begin();
	//	for (int i = 0; i < f_stageNum; i++)
	//	{
	//		l_itr++;
	//	}
	//	auto y_itr = l_itr->begin();
	//	for (int i = 0; i < f_y; i++)
	//	{
	//		y_itr++;
	//	}
	//	auto xy_itr = y_itr->begin();
	//	for (int i = 0; i < f_x; i++)
	//	{
	//		xy_itr++;
	//	}
	//	return *xy_itr;
	//};

	static KazMath::Transform3D GetPlayerStartPosition(int f_stageNum);
};
