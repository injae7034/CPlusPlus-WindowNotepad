#include "FindingDialog.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include"resource.h"
#include "GlyphFinder.h"
#include "SelectingTexts.h"
#include "RowAutoChange.h"
#include "Row.h"

BEGIN_MESSAGE_MAP(FindingDialog, CFindReplaceDialog)
	ON_EN_CHANGE(IDC_EDIT_FINDINGCONTENT, OnFindingContentEditTyped)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnCancelButtonClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//CFindReplaceDialog�� �⺻���� �͵��� ��ӹް�, ������ �󼼻����� �� ��üȭ��!
FindingDialog::FindingDialog(CWnd* parent)
	:CFindReplaceDialog()
{
	this->notepadForm = (NotepadForm*)parent;
	//��ȭ���ڸ� ���� ���� IDD_FINDINGDIALOG�� �̿��ؼ� Ŀ���͸���¡�� �� �߰������ ��
	this->m_fr.Flags |= FR_ENABLETEMPLATE;// |= �� ���������� �����شٴ� �ǹ�(����)
	this->m_fr.hInstance = AfxGetResourceHandle();
	this->m_fr.lpTemplateName = MAKEINTRESOURCE(IDD);
	
}

//1. ã�� ������ �����찡 ������ ��
BOOL FindingDialog::OnInitDialog()
{
	CFindReplaceDialog::OnInitDialog();
	//IDC_CHECKBOX_WRAPAROUND�� �ڲ� ��Ȱ��ȭ�Ǽ� ���� �Ǵ°� ����Ʈ��
	//OnInitDialog���� ���̰� �ϰ� Ȱ��ȭ�� �����ش�.
	this->GetDlgItem(IDC_CHECKBOX_WRAPAROUND)->ShowWindow(SW_SHOW);
	this->GetDlgItem(IDC_CHECKBOX_WRAPAROUND)->EnableWindow(1);
	//ã�� ��ư�� ���ڰ� ������ ������ �ȵǱ� ������ ����Ʈ������ ��Ȱ��ȭ ��Ų��.
	this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(0);
	//1. '����' ������ư�� �������� �ʴ´�.
	((CButton*)GetDlgItem(IDC_RADIO_UP))->SetCheck(BST_UNCHECKED);
	//2. '�Ʒ���' ������ư�� �����Ѵ�.
	((CButton*)GetDlgItem(IDC_RADIO_DOWN))->SetCheck(BST_CHECKED);
	//3. '��/�ҹ��� ����' üũ�ڽ��� �����Ѵ�.
	((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->SetCheck(BST_CHECKED);
	//4. '������ ��ġ' üũ�ڽ��� �������� �ʴ´�.
	((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->SetCheck(BST_UNCHECKED);
	//2. �޸��忡�� ���õ� ���ڰ� ������
	if (this->notepadForm->isSelecting == true)
	{
		//�޸��忡 ���õ� ���ڵ��� �����Ѵ�.
		//2.1 ���õ� ���ڵ��� ���� ������ ���� �����Ѵ�.
		Glyph* copyRow = new Row();
		//2.2 �޸��忡�� ���� �ٰ� ������ ��ġ�� ���Ѵ�.
		Long currentRowPos = this->notepadForm->note->GetCurrent();
		Long currentLetterPos = this->notepadForm->current->GetCurrent();
		//2.3 ������ ���۵Ǵ� �ٰ� ����, ������ ������ �ٰ� ������ ��ġ�� ���Ѵ�.
		Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//������ ���۵Ǵ� ��
		Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//������ ���۵Ǵ� ����
		Long selectedEndRowPos = currentRowPos;//������ ������ ��
		Long selectedEndLetterPos = currentLetterPos;//������ ������ ����
		//2.4 ������ ����� ������� ������ ����(����)���� �����ϴ� �ٰ� ����, ������ �ٰ� ������ġ�� ���Ѵ�.
		Long startRowIndex = 0;//�����ϴ� ���� ��ġ
		Long startLetterIndex = 0;//�����ϴ� ���� ��ġ
		Long endRowIndex = 0;//������ ���� ��ġ
		Long endLetterIndex = 0;//������ ���� ��ġ
		Long i = 0;//�ݺ������
		Glyph* startRow = 0;//�����ϴ� ���� ��ġ
		Glyph* letter = 0;
		//2.5 ������ ���۵Ǵ� �ٰ� ������ ������ ���� ������(���簡 �� �� ������ �̷�������)
		if (selectedStartRowPos == selectedEndRowPos)
		{
			//2.5.1 ������ ���������� ����Ǿ�����
			if (selectedStartLetterPos < selectedEndLetterPos)
			{
				//2.5.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
				startLetterIndex = selectedStartLetterPos;
				//2.5.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				endLetterIndex = selectedEndLetterPos;
				//2.5.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedStartRowPos;

			}
			//2.5.2 ������ �������� ����Ǿ�����
			else
			{
				//2.5.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				startLetterIndex = selectedEndLetterPos;
				//2.5.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
				endLetterIndex = selectedStartLetterPos;
				//2.5.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedEndRowPos;
			}
			//2.5.3 �����ϴ� ���� ���Ѵ�.
			startRow = this->notepadForm->note->GetAt(startRowIndex);
			//2.5.4 �����ϴ� ������ġ���� ������ ���ڱ��� �����Ѵ�.
			i = startLetterIndex;
			while (i < endLetterIndex)
			{
				//2.5.4.1 �޸����� �����ϴ� �ٿ��� �ٿ��� ���ڸ� ���Ѵ�.
				letter = startRow->GetAt(i);
				//2.5.4.2 Ŭ�������� ���� ���� ��(�����ϴ� ��)�� ���ڸ� �߰��Ѵ�.
				copyRow->Add(letter->Clone());
				//2.5.4.3 �����ϴ� ������ ���� ���ڷ� �̵��ؼ� ���ڸ� �����Ѵ�.
				i++;
			}
		}
		//2.6 ������ �����ϴ� �ٰ� ������ ������ ���� ���� �ٸ���(������ �����ٿ� �������� �Ǿ� ������)
		else
		{
			//2.6.1 ������ ���������� ����Ǿ����� 
			if (selectedStartRowPos < selectedEndRowPos)
			{
				//2.6.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
				startLetterIndex = selectedStartLetterPos;
				//2.6.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				endLetterIndex = selectedEndLetterPos;
				//2.6.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedStartRowPos;
				//2.6.1.4 ������ ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
				endRowIndex = selectedEndRowPos;
			}
			//2.6.2 ������ �������� ����Ǿ�����
			else
			{
				//2.6.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				startLetterIndex = selectedEndLetterPos;
				//2.6.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
				endLetterIndex = selectedStartLetterPos;
				//2.6.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedEndRowPos;
				//2.6.2.4 ������ ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
				endRowIndex = selectedStartRowPos;
			}
			Glyph* endRow = 0;//������ ���� ��ġ
			//2.6.3 ������ �ٺ��� �������� �ݺ��Ѵ�.
			while (startRowIndex < endRowIndex)
			{
				//2.6.3.1 �����ϴ� ���� ���Ѵ�.
				startRow = this->notepadForm->note->GetAt(startRowIndex);
				//2.6.3.2 �����ϴ� ������ġ���� �����ϴ� ���� ������ ���ڱ��� �����Ѵ�.
				i = startLetterIndex;
				while (i < startRow->GetLength())
				{
					//2.6.3.2.1 �����ϴ� �ٿ��� ���ڸ� ���Ѵ�.
					letter = startRow->GetAt(i);
					//2.6.3.2.2 Ŭ�������� ���� ���� ��(�����ϴ� ��)�� ���ڸ� �߰��Ѵ�.
					copyRow->Add(letter->Clone());
					//2.6.3.2.3 �����ϴ� ������ ���� ���ڷ� �̵��ؼ� ���ڸ� �����Ѵ�.
					i++;
				}
				//2.6.3.3 �����ϴ� ���� ��ġ�� ������Ų��.
				startRowIndex++;
				//2.6.3.4 ���� ���� �ٷ� �̵��߱� ������ ������ġ�� 0���� �ʱ�ȭ���ش�.
				startLetterIndex = 0;
			}
			//2.6.4 �޸��忡�� ������ ���� ���Ѵ�.
			endRow = this->notepadForm->note->GetAt(startRowIndex);
			//2.6.5 ���� ��ġ�� ����ġ��Ų��.
			i = 0;
			//2.6.6 �޸����� ��¥�ٿ��� ������ ���ڱ��� �ݺ��Ѵ�.
			while (i < endLetterIndex)
			{
				//2.6.6.1 �޸����� ��¥�ٿ��� ���ڸ� ���Ѵ�.
				letter = endRow->GetAt(i);
				//2.6.6.2 Ŭ�������� ���� ���� ��(�����ϴ� ��)�� ���ڸ� �߰��Ѵ�.
				copyRow->Add(letter->Clone());
				//2.6.6.3 ���� ���ڷ� �̵��ؼ� ���ڸ� �����Ѵ�.
				i++;
			}
		}
		//2.7 �ٿ� ���� ���ڸ� ���Ѵ�.
		string keyword = copyRow->GetContent();
		//2.8 ���õ� ���ڸ� ����Ʈ��Ʈ�ѿ� �ٿ��ִ´�.
		this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->SetWindowText(keyword.c_str());
		//2.9 ���� �Ҵ�� ���� �Ҵ������Ѵ�.
		if (copyRow != 0)
		{
			delete copyRow;
		}
		//���õ� texts�� �ֱ� ������ ã�� ��ư�� Ȱ��ȭ ��Ų��.
		this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(1);
	}
	//3. ������.
	return FALSE;
}

//2. ã�� ���� ����Ʈ�� �ؽ�Ʈ�� �Է��� ��
void FindingDialog::OnFindingContentEditTyped()
{
	//1. ����Ʈ��Ʈ�ѿ� �����ִ� ���ڸ� �д´�.
	CString content;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(content);
	//2. content�� �ι��ڰ� �ƴϸ�
	if (content != "")
	{
		//2.1 ���õ� texts�� �ֱ� ������ ã�� ��ư�� Ȱ��ȭ�����ش�.
		this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(1);
	}
	//3. content�� �ι����̸�
	else
	{
		//3.1 ���õ� texts�� �ֱ� ������ ã�� ��ư�� ��Ȱ��ȭ�����ش�.
		this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(0);
	}
}

//3. ã�� ��ư�� Ŭ������ ��
void FindingDialog::OnFindButtonClicked()
{
	//1. GlyphFinder�� �����Ѵ�.
	GlyphFinder glyphFinder(this->notepadForm->note);
	//2. ����Ʈ��Ʈ�ѿ� �����ִ� ���ڸ� �д´�.
	CString keyword;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(keyword);
	//3. ���õ� üũ�ڽ��� ������ư�� �д´�.
	int wrapAroundChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->GetCheck();
	int matchCaseChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->GetCheck();
	int upChecked = ((CButton*)GetDlgItem(IDC_RADIO_UP))->GetCheck();
	int downChecked = ((CButton*)GetDlgItem(IDC_RADIO_DOWN))->GetCheck();
	Long findingStartRowIndex = 0;
	Long findingStartLetterIndex = 0;
	Long findingEndRowIndex = 0;
	Long findingEndLetterIndex = 0;
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	Long currentLetterIndex = this->notepadForm->current->GetCurrent();
	Long endRowIndex = this->notepadForm->note->GetLength() - 1;

	//4. '�Ʒ���' ���� ��ư, '��/�ҹ��� ����' üũ�ڽ��� ���õǾ� �ְ�, 
	//'������ ��ġ' üũ�ڽ��� ������ �ȵǾ�������
	if (downChecked == BST_CHECKED && matchCaseChecked == BST_CHECKED
		&& wrapAroundChecked == BST_UNCHECKED)
	{
		//4.1 �Ʒ��� ã�⸦ �����Ѵ�.
		glyphFinder.FindDown((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
			&findingEndRowIndex, &findingEndLetterIndex);
	}
	//5. '����' ���� ��ư, '��/�ҹ��� ����' üũ�ڽ��� ���õǾ� �ְ�,
	//'������ ��ġ' üũ�ڽ��� ������ �ȵǾ�������
	else if (upChecked == BST_CHECKED && matchCaseChecked == BST_CHECKED
		&& wrapAroundChecked == BST_UNCHECKED)
	{
		//5.1 ���õ� texts�� ������
		if (this->notepadForm->isSelecting == true)
		{
			//5.1.1 ĳ���� �̵���Ų��.
			this->notepadForm->note->Move(this->notepadForm->selectedStartYPos);
			this->notepadForm->current = this->notepadForm->note->
				GetAt(this->notepadForm->note->GetCurrent());
			this->notepadForm->current->Move(this->notepadForm->selectedStartXPos);
		}
		//5.2 ���� ã�⸦ �����Ѵ�.
		glyphFinder.FindUp((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
			&findingEndRowIndex, &findingEndLetterIndex);
	}
	//6. '�Ʒ���' ���� ��ư, '��/�ҹ��� ����', '������ ��ġ' üũ�ڽ��� ������ �Ǿ�������
	else if (downChecked == BST_CHECKED && matchCaseChecked == BST_CHECKED
		&& wrapAroundChecked == BST_CHECKED)
	{
		//6.1 �Ʒ��� ã�⸦ �����Ѵ�.
		glyphFinder.FindDown((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
			&findingEndRowIndex, &findingEndLetterIndex);
		//6.2 ã���� ������
		if (findingStartRowIndex == findingEndRowIndex &&
			findingStartLetterIndex == findingEndLetterIndex)
		{
			//6.2.1 ���� ���� ��ġ�� ���ڸ� �޸����� ���� ó������ ������.
			this->notepadForm->note->Move(0);
			this->notepadForm->current = this->notepadForm->note->GetAt(0);
			this->notepadForm->current->Move(0);
			//6.2.2 �Ʒ��� ã�⸦ �����Ѵ�.
			glyphFinder.FindDown((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
				&findingEndRowIndex, &findingEndLetterIndex);
		}
	}
	//7. '����' ���� ��ư, '��/�ҹ��� ����', '������ ��ġ' üũ�ڽ��� ������ �Ǿ�������,
	else if (upChecked == BST_CHECKED && matchCaseChecked == BST_CHECKED
		&& wrapAroundChecked == BST_CHECKED)
	{
		//7.1 ���õ� texts�� ������
		if (this->notepadForm->isSelecting == true)
		{
			//7.1.1 ĳ���� �̵���Ų��.
			this->notepadForm->note->Move(this->notepadForm->selectedStartYPos);
			this->notepadForm->current = this->notepadForm->note->
				GetAt(this->notepadForm->note->GetCurrent());
			this->notepadForm->current->Move(this->notepadForm->selectedStartXPos);
		}
		//7.2 ���� ã�⸦ �����Ѵ�.
		glyphFinder.FindUp((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
			&findingEndRowIndex, &findingEndLetterIndex);
		//7.3 ã���� ������
		if (findingStartRowIndex == findingEndRowIndex &&
			findingStartLetterIndex == findingEndLetterIndex)
		{
			//7.3.1 ���� ���� ��ġ�� ���ڸ� �޸����� ���� ���������� ������.
			this->notepadForm->note->Move(endRowIndex);
			this->notepadForm->current = this->notepadForm->note->GetAt(endRowIndex);
			this->notepadForm->current->Move(this->notepadForm->current->GetLength());
			//7.3.2 ���� ã�⸦ �����Ѵ�.
			glyphFinder.FindUp((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
				&findingEndRowIndex, &findingEndLetterIndex);
		}
	}
	//8. '�Ʒ���' ���� ��ư�� ���õǾ� �ְ� '��/�ҹ��� ����', '������ ��ġ' üũ�ڽ��� ������ �ȵǾ�������
	else if (downChecked == BST_CHECKED && matchCaseChecked == BST_UNCHECKED
		&& wrapAroundChecked == BST_UNCHECKED)
	{
		//8.1 ��/�ҹ��� ���о��� �Ʒ��� ã�⸦ �����Ѵ�.
		glyphFinder.FindDownWithMatchCase((LPCTSTR)keyword, &findingStartRowIndex,
			&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
	}
	//9. '����' ���� ��ư�� ���õǾ� �ְ� '��/�ҹ��� ����', '������ ��ġ' üũ�ڽ��� ������ �ȵǾ�������
	else if (upChecked == BST_CHECKED && matchCaseChecked == BST_UNCHECKED
		&& wrapAroundChecked == BST_UNCHECKED)
	{
		//9.1 ���õ� texts�� ������
		if (this->notepadForm->isSelecting == true)
		{
			//9.1.1 ĳ���� �̵���Ų��.
			this->notepadForm->note->Move(this->notepadForm->selectedStartYPos);
			this->notepadForm->current = this->notepadForm->note->
				GetAt(this->notepadForm->note->GetCurrent());
			this->notepadForm->current->Move(this->notepadForm->selectedStartXPos);
		}
		//9.2 ��/�ҹ��� ���о��� ���� ã�⸦ �����Ѵ�.
		glyphFinder.FindUpWithMatchCase((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
			&findingEndRowIndex, &findingEndLetterIndex);
	}
	//10. '�Ʒ���' ���� ��ư, '������ ��ġ' üũ�ڽ��� ������ �Ǿ��ְ�,
	//'��/�ҹ��� ����'�� ���� �ȵǾ�������
	else if (downChecked == BST_CHECKED && matchCaseChecked == BST_UNCHECKED
		&& wrapAroundChecked == BST_CHECKED)
	{
		//10.1 ��/�ҹ��� ���о��� �Ʒ��� ã�⸦ �����Ѵ�.
		glyphFinder.FindDownWithMatchCase((LPCTSTR)keyword, &findingStartRowIndex,
			&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
		//10.2 ã���� ������
		if (findingStartRowIndex == findingEndRowIndex &&
			findingStartLetterIndex == findingEndLetterIndex)
		{
			//10.2.1 ���� ���� ��ġ�� ���ڸ� �޸����� ���� ó������ ������.
			this->notepadForm->note->Move(0);
			this->notepadForm->current = this->notepadForm->note->GetAt(0);
			this->notepadForm->current->Move(0);
			//10.2.2 ��/�ҹ��� ���о��� �Ʒ��� ã�⸦ �����Ѵ�.
			glyphFinder.FindDownWithMatchCase((LPCTSTR)keyword, &findingStartRowIndex,
				&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
		}
	}
	//11. '����' ���� ��ư, '������ ��ġ' üũ�ڽ��� ������ �Ǿ��ְ�,
	//'��/�ҹ��� ����'�� ���� �ȵǾ�������
	else if (upChecked == BST_CHECKED && matchCaseChecked == BST_UNCHECKED
		&& wrapAroundChecked == BST_CHECKED)
	{
		//11.1 ���õ� texts�� ������
		if (this->notepadForm->isSelecting == true)
		{
			//11.1.1 ĳ���� �̵���Ų��.
			this->notepadForm->note->Move(this->notepadForm->selectedStartYPos);
			this->notepadForm->current = this->notepadForm->note->
				GetAt(this->notepadForm->note->GetCurrent());
			this->notepadForm->current->Move(this->notepadForm->selectedStartXPos);
		}
		//11.2 ��/�ҹ��� ���о��� ���� ã�⸦ �����Ѵ�.
		glyphFinder.FindUpWithMatchCase((LPCTSTR)keyword, &findingStartRowIndex,
			&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
		//11.3 ã���� ������
		if (findingStartRowIndex == findingEndRowIndex &&
			findingStartLetterIndex == findingEndLetterIndex)
		{
			//11.3.1 ���� ���� ��ġ�� ���ڸ� �޸����� ���� ���������� ������.
			this->notepadForm->note->Move(endRowIndex);
			this->notepadForm->current = this->notepadForm->note->GetAt(endRowIndex);
			this->notepadForm->current->Move(this->notepadForm->current->GetLength());
			//11.3.2 ��/�ҹ��� ���о��� ���� ã�⸦ �����Ѵ�.
			glyphFinder.FindUpWithMatchCase((LPCTSTR)keyword, &findingStartRowIndex,
				&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
		}
	}

	//12. ã�� �� ������
	if (findingStartRowIndex != findingEndRowIndex ||
		findingStartLetterIndex != findingEndLetterIndex)
	{
		//12.1 ������ ó�� ���۵Ǹ�
		if (this->notepadForm->isSelecting == false)
		{
			//12.1.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
			this->notepadForm->isSelecting = true;
		}
		//12.2 �̹� ���õ� texts�� ������
		else
		{
			//12.2.1 ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������)
			this->notepadForm->selectingTexts->Undo();
			//12.2.2 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartXPos = 0;
			//12.2.3 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartYPos = 0;
		}
		//12.3 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
		this->notepadForm->selectedStartXPos = findingStartLetterIndex;
		//12.4 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
		this->notepadForm->selectedStartYPos = findingStartRowIndex;
		//12.5 ã�� ���ڸ� �����Ѵ�.
		this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
			findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
		//12.6 ĳ���� ��ġ�� �޸����� ã�� ���ڿ��� �ִ� ���� ã�� ���ڿ� ������ ������ġ�� �̵��Ѵ�.
		this->notepadForm->note->Move(findingEndRowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
		this->notepadForm->current->Move(findingEndLetterIndex);
		//12.7 ������ �Ǿ��� ������ �����ϱ�, �߶󳻱�, ���� �޴��� Ȱ��ȭ�����ش�.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	}
	//13. ã���� ������
	else
	{
		//13.1 ĳ���� ��ġ�� �޸����� ã�� ���ڿ��� �ִ� ���� ã�� ���ڿ� ������ ������ġ�� �̵��Ѵ�.
		this->notepadForm->note->Move(currentRowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		this->notepadForm->current->Move(currentLetterIndex);
		//13.2 "ã�� �� �����ϴ�." �޼����ڽ��� ����Ѵ�.
		string message = (LPCTSTR)keyword;
		message.insert(0, "\"");
		message += "\"";
		message += "��(��) ã�� �� �����ϴ�.";
		int messageBoxButton = MessageBox(message.c_str(), "�޸���", MB_OK);
	}
	//14. ĳ���� ��ġ�� ����Ǿ����� �˸���.
	this->notepadForm->Notify();
	//15. ��������� �����Ѵ�.
	this->notepadForm->Invalidate(TRUE);
}

//4. ��� ��ư�� Ŭ������ ��
void FindingDialog::OnCancelButtonClicked()
{
	//1. OnClose�� �޼����� ������.
	//SendMessage�� �޼����� ������ �ű⼭ ������ ������ ���� �޼����� ���� ��ü�� �ٽ� ���ƿͼ� ���Ŀ�
	//���ƿ� ������ ������ ���� �������� �����Ѵ�. �ٵ� ���⼭ ������ WM_CLOSE�� �޼����� ������ 
	//ReplacingDialog�� �Ҵ������� �Ǽ� ������� ������ �ٽ� ���ƿ� ���� ����.
	//�׷��� ���⼭�� �޼����� ������ ���� ��ü�� �ٽ� ���ƿ��� �ʰ� �ű⼭ ������ �ϰ� �״�� ������
	//PostMessage�� ����ؾ��Ѵ�.
}

//5.�ݱ��ư�� Ŭ������ ��
void FindingDialog::OnClose()
{
	//1. ã�� ���̾�α׸� �ݴ´�.
	CFindReplaceDialog::OnClose();
}

