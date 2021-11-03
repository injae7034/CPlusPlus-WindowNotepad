#include "ReplacingDialog.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include"resource.h"
#include "GlyphFinder.h"
#include "SelectingTexts.h"
#include "RowAutoChange.h"
#include "Row.h"
#include "DummyRow.h"

BEGIN_MESSAGE_MAP(ReplacingDialog, CFindReplaceDialog)
	ON_EN_CHANGE(IDC_EDIT_FINDINGCONTENT, OnFindingContentEditTyped)
	ON_BN_CLICKED(IDC_BUTTON_SWAP, OnSwapButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE, OnReplacedButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_REPLACEALL, OnReplaceAllButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnCancelButtonClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//CFindReplaceDialog�� �⺻���� �͵��� ��ӹް�, ������ �󼼻����� �� ��üȭ��!
ReplacingDialog::ReplacingDialog(CWnd* parent)
	:CFindReplaceDialog(), findingKeyword(), replacingKeyword()
{
	this->notepadForm = (NotepadForm*)parent;
	this->matchCaseChecked = BST_CHECKED;
	//��ȭ���ڸ� ���� ���� IDD_FINDINGDIALOG�� �̿��ؼ� Ŀ���͸���¡�� �� �߰������ ��
	this->m_fr.Flags |= FR_ENABLETEMPLATE;// |= �� ���������� �����شٴ� �ǹ�(����)
	this->m_fr.hInstance = AfxGetResourceHandle();
	this->m_fr.lpTemplateName = MAKEINTRESOURCE(IDD);

}

BOOL ReplacingDialog::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return this->PostMessage(WM_CLOSE);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


//1. �ٲٱ� ������ �����찡 ������ ��
BOOL ReplacingDialog::OnInitDialog()
{
	CFindReplaceDialog::OnInitDialog();
	//IDC_CHECKBOX_WRAPAROUND�� �ڲ� ��Ȱ��ȭ�Ǽ� ���� �Ǵ°� ����Ʈ��
	//OnInitDialog���� ���̰� �ϰ� Ȱ��ȭ�� �����ش�.
	this->GetDlgItem(IDC_CHECKBOX_WRAPAROUND)->ShowWindow(SW_SHOW);
	this->GetDlgItem(IDC_CHECKBOX_WRAPAROUND)->EnableWindow(1);
	//Swap, ã��, �ٲٱ�, ��� �ٲٱ� ��ư�� ���ڰ� ������ ������ �ȵǱ� ������ ����Ʈ������ ��Ȱ��ȭ ��Ų��.
	this->GetDlgItem(IDC_BUTTON_SWAP)->EnableWindow(0);
	this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(0);
	this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(0);
	this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(0);
	//������Ʈ���� ����� ������ư�� üũ�ڽ� ������ �����ͼ� ������ ǥ���Ѵ�.
	int matchCaseChecked = AfxGetApp()->GetProfileInt("NotepadSection",
		"ReplacingDialogMatchCaseChecked", BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->SetCheck(matchCaseChecked);
	int wrapAroundChecked = AfxGetApp()->GetProfileInt("NotepadSection",
		"ReplacingDialogWrapAroundChecked", BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->SetCheck(wrapAroundChecked);
	//������Ʈ���� ����� �ٲٱ� �ܾ �����´�.
	CString replacingWord = AfxGetApp()->GetProfileString("NotepadSection", "ReplacingWord",
		"");
	//������Ʈ���� ����� ���ڸ� ����Ʈ��Ʈ�ѿ� �ٿ��ִ´�.
	this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->SetWindowText(replacingWord);
	//3. �޸��忡�� ���õ� ���ڰ� ������
	if (this->notepadForm->isSelecting == true)
	{
		//�޸��忡 ���õ� ���ڵ��� �����Ѵ�.
		//3.1 ���õ� ���ڵ��� ���� ������ ���� �����Ѵ�.
		Glyph* copyRow = new Row();
		//3.2 �޸��忡�� ���� �ٰ� ������ ��ġ�� ���Ѵ�.
		Long currentRowPos = this->notepadForm->note->GetCurrent();
		Long currentLetterPos = this->notepadForm->current->GetCurrent();
		//3.3 ������ ���۵Ǵ� �ٰ� ����, ������ ������ �ٰ� ������ ��ġ�� ���Ѵ�.
		Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//������ ���۵Ǵ� ��
		Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//������ ���۵Ǵ� ����
		Long selectedEndRowPos = currentRowPos;//������ ������ ��
		Long selectedEndLetterPos = currentLetterPos;//������ ������ ����
		//3.4 ������ ����� ������� ������ ����(����)���� �����ϴ� �ٰ� ����, ������ �ٰ� ������ġ�� ���Ѵ�.
		Long startRowIndex = 0;//�����ϴ� ���� ��ġ
		Long startLetterIndex = 0;//�����ϴ� ���� ��ġ
		Long endRowIndex = 0;//������ ���� ��ġ
		Long endLetterIndex = 0;//������ ���� ��ġ
		Long i = 0;//�ݺ������
		Glyph* startRow = 0;//�����ϴ� ���� ��ġ
		Glyph* letter = 0;
		//3.5 ������ ���۵Ǵ� �ٰ� ������ ������ ���� ������(���簡 �� �� ������ �̷�������)
		if (selectedStartRowPos == selectedEndRowPos)
		{
			//3.5.1 ������ ���������� ����Ǿ�����
			if (selectedStartLetterPos < selectedEndLetterPos)
			{
				//3.5.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
				startLetterIndex = selectedStartLetterPos;
				//3.5.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				endLetterIndex = selectedEndLetterPos;
				//3.5.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedStartRowPos;

			}
			//3.5.2 ������ �������� ����Ǿ�����
			else
			{
				//3.5.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				startLetterIndex = selectedEndLetterPos;
				//3.5.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
				endLetterIndex = selectedStartLetterPos;
				//3.5.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedEndRowPos;
			}
			//3.5.3 �����ϴ� ���� ���Ѵ�.
			startRow = this->notepadForm->note->GetAt(startRowIndex);
			//3.5.4 �����ϴ� ������ġ���� ������ ���ڱ��� �����Ѵ�.
			i = startLetterIndex;
			while (i < endLetterIndex)
			{
				//3.5.4.1 �޸����� �����ϴ� �ٿ��� �ٿ��� ���ڸ� ���Ѵ�.
				letter = startRow->GetAt(i);
				//3.5.4.2 Ŭ�������� ���� ���� ��(�����ϴ� ��)�� ���ڸ� �߰��Ѵ�.
				copyRow->Add(letter->Clone());
				//3.5.4.3 �����ϴ� ������ ���� ���ڷ� �̵��ؼ� ���ڸ� �����Ѵ�.
				i++;
			}
		}
		//3.6 ������ �����ϴ� �ٰ� ������ ������ ���� ���� �ٸ���(������ �����ٿ� �������� �Ǿ� ������)
		else
		{
			//3.6.1 ������ ���������� ����Ǿ����� 
			if (selectedStartRowPos < selectedEndRowPos)
			{
				//3.6.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
				startLetterIndex = selectedStartLetterPos;
				//3.6.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				endLetterIndex = selectedEndLetterPos;
				//3.6.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedStartRowPos;
				//3.6.1.4 ������ ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
				endRowIndex = selectedEndRowPos;
			}
			//3.6.2 ������ �������� ����Ǿ�����
			else
			{
				//3.6.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				startLetterIndex = selectedEndLetterPos;
				//3.6.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
				endLetterIndex = selectedStartLetterPos;
				//3.6.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedEndRowPos;
				//3.6.2.4 ������ ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
				endRowIndex = selectedStartRowPos;
			}
			Glyph* endRow = 0;//������ ���� ��ġ
			//3.6.3 ������ �ٺ��� �������� �ݺ��Ѵ�.
			while (startRowIndex < endRowIndex)
			{
				//3.6.3.1 �����ϴ� ���� ���Ѵ�.
				startRow = this->notepadForm->note->GetAt(startRowIndex);
				//3.6.3.2 �����ϴ� ������ġ���� �����ϴ� ���� ������ ���ڱ��� �����Ѵ�.
				i = startLetterIndex;
				while (i < startRow->GetLength())
				{
					//3.6.3.2.1 �����ϴ� �ٿ��� ���ڸ� ���Ѵ�.
					letter = startRow->GetAt(i);
					//3.6.3.2.2 Ŭ�������� ���� ���� ��(�����ϴ� ��)�� ���ڸ� �߰��Ѵ�.
					copyRow->Add(letter->Clone());
					//3.6.3.2.3 �����ϴ� ������ ���� ���ڷ� �̵��ؼ� ���ڸ� �����Ѵ�.
					i++;
				}
				//3.6.3.3 �����ϴ� ���� ��ġ�� ������Ų��.
				startRowIndex++;
				//3.6.3.4 ���� ���� �ٷ� �̵��߱� ������ ������ġ�� 0���� �ʱ�ȭ���ش�.
				startLetterIndex = 0;
			}
			//3.6.4 �޸��忡�� ������ ���� ���Ѵ�.
			endRow = this->notepadForm->note->GetAt(startRowIndex);
			//3.6.5 ���� ��ġ�� ����ġ��Ų��.
			i = 0;
			//3.6.6 �޸����� ��¥�ٿ��� ������ ���ڱ��� �ݺ��Ѵ�.
			while (i < endLetterIndex)
			{
				//3.6.6.1 �޸����� ��¥�ٿ��� ���ڸ� ���Ѵ�.
				letter = endRow->GetAt(i);
				//3.6.6.2 Ŭ�������� ���� ���� ��(�����ϴ� ��)�� ���ڸ� �߰��Ѵ�.
				copyRow->Add(letter->Clone());
				//3.6.6.3 ���� ���ڷ� �̵��ؼ� ���ڸ� �����Ѵ�.
				i++;
			}
		}
		//3.7 //2.7 �޸����� ���� �ٿ��� ó�� ���ں��� ���� ���ڱ����� content�� ���Ѵ�.
		i = 0;
		letter = 0;
		string letterContent = "";
		string keyword = "";
		while (i < copyRow->GetLength())
		{
			//2.7.1 ���ڸ� ���Ѵ�.
			letter = copyRow->GetAt(i);
			letterContent = letter->GetContent();
			//2.7.2 ���� content�� �����ش�.
			keyword += letterContent;
			i++;
		}
		//3.8 ���õ� ���ڸ� ����Ʈ��Ʈ�ѿ� �ٿ��ִ´�.
		this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->SetWindowText(keyword.c_str());
		//3.9 ���� �Ҵ�� ���� �Ҵ������Ѵ�.
		if (copyRow != 0)
		{
			delete copyRow;
		}
		//3.10 ���õ� texts�� �ֱ� ������ Swap, ã��, �ٲٱ�, ��� �ٲٱ⸦ Ȱ��ȭ�����ش�.
		this->GetDlgItem(IDC_BUTTON_SWAP)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(1);
	}
	//4. ���õ� ���ڰ� ������
	else
	{
		//4.1 ������Ʈ���� ����� ���ڸ� �д´�.
		CString registerKeyword = AfxGetApp()->GetProfileString("NotepadSection",
			"ReplacingDialogFindingWord", "");
		//4.2 ������Ʈ���� ����� ���ڰ� ������
		if (registerKeyword.Compare("") != 0)
		{
			//4.2.1 ����� ���ڸ� ����Ʈ��Ʈ�ѿ� �ٿ��ִ´�.
			this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->SetWindowText(registerKeyword);
			//4.2.2 ����� ���ڰ� �ֱ� ������ ã�� ��ư�� Ȱ��ȭ ��Ų��.
			this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(1);
		}
	}
	//5. ������.
	return FALSE;
}

//2. ã�� ���� ����Ʈ�� �ؽ�Ʈ�� �Է��� ��
void ReplacingDialog::OnFindingContentEditTyped()
{
	//1. ����Ʈ��Ʈ�ѿ� �����ִ� ���ڸ� �д´�.
	CString content;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(content);
	//2. content�� �ι��ڰ� �ƴϸ�
	if (content != "")
	{
		//2.1 ���õ� texts�� �ֱ� ������ swap, ã��, �ٲٱ�, ��� �ٲٱ⸦ Ȱ��ȭ�����ش�.
		this->GetDlgItem(IDC_BUTTON_SWAP)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(1);
	}
	//3. content�� �ι����̸�
	else
	{
		//3.1 ���õ� texts�� �ֱ� ������ swap, ã��, �ٲٱ�, ��� �ٲٱ⸦ ��Ȱ��ȭ�����ش�.
		this->GetDlgItem(IDC_BUTTON_SWAP)->EnableWindow(0);
		this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(0);
		this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(0);
		this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(0);
	}
}

//3. ã�� ����� �ٲ� ������ �ٲٱ� ��ư�� ���� ��
void ReplacingDialog::OnSwapButtonClicked()
{
	//1. ã�� ����Ʈ ��Ʈ�ѿ� �����ִ� ���ڸ� �о �����Ѵ�.
	CString findingKeyword;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(findingKeyword);
	//2. �ٲٱ� ����Ʈ ��Ʈ�ѿ� �����ִ� ���ڸ� �о �����Ѵ�.
	CString replacingKeyword;
	this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->GetWindowText(replacingKeyword);
	//3. ã�� ����Ʈ ��Ʈ�ѿ��� ���� ���ڸ� �ٲٱ� ����Ʈ ��Ʈ�ѿ� ����Ѵ�.
	this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->SetWindowText(findingKeyword);
	//4. �ٲٱ� ����Ʈ ��Ʈ�ѿ��� ���� ���ڸ� ã�� ����Ʈ ��Ʈ�ѿ� ����Ѵ�.
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->SetWindowText(replacingKeyword);
}

//4. ã�� ��ư�� Ŭ������ ��
void ReplacingDialog::OnFindButtonClicked()
{
	//1. GlyphFinder�� �����Ѵ�.
	GlyphFinder glyphFinder(this->notepadForm->note);
	//2. ã�� ����Ʈ��Ʈ�ѿ� �����ִ� ���ڸ� �д´�.
	CString keyword;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(keyword);
	//����Ʈ��Ʈ�ѿ� ���� �ִ� ���ڸ� ������Ʈ���� �����Ѵ�.
	AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingDialogFindingWord", keyword);
	//2. �ٲٱ� ����Ʈ ��Ʈ�ѿ� �����ִ� ���ڸ� �д´�.
	CString replacingKeyword;
	this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->GetWindowText(replacingKeyword);
	this->replacingKeyword = replacingKeyword;
	//����Ʈ��Ʈ�ѿ� ���� �ִ� ���ڸ� ������Ʈ���� �����Ѵ�.
	AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingWord", replacingKeyword);
	//3. ���õ� üũ�ڽ��� �д´�.
	int wrapAroundChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->GetCheck();
	this->matchCaseChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->GetCheck();
	//���õ� üũ�ڽ��� ������ư�� ������ ������Ʈ���� �����Ѵ�.
	AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogWrapAroundChecked",
		wrapAroundChecked);
	AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogMatchCaseChecked",
		this->matchCaseChecked);

	Long findingStartRowIndex = 0;
	Long findingStartLetterIndex = 0;
	Long findingEndRowIndex = 0;
	Long findingEndLetterIndex = 0;
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	Long currentLetterIndex = this->notepadForm->current->GetCurrent();
	Long endRowIndex = this->notepadForm->note->GetLength() - 1;

	//4. '��/�ҹ��� ����' üũ�ڽ��� ���õǾ� �ְ�, '������ ��ġ' üũ�ڽ��� ������ �ȵǾ�������
	if (this->matchCaseChecked == BST_CHECKED && wrapAroundChecked == BST_UNCHECKED)
	{
		//4.1 �Ʒ��� ã�⸦ �����Ѵ�.
		glyphFinder.FindDown((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
			&findingEndRowIndex, &findingEndLetterIndex);
	}
	//5. '��/�ҹ��� ����', '������ ��ġ' üũ�ڽ��� ������ �Ǿ�������
	else if (this->matchCaseChecked == BST_CHECKED && wrapAroundChecked == BST_CHECKED)
	{
		//5.1 �Ʒ��� ã�⸦ �����Ѵ�.
		glyphFinder.FindDown((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
			&findingEndRowIndex, &findingEndLetterIndex);
		//5.2 ã���� ������
		if (findingStartRowIndex == findingEndRowIndex &&
			findingStartLetterIndex == findingEndLetterIndex)
		{
			//5.2.1 ���� ���� ��ġ�� ���ڸ� �޸����� ���� ó������ ������.
			this->notepadForm->note->Move(0);
			this->notepadForm->current = this->notepadForm->note->GetAt(0);
			this->notepadForm->current->Move(0);
			//5.2.2 �Ʒ��� ã�⸦ �����Ѵ�.
			glyphFinder.FindDown((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
				&findingEndRowIndex, &findingEndLetterIndex);
		}
	}
	//6. '��/�ҹ��� ����', '������ ��ġ' üũ�ڽ��� ������ �ȵǾ�������
	else if (this->matchCaseChecked == BST_UNCHECKED && wrapAroundChecked == BST_UNCHECKED)
	{
		//6.1 ��/�ҹ��� ���о��� �Ʒ��� ã�⸦ �����Ѵ�.
		glyphFinder.FindDownWithMatchCase((LPCTSTR)keyword, &findingStartRowIndex,
			&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
	}
	//7. '������ ��ġ' üũ�ڽ��� ������ �Ǿ��ְ� '��/�ҹ��� ����'�� ���� �ȵǾ�������
	else if (this->matchCaseChecked == BST_UNCHECKED && wrapAroundChecked == BST_CHECKED)
	{
		//7.1 ��/�ҹ��� ���о��� �Ʒ��� ã�⸦ �����Ѵ�.
		glyphFinder.FindDownWithMatchCase((LPCTSTR)keyword, &findingStartRowIndex,
			&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
		//7.2 ã���� ������
		if (findingStartRowIndex == findingEndRowIndex &&
			findingStartLetterIndex == findingEndLetterIndex)
		{
			//7.2.1 ���� ���� ��ġ�� ���ڸ� �޸����� ���� ó������ ������.
			this->notepadForm->note->Move(0);
			this->notepadForm->current = this->notepadForm->note->GetAt(0);
			this->notepadForm->current->Move(0);
			//7.2.2 ��/�ҹ��� ���о��� �Ʒ��� ã�⸦ �����Ѵ�.
			glyphFinder.FindDownWithMatchCase((LPCTSTR)keyword, &findingStartRowIndex,
				&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
		}
	}
	//8. ã�� �� ������
	if (findingStartRowIndex != findingEndRowIndex ||
		findingStartLetterIndex != findingEndLetterIndex)
	{
		//8.1 ������ ����ǰ� �ִ� ���̾�����
		if (this->notepadForm->isSelecting == true)
		{
			//8.1.1 ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
			this->notepadForm->selectingTexts->Undo();
			//8.1.2 ������ ���� ���·� �ٲ۴�.
			this->notepadForm->isSelecting = false;
			//8.1.3 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartXPos = 0;
			//8.1.4 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartYPos = 0;
		}
		//8.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
		this->notepadForm->selectedStartXPos = findingStartLetterIndex;
		//8.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
		this->notepadForm->selectedStartYPos = findingStartRowIndex;
		//8.4 ������ �ٽ� ���۵Ǳ� ������ ������ �ٽ� ���۵��� ǥ���Ѵ�.
		this->notepadForm->isSelecting = true;
		//8.5 ã�� ���ڸ� �����Ѵ�.
		this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
			findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
		//8.6 ĳ���� ��ġ�� �޸����� ã�� ���ڿ��� �ִ� ���� ã�� ���ڿ� ������ ������ġ�� �̵��Ѵ�.
		this->notepadForm->note->Move(findingEndRowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
		this->notepadForm->current->Move(findingEndLetterIndex);
		//8.7 ������ �Ǿ��� ������ �����ϱ�, �߶󳻱�, ���� �޴��� Ȱ��ȭ�����ش�.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	}
	//9. ã���� ������
	else
	{
		//9.1 ĳ���� ��ġ�� �޸����� ã�� ���ڿ��� �ִ� ���� ã�� ���ڿ� ������ ������ġ�� �̵��Ѵ�.
		this->notepadForm->note->Move(currentRowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		this->notepadForm->current->Move(currentLetterIndex);
		//9.2 "ã�� �� �����ϴ�." �޼����ڽ��� ����Ѵ�.
		string message = (LPCTSTR)keyword;
		message.insert(0, "\"");
		message += "\"";
		message += "��(��) ã�� �� �����ϴ�.";
		int messageBoxButton = MessageBox(message.c_str(), "�޸���", MB_OK);
	}
	//10. ĳ���� ��ġ�� ����Ǿ����� �˸���.
	this->notepadForm->Notify();
	//11. ��������� �����Ѵ�.
	this->notepadForm->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

//5. �ٲٱ� ��ư�� Ŭ������ ��
void ReplacingDialog::OnReplacedButtonClicked()
{
	//1. ���õ� texts�� ������
	if (this->notepadForm->isSelecting == false)
	{
		//1.1 'ã�� ��ư�� Ŭ������ ��'�� �޼����� ������.
		//�����쿡�� ��ư�� Ŭ������ �� �޼����� WM_COMMAND�̴�
		this->SendMessage(WM_COMMAND, IDC_BUTTON_FIND);
	}
	//2. ���õ� texts�� ������
	else
	{
		// ���õ� texts�� �д´�.
		Long i = 0;
		string content = "";
		//���� ���� ��ġ�� ������ġ�� ���Ѵ�.
		Long currentRowIndex = this->notepadForm->note->GetCurrent();
		Long currentLetterIndex = this->notepadForm->current->GetCurrent();
		Long nextRowIndex = 0;
		Glyph* row = 0;
		Glyph* letter = 0;
		//������ ���۵Ǵ� �ٰ� ������ ������ ���� ���Ѵ�.
		if (this->notepadForm->selectedStartYPos < currentRowIndex)
		{
			//������ ���۵Ǵ� ���� ���Ѵ�.
			row = this->notepadForm->note->GetAt(this->notepadForm->selectedStartYPos);
			//������ ���۵Ǵ� ���� ������ ���۵Ǵ� ���ں��� ������ content�� �о ������Ų��.
			i = this->notepadForm->selectedStartXPos;
			while (i < row->GetLength())
			{
				//���ڸ� ���Ѵ�.
				letter = row->GetAt(i);
				//content�� ������Ų��.
				content += letter->GetContent();
				//���� ���ڷ� �̵��Ѵ�.
				i++;
			}
			//���� �ٷ� �̵��Ѵ�.
			nextRowIndex = this->notepadForm->selectedStartYPos + 1;
			//���� ���� ���� ���� ��ġ���� �������� �ݺ��Ѵ�.
			while (nextRowIndex < currentRowIndex)
			{
				//���� ���Ѵ�.
				row = this->notepadForm->note->GetAt(nextRowIndex);
				//���� ��¥ ���̸�
				if (!dynamic_cast<DummyRow*>(row))
				{
					//���� ���� content�� �߰��ϱ� ���� content�� ���๮�ڸ� �߰��Ѵ�.
					content += '\n';
				}
				//���� ���� ������ ���ڱ��� �ݺ��Ѵ�.
				i = 0;
				while (i < row->GetLength())
				{
					//���ڸ� ���Ѵ�.
					letter = row->GetAt(i);
					//content�� ������Ų��.
					content += letter->GetContent();
					//���� ���ڷ� �̵��Ѵ�.
					i++;
				}
				//���� �ٷ� �̵��Ѵ�.
				nextRowIndex++;
			}
			//������ ������ ���� ���Ѵ�.
			row = this->notepadForm->note->GetAt(currentRowIndex);
			//������ ������ ���� ��¥ ���̸�
			if (!dynamic_cast<DummyRow*>(row))
			{
				//���� ���� content�� �߰��ϱ� ���� content�� ���๮�ڸ� �߰��Ѵ�.
				content += '\n';
			}
			//������ ������ ���� ó�� ���ں��� ������ ������ ���ڱ��� �ݺ��Ѵ�.
			i = 0;
			while (i < currentLetterIndex)
			{
				//���ڸ� ���Ѵ�.
				letter = row->GetAt(i);
				//content�� ������Ų��.
				content += letter->GetContent();
				//���� ���ڷ� �̵��Ѵ�.
				i++;
			}
		}
		//������ ���۵Ǵ� �ٰ� ������ ������ ���� ��ġ�� ������
		else
		{
			//������ ���۵Ǵ� ���� ���Ѵ�.
			row = this->notepadForm->note->GetAt(this->notepadForm->selectedStartYPos);
			//������ ���۵Ǵ� ���� ������ ���۵Ǵ� ���ں��� ������ ������ ���ڱ��� content�� �о ������Ų��
			i = this->notepadForm->selectedStartXPos;
			while (i < currentLetterIndex)
			{
				//���ڸ� ���Ѵ�.
				letter = row->GetAt(i);
				//content�� ������Ų��.
				content += letter->GetContent();
				//���� ���ڷ� �̵��Ѵ�.
				i++;
			}
		}
		// ã�� ����Ʈ��Ʈ�ѿ� �����ִ� ���ڸ� �д´�.
		bool isMatched = false;
		CString keyword;
		this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(keyword);
		//����Ʈ��Ʈ�ѿ� ���� �ִ� ���ڸ� ������Ʈ���� �����Ѵ�.
		AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingDialogFindingWord",
			keyword);
		string word(keyword);
		//3. ���õ� üũ�ڽ��� �д´�.
		int wrapAroundChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->GetCheck();
		this->matchCaseChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->GetCheck();
		//���õ� üũ�ڽ��� ������ư�� ������ ������Ʈ���� �����Ѵ�.
		AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogWrapAroundChecked",
			wrapAroundChecked);
		AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogMatchCaseChecked", 
			this->matchCaseChecked);
		// ��/�ҹ��� ������ üũ�Ǿ� ������
		if (this->matchCaseChecked == true)
		{
			if (content.compare(word) == 0)
			{
				isMatched = true;
			}
		}
		//��/�ҹ��� ������ üũ �ȵǾ� ������
		else
		{
			//8.4.5.1 ��/�ҹ��� ���о��� �����ش�.
			if (_stricmp(content.c_str(), word.c_str()) == 0)
			{
				isMatched = true;
			}
		}
		//2.8.1 �ٲٱ� ����Ʈ ��Ʈ�ѿ� �����ִ� ���ڸ� �д´�.
		CString replacingWord;
		this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->GetWindowText(replacingWord);
		this->replacingKeyword = (LPCTSTR)replacingWord;
		//����Ʈ��Ʈ�ѿ� ���� �ִ� ���ڸ� ������Ʈ���� �����Ѵ�.
		AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingWord", replacingWord);
		//���õǾ� �ִ� ���ڿ� ã�� ���ڰ� ���� ��ġ�ϸ�
		if (isMatched == true)
		{
			//2.8.1 OnReplaceButtonClikedCommand�� �޼����� ������.
			this->notepadForm->SendMessage(WM_COMMAND, ID_ONREPLACEBUTTONCLICKEDCOMMAND);
			//2.8.2 �ٲ� �ܾ �ٲ�� ������ �޸����� �ٲ� �ܾ �ʱ�ȭ���ش�.
			this->replacingKeyword = "";
			//2.8.3 'ã�� ��ư�� Ŭ������ ��'�� �޼����� ������.
			//�����쿡�� ��ư�� Ŭ������ �� �޼����� WM_COMMAND�̴�
			this->SendMessage(WM_COMMAND, IDC_BUTTON_FIND);
		}
		// ���õǾ� �ִ� ���ڿ� ã�� ���ڰ� ���� ��ġ���� ������
		else
		{
			//2.8.4 'ã�� ��ư�� Ŭ������ ��'�� �޼����� ������.
			//�����쿡�� ��ư�� Ŭ������ �� �޼����� WM_COMMAND�̴�
			this->SendMessage(WM_COMMAND, IDC_BUTTON_FIND);
		}
	}
}

//6. ��� �ٲٱ� ��ư�� Ŭ������ ��
void ReplacingDialog::OnReplaceAllButtonClicked()
{
	//1. ã�� ����Ʈ ��Ʈ�ѿ� �����ִ� ���ڸ� �д´�.
	CString findingKeyword;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(findingKeyword);
	this->findingKeyword = findingKeyword;
	//����Ʈ��Ʈ�ѿ� ���� �ִ� ���ڸ� ������Ʈ���� �����Ѵ�.
	AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingDialogFindingWord",
		findingKeyword);
	//2. �ٲٱ� ����Ʈ ��Ʈ�ѿ� �����ִ� ���ڸ� �д´�.
	CString replacingKeyword;
	this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->GetWindowText(replacingKeyword);
	this->replacingKeyword = replacingKeyword;
	//����Ʈ��Ʈ�ѿ� ���� �ִ� ���ڸ� ������Ʈ���� �����Ѵ�.
	AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingWord", replacingKeyword);
	//3. ���õ� üũ�ڽ��� �д´�.
	int wrapAroundChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->GetCheck();
	this->matchCaseChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->GetCheck();
	//���õ� üũ�ڽ��� ������ư�� ������ ������Ʈ���� �����Ѵ�.
	AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogWrapAroundChecked",
		wrapAroundChecked);
	AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogMatchCaseChecked",
		this->matchCaseChecked);
	//4. OnReplaceAllButtonClikedCommand�� �޼����� ������.
	this->notepadForm->SendMessage(WM_COMMAND, ID_ONREPLACEALLBUTTONCLICKEDCOMMAND);
}

//6. ��� ��ư�� Ŭ������ ��
void ReplacingDialog::OnCancelButtonClicked()
{
	//1. OnClose�� �޼����� ������.
	//SendMessage�� �޼����� ������ �ű⼭ ������ ������ ���� �޼����� ���� ��ü�� �ٽ� ���ƿͼ� ���Ŀ�
	//���ƿ� ������ ������ ���� �������� �����Ѵ�. �ٵ� ���⼭ ������ WM_CLOSE�� �޼����� ������ 
	//ReplacingDialog�� �Ҵ������� �Ǽ� ������� ������ �ٽ� ���ƿ� ���� ����.
	//�׷��� ���⼭�� �޼����� ������ ���� ��ü�� �ٽ� ���ƿ��� �ʰ� �ű⼭ ������ �ϰ� �״�� ������
	//PostMessage�� ����ؾ��Ѵ�.
	this->PostMessage(WM_CLOSE);
}

//7.�ݱ��ư�� Ŭ������ ��
void ReplacingDialog::OnClose()
{
	//1. ã�� ����Ʈ ��Ʈ�ѿ� �����ִ� ���ڸ� �д´�.
	CString findingKeyword;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(findingKeyword);
	this->findingKeyword = findingKeyword;
	//����Ʈ��Ʈ�ѿ� ���� �ִ� ���ڸ� ������Ʈ���� �����Ѵ�.
	AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingDialogFindingWord",
		findingKeyword);
	//2. �ٲٱ� ����Ʈ ��Ʈ�ѿ� �����ִ� ���ڸ� �д´�.
	CString replacingKeyword;
	this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->GetWindowText(replacingKeyword);
	this->replacingKeyword = replacingKeyword;
	//����Ʈ��Ʈ�ѿ� ���� �ִ� ���ڸ� ������Ʈ���� �����Ѵ�.
	AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingWord", replacingKeyword);
	//3. ���õ� üũ�ڽ��� �д´�.
	int wrapAroundChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->GetCheck();
	this->matchCaseChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->GetCheck();
	//���õ� üũ�ڽ��� ������ư�� ������ ������Ʈ���� �����Ѵ�.
	AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogWrapAroundChecked",
		wrapAroundChecked);
	AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogMatchCaseChecked",
		this->matchCaseChecked);

	this->notepadForm->findReplaceDialog = 0;
	//1. �ٲٱ� ���̾�α׸� �ݴ´�.
	CFindReplaceDialog::OnClose();
}
