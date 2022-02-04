#include "FindCommand.h"
#include "FindingDialog.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog�������

//����Ʈ������
FindCommand::FindCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void FindCommand::Execute()
{
	// ã�� ������ �����츦 ����.
	// ã�� ������ ������� ������ ���� �Ҵ��ؼ� ��������� �Ѵ�.
	// NotepadForm�� ����� findDialog�� FindDialog�� �Ҵ��Ѵ�.
	FindingDialog *findingDialog= new FindingDialog(this->notepadForm);
	// ������ ã�� ������ �����츦 �� �� ��ü������ �������ش�.
	findingDialog->Create(TRUE, _T(""), _T(""), FR_DOWN, this->notepadForm);
	//Modal ��ȭ���ڴ� DoModal�� ȭ���� ����, Modaless ��ȭ���ڴ� ShowWindow�� ȭ���� ����.
	findingDialog->ShowWindow(SW_SHOW);

}

//�Ҹ���
FindCommand::~FindCommand()
{

}