#include "NotepadApp.h"
#include "NotePadForm.h"
BOOL NotepadApp::InitInstance()
{
	NotepadForm* notepadForm = new NotepadForm;         // B
	notepadForm->Create(0, "�޸���");// C
	notepadForm->ShowWindow(SW_SHOW);               // D
	notepadForm->UpdateWindow();
	m_pMainWnd = notepadForm;                         // E

	return TRUE;
}
NotepadApp notepadApp;