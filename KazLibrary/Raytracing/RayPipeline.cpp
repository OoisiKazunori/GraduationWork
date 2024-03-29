#include "RayPipeline.h"
#include "../Raytracing/RayPipeline.h"
#include "../Raytracing/RayRootsignature.h"
#include "../Raytracing/HitGroupMgr.h"
#include "../DirectXCommon/DirectX12Device.h"
#include "../DirectXCommon/DirectX12CmdList.h"
#include "../Buffer/DescriptorHeapMgr.h"
#include "../DirectXCommon/DirectX12.h"
#include "../Raytracing/RayShaderStorage.h"
#include "../Raytracing/Tlas.h"
#include "../Buffer/GBufferMgr.h"
#include "../DirectXCommon/DirectX12.h"
#include "../PostEffect/LensFlare.h"
#include <DirectXMath.h>
#include"../PostEffect/Outline.h"
#include "../Game/Player/PlayerStatus.h"

DirectX12* Raytracing::RayPipeline::m_refDirectX12 = nullptr;

namespace Raytracing {

	RayPipeline::RayPipeline(const std::vector<RayPipelineShaderData>& arg_inputData, HitGroupMgr::HITGROUP_ID arg_useHitGroup, int arg_SRVCount, int arg_CBVCount, int arg_UAVCount, int arg_payloadSize, int arg_attribSize, int arg_reflectionCount)
	{

		/*===== セッティング処理 =====*/

		//ヒットグループ名を保存。
		m_hitGroupName = HitGroupMgr::Instance()->hitGroupNames[arg_useHitGroup];

		//入力されたデータを保存する。
		const int INPUT_COUNT = static_cast<int>(arg_inputData.size());
		for (int index = 0; index < INPUT_COUNT; ++index) {

			//入力されたデータ構造体。
			RayPipelineShaderData buff;

			//保存する。
			buff.m_shaderPath = arg_inputData[index].m_shaderPath;

			//保存されているエントリポイントを保存。
			const int RG_ENTRY_COUNT = static_cast<int>(arg_inputData[index].m_rayGenEntryPoint.size());
			for (int entryPointIndex = 0; entryPointIndex < RG_ENTRY_COUNT; ++entryPointIndex) {

				//保存する。
				buff.m_rayGenEntryPoint.push_back(arg_inputData[index].m_rayGenEntryPoint[entryPointIndex]);

			}
			const int MS_ENTRY_COUNT = static_cast<int>(arg_inputData[index].m_missEntryPoint.size());
			for (int entryPointIndex = 0; entryPointIndex < MS_ENTRY_COUNT; ++entryPointIndex) {

				//保存する。
				buff.m_missEntryPoint.push_back(arg_inputData[index].m_missEntryPoint[entryPointIndex]);

			}

			//保存する。
			const int HS_ENTRY_COUNT = static_cast<int>(arg_inputData[index].m_hitgroupEntryPoint.size());
			for (int entryPointIndex = 0; entryPointIndex < HS_ENTRY_COUNT; ++entryPointIndex) {
				buff.m_hitgroupEntryPoint.push_back(arg_inputData[index].m_hitgroupEntryPoint[entryPointIndex]);
			}

			//保存する。
			m_shaderData.push_back(buff);

		}

		//グローバルルートシグネチャを設定。
		m_globalRootSig = std::make_shared<RayRootsignature>();
		//パラメーターt0にTLAS(SRV)を設定。
		for (int index = 0; index < arg_SRVCount; ++index)m_globalRootSig->AddRootparam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, index);
		//パラメーターb0にカメラ用バッファを設定。
		for (int index = 0; index < arg_CBVCount; ++index) {
			m_globalRootSig->AddRootparam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, index);
		}
		//パラメーターu0に出力用バッファを設定。
		for (int index = 0; index < arg_UAVCount; ++index)m_globalRootSig->AddRootparam(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, index);

		//ルートシグネチャを生成。
		m_globalRootSig->Build(false, L"GlobalRootSig");

		//ステートオブジェクトの設定を保存しておくようの構造体。
		CD3DX12_STATE_OBJECT_DESC subobjects;
		//ステートオブジェクトの状態を設定。
		subobjects.SetStateObjectType(D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE);

		//シェーダーを読み込む。
		const int SHADER_COUNT = static_cast<int>(m_shaderData.size());
		for (int index = 0; index < SHADER_COUNT; ++index) {

			m_shaderCode.emplace_back();

			//シェーダーをコンパイルする。
			RayShaderStorage::Instance()->LoadShaderForDXC(m_shaderData[index].m_shaderPath, "lib_6_4", "");

			//シェーダーを読み込む。
			m_shaderCode[index].BytecodeLength = static_cast<SIZE_T>(RayShaderStorage::Instance()->GetShaderBin(m_shaderData[index].m_shaderPath).size());
			m_shaderCode[index].pShaderBytecode = RayShaderStorage::Instance()->GetShaderBin(m_shaderData[index].m_shaderPath).data();

			//シェーダーの各関数レコードの登録。
			auto dxilLib = subobjects.CreateSubobject<CD3DX12_DXIL_LIBRARY_SUBOBJECT>();
			dxilLib->SetDXILLibrary(&m_shaderCode[index]);

			//シェーダーのエントリポイントを登録。
			const int RG_ENTRY_COUNT = static_cast<int>(arg_inputData[index].m_rayGenEntryPoint.size());
			for (int entryPointIndex = 0; entryPointIndex < RG_ENTRY_COUNT; ++entryPointIndex) {

				//保存する。
				dxilLib->DefineExport(m_shaderData[index].m_rayGenEntryPoint[entryPointIndex]);

			}
			const int MS_ENTRY_COUNT = static_cast<int>(arg_inputData[index].m_missEntryPoint.size());
			for (int entryPointIndex = 0; entryPointIndex < MS_ENTRY_COUNT; ++entryPointIndex) {

				//保存する。
				dxilLib->DefineExport(m_shaderData[index].m_missEntryPoint[entryPointIndex]);

			}
			const int HG_ENTRY_COUNT = static_cast<int>(arg_inputData[index].m_hitgroupEntryPoint.size());
			for (int entryPointIndex = 0; entryPointIndex < HG_ENTRY_COUNT; ++entryPointIndex) {

				//保存する。
				dxilLib->DefineExport(m_shaderData[index].m_hitgroupEntryPoint[entryPointIndex]);

			}

		}

		//ヒットグループの設定。
		auto hitGroup_ = subobjects.CreateSubobject<CD3DX12_HIT_GROUP_SUBOBJECT>();
		hitGroup_->SetHitGroupType(D3D12_HIT_GROUP_TYPE_TRIANGLES);

		//ClosestHitShaderをエントリポイントを保存。
		if (HitGroupMgr::Instance()->GetCHFlag(arg_useHitGroup)) {
			hitGroup_->SetClosestHitShaderImport(HitGroupMgr::Instance()->GetCH(arg_useHitGroup));
		}
		//AnyHitShaderのエントリポイントを保存。
		if (HitGroupMgr::Instance()->GetAHFlag(arg_useHitGroup)) {
			hitGroup_->SetAnyHitShaderImport(HitGroupMgr::Instance()->GetAH(arg_useHitGroup));
		}
		//IntersectShaderのエントリポイントを保存。
		if (HitGroupMgr::Instance()->GetISFlag(arg_useHitGroup)) {
			hitGroup_->SetIntersectionShaderImport(HitGroupMgr::Instance()->GetIS(arg_useHitGroup));
		}
		//ヒットグループ名を保存。
		hitGroup_->SetHitGroupExport(HitGroupMgr::Instance()->hitGroupNames[arg_useHitGroup]);

		//グローバルルートシグネチャの設定。
		auto rootSig = subobjects.CreateSubobject<CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT>();
		rootSig->SetRootSignature(m_globalRootSig->GetRootSig().Get());

		//HitGroupのローカルルートシグネチャを設定。
		auto chLocalRootSig = subobjects.CreateSubobject<CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT>();
		chLocalRootSig->SetRootSignature(HitGroupMgr::Instance()->GetLocalRootSig(arg_useHitGroup)->GetRootSig().Get());
		auto chAssocModel = subobjects.CreateSubobject<CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT>();
		chAssocModel->AddExport(HitGroupMgr::Instance()->hitGroupNames[arg_useHitGroup]);
		chAssocModel->SetSubobjectToAssociate(*chLocalRootSig);

		//シェーダーの設定。
		auto shaderConfig = subobjects.CreateSubobject<CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT>();
		shaderConfig->Config(arg_payloadSize, arg_attribSize);

		//パイプラインの設定。
		auto pipelineConfig = subobjects.CreateSubobject<CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT>();
		pipelineConfig->Config(arg_reflectionCount);

		//生成する。
		DirectX12Device::Instance()->dev->CreateStateObject(
			subobjects, IID_PPV_ARGS(&m_stateObject)
		);
		m_stateObject->SetName(L"StateObject");

		//Blasの保持数
		m_numBlas = 0;

		//レンズフレア
		m_lensFlare = std::make_shared<PostEffect::LensFlare>(GBufferMgr::Instance()->GetLensFlareBuffer(), GBufferMgr::Instance()->GetEmissiveGBuffer());

		m_outlineNoiseData.m_timer = 0.0f;
		m_outlineNoiseData.m_noisePower = 4.0f;
		m_outlineNoiseData.m_noiseHorizontalLine.x = 0.0f;
		m_outlineNoiseData.m_noiseHorizontalLine.y = 720.0f / 4.0f * 1.0f;
		m_outlineNoiseData.m_noiseHorizontalLine.z = 720.0f / 4.0f * 2.0f;
		m_outlineNoiseData.m_noiseHorizontalLine.a = 720.0f / 4.0f * 3.0f;
		m_outlineNoiseConstBufferData = KazBufferHelper::SetConstBufferData(sizeof(OutlineNoiseData));
		m_outlineNoiseConstBufferData.bufferWrapper->TransData(&m_outlineNoiseData, sizeof(OutlineNoiseData));

		//アウトライン合成用シェーダー
		{
			std::vector<KazBufferHelper::BufferData>extraBuffer =
			{
				 GBufferMgr::Instance()->GetRayTracingBuffer(),
				 GBufferMgr::Instance()->GetLensFlareBuffer(),
				 GBufferMgr::Instance()->m_outline->GetOutputAlbedoTexture(),
				 GBufferMgr::Instance()->m_outline->GetOutputEmissiveTexture(),
				 m_outlineNoiseConstBufferData
			};
			extraBuffer[0].rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
			extraBuffer[0].rootParamType = GRAPHICS_PRAMTYPE_TEX;
			extraBuffer[1].rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
			extraBuffer[1].rootParamType = GRAPHICS_PRAMTYPE_TEX2;
			extraBuffer[2].rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
			extraBuffer[2].rootParamType = GRAPHICS_PRAMTYPE_TEX3;
			extraBuffer[3].rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
			extraBuffer[3].rootParamType = GRAPHICS_PRAMTYPE_TEX4;
			extraBuffer[4].rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
			extraBuffer[4].rootParamType = GRAPHICS_PRAMTYPE_DATA;
			m_outlineComposeShader.Generate(ShaderOptionData(KazFilePathName::RelativeShaderPath + "PostEffect/Outline/" + "ComposeOutline.hlsl", "main", "cs_6_4", SHADER_TYPE_COMPUTE), extraBuffer);
		}
		{
			//std::vector<KazBufferHelper::BufferData>extraBuffer =
			//{
			//	 GBufferMgr::Instance()->m_outlineBuffer,
			//};
			//extraBuffer[0].rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
			//extraBuffer[0].rootParamType = GRAPHICS_PRAMTYPE_TEX;
			//m_outlineCleanShader.Generate(ShaderOptionData(KazFilePathName::RelativeShaderPath + "PostEffect/Outline/" + "BlackOut.hlsl", "main", "cs_6_4", SHADER_TYPE_COMPUTE), extraBuffer);
		}

	}

	void RayPipeline::BuildShaderTable(BlasVector arg_blacVector, int arg_dispatchX, int arg_dispatchY)
	{

		/*===== シェーダーテーブルを構築 =====*/

		//コンテナに登録されているBlasの数。
		int blasRefCount = arg_blacVector.GetBlasRefCount();

		//Blasの数が0だったら何もしない。
		if (blasRefCount == 0) return;

		//Blasの参照数がパイプラインに含まれているBlasの数を上回ったらシェーダーテーブルを再構築する。
		//if (m_numBlas < blasRefCount) {

			//再構築。
		ConstructionShaderTable(arg_blacVector, arg_dispatchX, arg_dispatchY);

		m_numBlas = blasRefCount;

		//}
		//else {

		//	//再構築せずにBlasの情報を更新。
		//	MapHitGroupInfo(arg_blacVector);

		//}

	}

	void RayPipeline::ConstructionShaderTable(BlasVector arg_blacVector, int arg_dispatchX, int arg_dispatchY)
	{

		/*===== シェーダーテーブルを構築 =====*/

		//シェーダーテーブルのサイズを計算。
		const auto ShaderRecordAlignment = D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT;

		//RayGenerationシェーダーではローカルルートシグネチャ未使用。
		m_raygenRecordSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
		m_raygenRecordSize = RoundUp(m_raygenRecordSize, ShaderRecordAlignment);

		//Missシェーダーではローカルルートシグネチャ未使用。
		m_missRecordSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
		m_missRecordSize = RoundUp(m_missRecordSize, ShaderRecordAlignment);

		//ヒットグループでは、保存されているヒットグループの中から最大のサイズのものでデータを確保する。
		m_hitgroupRecordSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
		m_hitgroupRecordSize += GetLargestDataSizeInHitGroup();
		m_hitgroupRecordSize = RoundUp(m_hitgroupRecordSize, ShaderRecordAlignment);

		//使用する各シェーダーの個数より、シェーダーテーブルのサイズを求める。
		m_hitgroupCount = HitGroupMgr::Instance()->GetHitGroupCount();
		m_raygenSize = GetRayGenerationCount() * m_raygenRecordSize;
		m_missSize = GetMissCount() * m_missRecordSize;
		m_hitGroupSize = m_hitgroupCount * m_hitgroupRecordSize;

		//各テーブルの開始位置にアライメント制約があるので調整する。
		m_tableAlign = D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT;
		m_raygenRegion = RoundUp(m_raygenRecordSize, m_tableAlign);
		m_missRegion = RoundUp(m_missSize, m_tableAlign);
		m_hitgroupRegion = RoundUp(m_hitGroupSize, m_tableAlign);

		//生成されたBLASの数。
		const int BLAS_COUNT = arg_blacVector.GetBlasRefCount();

		//シェーダーテーブルのサイズ。
		m_tableSize = m_raygenRegion + m_missRegion + m_hitgroupRegion * BLAS_COUNT;

		/*========== シェーダーテーブルの構築 ==========*/

		//シェーダーテーブル確保。
		m_shaderTable = CreateBuffer(
			m_tableSize, D3D12_RESOURCE_FLAG_NONE,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_HEAP_TYPE_UPLOAD,
			L"ShaderTable0");
		m_shaderTalbeMapAddress = nullptr;
		HRESULT result = m_shaderTable->Map(0, nullptr, &m_shaderTalbeMapAddress);

		result = m_stateObject.As(&m_rtsoProps);

		//シェーダーテーブルを書き込み、レイを設定する。
		WriteShadetTalbeAndSettingRay(arg_blacVector, arg_dispatchX, arg_dispatchY);

	}

	void RayPipeline::MapHitGroupInfo(BlasVector arg_blacVector)
	{

		/*===== HitGroupの情報を転送 =====*/

		uint8_t* pStart = static_cast<uint8_t*>(m_shaderTalbeMapAddress);

		//Hit Group 用のシェーダーレコードを書き込み。
		uint8_t* hitgroupStart = pStart + m_raygenRegion + m_missRegion;
		{

			uint8_t* pRecord = hitgroupStart;

			pRecord = arg_blacVector.WriteShaderRecord(pRecord, m_hitgroupRecordSize, m_stateObject, m_hitGroupName);

		}

	}

	void RayPipeline::TraceRay(Tlas arg_tlas)
	{

		/*===== レイトレーシングを実行 =====*/

		PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, "Raytracing");

		//レイトレで使用するリソース類をセット。
		SetRaytracingResource(arg_tlas);

		//レイトレーシングを実行。
		DirectX12CmdList::Instance()->cmdList->DispatchRays(&m_dispatchRayDesc);

		PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());

		/*===== 書き込んだアウトラインを合成 =====*/

		PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, "Outline");

		//アウトラインを計算
		GBufferMgr::Instance()->m_outline->Apply();

		//アウトラインをGBufferに書き込むと同時にノイズをかける。
		DispatchData dispatchData;
		dispatchData.x = static_cast<UINT>(1280 / 16) + 1;
		dispatchData.y = static_cast<UINT>(720 / 16) + 1;
		dispatchData.z = static_cast<UINT>(1);
		m_outlineComposeShader.Compute(dispatchData);

		//dispatchData.x = static_cast<UINT>(1280 / 16) + 1;
		//dispatchData.y = static_cast<UINT>(720 / 16) + 1;
		//dispatchData.z = static_cast<UINT>(1);
		//m_outlineCleanShader.Compute(dispatchData);

		PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());

		/*===== コピーコマンドを積む =====*/

		PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, "Lensflare");

		//UAVのバリアを貼る。
		UAVBarrier({ GBufferMgr::Instance()->GetLensFlareBuffer() , GBufferMgr::Instance()->GetRayTracingBuffer() });

		//レンズフレア用のテクスチャにガウシアンブラーをかけてフレアを表現する。
		GBufferMgr::Instance()->ApplyLensFlareBlur();

		//レンズフレアをかける。
		m_lensFlare->Apply();

		//シーン情報にレンズフレアを合成する。
		GBufferMgr::Instance()->ComposeLensFlareAndScene();

		//UAVのバリアを貼る。
		UAVBarrier({ GBufferMgr::Instance()->GetLensFlareBuffer() , GBufferMgr::Instance()->GetRayTracingBuffer() });

		//バックバッファの状態を遷移。
		auto backBufferIndex = m_refDirectX12->swapchain->GetCurrentBackBufferIndex();
		BufferStatesTransition(m_refDirectX12->GetBackBuffer()[backBufferIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE);

		//バックバッファ一時保存用テクスチャのステータスをコピー用に変更。
		BufferStatesTransition(GBufferMgr::Instance()->GetBackBufferCopyBuffer().bufferWrapper->GetBuffer().Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_DEST);

		//コピーを実行。
		DirectX12CmdList::Instance()->cmdList->CopyResource(GBufferMgr::Instance()->GetBackBufferCopyBuffer().bufferWrapper->GetBuffer().Get(), m_refDirectX12->GetBackBuffer()[backBufferIndex].Get());

		//バックバッファ一時保存用テクスチャのステータスを元に戻す。
		BufferStatesTransition(GBufferMgr::Instance()->GetBackBufferCopyBuffer().bufferWrapper->GetBuffer().Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

		//バックバッファの状態を元に戻す。
		BufferStatesTransition(m_refDirectX12->GetBackBuffer()[backBufferIndex].Get(), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COPY_DEST);

		PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());

		/*===== バックバッファを合成する =====*/

		PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, "ComposeBackBuffer");

		//合成を実行。
		GBufferMgr::Instance()->ComposeBackBuffer();

		//バックバッファ一時保存用テクスチャのステータスをコピー用に変更。
		BufferStatesTransition(GBufferMgr::Instance()->GetBackBufferCompositeBuffer().bufferWrapper->GetBuffer().Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);

		//コピーを実行。
		DirectX12CmdList::Instance()->cmdList->CopyResource(m_refDirectX12->GetBackBuffer()[backBufferIndex].Get(), GBufferMgr::Instance()->GetBackBufferCompositeBuffer().bufferWrapper->GetBuffer().Get());

		//バックバッファ一時保存用テクスチャのステータスを元に戻す。
		BufferStatesTransition(GBufferMgr::Instance()->GetBackBufferCompositeBuffer().bufferWrapper->GetBuffer().Get(), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

		//バックバッファの状態を元に戻す。
		BufferStatesTransition(m_refDirectX12->GetBackBuffer()[backBufferIndex].Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_RENDER_TARGET);

		PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());

	}

	void RayPipeline::Update()
	{

		//アウトラインにかけるノイズのタイマーの更新。
		m_outlineNoiseData.m_timer += 1.0f;


		//アウトラインにかけるノイズの縦線の更新
		float HORIZONAL_LINE_SPEED = 2.0f;
		if (PlayerStatus::Instance()->m_isFound) {
			HORIZONAL_LINE_SPEED = 4.0f;
		}
		m_outlineNoiseData.m_noiseHorizontalLine.x -= HORIZONAL_LINE_SPEED;
		if (m_outlineNoiseData.m_noiseHorizontalLine.x < 0.0f) {
			m_outlineNoiseData.m_noiseHorizontalLine.x = 720.0f;
		}
		m_outlineNoiseData.m_noiseHorizontalLine.y -= HORIZONAL_LINE_SPEED;
		if (m_outlineNoiseData.m_noiseHorizontalLine.y < 0.0f) {
			m_outlineNoiseData.m_noiseHorizontalLine.y = 720.0f;
		}
		m_outlineNoiseData.m_noiseHorizontalLine.z -= HORIZONAL_LINE_SPEED;
		if (m_outlineNoiseData.m_noiseHorizontalLine.z < 0.0f) {
			m_outlineNoiseData.m_noiseHorizontalLine.z = 720.0f;
		}
		m_outlineNoiseData.m_noiseHorizontalLine.a -= HORIZONAL_LINE_SPEED;
		if (m_outlineNoiseData.m_noiseHorizontalLine.a < 0.0f) {
			m_outlineNoiseData.m_noiseHorizontalLine.a = 720.0f;
		}

		if (PlayerStatus::Instance()->m_isFound) {

			m_outlineNoiseData.m_noisePower += (12.0f - m_outlineNoiseData.m_noisePower) / 4.0f;

		}
		else {

			m_outlineNoiseData.m_noisePower += (4.0f - m_outlineNoiseData.m_noisePower) / 4.0f;

		}

		m_outlineNoiseConstBufferData.bufferWrapper->TransData(&m_outlineNoiseData, sizeof(OutlineNoiseData));

	}

	UINT RayPipeline::GetLargestDataSizeInHitGroup()
	{

		/*===== 全てのヒットグループの中で最もデータサイズが大きいものを取得する =====*/

		UINT largestDataSize = 0;

		const int HIT_GROUP_COUNT = HitGroupMgr::Instance()->GetHitGroupCount();
		for (int index = 0; index < HIT_GROUP_COUNT; ++index) {

			//データサイズを取得。
			UINT dataSize = 0;
			dataSize += sizeof(D3D12_GPU_DESCRIPTOR_HANDLE) * HitGroupMgr::Instance()->GetSRVCount(static_cast<HitGroupMgr::HITGROUP_ID>(index));
			dataSize += sizeof(D3D12_GPU_VIRTUAL_ADDRESS) * HitGroupMgr::Instance()->GetCBVCount(static_cast<HitGroupMgr::HITGROUP_ID>(index));
			dataSize += sizeof(D3D12_GPU_DESCRIPTOR_HANDLE) * HitGroupMgr::Instance()->GetUAVCount(static_cast<HitGroupMgr::HITGROUP_ID>(index));

			//取得したデータサイズが保存されているデータサイズより小さかったら処理を飛ばす。
			if (dataSize < largestDataSize) continue;

			//取得したデータサイズを保存する。
			largestDataSize = dataSize;

		}

		return largestDataSize;

	}

	void RayPipeline::WriteShadetTalbeAndSettingRay(BlasVector arg_blacVector, int arg_dispatchX, int arg_dispatchY)
	{

		/*===== シェーダーテーブルを書き込み、レイを設定する =====*/

		//各シェーダーレコードを書き込んでいく。
		uint8_t* pStart = static_cast<uint8_t*>(m_shaderTalbeMapAddress);

		//RayGeneration 用のシェーダーレコードを書き込み。
		uint8_t* rgsStart = pStart;
		{
			uint8_t* p = rgsStart;

			const int SHADER_DATA_COUNT = static_cast<int>(m_shaderData.size());
			for (int index = 0; index < SHADER_DATA_COUNT; ++index) {

				const int RG_COUNT = static_cast<int>(m_shaderData[index].m_rayGenEntryPoint.size());
				for (int rgIndex = 0; rgIndex < RG_COUNT; ++rgIndex) {

					void* id = m_rtsoProps->GetShaderIdentifier(m_shaderData[index].m_rayGenEntryPoint[rgIndex]);
					p += WriteShaderIdentifier(p, id);

				}

			}

		}

		//Miss Shader 用のシェーダーレコードを書き込み。
		uint8_t* missStart = pStart + m_raygenRegion;
		{
			uint8_t* p = missStart;

			const int SHADER_DATA_COUNT = static_cast<int>(m_shaderData.size());
			for (int index = 0; index < SHADER_DATA_COUNT; ++index) {

				const int MS_COUNT = static_cast<int>(m_shaderData[index].m_missEntryPoint.size());
				for (int msIndex = 0; msIndex < MS_COUNT; ++msIndex) {

					void* id = m_rtsoProps->GetShaderIdentifier(m_shaderData[index].m_missEntryPoint[msIndex]);
					p += WriteShaderIdentifier(p, id);

				}

			}

		}

		//Hit Group 用のシェーダーレコードを書き込み。
		uint8_t* hitgroupStart = pStart + m_raygenRegion + m_missRegion;
		{

			uint8_t* pRecord = hitgroupStart;

			pRecord = arg_blacVector.WriteShaderRecord(pRecord, m_hitgroupRecordSize, m_stateObject, m_hitGroupName);

		}

		//レイ発射時の設定を設定。

		//DispatchRays のために情報をセットしておく.
		auto startAddress = m_shaderTable->GetGPUVirtualAddress();
		//RayGenerationシェーダーの情報
		auto& shaderRecordRG = m_dispatchRayDesc.RayGenerationShaderRecord;
		shaderRecordRG.StartAddress = startAddress;
		shaderRecordRG.SizeInBytes = m_raygenSize;
		startAddress += m_raygenRegion;
		//Missシェーダーの情報
		auto& shaderRecordMS = m_dispatchRayDesc.MissShaderTable;
		shaderRecordMS.StartAddress = startAddress;
		shaderRecordMS.SizeInBytes = m_missSize;
		shaderRecordMS.StrideInBytes = m_missRecordSize;
		startAddress += m_missRegion;
		//HitGroupの情報
		auto& shaderRecordHG = m_dispatchRayDesc.HitGroupTable;
		shaderRecordHG.StartAddress = startAddress;
		shaderRecordHG.SizeInBytes = m_hitGroupSize;
		shaderRecordHG.StrideInBytes = m_hitgroupRecordSize;
		startAddress += m_hitgroupRegion;
		//レイの情報
		m_dispatchRayDesc.Width = arg_dispatchX;
		m_dispatchRayDesc.Height = arg_dispatchY;
		m_dispatchRayDesc.Depth = 1;

	}

	UINT RayPipeline::WriteShaderIdentifier(void* arg_dst, const void* arg_shaderId)
	{
		memcpy(arg_dst, arg_shaderId, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
		return D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
	}

	int RayPipeline::GetRayGenerationCount()
	{
		int count = 0;

		const int SHADER_DATA_COUNT = static_cast<int>(m_shaderData.size());
		for (int index = 0; index < SHADER_DATA_COUNT; ++index) {

			count += static_cast<int>(m_shaderData[index].m_rayGenEntryPoint.size());

		}

		return count;

	}

	int RayPipeline::GetMissCount()
	{
		int count = 0;

		const int SHADER_DATA_COUNT = static_cast<int>(m_shaderData.size());
		for (int index = 0; index < SHADER_DATA_COUNT; ++index) {

			count += static_cast<int>(m_shaderData[index].m_missEntryPoint.size());

		}

		return count;

	}

	Microsoft::WRL::ComPtr<ID3D12Resource> RayPipeline::CreateBuffer(size_t arg_size, D3D12_RESOURCE_FLAGS arg_flag, D3D12_RESOURCE_STATES arg_initialState, D3D12_HEAP_TYPE arg_heapType, const wchar_t* arg_name)
	{

		/*===== バッファを生成 =====*/

		D3D12_HEAP_PROPERTIES heapProps{};
		if (arg_heapType == D3D12_HEAP_TYPE_DEFAULT) {
			heapProps = D3D12_HEAP_PROPERTIES{
			D3D12_HEAP_TYPE_DEFAULT, D3D12_CPU_PAGE_PROPERTY_UNKNOWN, D3D12_MEMORY_POOL_UNKNOWN, 1, 1
			};
		}
		if (arg_heapType == D3D12_HEAP_TYPE_UPLOAD) {
			heapProps = D3D12_HEAP_PROPERTIES{
			D3D12_HEAP_TYPE_UPLOAD, D3D12_CPU_PAGE_PROPERTY_UNKNOWN, D3D12_MEMORY_POOL_UNKNOWN, 1, 1
			};
		}
		HRESULT hr;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		D3D12_RESOURCE_DESC resDesc{};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Alignment = 0;
		resDesc.Width = arg_size;
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.Format = DXGI_FORMAT_UNKNOWN;
		resDesc.SampleDesc = { 1, 0 };
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resDesc.Flags = arg_flag;

		hr = DirectX12Device::Instance()->dev->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			arg_initialState,
			nullptr,
			IID_PPV_ARGS(resource.ReleaseAndGetAddressOf())
		);
		if (FAILED(hr)) {
			OutputDebugStringA("CreateBuffer failed.\n");
		}
		if (resource != nullptr && arg_name != nullptr) {
			resource->SetName(arg_name);
		}
		return resource;

	}

	void RayPipeline::UAVBarrier(const std::vector<KazBufferHelper::BufferData>& arg_bufferArray)
	{
		std::vector<D3D12_RESOURCE_BARRIER> barrier;

		for (const auto& index : arg_bufferArray) {
			barrier.emplace_back(CD3DX12_RESOURCE_BARRIER::UAV(index.bufferWrapper->GetBuffer().Get()));
		}

		DirectX12CmdList::Instance()->cmdList->ResourceBarrier(static_cast<UINT>(barrier.size()), barrier.data());
	}

	void RayPipeline::BufferStatesTransition(ID3D12Resource* arg_resource, D3D12_RESOURCE_STATES arg_before, D3D12_RESOURCE_STATES arg_after)
	{
		D3D12_RESOURCE_BARRIER barriers[] = {
			CD3DX12_RESOURCE_BARRIER::Transition(
			arg_resource,
			arg_before,
			arg_after),
		};
		DirectX12CmdList::Instance()->cmdList->ResourceBarrier(_countof(barriers), barriers);
	}

	void RayPipeline::SetRaytracingResource(Tlas arg_tlas)
	{

		//グローバルルートシグネチャで使うと宣言しているリソースらをセット。
		DescriptorHeapMgr::Instance()->SetDescriptorHeap();
		DirectX12CmdList::Instance()->cmdList->SetComputeRootSignature(GetGlobalRootSig()->GetRootSig().Get());

		//TLASを設定。
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(arg_tlas.GetDescHeapHandle()));

		//GBufferを構築。
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, GBufferMgr::Instance()->GetGPUHandle(GBufferMgr::ALBEDO));	//アルベド
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(2, GBufferMgr::Instance()->GetGPUHandle(GBufferMgr::NORMAL));	//法線
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(3, GBufferMgr::Instance()->GetGPUHandle(GBufferMgr::R_M_S_ID));//マテリアル
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(4, GBufferMgr::Instance()->GetGPUHandle(GBufferMgr::WORLD));	//ワールド座標
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(5, GBufferMgr::Instance()->GetGPUHandle(GBufferMgr::EMISSIVE));	//ワールド座標

		//DirectX12CmdList::Instance()->cmdList->SetComputeRootShaderResourceView(6, m_refEchoStructuredBufferData->bufferWrapper->GetGpuAddress());

		//カメラ用定数バッファをセット。
		DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(6, GBufferMgr::Instance()->GetEyePosBuffer().bufferWrapper->GetGpuAddress());
		DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(7, GBufferMgr::Instance()->m_lightBuffer.bufferWrapper->GetGpuAddress());
		DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(8, m_refEchoConstBufferData->bufferWrapper->GetGpuAddress());	//エコーのデータ

		//書き込み用UAV
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(9, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(GBufferMgr::Instance()->GetRayTracingBuffer().bufferWrapper->GetViewHandle()));	//レイトレ出力用
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(10, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(m_refVolumeNoiseTexture->bufferWrapper->GetViewHandle()));	//ボリュームフォグ用テクスチャ
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(11, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(GBufferMgr::Instance()->GetLensFlareBuffer().bufferWrapper->GetViewHandle()));	//レンズフレア用テクスチャ
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(12, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(GBufferMgr::Instance()->GetEmissiveGBuffer().bufferWrapper->GetViewHandle()));	//ブルーム用テクスチャ
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(13, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(GBufferMgr::Instance()->m_outline->GetOutputAlbedoTexture().bufferWrapper->GetViewHandle()));	//アウトラインのアルベド(後でノイズを書けるため分離)
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(14, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(GBufferMgr::Instance()->m_outline->GetOutputEmissiveTexture().bufferWrapper->GetViewHandle()));	//アウトラインのエミッシブ(後でノイズをかけるため分離)

		//パイプラインを設定。
		DirectX12CmdList::Instance()->cmdList->SetPipelineState1(m_stateObject.Get());

	}

}