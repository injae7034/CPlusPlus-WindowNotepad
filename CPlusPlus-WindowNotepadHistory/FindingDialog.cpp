#include "FindingDialog.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include"resource.h"
#include "GlyphFinder.h"
#include "SelectingTexts.h"
#include "RowAutoChange.h"

BEGIN_MESSAGE_MAP(FindingDialog, CFindReplaceDialog)
	ON_BN_CLICKED(IDOK, OnFindButtonClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//CFindReplaceDialog�� �⺻���� �͵��� ��ӹް�, ������ �󼼻����� �� ��üȭ��!
FindingDialog::FindingDialog(CWnd* parent)
	:CFindReplaceDialog()
{
	this->notepadForm = (NotepadForm*)parent;
}

//1. ã�� ������ �����찡 ������ ��
BOOL FindingDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	//1. '�Ʒ���' ������ư�� �����Ѵ�.
	((CButton*)GetDlgItem(IDC_RADIO_UP))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_RADIO_DOWN))->SetCheck(BST_CHECKED);
	//2. �޸��忡�� ���õ� ���ڰ� ������
	if (this->notepadForm->isSelecting == true)
	{
		//2.1 ���õ� ���ڸ� ���Ѵ�.

		//2.1 ���õ� ���ڸ� ����Ʈ��Ʈ�ѿ� �ٿ��ִ´�.
		//CString keyword = this->notepadForm->GetDlgItem(IDC_EDIT_FINDINGCONTENT)
		//	->SetWindowText();
	}
	//2. ������.
	return FALSE;
}

//2. ã�� ��ư�� Ŭ������ ��
void FindingDialog::OnFindButtonClicked()
{
	//1. GlyphFinder�� �����Ѵ�.
	GlyphFinder glyphFinder(this->notepadForm->note);
	//2. ����Ʈ��Ʈ�ѿ� �����ִ� ���ڸ� �д´�.
	CString keyword;
	GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(keyword);
#if 0
	//3. �޸��忡�� �ڵ������� �Ǿ� ������
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//3.1 �ڵ����� ���� ���� ��ġ�� ���Ѵ�.
		RowAutoChange rowAutoChange(this->notepadForm);
		Long changedRowPos = this->notepadForm->note->GetCurrent();
		Long changedLetterPos = this->notepadForm->current->GetCurrent();
		Long originRowPos = 0;
		Long originLetterPos = 0;
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
	}
#endif
	//3. �Ʒ��� ã�⸦ �����Ѵ�.
	Long findingRowIndex = 0;
	Long findingLetterIndex = 0;
	Long keywordLetterCount = 0;
	glyphFinder.FindDown((LPCTSTR)keyword, &findingRowIndex, &findingLetterIndex,
		&keywordLetterCount);

	//4. �ڵ������� �Ǿ� ������
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//4.1 �ڵ������� ���� ���� ���� ��ġ�� FindDown���� ��µǱ� ������ 
		//�ڵ������� �Ǿ��� ���� ��ġ�� �����ش�.
		RowAutoChange rowAutoChange(this->notepadForm);
		Long changedRowPos = 0;
		Long changedLetterPos = 0;
		Long originRowPos = findingRowIndex;
		Long originLetterPos = findingLetterIndex;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		findingRowIndex = changedRowPos;
		findingLetterIndex = changedLetterPos;
	}

	//_stricmp;//��ҹ��� ���� �����ϴ� �Լ�(���� ��ҹ��ڸ� ���� ���� ���ִ� �Լ�)

	//5. ã�� ���ڸ� �����Ѵ�.
	this->notepadForm->selectingTexts->DoNext(findingRowIndex,
		findingLetterIndex - keywordLetterCount, findingRowIndex, findingLetterIndex);
	//6. ĳ���� ��ġ�� �����Ѵ�.
	this->notepadForm->note->Move(findingRowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(findingRowIndex);
	this->notepadForm->current->Move(findingLetterIndex);
	//7. ĳ���� ��ġ�� ����Ǿ����� �˸���.
	this->notepadForm->Notify();
	//8. ��������� �����Ѵ�.
	this->notepadForm->Invalidate(TRUE);
}


//3.�ݱ��ư�� Ŭ������ ��
void FindingDialog::OnClose()
{
	this->EndDialog(0);
}

