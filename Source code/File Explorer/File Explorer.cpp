// File Explorer.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "File Explorer.h"
#include <windowsx.h>
#include <WinUser.h>

#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")


#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#define MAX_LOADSTRING 100

using namespace std;

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hwnd);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnDestroy(HWND hwnd);
LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm);
HINSTANCE g_hInst;

HWND hwndTreeview;
HWND hwndlistview;

LVCOLUMN g_NameLVC;
LVCOLUMN g_TypeLVC;
LVCOLUMN g_DateModLVC;
LVCOLUMN g_SizeLVC;

HTREEITEM g_hThisPCItem;

int nItemCountLV = 0;

HWND CreateATreeView(HWND hwndParent);
HWND CreateAListView(HWND hwndParent);
BOOL InitTreeViewImageList(HWND hTV);
BOOL LoadThisPCToTreeView(HWND hTV);
BOOL LoadFoldersInDrive(HTREEITEM hDriveTreeItem, LPWSTR lpPath);
BOOL AddListViewColumns(HWND hwndLV);
BOOL InsertListViewItems(HWND hWndListView, LPWSTR pszText, const int index
	, LPARAM lParam, LPWSTR lpType, LPWSTR lpDateMod, LPWSTR lpSize);

// Global Variables:
HINSTANCE hInst;                                // current instance
TCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_FILEEXPLORER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FILEEXPLORER));

	MSG msg;

	// Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_FILEEXPLORER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FILEEXPLORER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 50, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_NOTIFY, OnNotify);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	// Parse the menu selections:
	switch (id)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hwnd);
		break;
	}
}

void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	// TODO: Add any drawing code that uses hdc here...

	EndPaint(hwnd, &ps);
}

void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	//Ensure that common control DLL is loaded
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	hwndTreeview = CreateATreeView(hwnd);
	LoadThisPCToTreeView(hwndTreeview);
	InitTreeViewImageList(hwndTreeview);

	hwndlistview = CreateAListView(hwnd);
	AddListViewColumns(hwndlistview);


	return true;
}

LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm)
{
	switch (idFrom)
	{
		case IDC_TREEVIEW:
			{
						 if (pnm->code == TVN_SELCHANGED)
						 {
							 HTREEITEM hCurItem = TreeView_GetNextItem(hwndTreeview, NULL, TVGN_CARET);
							 LPWSTR buffer = new TCHAR[255];
							 TVITEMEX tvItem;
							 tvItem.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;
							 tvItem.hItem = hCurItem;
							 tvItem.pszText = buffer;
							 tvItem.cchTextMax = 255;
							 TreeView_GetItem(hwndTreeview, &tvItem);
							
							 LoadFoldersInDrive(hCurItem, (LPWSTR)tvItem.lParam);
	
							 delete[] buffer;
						 }
			}
				break;
		case IDC_LISTVIEW:
			{
						 if (pnm->code == NM_DBLCLK) {
							 TCHAR *buffer = new TCHAR[255];
							 int curIndex = ListView_GetNextItem(hwndlistview, -1, LVNI_SELECTED);
							 LVITEM lvItem;
							 lvItem.mask = LVIF_TEXT | LVIF_PARAM;
							 lvItem.iItem = curIndex;
							 lvItem.iSubItem = 0;
							 lvItem.pszText = buffer;
							 lvItem.cchTextMax = 255;
							 ListView_GetItem(hwndlistview, &lvItem);
							 //Editing
							 LPWSTR fPath = (TCHAR*)lvItem.lParam;
							 LoadFoldersInDrive(NULL, (LPWSTR)lvItem.lParam);
						 }
			}
				break;
		default:
			
			   break;

	}
	return 0;
}

//https://msdn.microsoft.com/en-us/library/windows/desktop/hh298371(v=vs.85).aspx
HWND CreateATreeView(HWND hwndParent)
{
	RECT rcClient;
	HWND hwndTV;
	InitCommonControls();
	GetClientRect(hwndParent, &rcClient);

	hwndTV = CreateWindowEx(0, WC_TREEVIEW, L"", WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES,
		0, 0, (rcClient.right - rcClient.left) / 4, rcClient.bottom - rcClient.top, hwndParent, (HMENU)IDC_TREEVIEW, g_hInst, NULL);

	return hwndTV;
}

HWND CreateAListView(HWND hwndParent)
{
	RECT rcClient;
	HWND hwndLV;
	InitCommonControls();
	GetClientRect(hwndParent, &rcClient);
	hwndLV = CreateWindow(WC_LISTVIEW, L"", WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SINGLESEL,
		(rcClient.right - rcClient.left) / 4, 0, (rcClient.right - rcClient.left) * 3 / 4, rcClient.bottom - rcClient.top, hwndParent, (HMENU)IDC_LISTVIEW, g_hInst, NULL);

	return hwndLV;
}

//https://msdn.microsoft.com/en-us/library/windows/desktop/hh298406(v=vs.85).aspx
BOOL InitTreeViewImageList(HWND hTV)
{
	HIMAGELIST himl;
	himl = ImageList_Create(10, 10, NULL, 10, 10);

	HICON hIcon;
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_THISPC)); //0
	ImageList_AddIcon(himl, hIcon);


	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DRIVE)); //1
	ImageList_AddIcon(himl, hIcon);

	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DRIVE_SELECTED)); //2
	ImageList_AddIcon(himl, hIcon);

	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_FOLDER)); //3
	ImageList_AddIcon(himl, hIcon);

	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_FOLDER_SELECTED)); //4
	ImageList_AddIcon(himl, hIcon);

	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_CD_DVD)); //5
	ImageList_AddIcon(himl, hIcon);


	TreeView_SetImageList(hTV, himl, TVSIL_NORMAL);
	return true;
}

//https://msdn.microsoft.com/en-us/library/windows/desktop/hh298344(v=vs.85).aspx
//BOOL AddListViewColumns(HWND hWndListView)
//{
//	TCHAR szText[256];     // Temporary buffer.
//	LVCOLUMN lvc;
//	int iCol;
//
//	// Initialize the LVCOLUMN structure.
//	// The mask specifies that the format, width, text,
//	// and subitem members of the structure are valid.
//	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
//
//	// Add the columns.
//	for (iCol = 0; iCol < 4; iCol++)
//	{
//		lvc.iSubItem = iCol;
//		lvc.pszText = szText;
//		lvc.cx = 200;               // Width of column in pixels.
//
//		if (iCol < 2)
//			lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
//		else
//			lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.
//
//		// Load the names of the column headings from the string resources.
//		LoadString(g_hInst,
//			IDS_FIRSTCOLUMN + iCol,
//			szText,
//			sizeof(szText) / sizeof(szText[0]));
//
//		// Insert the columns into the list view.
//		if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
//			return FALSE;
//	}
//
//	return TRUE;
//}

BOOL AddListViewColumns(HWND hWndListView)
{
	RECT rcClient;
	GetClientRect(hWndListView, &rcClient);
	int width = rcClient.right - rcClient.left;
	g_NameLVC.mask = LVCF_TEXT | LVCF_IMAGE | LVCF_WIDTH;
	g_NameLVC.fmt = LVCFMT_LEFT;
	g_NameLVC.cx = width * 3 / 7;
	g_NameLVC.pszText = L"Name";
	ListView_InsertColumn(hWndListView, 0, &g_NameLVC);

	g_TypeLVC.mask = LVCF_TEXT | LVCF_WIDTH;
	g_TypeLVC.fmt = LVCFMT_LEFT;
	g_TypeLVC.cx = width * 1 / 7;
	g_TypeLVC.pszText = L"Type";
	ListView_InsertColumn(hWndListView, 1, &g_TypeLVC);

	g_DateModLVC.mask = LVCF_TEXT | LVCF_WIDTH;
	g_DateModLVC.fmt = LVCFMT_LEFT;
	g_DateModLVC.cx = width * 3 / 10;
	g_DateModLVC.pszText = L"Date modified";
	ListView_InsertColumn(hWndListView, 2, &g_DateModLVC);

	g_SizeLVC.mask = LVCF_TEXT | LVCF_WIDTH;
	g_SizeLVC.fmt = LVCFMT_LEFT;
	g_SizeLVC.cx = width * 1 / 7;
	g_SizeLVC.pszText = L"size";
	ListView_InsertColumn(hWndListView, 3, &g_SizeLVC);
	return TRUE;
}
BOOL LoadThisPCToTreeView(HWND hTV)
{
	//Create root item: This PC
	TVINSERTSTRUCT tvins;
	tvins.hParent = NULL;
	tvins.hInsertAfter = TVI_ROOT;
	tvins.item.mask = TVIF_CHILDREN | TVIF_IMAGE | TVIF_PARAM | TVIF_TEXT | TVIF_SELECTEDIMAGE | LVTVIF_EXTENDED;
	tvins.item.cChildren = true;

	tvins.item.iImage = 0;
	tvins.item.iSelectedImage = 0;
	tvins.item.pszText = L"This PC";
	tvins.item.lParam = (LPARAM)L"ThisPC";
	g_hThisPCItem = TreeView_InsertItem(hTV, &tvins);
	//Added Item This PC


	//Load Logical Drive
	DWORD nBufferLen = 105;
	LPWSTR lpBuffer = new TCHAR[nBufferLen];
	// lpBuffer =  C:\\0D:\\0E:\\0F:\\0\\0
	GetLogicalDriveStrings(nBufferLen, lpBuffer);

	int i = 0, j = 0;


	while (lpBuffer[i] != 0 && lpBuffer[i + 1] != 0)
	{
		LPWSTR lpDriveName = new TCHAR[nBufferLen];
		//Get Drive
		for (j = 0; lpBuffer[j] != 0; j++)
		{
			lpDriveName[j] = lpBuffer[i];
			i++;
		}
		lpDriveName[j] = 0;
		i++;

		//Get Volume name
		LPWSTR lpVolumeNameBuffer = new TCHAR[nBufferLen];
		GetVolumeInformation(lpDriveName, lpVolumeNameBuffer, nBufferLen, NULL, NULL, NULL, NULL, NULL);
		
		//GetDriveType
		if (GetDriveType(lpDriveName) == DRIVE_CDROM)
		{
			lpVolumeNameBuffer = L"CD-ROM Drive";
			tvins.item.iImage = 5;
			tvins.item.iSelectedImage = 5;
		}
		else
		{
			tvins.item.iImage = 1;
			tvins.item.iSelectedImage = 2;
		}

		LPWSTR lpDisplayNameBuffer = new TCHAR[nBufferLen];
		StrCpy(lpDisplayNameBuffer, lpVolumeNameBuffer);
		StrCat(lpDisplayNameBuffer, L" (");
		StrCat(lpDisplayNameBuffer, lpDriveName);
		StrCat(lpDisplayNameBuffer, L" )");
		tvins.hParent = g_hThisPCItem;
		tvins.hInsertAfter = TVI_LAST;

		tvins.item.pszText = lpDisplayNameBuffer;
		tvins.item.lParam = (LPARAM)lpDriveName;
		HTREEITEM hDriveTreeItem = TreeView_InsertItem(hTV, &tvins);

	}
	TreeView_Expand(hTV, g_hThisPCItem, TVE_EXPAND);
	delete[] lpBuffer;
	return true;
}
BOOL LoadFoldersInDrive(HTREEITEM hDriveTreeItem, LPWSTR lpPath)
{
	ListView_DeleteAllItems(hwndlistview);
	TVINSERTSTRUCT tvins;

	tvins.hParent = hDriveTreeItem;
	tvins.hInsertAfter = TVI_LAST;
	tvins.item.mask = TVIF_CHILDREN | TVIF_IMAGE | TVIF_TEXT | TVIF_PARAM;

	LPWSTR lpDriveFullPath = new TCHAR[wcslen(lpPath) + 2];
	StrCpy(lpDriveFullPath, lpPath);
	StrCat(lpDriveFullPath, L"\\*");

	WIN32_FIND_DATA fd; // 
	HANDLE hFile = FindFirstFileW(lpDriveFullPath, &fd);
	BOOL bFound = true;
	int index = 0;
	while (bFound == true)
	{
		if (((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN)
			&& ((fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != FILE_ATTRIBUTE_SYSTEM)
			&& (StrCmp(fd.cFileName, _T(".")) != 0) && (StrCmp(fd.cFileName, _T("..")) != 0))
		{
			LPWSTR lpFolderPath = new TCHAR[wcslen(lpPath) + wcslen(fd.cFileName) + 2];
			StrCpy(lpFolderPath, lpPath);
			StrCat(lpFolderPath, fd.cFileName);
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				StrCat(lpFolderPath, L"\\");
				tvins.item.lParam = (LPARAM)lpFolderPath;  /*in param path like: D:\workspace*/
				tvins.item.iImage = 3;
				tvins.item.iSelectedImage = 4;
				tvins.item.pszText = fd.cFileName;
				HTREEITEM hParent = TreeView_InsertItem(hwndTreeview, &tvins);
				InsertListViewItems(hwndlistview, fd.cFileName, index, (LPARAM)lpFolderPath, _T("Thu muc"), L"Updating date", L"Updating SIZE");
				index++;
				//Chen vao ListView
			}
			else {
				//InsertListViewItems(hwndlistview, fd.cFileName, index, (LPARAM)lpFolderPath, _T("Tap tin"), L"Updating date", L"Updating SIZE");
			}

		}
		bFound = FindNextFileW(hFile, &fd);
	}
	hFile = FindFirstFileW(lpDriveFullPath, &fd);
	bFound = TRUE;
	int fIndex = index;
	while (bFound == TRUE)
	{
		if (((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN)
			&& ((fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != FILE_ATTRIBUTE_SYSTEM)
			&& (StrCmp(fd.cFileName, _T(".")) != 0) && (StrCmp(fd.cFileName, _T("..")) != 0))
		{
			LPWSTR lpFolderPath = new TCHAR[255];
			StrCpy(lpFolderPath, lpPath);
			StrCat(lpFolderPath, fd.cFileName);
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			}
			else {
				InsertListViewItems(hwndlistview, fd.cFileName, index, (LPARAM)lpFolderPath, L"Tap tin", L"Updating date", L"Updating SIZE");
				fIndex++;
			}
		}
		bFound = FindNextFileW(hFile, &fd);
	}
	return true;
}

BOOL InsertListViewItems(HWND hWndListView, LPWSTR pszText, const int index, LPARAM lParam, LPWSTR lpType, LPWSTR lpDateMod, LPWSTR lpSize)
{
	LVITEMW lvI;
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | TVIF_SELECTEDIMAGE;
	lvI.iSubItem = 0;
	lvI.pszText = pszText;
	lvI.iItem = index;
	lvI.lParam = lParam;
	ListView_InsertItem(hWndListView, &lvI);
	ListView_SetItemText(hWndListView, index, 1, lpType);
	ListView_SetItemText(hWndListView, index, 2, lpDateMod);
	ListView_SetItemText(hWndListView, index, 3, lpSize);
	return TRUE;
}

BOOL MyInitListViewImageList(HWND hwndLV)
{
	HIMAGELIST himl; //Là biến ImageList
	himl = ImageList_Create(10, 10, NULL, 10, 10); 

	HICON hIcon;
	hIcon = LoadIcon(hInst, MAKEINTRESOURCEW(IDI_FOLDER));
	ImageList_AddIcon(himl, hIcon); //0
	hIcon = LoadIcon(hInst, MAKEINTRESOURCEW(IDI_NOT_FOLDER));
	ImageList_AddIcon(himl, hIcon); //1

	ListView_SetImageList(hwndlistview, himl, LVSIL_NORMAL);
	return true;
}
