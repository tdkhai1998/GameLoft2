#include "Application.h"
#include "render\videoDriver.h"
#include <iostream>
#include <sstream>
using namespace std;

Application* Application::s_instance = nullptr;
static int g_Pos = 0;
Application::Application(int width, int height) : m_width(width),
	m_height(height)
{
	Application::s_instance = this;
	//key
	m_keyPressed = new bool[256];
	ZeroMemory(m_keyPressed, sizeof(bool) * 256);
}

LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return Application::s_instance->ProcessWindowMessage(hwnd, msg, wParam, lParam);
}

bool Application::Init(HINSTANCE hInstance)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = Application::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"Graphics+";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Cannot Register window", L"Error", MB_ICONEXCLAMATION | MB_OK);
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, SCREEN_W, SCREEN_H };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	HWND hwnd;
	hwnd = CreateWindow(wc.lpszClassName, L"Test C++", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);
	
	return true;
}
static DWORD start2 = GetTickCount();
static DWORD end2;
static Object* obj = new Object;
static int iPosMouseX = 0;
static float VantocX = 10;
static float VantocY = 0;
static float g = 10;
static int iPosMouseY = 0;
static int s = 0;
int newy = 0;
void Application::Update(float dt)
{
	if (g>0) {
		float yi = VantocY;
		VantocY += g*0.2;
		float y = (VantocY*VantocY - yi*yi) / (2 * g);
		if (obj->m_iposY + y > 480 - 50) {
			obj->m_iposY = 480 - 50;
			g *= -1.5;
		}
		else
			obj->m_iposY += y;
	}
	else {
		float yi = VantocY;
		VantocY += g*0.2;
		float y = (VantocY*VantocY - yi*yi) / (2 * g);
		obj->m_iposY -= y;
		if (VantocY <= 0) g = 10;
	}
	/*newy = newy + 2;
	if (newy == SCREEN_H-50)
	{
		newy = 100;
	}
	/*	if (obj->m_iposX >= SCREEN_W - 50 || obj->m_iposX <= 0) {
			VantocX *= -1;
		}
		if (obj->m_iposY >= SCREEN_H - 50 || obj->m_iposY <= 0) {
			VantocY *= -1;
		}
		if (VantocX != 0) {
			obj->m_iposX += VantocX;
			//VantocX -= 0.1;
		}
		if (VantocY != 0) {
			obj->m_iposY += VantocY;
			//VantocY -= 0.1;
		}*/
	//obj->m_iposX = iPosMouseX-25;
	//obj->m_iposY = iPosMouseY-25;
}

void Application::Render()
{
	VideoDriver::GetInstance()->DrawCircle(obj->m_iposX, obj->m_iposY, 50, 50);
}

void Application::Run()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		// Limit FPS
		DWORD start, end;
		start = GetTickCount();

		// Clean the screen each render
		VideoDriver::GetInstance()->CleanScreen();

		// Limit FPS
		end = GetTickCount();
		DWORD deltaTime = end - start;

		Update((float)deltaTime);
		Render();
		VideoDriver::GetInstance()->Render();
		if (deltaTime < 200.0 / LIMIT_FPS)
			Sleep(1000 / LIMIT_FPS - deltaTime);
	}
}

LRESULT Application::ProcessWindowMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_CREATE:
		VideoDriver::GetInstance()->Init(hwnd);
		break;
	case WM_COMMAND:
		break;
	case WM_PAINT:
		{
			break;
		}
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		onMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		onMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_MOUSEMOVE:
		onMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_KEYDOWN:
		onKeyDown(wParam);
		break;
	case WM_KEYUP:
		onKeyUp(wParam);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void Application::onMouseDown(WPARAM btnState, int x, int y)
{
	iPosMouseX = x;
	iPosMouseY = y;
}
void Application::onMouseUp(WPARAM btnState, int x, int y)
{

}
void Application::onMouseMove(WPARAM btnState, int x, int y)
{
	
}

void Application::onKeyDown(WPARAM wParam)
{
	m_keyPressed[wParam] = true;
}

void Application::onKeyUp(WPARAM wParam)
{
	m_keyPressed[wParam] = false;
}