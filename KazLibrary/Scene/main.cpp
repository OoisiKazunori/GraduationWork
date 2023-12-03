#include"../DirectXCommon/Base.h"
#include"../WinApi/WinApi.h"
#include"../WinApi/Msg.h"
#include"../DirectXCommon/DirectX12.h"
#include"../Scene/SceneManager.h"
#include"../Pipeline/PreCreateBasePipeLine.h"
#include"../Pipeline/PreCreateBaseRootSignature.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../RenderTarget/RenderTargetStatus.h"
#include"../Imgui/MyImgui.h"
#include"../Helper/OutPutDebugStringAndCheckResult.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Input/ControllerInputManager.h"
#include"../KazLibrary/Buffer/UavViewHandleMgr.h"
#include"Raytracing/RayPipeline.h"
#include<time.h>

//OgawJ


//ターゲットがデバックの時のみコンソール用のmain文を参照する
#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif // DEBUG
{
#ifdef _DEBUG
	//DebugLayer
	Microsoft::WRL::ComPtr<ID3D12Debug1> debug1;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug1))))
	{
		Microsoft::WRL::ComPtr<ID3D12Debug> spDebugController0;
		Microsoft::WRL::ComPtr<ID3D12Debug1> spDebugController1;


		D3D12GetDebugInterface(IID_PPV_ARGS(&spDebugController0));
		spDebugController0->QueryInterface(IID_PPV_ARGS(&spDebugController1));
		spDebugController1->EnableDebugLayer();
		spDebugController1->SetEnableGPUBasedValidation(false);
	}
#endif


	//CheckDirectXError
	int CheckWinError = 0;
	bool CheckMessageFlag = true;
	HRESULT CheckDirectXError = 0;


	WinApi winApi;
	Msg msg;
	DirectX12 directX;
	CheckWinError = winApi.CreateMyWindow(WIN_X, WIN_Y);
	if (CheckResult("ウィンドウの生成", CheckWinError))
	{
		return 0;
	}
	CheckDirectXError = directX.Create(WIN_X, WIN_Y, winApi.hwnd);
	//CheckDirectXError = S_FALSE;
	if (CheckResult("DirextX12の生成", CheckDirectXError))
	{
		return 0;
	}

#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
	DirectX12Device::Instance()->dev->QueryInterface(IID_PPV_ARGS(&infoQueue));

	D3D12_MESSAGE_ID denyIds[] = {
	  D3D12_MESSAGE_ID_GPU_BASED_VALIDATION_RESOURCE_STATE_IMPRECISE,
	};
	D3D12_MESSAGE_SEVERITY severities[] = {
	  D3D12_MESSAGE_SEVERITY_INFO
	};
	D3D12_INFO_QUEUE_FILTER filter{};
	filter.DenyList.NumIDs = _countof(denyIds);
	filter.DenyList.pIDList = denyIds;
	filter.DenyList.NumSeverities = _countof(severities);
	filter.DenyList.pSeverityList = severities;
	infoQueue->PushStorageFilter(&filter);
#endif 
	//使いまわす予定の頂点情報生成
	VertexBufferMgr::Instance()->GenerateBasicVertex();

	MyImgui imgui;
	imgui.Create(winApi.hwnd);

	RenderTargetStatus::Instance()->CreateDoubleBuffer(directX.swapchain);

	long lCppVersion = __cplusplus;
	std::cout << "現在使用しているC++:" << lCppVersion << "\n";

	//PreCreateBaseRootSignature prepareR;
	//PreCreateBasePipeLine prepareP;

	KeyBoradInputManager::Instance()->CreateDevice(&winApi.hwnd, &winApi.window.hInstance);
	UavViewHandleMgr::Instance()->Init();

	srand(static_cast<UINT>(time(NULL)));
	SceneManager sm;

	Raytracing::RayPipeline::SetDirectX12(&directX);

	//GPUで処理させたい物をCPUで参照させたい時、コンストラクタで処理させてInitで参照するようにする為のフラグ
	bool lStop1FlameFlag = false;

	RECT wrc = { 0,0,1280,720 };
	OutputDebugStringA("ゲームのメインループを開始します\n");

	bool isWindowClip = false;

	while (CheckMessageFlag)
	{
//#ifdef _DEBUG
//		if (KeyBoradInputManager::Instance()->InputTrigger(DIK_9)) {
//			isWindowClip = !isWindowClip;
//		}
//		if (isWindowClip)
//		{
//			GetWindowRect(winApi.hwnd, &wrc);
//
//			//ぴったりだとちょっと画面からカーソルがはみ出るので、少し小さくする。
//			const LONG OFFSET = 300;
//			wrc.left += OFFSET;
//			wrc.right -= OFFSET;
//			wrc.bottom -= OFFSET;
//			wrc.top += OFFSET;
//
//			ClipCursor(&wrc);
//			//マウス非表示
//			ShowCursor(false);
//		}
//		else
//		{
//			//マウスを画面外に出す
//			ClipCursor(nullptr);
//			ShowCursor(true);
//		}
//#endif // _DEBUG
//
////#ifdef DEBUG
//		GetWindowRect(winApi.hwnd, &wrc);
//
//		//ぴったりだとちょっと画面からカーソルがはみ出るので、少し小さくする。
//		const LONG OFFSET = 300;
//		wrc.left += OFFSET;
//		wrc.right -= OFFSET;
//		wrc.bottom -= OFFSET;
//		wrc.top += OFFSET;
//
//		ClipCursor(&wrc);
//		//マウス非表示
//		ShowCursor(false);
////#endif // _DEBUG
		CheckMessageFlag = msg.CheckMessage();
		imgui.NewFlame();
		KeyBoradInputManager::Instance()->InputLog();
		ControllerInputManager::Instance()->InputLog();
#ifdef DEBUG
		winApi.FPS();
#endif

		if (lStop1FlameFlag)
		{
			sm.Update();
			sm.Draw();
		}

		if (sm.endGameFlag || Menu::GetIsGameEnd())
		{
			break;
		}
		/*if (sm.endGameFlag || KeyBoradInputManager::Instance()->InputTrigger(DIK_F3) || Menu::GetIsGameEnd())
		{
			break;
		}*/


		imgui.Set();
		CameraMgr::Instance()->Record();
		if (lStop1FlameFlag)
		{
			RenderTargetStatus::Instance()->SwapResourceBarrier();
		}
		directX.ActCommand();

		lStop1FlameFlag = true;
	}
	winApi.UnregisterWindowClass();
	ClipCursor(nullptr);

	return 0;
}

