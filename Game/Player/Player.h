#pragma once
#include"../KazLibrary/Render/BasicDraw.h"
#include"../UI/UI.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../Game/UI/UI.h"

class MeshCollision;
class BulletMgr;
class Camera;
class ThrowableObjectController;

class Player {

private:

	BasicDraw::BasicModelRender m_model;	//�g�p���郂�f��
	KazMath::Transform3D m_transform;		//���f���̕`��Ɏg�p����g�����X�t�H�[�����
	KazMath::Vec3<float> m_prevPos;

	BasicDraw::BasicModelRender m_mk23Model;	//�g�p���郂�f��
	KazMath::Transform3D m_weaponTransform;
	KazMath::Vec3<float> m_weaponPosOffset;		//�e�̃��f����z�u����I�t�Z�b�g�BADS���Ă���ʒu����Ƃ���ADS���Ă��Ȃ��ʒu�ɂ��炵���肷��Ƃ��Ɏg�p����B
	KazMath::Vec3<float> m_gunReaction;
	const float GUN_REACTION = 0.25f;

	bool m_onGround;
	bool m_isADS;		//�e���\���Ă����Ԃ��H

	//�����蔻��p�̃��f��
	BasicDraw::BasicModelRender m_collisionModel;	//�g�p���郂�f��
	std::shared_ptr<MeshCollision> m_meshCollision;

	float m_gravity;
	const float GRAVITY = 0.05f;

	//�ړ����x
	const float MOVE_SPEED_STAND = 0.5f;
	const float MOVE_SPEED_SQUAT = 0.3f;
	const float MOVE_SPEED_CREEPING = 0.15f;

	//���˂̒x��
	int m_shotDelay;
	const int SHOT_DELAY = 20;

	//�S���̃^�C�}�[
	int m_heatbeatTimer;
	const int HEARTBEAT_TIMER = 45;
	const int HEARTBEAT_TIMER_FOUND = 32;
	SoundData m_heartbeatSE;

	bool m_isFoundToEnemy;

	SoundData m_playerShotSE;
	SoundData m_sonarSE;
	SoundData m_adsSE;

	//�p���̃X�e�[�^�X
	enum class PlayerAttitude {
		STAND,
		SQUAT,
	}m_playerAttitude;


public:

	Player(DrawingByRasterize& arg_rasterize, KazMath::Transform3D f_startPos);

	void Init();

	void Update(std::weak_ptr<Camera> arg_camera, WeponUIManager::WeponNumber arg_weaponNumber, std::weak_ptr<BulletMgr> arg_bulletMgr, std::weak_ptr<ThrowableObjectController> arg_throwableObjectController, std::list<std::shared_ptr<MeshCollision>> f_stageColliders, HPUI& arg_hpUI);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	KazMath::Transform3D GetTransform() { return m_transform; }
	bool GetIsADS() { return m_isADS; }

private:

	void Input(std::weak_ptr<Camera> arg_camera, std::weak_ptr<BulletMgr> arg_bulletMgr, WeponUIManager::WeponNumber arg_weaponNumber, std::weak_ptr<ThrowableObjectController> arg_throwableObjectController);
	void Rotate(std::weak_ptr<Camera> arg_camera);
	void Collision(std::list<std::shared_ptr<MeshCollision>> f_stageColliders);
	float GetMoveSpeed();

};