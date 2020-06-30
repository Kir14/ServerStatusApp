// ServerStatusApp.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "ServerStatusApp.h"
#include <deque>
#include <string>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hEdit;
HWND hBtnOn, hBtnOff, hBtnClients;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void Pipes();
void AppendText(HWND, LPCTSTR);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Разместите код здесь.

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SERVERSTATUSAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SERVERSTATUSAPP));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SERVERSTATUSAPP));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SERVERSTATUSAPP);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 480, 620, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		hEdit = CreateWindow(L"Edit", NULL, WS_CHILD | WS_VISIBLE | ES_READONLY |
			WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
			0, 0, 0, 0, hWnd, (HMENU)1, hInst, NULL);
		hBtnClients = CreateWindow(L"Button", L"Clients",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			0, 0, 0, 0, hWnd, (HMENU)1, hInst, NULL);
		
		break;
	case WM_SIZE:
		MoveWindow(hEdit, 0, 50, LOWORD(lParam), HIWORD(lParam) - 50, TRUE);
		MoveWindow(hBtnClients, LOWORD(lParam) / 2 - 50, 0, 100, 50, TRUE);
		break;
	case WM_COMMAND:
	{
		if (lParam == (LPARAM)hBtnClients)
			CreateThread(
				NULL,              // no security attribute 
				0,                 // default stack size 
				(LPTHREAD_START_ROUTINE)Pipes,    // функция обработки сообщений
				NULL,    // параметр потока
				0,                 // not suspended 
				NULL);      // возврат id потока)
		// Разобрать выбор в меню:
		switch (LOWORD(wParam))
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void Pipes()
{
	HANDLE hPipe;
	BOOL   fSuccess = FALSE;
	DWORD  cbRead, cbToWrite, cbWritten, dwMode;
	//LPWSTR pipename = (LPWSTR)"\\\\.\\pipe\\Server";//Имя канала
	WCHAR pipename[80] = L"\\\\.\\pipe\\Server";

	while (1)
	{
		hPipe = CreateFile(
			pipename,		// channel name
			GENERIC_READ |  // access the file
			GENERIC_WRITE,
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0,              // default attributes 
			NULL);          // no template file 

		// Брейк если труба действует.
		if (hPipe != INVALID_HANDLE_VALUE)
			break;

		// Не удалось открыть канал
		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			SetWindowText(hEdit, L"The channel could not be opened\n");
			return;
		}

		// Интервал ожидания, по ходу не работает.
		if (!WaitNamedPipe(pipename, 20000))
		{
			SetWindowText(hEdit, L"Could not open pipe: 20 second wait timed out.");
			return;
		}
	}

	// Конектимся к нашему каналу.
	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		hPipe,    // channel name
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (!fSuccess)
	{
		SetWindowText(hEdit, L"SetNamedPipeHandleState failed.\n");
		return;
	}

	// Отправка сообщений серверу
	for (;;)
	{


		/*//Запись в канал
		fSuccess = WriteFile(
			hPipe,                  // имя канала
			lpvMessage.c_str(),             // message
			cbToWrite,              // message length
			&cbWritten,             // bytes written
			NULL);                  // not overlapped

		if (!fSuccess)
		{
			printf("Записать файла pipe не удалось. GLE=%d\n", GetLastError());
			system("pause");
			return -1;
		}*/
		char buffer[1000] = "";
		//Чтение ответа от сервера
		fSuccess = ReadFile(
			hPipe,    //  channel name
			buffer,    // buffer to receive reply 
			1000 * sizeof(char),  // size of buffer 
			&cbRead,  // number of bytes read 
			NULL);    // not overlapped 



		if (!fSuccess)
		{
			SetWindowText(hEdit, L"File reading failed\n");
			return;
		}

		/*for (auto it : names)
		{
			TCHAR buf[100];
			MultiByteToWideChar(CP_ACP, 0, it.c_str(), it.size(), buf, 100);
			wcscat_s(buffer, buf);
		}
		int n = _tclen(buffer);
		buffer[n++]='\0';*/



		SetWindowText(hEdit, L"connect clients:\r\n");

		char* next_token = nullptr;
		char* pch = strtok_s(buffer, "\n", &next_token); // во втором параметре указаны разделитель (пробел, запятая, точка, тире)

		//if(pch!=NULL) AppendText(hEdit, pch);
		while (pch != NULL)                         // пока есть лексемы
		{
			TCHAR buf[100] = L"";
			MultiByteToWideChar(CP_ACP, 0, pch, 100, buf, 100);
			wcscat_s(buf, L"\r\n");
			AppendText(hEdit, buf);
			pch = strtok_s(NULL, "\n", &next_token);

		}
	}
	CloseHandle(hPipe);
}

void AppendText(HWND hEditWnd, LPCTSTR Text)
{
	int idx = GetWindowTextLength(hEditWnd);
	SendMessage(hEditWnd, EM_SETSEL, (WPARAM)idx, (LPARAM)idx);
	SendMessage(hEditWnd, EM_REPLACESEL, 0, (LPARAM)Text);
}