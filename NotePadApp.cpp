#include "NotepadApp.h"
#include "NotePadForm.h"
#include <afxdisp.h>

BOOL NotepadApp::InitInstance()
{
	CWinApp::InitInstance();
	NotepadForm* notepadForm = new NotepadForm;

	this->m_pMainWnd= notepadForm;
	//������Ʈ�� ������
	//������Ʈ�� �����⿡ ������ �����ϱ� ���� �ʿ���
	//������Ʈ���� �ڵ����࿩��, ��Ʈ, �޸��� ȭ��ũ�� ���� �����ؼ� ���α׷��� ����Ǿ �� ���� ����Ǿ�
	//�ٽ� ���α׷��� ������ѵ� �� ���� �״�� ������ �� ����!
	SetRegistryKey("NotepadForm");
	LoadStdProfileSettings(8);

	notepadForm->Create(0, "�޸���");
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CoInitialize(NULL);
	AfxEnableControlContainer();
	notepadForm->ShowWindow(SW_SHOW);
	notepadForm->UpdateWindow();

	return TRUE;
}
NotepadApp notepadApp;