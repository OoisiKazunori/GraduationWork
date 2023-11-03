#pragma once
#include<vector>
#include <iostream>
#include <source_location>
#include"../KazLibrary/Math/KazMath.h"
#include"../Pipeline/Shader.h"
#include"../KazLibrary/Buffer/BufferDuplicateBlocking.h"
#include"../KazLibrary/Buffer/DrawFuncData.h"

/// <summary>
/// �X�^�b�N���ꂽ�������Ƀ��X�^���C�Y�ŕ`�悷��
/// </summary>
class DrawingByRasterize
{
public:
	DrawingByRasterize();

	//�`����̐������߂�ς�
	const DrawFuncData::DrawData* SetPipeline(DrawFuncData::DrawCallData& arg_drawData, bool arg_deleteInSceneFlag = false);
	/// <summary>
	/// �X�^�b�N���ꂽ�`����̐���(�}���`�X���b�h�Ή��ׂ̈Ɉꊇ�Ő������鏈��)
	/// </summary>
	void GeneratePipeline();
	/// <summary>
	/// �O�V�[���̕`�施�ߔj��
	/// </summary>
	void ReleasePipeline();
	/// <summary>
	/// �V�[�����ŕ`�施�ߔj��
	/// </summary>
	void ReleasePipelineInScene();

	void ObjectRender(const DrawFuncData::DrawData* arg_drawData);
	void UIRender(const DrawFuncData::DrawData* arg_drawData);

	void SortAndRender();
	void UISortAndRender();


	const DrawFuncData::DrawData* GenerateSceneChangePipeline(DrawFuncData::DrawCallData*arg_drawCall);
private:

	//���O��������-----
	//�`�搶�����߂̃L���[
	std::list<DrawFuncData::DrawCallData*>m_drawCallStackDataArray;
	//�`�施�߂̃f�[�^
	std::vector<std::unique_ptr<DrawFuncData::DrawData>>m_drawCallArray;
	//�`�施�߂̃L���[
	std::list<const DrawFuncData::DrawData*>m_stackDataArray;
	std::list<const DrawFuncData::DrawData*>m_uiStackDataArray;
	//�폜���ꂽ�`����̃n���h��
	std::vector<int>m_deleteHandleArray;
	//�폜���ꂽ�n���h������`��p�C�v���C�������n���h��
	std::vector<int>m_generateFromHandleArray;

	std::unique_ptr<DrawFuncData::DrawData> m_sceneChange;


	//�p�C�v���C���̏��----------------------------------------

	PipelineDuplicateBlocking m_piplineBufferMgr;
	ShaderDuplicateBlocking m_shaderBufferMgr;
	RootSignatureDuplicateBlocking m_rootSignatureBufferMgr;

	//�p�C�v���C���̏��----------------------------------------

	//�����_�[�^�[�Q�b�g���----------------------------------------
	RESOURCE_HANDLE prevRenderTargetHandle;
	struct RenderTargetClearData
	{
		RESOURCE_HANDLE handle;
		bool isOpenFlag;
		bool clearFlag;
	};
	std::vector<RenderTargetClearData>renderTargetClearArray;
	//�����_�[�^�[�Q�b�g���----------------------------------------

	//�`��ɕK�v�ȃo�b�t�@���R�}���h���X�g�ɐς�
	void SetBufferOnCmdList(const  std::vector<KazBufferHelper::BufferData>& BUFFER_ARRAY, std::vector<RootSignatureParameter> ROOT_PARAM);

	//���_���̃Z�b�g
	void MultiMeshedDrawIndexInstanceCommand(const KazRenderHelper::MultipleMeshesDrawIndexInstanceCommandData& DATA, const std::vector<std::vector<KazBufferHelper::BufferData>>& MATERIAL_BUFFER, std::vector<RootSignatureParameter> ROOT_PARAM);
	void DrawIndexInstanceCommand(const KazRenderHelper::DrawIndexInstanceCommandData& DATA);
	void DrawInstanceCommand(const KazRenderHelper::DrawInstanceCommandData& DATA);

	void DrawExecuteIndirect(const KazRenderHelper::MultipleMeshesDrawIndexInstanceCommandData& DATA, const Microsoft::WRL::ComPtr<ID3D12CommandSignature>& arg_commandSignature, const DrawFuncData::ExcuteIndirectArgumentData& arg_argmentData);

	//�����̕`��֐�����Ăяo���ꂽ���G���[��������
	void ErrorCheck(RESOURCE_HANDLE HANDLE, const std::source_location& DRAW_SOURCE_LOCATION)
	{
		if (HANDLE == -1)
		{
			FailCheck(ErrorMail(DRAW_SOURCE_LOCATION));
			assert(0);
		}
	}

	std::string ErrorMail(const std::source_location& DRAW_SOURCE_LOCATION);

	void DrawCall(const std::list<const DrawFuncData::DrawData*> arg_drawCall);
};