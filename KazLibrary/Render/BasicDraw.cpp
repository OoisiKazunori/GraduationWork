#include "BasicDraw.h"

DrawFuncData::DrawCallData BasicDraw::SetModel(const std::shared_ptr<ModelInfomation>& arg_modelInfomation)
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

BasicDraw::BasicModelRender::BasicModelRender(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName, bool arg_deleteInSceneFlag) :
	m_model(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName), BasicDraw::SetModel(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName)))
{
	m_model.m_drawCommandData = arg_rasterize.SetPipeline(m_model.m_drawCommand, arg_deleteInSceneFlag);
}

BasicDraw::BasicModelRender::BasicModelRender(DrawingByRasterize& arg_rasterize, bool arg_deleteInSceneFlag)
{
	m_model.m_drawCommandData = arg_rasterize.SetPipeline(m_model.m_drawCommand, arg_deleteInSceneFlag);
}

BasicDraw::BasicModelRender::BasicModelRender()
{
}

void BasicDraw::BasicModelRender::Load(DrawingByRasterize& arg_rasterize, const std::string& arg_fileDir, const std::string& arg_fileName, bool arg_deleteInSceneFlag)
{
	std::shared_ptr<ModelInfomation>model(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName));
	m_model.Load(model, BasicDraw::SetModel(ModelLoader::Instance()->Load(arg_fileDir, arg_fileName)));
	m_model.m_drawCommand.m_deleteInSceneFlag = true;
	m_model.m_drawCommandData = arg_rasterize.SetPipeline(m_model.m_drawCommand, arg_deleteInSceneFlag);
}

BasicDraw::BasicTextureRender::BasicTextureRender(DrawingByRasterize& arg_rasterize, const std::string& arg_filePass, bool arg_isUIFlag) :
	m_tex(arg_rasterize, arg_filePass, BasicDraw::SetTex(), arg_isUIFlag)
{
	if (!arg_isUIFlag)
	{
		m_tex.m_drawCommand.renderTargetHandle = GBufferMgr::Instance()->GetRenderTarget()[0];
	}
}

BasicDraw::BasicTextureRender::BasicTextureRender(DrawingByRasterize& arg_rasterize, bool arg_isUIFlag) :m_tex(arg_rasterize, BasicDraw::SetTex(), arg_isUIFlag)
{
	if (!arg_isUIFlag)
	{
		m_tex.m_drawCommand.renderTargetHandle = GBufferMgr::Instance()->GetRenderTarget()[0];
	}
}
