#include "ReplaceCommand.h"
#include "ReplacingDialog.h"
#include "FindingDialog.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog�������

//����Ʈ������
ReplaceCommand::ReplaceCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void ReplaceCommand::Execute()
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
	// �ٲٱ� ������ �����츦 ����.
	// �ٲٱ� ������ ������� ������ ���� �Ҵ��ؼ� ��������� �Ѵ�.
	// NotepadForm�� ����� findDialog�� FindDialog�� �Ҵ��Ѵ�.
	this->notepadForm->findReplaceDialog = new ReplacingDialog(this->notepadForm);
	// ������ ã�� ������ �����츦 �� �� ��ü������ �������ش�.
	this->notepadForm->findReplaceDialog->Create
	(TRUE, _T(""), _T(""), FR_DOWN, this->notepadForm);
	//Modal ��ȭ���ڴ� DoModal�� ȭ���� ����, Modaless ��ȭ���ڴ� ShowWindow�� ȭ���� ����.
	this->notepadForm->findReplaceDialog->ShowWindow(SW_SHOW);
}

//�Ҹ���
ReplaceCommand::~ReplaceCommand()
{

}