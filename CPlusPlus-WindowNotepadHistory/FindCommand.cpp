#include "FindCommand.h"
#include "FindingDialog.h"
#include "ReplacingDialog.h"
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
	// ã�� ������ �����츦 ���� ���� ã�� ������ �����찡 �ִ��� Ȯ���ϰ� ������ �Ҵ������Ѵ�.
	if (this->notepadForm->findReplaceDialog != 0)
	{
		//CFindReplaceDialog�� �Ҵ������Ҷ��� delete��ſ� DestroyWindow�� �̿�����!
		this->notepadForm->findReplaceDialog->DestroyWindow();
		//delete this->notepadForm->findingDialog;
		//��۸� �����͸� 0���� �ȹٲ��ָ� �Ҵ������� ��µ� �ٽ� �Ҵ������� �Ϸ� ���ͼ� ����������.
		this->notepadForm->findReplaceDialog = 0;
	}

	// ã�� ������ �����츦 ����.
	// ã�� ������ ������� ������ ���� �Ҵ��ؼ� ��������� �Ѵ�.
	// NotepadForm�� ����� findDialog�� FindDialog�� �Ҵ��Ѵ�.
	this->notepadForm->findReplaceDialog = new FindingDialog(this->notepadForm);
	// ������ ã�� ������ �����츦 �� �� ��ü������ �������ش�.
	this->notepadForm->findReplaceDialog->Create
	(TRUE, _T(""), _T(""), FR_DOWN, this->notepadForm);
	//Modal ��ȭ���ڴ� DoModal�� ȭ���� ����, Modaless ��ȭ���ڴ� ShowWindow�� ȭ���� ����.
	this->notepadForm->findReplaceDialog->ShowWindow(SW_SHOW);
}

//�Ҹ���
FindCommand::~FindCommand()
{

}