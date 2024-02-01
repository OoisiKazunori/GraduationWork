#pragma once
#include <Helper/Compute.h>
#include "Helper/KazBufferHelper.h"
#include "Math/KazMath.h"
#include <array>

//�A�E�g���C�����o�͂���N���X�B
namespace PostEffect {

	class Outline {

	public:

		/*===== �ϐ� =====*/

		ComputeShader m_outlineShader;		//�A�E�g���C�����v�Z�B

		KazBufferHelper::BufferData m_outlineTargetWorld;		//�A�E�g���C����������Ώۂ̃e�N�X�`��
		KazBufferHelper::BufferData m_outlineTargetNormal;		//�A�E�g���C����������Ώۂ̃e�N�X�`��
		KazBufferHelper::BufferData m_outputAlbedoTexture;		//�A�E�g���C���̐F�e�N�X�`��
		KazBufferHelper::BufferData m_outputEmissiveTexture;	//�A�E�g���C���̃G�~�b�V�u�̃e�N�X�`��
		KazBufferHelper::BufferData m_inputOutlineWorldTexture;	//���_����I�u�W�F�N�g�܂ł̋���
		KazBufferHelper::BufferData m_outlineColorTexture;	//���_����I�u�W�F�N�g�܂ł̋���

		KazBufferHelper::BufferData m_outlineColorConstBuffer;	//�A�E�g���C���̐F
		struct OutlineData {
			KazMath::Vec4<float> m_color;
			KazMath::Vec3<float> m_outlineCenter;	//�A�E�g���C���������ɂ���ăJ�����O����ꍇ�̒��S�n�_
			float m_outlineLength;
			float m_isFound;
			float m_screenEdge;
		}m_outlineData;

		KazBufferHelper::BufferData m_echoConstBuffer;	//�A�E�g���C���̐F
		struct EchoData {
			KazMath::Vec3<float> m_color;
			float m_echoAlpha;
			KazMath::Vec3<float> m_center;	//�A�E�g���C���������ɂ���ăJ�����O����ꍇ�̒��S�n�_
			float m_echoRadius;
		}m_echoData;

	public:

		/*===== �֐� =====*/

		Outline(
			KazBufferHelper::BufferData arg_outlineTargetWorld,
			KazBufferHelper::BufferData arg_outlineTargetNormal,
			KazBufferHelper::BufferData arg_silhouetteRenderTargetBuffer,
			KazBufferHelper::BufferData arg_outlineColor,
			KazBufferHelper::BufferData arg_eyeBuffer,
			KazBufferHelper::BufferData arg_silhouetteBuffer
		);

		//�A�E�g���C������������
		void Apply();

		KazBufferHelper::BufferData GetOutputAlbedoTexture() { return m_outputAlbedoTexture; }
		KazBufferHelper::BufferData GetOutputEmissiveTexture() { return m_outputEmissiveTexture; }

		void SetOutlineCenterPos(KazMath::Vec3<float> arg_outlineCenterPos) { m_outlineData.m_outlineCenter = arg_outlineCenterPos; }

	};

}