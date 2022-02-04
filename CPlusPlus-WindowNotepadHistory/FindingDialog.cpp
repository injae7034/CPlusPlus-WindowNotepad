#include "FindingDialog.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include"resource.h"
#include "GlyphFinder.h"
#include "SelectingTexts.h"
#include "RowAutoChange.h"
#include "Row.h"

BEGIN_MESSAGE_MAP(FindingDialog, CFindReplaceDialog)
	ON_BN_CLICKED(IDOK, OnFindButtonClicked)
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
	//1. '�Ʒ���' ������ư�� �����Ѵ�.
	((CButton*)GetDlgItem(IDC_RADIO_UP))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_RADIO_DOWN))->SetCheck(BST_CHECKED);
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
	}
	//3. ������.
	return FALSE;
}

//_stricmp;//��ҹ��� ���� �����ϴ� �Լ�(���� ��ҹ��ڸ� ���� ���� ���ִ� �Լ�)

//2. ã�� ��ư�� Ŭ������ ��
void FindingDialog::OnFindButtonClicked()
{
	//1. GlyphFinder�� �����Ѵ�.
	GlyphFinder glyphFinder(this->notepadForm->note);
	//2. ����Ʈ��Ʈ�ѿ� �����ִ� ���ڸ� �д´�.
	CString keyword;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(keyword);
	//3. �Ʒ��� ã�⸦ �����Ѵ�.
	Long findingStartRowIndex = 0;
	Long findingStartLetterIndex = 0;
	Long findingEndRowIndex = 0;
	Long findingEndLetterIndex = 0;
	glyphFinder.FindDown((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
		&findingEndRowIndex, &findingEndLetterIndex);
	//4. ã�� �� ������
	if (findingStartRowIndex != findingEndRowIndex ||
		findingStartLetterIndex != findingEndLetterIndex)
	{
		//4.1 ������ ó�� ���۵Ǹ�
		if (this->notepadForm->isSelecting == false)
		{
			//4.1.1 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
			this->notepadForm->isSelecting = true;
		}
		//4.2 �̹� ���õ� texts�� ������
		else
		{
			//4.2.1 ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������)
			this->notepadForm->selectingTexts->Undo();
			//4.2.2 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartXPos = 0;
			//4.2.3 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartYPos = 0;
		}
		//4.3 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
		this->notepadForm->selectedStartXPos = findingStartLetterIndex;
		//4.4 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
		this->notepadForm->selectedStartYPos = findingStartRowIndex;
		//4.5 ã�� ���ڸ� �����Ѵ�.
		this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
			findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
		//4.6 ĳ���� ��ġ�� �޸����� ã�� ���ڿ��� �ִ� ���� ã�� ���ڿ� ������ ������ġ�� �̵��Ѵ�.
		this->notepadForm->note->Move(findingEndRowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
		this->notepadForm->current->Move(findingEndLetterIndex);
		//4.7 ������ �Ǿ��� ������ �����ϱ�, �߶󳻱�, ���� �޴��� Ȱ��ȭ�����ش�.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
		//4.8 ĳ���� ��ġ�� ����Ǿ����� �˸���.
		this->notepadForm->Notify();
		//4.9 ��������� �����Ѵ�.
		this->notepadForm->Invalidate(TRUE);
	}
	//5. ã���� ������
	else
	{

		//2.1 �޼����ڽ��� �޼����� �����Ѵ�.
		
		//2.3 SaveBox �޼��� ������ �����.
		//message.insert(0, "���� ������ ");

		//5.1 "ã�� �� �����ϴ�." �޼����ڽ��� ����Ѵ�.
		string message = (LPCTSTR)keyword;
		message.insert(0, "\"");
		message += "\"";
		message += "��(��) ã�� �� �����ϴ�.";
		int messageBoxButton = MessageBox(message.c_str(), "�޸���", MB_OK);
	}
}


//3.�ݱ��ư�� Ŭ������ ��
void FindingDialog::OnClose()
{
	this->EndDialog(0);
}

