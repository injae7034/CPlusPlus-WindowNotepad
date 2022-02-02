#include "NotepadApp.h"
#include "NotePadForm.h"
#include <afxdisp.h>

BOOL NotepadApp::InitInstance()
{
	NotepadForm* notepadForm = new NotepadForm;
	notepadForm->Create(0, "¸Þ¸ðÀå");
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CoInitialize(NULL);
	AfxEnableControlContainer();
	notepadForm->ShowWindow(SW_SHOW);
	notepadForm->UpdateWindow();
	m_pMainWnd = notepadForm;

	return TRUE;
}
NotepadApp notepadApp;