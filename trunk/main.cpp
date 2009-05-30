#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

HWND findTaskSwitchToolbar()
{
	HWND hwnd = FindWindow(_T("Shell_TrayWnd"), NULL);
	if (hwnd == NULL)
	{
		return NULL;
	}

	hwnd = GetWindow(hwnd, GW_CHILD);
	HWND hwndTaskSwitch = NULL;

	const int CLASS_NAME_LEN_MAX = 80;
	TCHAR classname[CLASS_NAME_LEN_MAX];

	while (hwnd)
	{
		GetClassName(hwnd, classname, CLASS_NAME_LEN_MAX);

		if (lstrcmpi(classname, _T("ReBarWindow32")) == 0)
		{
			hwnd = GetWindow(hwnd, GW_CHILD);
			while (hwnd)
			{
				GetClassName(hwnd, classname, CLASS_NAME_LEN_MAX);
				if (lstrcmpi(classname, _T("MSTaskSwWClass")) == 0)
				{
					hwndTaskSwitch = hwnd;
					break;
				}
				hwnd = GetWindow(hwnd, GW_HWNDNEXT);
			}
		}

		hwnd = GetWindow(hwnd, GW_HWNDNEXT);
	}

	if (hwndTaskSwitch == NULL)
	{
		return NULL;
	}

	HWND hwndToolbar = GetWindow(hwndTaskSwitch, GW_CHILD);
	return hwndToolbar;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwndToolbar = findTaskSwitchToolbar();
	if (hwndToolbar == NULL)
	{
		return -1;
	}

#ifdef TASKBARIC_XP

	int maxRows = SendMessage(hwndToolbar, TB_GETTEXTROWS, 0, 0);

	if (maxRows != 0)
	{
		maxRows = 0;
		SendMessage(hwndToolbar, TB_SETMAXTEXTROWS, maxRows, 0);
	}
	else
	{
		maxRows = 1;
		SendMessage(hwndToolbar, TB_SETMAXTEXTROWS, maxRows, 0);
	}

#else

	DWORD exStyle = SendMessage(hwndToolbar, TB_GETEXTENDEDSTYLE, 0, 0);

	if (exStyle & TBSTYLE_EX_MIXEDBUTTONS)
	{
		SendMessage(hwndToolbar, TB_SETEXTENDEDSTYLE, 0, exStyle & (~TBSTYLE_EX_MIXEDBUTTONS));
	}
	else
	{
		SendMessage(hwndToolbar, TB_SETEXTENDEDSTYLE, 0, exStyle | TBSTYLE_EX_MIXEDBUTTONS);
	}

#endif

	SendMessage(hwndToolbar, TB_AUTOSIZE, 0, 0);

	return 0;
}
