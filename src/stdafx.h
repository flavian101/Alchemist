#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <dxgi1_3.h>
#pragma comment(lib,"dxgi.lib")

#include <d3d11_4.h>
#include <d3dcompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")


#include <DirectXMath.h>
using namespace DirectX;
#include <wrl.h>
#include <wrl/client.h>

#define DIRECTINPUT_VERSION 0x0800 
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <dinput.h>
