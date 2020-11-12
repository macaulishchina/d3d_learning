#ifndef D3DAPP_H
#define D3DAPP_H

#include <d2d1.h>
#include <dwrite.h>
#include <wrl/client.h>
#include <string>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include "GameTimer.h"
#include "Mouse.h"
#include "Keyboard.h"

// 添加所有要引用的库
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "winmm.lib")

class D3DApp {
public:
	D3DApp(HINSTANCE hInstance);    // 在构造函数的初始化列表应当设置好初始参数
	virtual ~D3DApp();

	HINSTANCE AppInst()const;       // 获取应用实例的句柄
	HWND      MainWnd()const;       // 获取主窗口句柄
	float     AspectRatio()const;   // 获取屏幕宽高比

	int Run();                      // 运行程序，进行游戏主循环

	// 框架方法。客户派生类需要重载这些方法以实现特定的应用需求
	virtual bool Init();                      // 该父类方法需要初始化窗口和Direct3D部分
	virtual void OnResize();                  // 该父类方法需要在窗口大小变动的时候调用
	virtual void UpdateScene(float dt) = 0;   // 子类需要实现该方法，完成每一帧的更新
	virtual void DrawScene() = 0;             // 子类需要实现该方法，完成每一帧的绘制
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	// 窗口的消息回调函数
protected:
	bool InitMainWindow();       // 窗口初始化
	bool InitDirect2D();         // Direct2D初始化
	bool InitDirect3D();         // Direct3D初始化

	void CalculateFrameStats();  // 计算每秒帧数并在窗口显示

protected:

	HINSTANCE mAppInst;			// 应用实例句柄
	HWND      mMainWnd;			// 主窗口句柄
	bool      mAppPaused;       // 应用是否暂停
	bool      mMinimized;       // 应用是否最小化
	bool      mMaximized;       // 应用是否最大化
	bool      mResizing;        // 窗口大小是否变化
	bool	  mEnable4xMsaa;	// 是否开启4倍多重采样
	UINT      m4xMsaaQuality;   // MSAA支持的质量等级
	bool      mCULLRender;		//是否开启单面剔除
	bool	  mIsWireframeMode;	// 当前是否为线框模式

	GameTimer mTimer;           // 计时器

	// 使用模板别名(C++11)简化类型名
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	// 光栅化状态: 线框模式
	ComPtr<ID3D11RasterizerState> mRSWireframe;
	// Direct2D
	ComPtr<ID2D1Factory> mD2dFactory;					// D2D工厂
	ComPtr<ID2D1RenderTarget> mD2dRenderTarget;			// D2D渲染目标
	ComPtr<IDWriteFactory> mDwriteFactory;				// DWrite工厂
	// 字体样式
	ComPtr<ID2D1SolidColorBrush> mColorBrush;			// 单色笔刷
	ComPtr<IDWriteFont> mFont;							// 字体
	ComPtr<IDWriteTextFormat> mTextFormat;				// 文本格式
	// Direct3D 11
	ComPtr<ID3D11Device> mD3dDevice;                    // D3D11设备
	ComPtr<ID3D11DeviceContext> mD3dImmediateContext;   // D3D11设备上下文
	ComPtr<IDXGISwapChain> mSwapChain;                  // D3D11交换链
	// Direct3D 11.1
	ComPtr<ID3D11Device1> mD3dDevice1;                  // D3D11.1设备
	ComPtr<ID3D11DeviceContext1> mD3dImmediateContext1; // D3D11.1设备上下文
	ComPtr<IDXGISwapChain1> mSwapChain1;                // D3D11.1交换链
	// 常用资源
	ComPtr<ID3D11Texture2D> mDepthStencilBuffer;        // 深度模板缓冲区
	ComPtr<ID3D11RenderTargetView> mRenderTargetView;   // 渲染目标视图
	ComPtr<ID3D11DepthStencilView> mDepthStencilView;   // 深度模板视图
	D3D11_VIEWPORT mScreenViewport;                      // 视口

	// 派生类应该在构造函数设置好这些自定义的初始参数
	std::wstring mMainWndCaption;                        // 主窗口标题
	int mClientWidth;                                    // 视口宽度
	int mClientHeight;                                   // 视口高度

	//键鼠输入
	std::unique_ptr<DirectX::Mouse> mMouse;
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> mMouseTracker;
	std::unique_ptr<DirectX::Keyboard> mKeyboard;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> mKeyboardTracker;
};

#endif // D3DAPP_H