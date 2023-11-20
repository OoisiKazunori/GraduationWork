#pragma once
#include"../KazLibrary/Render/BasicDraw.h"

class MeshCollision;
class BulletMgr;
class Camera;
class ThrowableObjectController;

class Player {

private:

	BasicDraw::BasicModelRender m_model;	//�g�p���郂�f��
	KazMath::Transform3D m_transform;		//���f���̕`��Ɏg�p����g�����X�t�H�[�����
	KazMath::Vec3<float> m_prevPos;

	bool m_onGround;
	bool m_isADS;		//�e���\���Ă����Ԃ��H

	float m_gravity;
	const float GRAVITY = 0.05f;

	//�ړ����x
	const float MOVE_SPEED_STAND = 0.5f;
	const float MOVE_SPEED_SQUAT = 0.3f;
	const float MOVE_SPEED_CREEPING = 0.15f;

	//�p���̃X�e�[�^�X
	enum class PlayerAttitude {
		STAND,
		SQUAT,
		CREEPING,
	}m_playerAttitude;

public:

	Player(DrawingByRasterize& arg_rasterize, KazMath::Transform3D f_startPos);

	void Init();

	void Update(std::weak_ptr<Camera> arg_camera, std::weak_ptr<MeshCollision> arg_stageMeshCollision, std::weak_ptr<BulletMgr> arg_bulletMgr, std::weak_ptr<ThrowableObjectController> arg_throwableObjectController, std::list<std::shared_ptr<MeshCollision>> f_stageColliders);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	KazMath::Transform3D GetTransform() { return m_transform; }
	bool GetIsADS() { return m_isADS; }

private:

	void Input(std::weak_ptr<Camera> arg_camera, std::weak_ptr<BulletMgr> arg_bulletMgr);
	void Rotate(std::weak_ptr<Camera> arg_camera);
	void Collision(std::weak_ptr<MeshCollision> arg_meshCollision);
	float GetMoveSpeed();

};