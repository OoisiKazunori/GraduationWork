#include "BasicDraw.h"

DrawFuncData::DrawCallData BasicDraw::SetModel(const std::shared_ptr<ModelInfomation>& arg_modelInfomation)
{
	return DrawFuncData::SetDefferdRenderingModelAnimation(arg_modelInfomation);
}

DrawFuncData::DrawCallData BasicDraw::SetInstanceModel(const std::shared_ptr<ModelInfomation>& arg_modelInfomation)
{
	return DrawFuncData::SetDefferdRenderingModelAnimationInstance(arg_modelInfomation, false);
}

DrawFuncData::DrawCallData BasicDraw::SetTex()
{
	DrawFuncData::PipelineGenerateData lData;
	lData.desc = DrawFuncPipelineData::SetTex();
	lData.shaderDataArray.emplace_back(KazFilePathName::RelativeShaderPath + "ShaderFile/" + "MultiSprite.hlsl", "VSmain", "vs_6_4", SHADER_TYPE_VERTEX);
	lData.shaderDataArray.emplace_back(KazFilePathName::RelativeShaderPath + "ShaderFile/" + "MultiSprite.hlsl", "PSmain", "ps_6_4", SHADER_TYPE_PIXEL);
	lData.blendMode = DrawFuncPipelineData::PipelineBlendModeEnum::ALPHA;

	//レンダーターゲットの設定
	lData.desc.NumRenderTargets = static_cast<UINT>(GBufferMgr::Instance()->GetRenderTargetFormat().size());
	for (int i = 0; i < GBufferMgr::Instance()->GetRenderTargetFormat().size(); ++i)
	{
		lData.desc.RTVFormats[i] = GBufferMgr::Instance()->GetRenderTargetFormat()[i];
	}

	return DrawFuncData::SetSpriteAlphaData(lData);
}

BasicDraw::BasicModelRender::BasicModelRender(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName, bool arg_deletePipelineInScene) :
	m_model(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName), BasicDraw::SetModel(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName)))
{
	m_model.m_drawCommandData = arg_rasterize.SetPipeline(m_model.m_drawCommand, arg_deletePipelineInScene);
}

BasicDraw::BasicModelRender::BasicModelRender(DrawingByRasterize& arg_rasterize, bool arg_deletePipelineInScene)
{
	m_model.m_drawCommandData = arg_rasterize.SetPipeline(m_model.m_drawCommand, arg_deletePipelineInScene);
}

BasicDraw::BasicModelRender::BasicModelRender()
{
}

void BasicDraw::BasicModelRender::Load(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName)
{
	std::shared_ptr<ModelInfomation>model(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName));
	m_model.Load(model, BasicDraw::SetModel(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName)));
	m_model.m_drawCommandData = arg_rasterize.SetPipeline(m_model.m_drawCommand);
}

void BasicDraw::BasicModelRender::Load(DrawingByRasterize& arg_rasterize, const std::shared_ptr<ModelInfomation>& arg_modelInfomation, const DrawFuncData::DrawCallData& arg_drawCall)
{
	std::shared_ptr<ModelInfomation>model(arg_modelInfomation);
	m_model.Load(model, arg_drawCall);
	m_model.m_drawCommandData = arg_rasterize.SetPipeline(m_model.m_drawCommand);
}

BasicDraw::BasicTextureRender::BasicTextureRender(DrawingByRasterize& arg_rasterize, const std::string& arg_filePass, bool arg_isUIFlag, bool arg_deletePipelineInScene) :
	m_tex(arg_rasterize, arg_filePass, BasicDraw::SetTex(), arg_isUIFlag, arg_deletePipelineInScene)
{
	if (!arg_isUIFlag)
	{
		m_tex.m_drawCommand.renderTargetHandle = GBufferMgr::Instance()->GetRenderTarget()[0];
	}
}

BasicDraw::BasicTextureRender::BasicTextureRender(DrawingByRasterize& arg_rasterize, const char* arg_filePass, bool arg_isUIFlag, bool arg_deletePipelineInScene) :
	m_tex(arg_rasterize, arg_filePass, BasicDraw::SetTex(), arg_isUIFlag, arg_deletePipelineInScene)
{
	if (!arg_isUIFlag)
	{
		m_tex.m_drawCommand.renderTargetHandle = GBufferMgr::Instance()->GetRenderTarget()[0];
	}
}

BasicDraw::BasicTextureRender::BasicTextureRender(DrawingByRasterize& arg_rasterize, bool arg_isUIFlag, bool arg_deletePipelineInScene) :m_tex(arg_rasterize, BasicDraw::SetTex(), arg_isUIFlag, arg_deletePipelineInScene)
{
	if (!arg_isUIFlag)
	{
		m_tex.m_drawCommand.renderTargetHandle = GBufferMgr::Instance()->GetRenderTarget()[0];
	}
}

BasicDraw::BasicTextureRender::BasicTextureRender(DrawingByRasterize& arg_rasterize, const DrawFuncData::DrawCallData& arg_drawCall, bool arg_isUIFlag, bool arg_deletePipelineInScene)
	:m_tex(arg_rasterize, arg_drawCall, arg_isUIFlag, arg_deletePipelineInScene)
{
	if (!arg_isUIFlag)
	{
		m_tex.m_drawCommand.renderTargetHandle = GBufferMgr::Instance()->GetRenderTarget()[0];
	}
}

BasicDraw::BasicTextureRender::BasicTextureRender()
{
}

BasicDraw::BasicLineRender::BasicLineRender(DrawingByRasterize& arg_rasterize) :
	m_render(arg_rasterize, DrawFuncData::SetLine(DrawFuncData::GetBasicGBufferShader()))
{
}

BasicDraw::BasicLineRender::BasicLineRender()
{
}

void BasicDraw::BasicLineRender::Generate(DrawingByRasterize& arg_rasterize)
{
	m_render.Generate(arg_rasterize, DrawFuncData::SetLine(DrawFuncData::GetBasicGBufferShader()));
}

BasicDraw::SilhouetteModelRender::SilhouetteModelRender(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName, bool arg_deletePipelineInScene)
	: m_modelSilhouette(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName), DrawFuncData::SetDefferdRenderingModelAnimationOutline(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName)))
{
	m_modelSilhouette.m_drawCommandData = arg_rasterize.SetPipeline(m_modelSilhouette.m_drawCommand, arg_deletePipelineInScene);
}

BasicDraw::SilhouetteModelRender::SilhouetteModelRender()
{
}

void BasicDraw::SilhouetteModelRender::Load(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName)
{
	std::shared_ptr<ModelInfomation>model(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName));
	m_modelSilhouette.Load(model, DrawFuncData::SetDefferdRenderingModelAnimationOutline(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName)));
	m_modelSilhouette.m_drawCommandData = arg_rasterize.SetPipeline(m_modelSilhouette.m_drawCommand);
}

void BasicDraw::SilhouetteModelRender::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, KazMath::Transform3D& arg_trasform3D, const KazMath::Color& arg_addColor, float arg_timeScale)
{
	m_modelSilhouette.m_drawCommand.extraBufferArray[4] = GBufferMgr::Instance()->GetEyePosBuffer();
	m_modelSilhouette.m_drawCommand.extraBufferArray[4].rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
	m_modelSilhouette.m_drawCommand.extraBufferArray[4].rootParamType = GRAPHICS_PRAMTYPE_DATA5;
	m_modelSilhouette.Draw(arg_rasterize, arg_blasVec, arg_trasform3D);
	//m_model.Draw(arg_rasterize, arg_blasVec, arg_trasform3D);
}

BasicDraw::BasicModelInstanceRender::BasicModelInstanceRender(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName, bool arg_deletePipelineInScene)
	:m_model(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName), BasicDraw::SetInstanceModel(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName)))
{
	m_model.m_drawCommand.drawIndexInstanceCommandData.drawIndexInstancedData.instanceCount = 10000;
	m_model.m_drawCommandData = arg_rasterize.SetPipeline(m_model.m_drawCommand, arg_deletePipelineInScene);
}


BasicDraw::BasicModelInstanceRender::BasicModelInstanceRender(DrawingByRasterize& arg_rasterize, bool arg_deletePipelineInScene)
{
	m_model.m_drawCommand.drawIndexInstanceCommandData.drawIndexInstancedData.instanceCount = 10000;
	m_model.m_drawCommandData = arg_rasterize.SetPipeline(m_model.m_drawCommand, arg_deletePipelineInScene);
}

BasicDraw::BasicModelInstanceRender::BasicModelInstanceRender()
{
}

void BasicDraw::BasicModelInstanceRender::Load(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName)
{
	std::shared_ptr<ModelInfomation>model(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName));
	m_model.Load(model, BasicDraw::SetInstanceModel(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName)));
	m_model.m_drawCommand.drawIndexInstanceCommandData.drawIndexInstancedData.instanceCount = 10000;
	m_model.m_drawCommandData = arg_rasterize.SetPipeline(m_model.m_drawCommand);
}

void BasicDraw::BasicModelInstanceRender::Load(DrawingByRasterize& arg_rasterize, const std::shared_ptr<ModelInfomation>& arg_modelInfomation, const DrawFuncData::DrawCallData& arg_drawCall)
{
	std::shared_ptr<ModelInfomation>model(arg_modelInfomation);
	m_model.Load(model, arg_drawCall);
	m_model.m_drawCommand.drawIndexInstanceCommandData.drawIndexInstancedData.instanceCount = 10000;
	m_model.m_drawCommandData = arg_rasterize.SetPipeline(m_model.m_drawCommand);
}

void BasicDraw::BasicModelInstanceRender::UploadTransformMatrix(std::vector<KazMath::Transform3D> arg_transform)
{
	std::vector<DirectX::XMMATRIX>matrixArray;
	for (auto& obj : arg_transform)
	{
		matrixArray.emplace_back(obj.GetMat());
	}
	m_uploadTransformBuffer.bufferWrapper->TransData(matrixArray.data(), sizeof(DirectX::XMMATRIX) * (int)arg_transform.size());
	m_vramTransformBuffer.bufferWrapper->CopyBuffer(m_uploadTransformBuffer.bufferWrapper);

	//バッファのセット
	m_vramTransformBuffer.rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
	m_vramTransformBuffer.rootParamType = GRAPHICS_PRAMTYPE_DATA;
	m_model.m_drawCommand.extraBufferArray[m_model.m_drawCommand.extraBufferArray.size() - 2] = m_compute.m_extraBufferArray.back();
}

void BasicDraw::BasicModelInstanceRender::UploadColor(std::vector<KazMath::Color> arg_color)
{
	std::vector<DirectX::XMFLOAT4>colorArray;
	for (auto& obj : arg_color)
	{
		colorArray.emplace_back(obj.ConvertColorRateToXMFLOAT4());
	}
	m_uploadColorBuffer.bufferWrapper->TransData(colorArray.data(), sizeof(DirectX::XMFLOAT4) * (int)colorArray.size());
	m_vramColorBuffer.bufferWrapper->CopyBuffer(m_uploadColorBuffer.bufferWrapper);
}

void BasicDraw::BasicModelInstanceRender::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blas)
{
	m_compute.Compute({ m_dispatchNum,1,1 });
	m_model.Draw(arg_rasterize, arg_blas, KazMath::Transform3D());
}

void BasicDraw::BasicModelInstanceRender::GenerateTransformBuffer(int arg_elementNum)
{
	m_uploadTransformBuffer = KazBufferHelper::SetUploadBufferData(sizeof(DirectX::XMMATRIX) * arg_elementNum, "ModelInstance-RAM");
	m_vramTransformBuffer = KazBufferHelper::SetGPUBufferData(sizeof(DirectX::XMMATRIX) * arg_elementNum, "ModelInstance-VRAM");
	m_vramTransformBuffer.bufferWrapper->ChangeBarrierUAV();

	m_uploadColorBuffer = KazBufferHelper::SetUploadBufferData(sizeof(DirectX::XMFLOAT4) * arg_elementNum, "ModelInstanceColor-RAM");
	m_vramColorBuffer = KazBufferHelper::SetGPUBufferData(sizeof(DirectX::XMFLOAT4) * arg_elementNum, "ModelInstanceColor-VRAM");
	m_vramColorBuffer.bufferWrapper->ChangeBarrierUAV();


	RootSignatureDataTest rootsignature;
	rootsignature.rangeArray.emplace_back(GRAPHICS_RANGE_TYPE_UAV_VIEW, GRAPHICS_PRAMTYPE_DATA);
	rootsignature.rangeArray.emplace_back(GRAPHICS_RANGE_TYPE_UAV_VIEW, GRAPHICS_PRAMTYPE_DATA2);
	rootsignature.rangeArray.emplace_back(GRAPHICS_RANGE_TYPE_UAV_VIEW, GRAPHICS_PRAMTYPE_DATA3);
	rootsignature.rangeArray.emplace_back(GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DATA);
	m_compute.Generate(ShaderOptionData("Resource/ShaderFiles/ComputeShader/InstanceModel.hlsl", "CSmain", "cs_6_4"), rootsignature);

	m_compute.m_extraBufferArray.emplace_back(m_vramTransformBuffer);
	m_compute.m_extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
	m_compute.m_extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_DATA;

	m_compute.m_extraBufferArray.emplace_back(m_vramColorBuffer);
	m_compute.m_extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
	m_compute.m_extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_DATA2;

	m_compute.m_extraBufferArray.emplace_back(CameraMgr::Instance()->m_cameraBuffer);
	m_compute.m_extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
	m_compute.m_extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_DATA;
	struct OutputData
	{
		DirectX::XMMATRIX m_mat;
		DirectX::XMFLOAT4 m_color;
	};
	m_compute.m_extraBufferArray.emplace_back(KazBufferHelper::SetGPUBufferData(sizeof(OutputData) * arg_elementNum));
	m_compute.m_extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
	m_compute.m_extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_DATA3;


	m_dispatchNum = (UINT)arg_elementNum / 1024;
}
