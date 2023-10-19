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
class EnemySpornData
{
	std::string m_enemyName;
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotition;
	DirectX::XMFLOAT3 m_scale;
};

// �}�b�v�f�[�^
class MapManager
{
	//
	static std::list<std::list<MapObject>> m_maps;
	//�t�H���_���̃t�@�C���l�[�����擾
	static bool GetFileNames(std::string f_folderPath, std::list<std::string>& f_fileNames);
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
	static KazMath::Transform3D GetPlayerStartPosition(int f_stageNum);
};
