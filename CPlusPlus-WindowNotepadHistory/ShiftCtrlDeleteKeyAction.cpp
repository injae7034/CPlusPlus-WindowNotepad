#include "ShiftCtrlDeleteKeyAction.h"
#include "Glyph.h"

//����Ʈ������
ShiftCtrlDeleteKeyAction::ShiftCtrlDeleteKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void ShiftCtrlDeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	//2. ���� ������ ��ġ�� ���Ѵ�.
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//3. �޸��忡�� ���õ� texts�� ������
	if (this->notepadForm->isSelecting == false)
	{
		//3.1 ���� ��Ʈ�� ������ ���� ��ġ�� ���Ѵ�.
		Long lastRowPos = this->notepadForm->note->GetLength() - 1;
		//3.2 ���� �ٿ��� ������ ������ ��ġ�� ���Ѵ�.
		Long lastLetterPos = this->notepadForm->current->GetLength();
		//���� ������ ���̸� ���� ���� �� ����, ������ �ٿ��� ���� ��ġ�� �������� ������ �ƹ��͵� ���Ͼ
		// ���� ���� ��ġ�� �������� �ƴϰ�, ���� ���� ��ġ�� �������̸� ���� ���� ���� �ٷ� ���Խ�Ų��.
		//3.3 ���� ���� ��ġ�� ��Ʈ�� ������ �� ��ġ���� �۰�, ���� ���� ��ġ�� �������̸�
		Glyph* currentRow = 0;
		Glyph* nextRow = 0;
		if (currentRowPos < lastRowPos && currentLetterPos == lastLetterPos)
		{
			//3.3.1 ���� ���� ���Ѵ�.
			currentRow = this->notepadForm->note->GetAt(currentRowPos);
			//3.3.2 ���� ���� ���� ���� ���Ѵ�.
			nextRow = this->notepadForm->note->GetAt(currentRowPos + 1);
			//3.3.3 ���� ���� ���� �ٿ� ��ģ��.
			nextRow->Join(currentRow);
			//3.3.4 Note���� ���� ���� �ּҸ� �����.
			this->notepadForm->note->Remove(currentRowPos + 1);
			//3.3.5 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
			Long index = this->notepadForm->current->First();
			while (index < lastLetterPos)
			{
				this->notepadForm->current->Next();
				index++;
				//�̷����ϸ� index�� ���� overflow�� ���� �ʱ� ������ �ݺ����� ��� �� ���Եǰ�,
				//�׷� �ᱹ�� ���ѹݺ��� �ȴ�.!!
				//index = this->current->Next();
			}
			//3.3.6 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.3.7 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->isDirty = true;
		}
		// ���� ���� ��ġ�� �������� �ƴ� ��(���� ���� �������̵� �ƴϵ� �������)
		//���� ������ ���� ���ں��� ���� ������ ���ڱ��� �� �����.
		//3.4 ���� ���� ��ġ�� �������� �ƴϸ�
		else if (currentLetterPos < lastLetterPos)
		{
			//3.4.1 ������ ������ġ�� ���������ġ���� ū���� �ݺ��Ѵ�.
			while (lastLetterPos > currentLetterPos)
			{
				//3.4.1.1 ���ڸ� �����.
				this->notepadForm->current->Remove(lastLetterPos - 1);
				lastLetterPos--;
			}
			//3.4.2 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.4.3 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->isDirty = true;
			//3.4.4 �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
			UINT state = this->notepadForm->GetMenu()->
				GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
			//3.4.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
			if (state == MF_CHECKED)
			{
				//3.4.5.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
		}
	}
	//4. �޸��忡�� ���õ� texts�� ������
	else
	{
		Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//������ ���۵Ǵ� ��
		Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//������ ���۵Ǵ� ����
		Long selectedEndRowPos = currentRowPos;//������ ������ ��
		Long selectedEndLetterPos = currentLetterPos;//������ ������ ����
		Glyph* selectedStartRow = 0;//������ ���۵Ǵ� ���� �ּҸ� ���� ����
		//4.1 ������ ���۵Ǵ� �ٰ� ������ ������ ���� ������
		//(�� �� �������� ������ �Ǿ� �����Ƿ� ���� �������� �ʰ� ���ڵ鸸 ������)
		if (selectedStartRowPos == selectedEndRowPos)
		{
			//4.1.1 ������ ���۵Ǵ� ���� ���Ѵ�.
			selectedStartRow = this->notepadForm->note->GetAt(selectedStartRowPos);
			//4.1.2 ������ ���������� ����Ǿ����� 
			//������ ���۵� ���ں��� ������ ������ ���ڱ��� �����.
			while (selectedStartLetterPos < selectedEndLetterPos)
			{
				//4.1.2.1 ������ ���۵Ǵ� ���ڸ� �����.
				selectedStartRow->Remove(selectedStartLetterPos);
				//4.1.2.2 ������ ���۵� ���ڰ� �������鼭 ���� ������ �ٰ� ������ ���۵� ������
				//���� ���ڰ� ������ ���۵Ǵ� ������ ��ġ�� �մ���� ���� �ǹǷ� 
				//������ ������ ���� ���� -1 ���ҽ�Ų��. 
				selectedEndLetterPos--;
			}
			//4.1.3 ������ �������� ����Ǿ�����
			//������ ������ ���ں��� ������ ���۵Ǵ� ���ڱ��� �����.
			while (selectedEndLetterPos < selectedStartLetterPos)
			{
				//4.1.3.1 ������ ������ ���ڸ� �����.
				selectedStartRow->Remove(selectedEndLetterPos);
				//4.1.3.2 ������ ������ ���ڰ� �������鼭 ���� ������ �ٰ� ������ ������ ������
				//���� ���ڰ� ������ ������ ������ ��ġ�� �մ���� ���� �ǹǷ� 
				//������ ���۵Ǵ� ���� ���� -1 ���ҽ�Ų��.
				selectedStartLetterPos--;
			}
		}
		//4.2 ������ ���۵Ǵ� �ٰ� ������ ������ ���� ���� �ٸ���
		//(������ ���� �ٿ� ���ļ� �Ǿ� �ֱ� ������ ���ڰ� �� ������ ���� ����������)
		else
		{
			Glyph* row = 0;//���� �ּҸ� ���� ����
			Long letterIndex = 0;//���� ��ġ
			Glyph* selectedEndRow = 0;//������ ������ ���� �ּҸ� ���� ����
			//4.2.1 ������ ���������� ����Ǿ����� 
			//������ ���۵� �ٺ��� ������ ������ �ٱ��� ���ڸ� �����.
			if (selectedStartRowPos < selectedEndRowPos)
			{
				//4.2.1.1 ������ ���۵Ǵ� ���� ���Ѵ�.
				selectedStartRow = this->notepadForm->note->GetAt(selectedStartRowPos);
				//4.2.1.2 ������ ���۵Ǵ� ������ġ���� ������ ���۵Ǵ� ���� ���������ڱ��� �����.
				while (selectedStartLetterPos < selectedStartRow->GetLength())
				{
					//4.2.1.2.1 ������ ���۵Ǵ� ���� ���ڸ� �����.
					selectedStartRow->Remove(selectedStartLetterPos);
				}
				//4.2.1.3 ������ ���۵Ǵ� ���� �����ٺ��� ������ ������ �������� ���ڿ� ���� �����.
				selectedStartRowPos++;
				while (selectedStartRowPos < selectedEndRowPos)
				{
					//4.2.1.3.1 ���� ���Ѵ�.
					row = this->notepadForm->note->GetAt(selectedStartRowPos);
					//4.2.1.3.2 ������ġ�� ����ġ��Ų��.
					letterIndex = 0;
					//4.2.1.3.3 �ٿ��� ������ ���ڱ��� �ݺ��Ѵ�.
					while (letterIndex < row->GetLength())
					{
						//4.2.1.3.3.1 ���� ���ڸ� �����.
						row->Remove(letterIndex);
					}
					//4.2.1.3.4 ���� ���ڸ� �������⶧���� �޸��忡�� ���� �����.
					this->notepadForm->note->Remove(selectedStartRowPos);
					//4.2.1.3.5 ���� ������ ������ ������ ������ ���� ��ġ�� ��ĭ �մ������.
					selectedEndRowPos--;
				}
				//4.2.1.4 ������ ������ ���� ���Ѵ�.
				selectedEndRow = this->notepadForm->note->GetAt(selectedEndRowPos);
				//4.2.1.5 ������ ������ ���� ó������ ������ ������ ���ڱ��� ���ڸ� �����.
				letterIndex = 0;
				while (letterIndex < selectedEndLetterPos)
				{
					//4.2.1.5.1 ������ ������ ���� ���ڸ� �����.
					selectedEndRow->Remove(letterIndex);
					//4.2.1.5.2 ������ ������ ���� ù���ڸ� ����� ���� ���ں��� ������ ��ĭ��
					//������� ������ ������ ������ ������ġ�� -1 ���ҽ�Ų��.
					selectedEndLetterPos--;
				}
				//4.2.1.6 ������ ������ ���� ������ ���۵Ǵ� �ٷ� Join��Ų��.
				selectedEndRow->Join(selectedStartRow);
				//4.2.1.7 ������ ������ ���� ������ ���۵Ǵ� �ٷ� Join�Ǿ��� ������
				//������ ������ ���� �޸��忡�� �����.
				this->notepadForm->note->Remove(selectedEndRowPos);
				//4.2.1.8 ���� ���� ��ġ�� ������ ���۵� ��ġ�� �����Ѵ�.
				this->notepadForm->current = this->notepadForm->note->
					GetAt(this->notepadForm->selectedStartYPos);
				//4.2.1.9 ���� ������ ��ġ�� ������ ���۵� ��ġ�� �����Ѵ�.
				this->notepadForm->current->Move(this->notepadForm->selectedStartXPos);
			}
			//4.2.2 ������ �������� ����Ǿ����� 
			//������ ������ �ٺ��� ������ ���۵Ǵ� �ٱ��� ���ڸ� �����.
			else
			{
				//4.2.2.1 ������ ������ ���� ���Ѵ�.
				selectedEndRow = this->notepadForm->note->GetAt(selectedEndRowPos);
				//4.2.2.2 ������ ������ ������ġ���� ������ ������ ���� ���������ڱ��� �����.
				while (selectedEndLetterPos < selectedEndRow->GetLength())
				{
					//4.2.2.2.1 ������ ������ ���� ���ڸ� �����.
					selectedEndRow->Remove(selectedEndLetterPos);
				}
				//4.2.2.3 ������ ������ ���� �����ٺ��� ������ ���۵Ǵ� �������� ���ڿ� ���� �����.
				selectedEndRowPos++;
				while (selectedEndRowPos < selectedStartRowPos)
				{
					//4.2.2.3.1 ���� ���Ѵ�.
					row = this->notepadForm->note->GetAt(selectedEndRowPos);
					//4.2.2.3.2 ������ġ�� ����ġ��Ų��.
					letterIndex = 0;
					//4.2.2.3.3 �ٿ��� ������ ���ڱ��� �ݺ��Ѵ�.
					while (letterIndex < row->GetLength())
					{
						//4.2.2.3.3.1 ���� ���ڸ� �����.
						row->Remove(letterIndex);
					}
					//4.2.2.3.4 ���� ���ڸ� �������⶧���� �޸��忡�� ���� �����.
					this->notepadForm->note->Remove(selectedEndRowPos);
					//4.2.2.3.5 ���� ������ ������ ������ ���۵Ǵ� ���� ��ġ�� ��ĭ �մ������.
					selectedStartRowPos--;
				}
				//4.2.2.4 ������ ���۵Ǵ� ���� ���Ѵ�.
				selectedStartRow = this->notepadForm->note->GetAt(selectedStartRowPos);
				//4.2.2.5 ������ ���۵Ǵ� ���� ó������ ������ ���۵Ǵ� ���ڱ��� ���ڸ� �����.
				letterIndex = 0;
				while (letterIndex < selectedStartLetterPos)
				{
					//4.2.1.5.1 ������ ���۵Ǵ� ���� ���ڸ� �����.
					selectedStartRow->Remove(letterIndex);
					//4.2.1.5.2 ������ ���۵Ǵ� ���� ù���ڸ� ����� ���� ���ں��� ������ ��ĭ��
					//������� ������ ������ ���۵Ǵ� ������ġ�� -1 ���ҽ�Ų��.
					selectedStartLetterPos--;
				}
				//4.2.2.6 ������ ���۵Ǵ� ���� ������ ������ �ٷ� Join��Ų��.
				selectedStartRow->Join(selectedEndRow);
				//4.2.2.7 ������ ���۵Ǵ� ���� ������ ������ �ٷ� Join�Ǿ��� ������
				//������ ���۵Ǵ� ���� �޸��忡�� �����.
				this->notepadForm->note->Remove(selectedStartRowPos);
				//4.2.2.8 ���� ������ ��ġ�� ������ ������ ������ġ�� �����Ѵ�.
				this->notepadForm->current->Move(selectedEndLetterPos);
			}
		}
		//4.3 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//4.4 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->isDirty = true;
		//4.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
		UINT state = this->notepadForm->GetMenu()->
			GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
		//4.6 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (state == MF_CHECKED)
		{
			//4.6.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
		//4.7 �޸��忡�� ���õ� texts�� �� ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = false;
	}
}


//�Ҹ���
ShiftCtrlDeleteKeyAction::~ShiftCtrlDeleteKeyAction()
{

}