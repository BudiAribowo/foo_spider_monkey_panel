#pragma once

#include "resource.h"

class CDialogGoto : public CDialogImpl<CDialogGoto>
{
public:
	enum
	{
		IDD = IDD_DIALOG_GOTO
	};

	BEGIN_MSG_MAP(CDialogGoto)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
		END_MSG_MAP()

	CDialogGoto(HWND p_hedit) : m_hedit(p_hedit)
	{
	}

	LRESULT OnInitDialog(HWND hwndFocus, LPARAM lParam);
	LRESULT OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl);
private:
	HWND m_hedit;
};
