#pragma once
#include <Helper/Compute.h>
#include "Helper/KazBufferHelper.h"
#include "Math/KazMath.h"
#include <array>

//アウトラインを出力するクラス。
namespace PostEffect {

	class Outline {

	public:

		/*===== 変数 =====*/

		ComputeShader m_outlineShader;		//アウトラインを計算。

		KazBufferHelper::BufferData m_outlineTargetWorld;		//アウトラインをかける対象のテクスチャ
		KazBufferHelper::BufferData m_outlineTargetNormal;		//アウトラインをかける対象のテクスチャ
		KazBufferHelper::BufferData m_outputAlbedoTexture;		//アウトラインの色テクスチャ
		KazBufferHelper::BufferData m_outputEmissiveTexture;	//アウトラインのエミッシブのテクスチャ
		KazBufferHelper::BufferData m_inputOutlineWorldTexture;	//視点からオブジェクトまでの距離
		KazBufferHelper::BufferData m_outlineColorTexture;	//視点からオブジェクトまでの距離

		KazBufferHelper::BufferData m_outlineColorConstBuffer;	//アウトラインの色
		struct OutlineData {
			KazMath::Vec4<float> m_color;
			KazMath::Vec3<float> m_outlineCenter;	//アウトラインを距離によってカリングする場合の中心地点
			float m_outlineLength;
			float m_isFound;
			float m_screenEdge;
		}m_outlineData;

		KazBufferHelper::BufferData m_echoConstBuffer;	//アウトラインの色
		struct EchoData {
			KazMath::Vec3<float> m_color;
			float m_echoAlpha;
			KazMath::Vec3<float> m_center;	//アウトラインを距離によってカリングする場合の中心地点
			float m_echoRadius;
		}m_echoData;

	public:

		/*===== 関数 =====*/

		Outline(
			KazBufferHelper::BufferData arg_outlineTargetWorld,
			KazBufferHelper::BufferData arg_outlineTargetNormal,
			KazBufferHelper::BufferData arg_silhouetteRenderTargetBuffer,
			KazBufferHelper::BufferData arg_outlineColor,
			KazBufferHelper::BufferData arg_eyeBuffer,
			KazBufferHelper::BufferData arg_silhouetteBuffer
		);

		//アウトラインを書き込む
		void Apply();

		KazBufferHelper::BufferData GetOutputAlbedoTexture() { return m_outputAlbedoTexture; }
		KazBufferHelper::BufferData GetOutputEmissiveTexture() { return m_outputEmissiveTexture; }

		void SetOutlineCenterPos(KazMath::Vec3<float> arg_outlineCenterPos) { m_outlineData.m_outlineCenter = arg_outlineCenterPos; }

	};

}