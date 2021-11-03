#include "NotepadApp.h"
#include "NotePadForm.h"
#include <afxdisp.h>

BOOL NotepadApp::InitInstance()
{
	CWinApp::InitInstance();
	NotepadForm* notepadForm = new NotepadForm;

	this->m_pMainWnd= notepadForm;
	//레지스트리 편집기
	//레지스트리 편집기에 파일을 생성하기 위해 필요함
	//레지스트리에 자동개행여부, 폰트, 메모장 화면크기 등을 저장해서 프로그램이 종료되어도 그 값이 저장되어
	//다시 프로그램을 실행시켜도 그 값을 그대로 유지할 수 있음!
	SetRegistryKey("NotepadForm");
	LoadStdProfileSettings(8);

	notepadForm->Create(0, "메모장");
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CoInitialize(NULL);
	AfxEnableControlContainer();
	notepadForm->ShowWindow(SW_SHOW);
	notepadForm->UpdateWindow();

	return TRUE;
}
NotepadApp notepadApp;