#include "BasicDraw.h"

DrawFuncData::DrawCallData BasicDraw::SetModel(const std::shared_ptr<ModelInfomation> &arg_modelInfomation)
{
	return DrawFuncData::SetDefferdRenderingModelAnimation(arg_modelInfomation);
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

void BasicDraw::BasicModelRender::Load(DrawingByRasterize &arg_rasterize, const std::string &arg_fileDir, const std::string &arg_fileName)
{
	std::shared_ptr<ModelInfomation>model(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName));
	m_model.Load(model, BasicDraw::SetModel(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName)));
	m_model.m_drawCommandData = arg_rasterize.SetPipeline(m_model.m_drawCommand);
}

void BasicDraw::BasicModelRender::Load(DrawingByRasterize &arg_rasterize, const std::shared_ptr<ModelInfomation> &arg_modelInfomation, const DrawFuncData::DrawCallData &arg_drawCall)
{
	std::shared_ptr<ModelInfomation>model(arg_modelInfomation);
	m_model.Load(model, arg_drawCall);
	m_model.m_drawCommandData = arg_rasterize.SetPipeline(m_model.m_drawCommand);
}

BasicDraw::BasicTextureRender::BasicTextureRender(DrawingByRasterize &arg_rasterize, const std::string &arg_filePass, bool arg_isUIFlag, bool arg_deletePipelineInScene) :
	m_tex(arg_rasterize, arg_filePass, BasicDraw::SetTex(), arg_isUIFlag, arg_deletePipelineInScene)
{
	if (!arg_isUIFlag)
	{
		m_tex.m_drawCommand.renderTargetHandle = GBufferMgr::Instance()->GetRenderTarget()[0];
	}
}

BasicDraw::BasicTextureRender::BasicTextureRender(DrawingByRasterize &arg_rasterize, const char *arg_filePass, bool arg_isUIFlag, bool arg_deletePipelineInScene) :
	m_tex(arg_rasterize, arg_filePass, BasicDraw::SetTex(), arg_isUIFlag, arg_deletePipelineInScene)
{
	if (!arg_isUIFlag)
	{
		m_tex.m_drawCommand.renderTargetHandle = GBufferMgr::Instance()->GetRenderTarget()[0];
	}
}

BasicDraw::BasicTextureRender::BasicTextureRender(DrawingByRasterize &arg_rasterize, bool arg_isUIFlag, bool arg_deletePipelineInScene) :m_tex(arg_rasterize, BasicDraw::SetTex(), arg_isUIFlag, arg_deletePipelineInScene)
{
	if (!arg_isUIFlag)
	{
		m_tex.m_drawCommand.renderTargetHandle = GBufferMgr::Instance()->GetRenderTarget()[0];
	}
}

BasicDraw::BasicTextureRender::BasicTextureRender(DrawingByRasterize &arg_rasterize, const DrawFuncData::DrawCallData &arg_drawCall, bool arg_isUIFlag, bool arg_deletePipelineInScene)
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

BasicDraw::BasicLineRender::BasicLineRender(DrawingByRasterize &arg_rasterize) :
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

BasicDraw::SilhouetteModelRender::SilhouetteModelRender(DrawingByRasterize &arg_rasterize, const std::string &arg_fileDir, const std::string &arg_fileName, bool arg_deletePipelineInScene)
	:	m_modelSilhouette(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName), DrawFuncData::SetDefferdRenderingModelAnimationOutline(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName)))
{
	m_modelSilhouette.m_drawCommandData = arg_rasterize.SetPipeline(m_modelSilhouette.m_drawCommand, arg_deletePipelineInScene);
}

BasicDraw::SilhouetteModelRender::SilhouetteModelRender()
{
}

void BasicDraw::SilhouetteModelRender::Load(DrawingByRasterize &arg_rasterize, const std::string &arg_fileDir, const std::string &arg_fileName)
{
	std::shared_ptr<ModelInfomation>model(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName));
	m_modelSilhouette.Load(model, DrawFuncData::SetDefferdRenderingModelAnimationOutline(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName)));
	m_modelSilhouette.m_drawCommandData = arg_rasterize.SetPipeline(m_modelSilhouette.m_drawCommand);
}

void BasicDraw::SilhouetteModelRender::Draw(DrawingByRasterize &arg_rasterize, Raytracing::BlasVector &arg_blasVec, KazMath::Transform3D &arg_trasform3D, const KazMath::Color &arg_addColor, float arg_timeScale)
{
	m_modelSilhouette.m_drawCommand.extraBufferArray[4] = GBufferMgr::Instance()->GetEyePosBuffer();
	m_modelSilhouette.m_drawCommand.extraBufferArray[4].rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
	m_modelSilhouette.m_drawCommand.extraBufferArray[4].rootParamType = GRAPHICS_PRAMTYPE_DATA5;
	m_modelSilhouette.Draw(arg_rasterize, arg_blasVec, arg_trasform3D);
	//m_model.Draw(arg_rasterize, arg_blasVec, arg_trasform3D);
}
