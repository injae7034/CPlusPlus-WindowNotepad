#include "ReplacingDialog.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include"resource.h"
#include "GlyphFinder.h"
#include "SelectingTexts.h"
#include "RowAutoChange.h"
#include "Row.h"
#include "GlyphReplacement.h"

BEGIN_MESSAGE_MAP(ReplacingDialog, CFindReplaceDialog)
	ON_EN_CHANGE(IDC_EDIT_FINDINGCONTENT, OnFindingContentEditTyped)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE, OnReplacedButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_REPLACEALL, OnReplaceAllButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnCancelButtonClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//CFindReplaceDialog�� �⺻���� �͵��� ��ӹް�, ������ �󼼻����� �� ��üȭ��!
ReplacingDialog::ReplacingDialog(CWnd* parent)
	:CFindReplaceDialog()
{
	this->notepadForm = (NotepadForm*)parent;
	//��ȭ���ڸ� ���� ���� IDD_FINDINGDIALOG�� �̿��ؼ� Ŀ���͸���¡�� �� �߰������ ��
	this->m_fr.Flags |= FR_ENABLETEMPLATE;// |= �� ���������� �����شٴ� �ǹ�(����)
	this->m_fr.hInstance = AfxGetResourceHandle();
	this->m_fr.lpTemplateName = MAKEINTRESOURCE(IDD);

}

//1. �ٲٱ� ������ �����찡 ������ ��
BOOL ReplacingDialog::OnInitDialog()
{
	CFindReplaceDialog::OnInitDialog();
	//IDC_CHECKBOX_WRAPAROUND�� �ڲ� ��Ȱ��ȭ�Ǽ� ���� �Ǵ°� ����Ʈ��
	//OnInitDialog���� ���̰� �ϰ� Ȱ��ȭ�� �����ش�.
	this->GetDlgItem(IDC_CHECKBOX_WRAPAROUND)->ShowWindow(SW_SHOW);
	this->GetDlgItem(IDC_CHECKBOX_WRAPAROUND)->EnableWindow(1);
	//ã��, �ٲٱ�, ��� �ٲٱ� ��ư�� ���ڰ� ������ ������ �ȵǱ� ������ ����Ʈ������ ��Ȱ��ȭ ��Ų��.
	this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(0);
	this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(0);
	this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(0);
	//1. '��/�ҹ��� ����' üũ�ڽ��� �����Ѵ�.
	((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->SetCheck(BST_CHECKED);
	//2. '������ ��ġ' üũ�ڽ��� �������� �ʴ´�.
	((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->SetCheck(BST_UNCHECKED);
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
		//3.7 �ٿ� ���� ���ڸ� ���Ѵ�.
		string keyword = copyRow->GetContent();
		//3.8 ���õ� ���ڸ� ����Ʈ��Ʈ�ѿ� �ٿ��ִ´�.
		this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->SetWindowText(keyword.c_str());
		//3.9 ���� �Ҵ�� ���� �Ҵ������Ѵ�.
		if (copyRow != 0)
		{
			delete copyRow;
		}
		//3.10 ���õ� texts�� �ֱ� ������ ã��, �ٲٱ�, ��� �ٲٱ⸦ Ȱ��ȭ�����ش�.
		this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(1);
	}
	//4. ������.
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
		//2.1 ���õ� texts�� �ֱ� ������ ã��, �ٲٱ�, ��� �ٲٱ⸦ Ȱ��ȭ�����ش�.
		this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(1);
	}
	//3. content�� �ι����̸�
	else
	{
		//3.1 ���õ� texts�� �ֱ� ������ ã��, �ٲٱ�, ��� �ٲٱ⸦ ��Ȱ��ȭ�����ش�.
		this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(0);
		this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(0);
		this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(0);
	}
}

//3. ã�� ��ư�� Ŭ������ ��
void ReplacingDialog::OnFindButtonClicked()
{
	//1. GlyphFinder�� �����Ѵ�.
	GlyphFinder glyphFinder(this->notepadForm->note);
	//2. ����Ʈ��Ʈ�ѿ� �����ִ� ���ڸ� �д´�.
	CString keyword;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(keyword);
	//3. ���õ� üũ�ڽ��� �д´�.
	int wrapAroundChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->GetCheck();
	int matchCaseChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->GetCheck();

	Long findingStartRowIndex = 0;
	Long findingStartLetterIndex = 0;
	Long findingEndRowIndex = 0;
	Long findingEndLetterIndex = 0;
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	Long currentLetterIndex = this->notepadForm->current->GetCurrent();
	Long endRowIndex = this->notepadForm->note->GetLength() - 1;

	//4. '��/�ҹ��� ����' üũ�ڽ��� ���õǾ� �ְ�, '������ ��ġ' üũ�ڽ��� ������ �ȵǾ�������
	if (matchCaseChecked == BST_CHECKED && wrapAroundChecked == BST_UNCHECKED)
	{
		//4.1 �Ʒ��� ã�⸦ �����Ѵ�.
		glyphFinder.FindDown((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
			&findingEndRowIndex, &findingEndLetterIndex);
	}
	//5. '��/�ҹ��� ����', '������ ��ġ' üũ�ڽ��� ������ �Ǿ�������
	else if (matchCaseChecked == BST_CHECKED && wrapAroundChecked == BST_CHECKED)
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
	else if (matchCaseChecked == BST_UNCHECKED && wrapAroundChecked == BST_UNCHECKED)
	{
		//6.1 ��/�ҹ��� ���о��� �Ʒ��� ã�⸦ �����Ѵ�.
		glyphFinder.FindDownWithMatchCase((LPCTSTR)keyword, &findingStartRowIndex,
			&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
	}
	//7. '������ ��ġ' üũ�ڽ��� ������ �Ǿ��ְ� '��/�ҹ��� ����'�� ���� �ȵǾ�������
	else if (matchCaseChecked == BST_UNCHECKED && wrapAroundChecked == BST_CHECKED)
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
		//8.1 ������ ó�� ���۵Ǹ�
		if (this->notepadForm->isSelecting == false)
		{
			//8.1.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
			this->notepadForm->isSelecting = true;
		}
		//8.2 �̹� ���õ� texts�� ������
		else
		{
			//8.2.1 ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������)
			this->notepadForm->selectingTexts->Undo();
			//8.2.2 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartXPos = 0;
			//8.2.3 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartYPos = 0;
		}
		//8.3 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
		this->notepadForm->selectedStartXPos = findingStartLetterIndex;
		//8.4 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
		this->notepadForm->selectedStartYPos = findingStartRowIndex;
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
	this->notepadForm->Invalidate(TRUE);
}

//4. �ٲٱ� ��ư�� Ŭ������ ��
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
		//2.1 GlyphReplacement�� �����Ѵ�.
		GlyphReplacement glyphReplacement(this->notepadForm->note);
		string keyword;
		glyphReplacement.Replace(keyword, this->notepadForm->selectedStartYPos,
			this->notepadForm->selectedStartXPos);
	}
	//10. ĳ���� ��ġ�� ����Ǿ����� �˸���.
	this->notepadForm->Notify();
	//11. ��������� �����Ѵ�.
	this->notepadForm->Invalidate(TRUE);
}

//4. ��� �ٲٱ� ��ư�� Ŭ������ ��
void ReplacingDialog::OnReplaceAllButtonClicked()
{

}

//5. ��� ��ư�� Ŭ������ ��
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

//3.�ݱ��ư�� Ŭ������ ��
void ReplacingDialog::OnClose()
{
	//1. �ٲٱ� ���̾�α׸� �ݴ´�.
	CFindReplaceDialog::OnClose();
}
