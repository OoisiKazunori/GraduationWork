#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include "../Debug/ParameterMgr.h"
#include <DirectXMath.h>
#include <list>

//�؂��Ȃǂ̔z�u
class MapObject
{
public:
	std::string m_objetName;
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotition;
	DirectX::XMFLOAT3 m_scale;
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
	MapManager(){};
	//����json�f�[�^������ꍇ
	MapManager(ParameterMgr f_parm){};
	MapManager(int a) {};
	static void Init();
	static void Update();
	//���ݗL���ȃX�e�[�W�ԍ�
	static void Draw(int f_stageNumber);
};
