#include "CtrlBackSpaceKeyAction.h"
#include "Glyph.h"

//����Ʈ������
CtrlBackSpaceKeyAction::CtrlBackSpaceKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlBackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	//2. ���� ������ ��ġ�� ���Ѵ�.
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//3. �޸��忡�� ���õ� texts�� ������
	if (this->notepadForm->isSelecting == false)
	{
		//���� ù ���̸� ���� ���� �� ����, ���� ù �ٿ��� ���� ��ġ�� ���� ó���� ������ �ƹ��͵� ���Ͼ
		// ���� ���� ��ġ�� ���� ó���� �ƴϰ�, ���� ���� ��ġ�� ���� ó���̸� ���� ���� ���� �ٷ� ���Խ�Ų��.
		//3.1 ���� ���� ��ġ�� 0���� ũ��, ���� ���� ��ġ�� 0�̸�
		Glyph* currentRow = 0;
		Glyph* previousRow = 0;
		if (currentRowPos > 0 && currentLetterPos == 0)
		{
			//3.1.1 ���� ���� ���Ѵ�.
			currentRow = this->notepadForm->note->GetAt(currentRowPos);
			//3.1.2 ���� ���� ���� ���� ���Ѵ�.
			previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
			//3.1.3 ���� ���� ���� ���� ������ ���� ��ġ�� ���Ѵ�.
			Long letterPos = previousRow->GetLength();
			//3.1.3 ���� ���� ���� �ٿ� ��ģ��.
			currentRow->Join(previousRow);
			//3.1.4 Note���� ���� ���� �ּҸ� �����.
			this->notepadForm->note->Remove(currentRowPos);
			//3.1.5 ���� ���� �������� ������ ���� ���� �����Ѵ�.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//3.1.6 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
			//���� ���� ������ ���� ���� ó�� ���̿� ��ġ�ϵ��� �����Ѵ�.
			Long index = this->notepadForm->current->First();
			while (index < letterPos)
			{
				this->notepadForm->current->Next();
				index++;
				//�̷����ϸ� index�� ���� overflow�� ���� �ʱ� ������ �ݺ����� ��� �� ���Եǰ�,
				//�׷� �ᱹ�� ���ѹݺ��� �ȴ�.!!
				//index = this->current->Next();
			}
			//3.1.7 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.1.8 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->isDirty = true;
		}
		// ���� ���� ��ġ�� ó���� �ƴ� ��(���� ���� ó���̵� �ƴϵ� �������)
		// ���� ���ں��� ���� �������� �ܾ� ������ ���ڵ��� �����.
		//3.2 ���� ���� ��ġ�� ó���� �ƴϸ�
		else if (currentLetterPos > 0)
		{
			//3.2.1 ���� �������� �ܾ������ �̵��� ���� ������ġ�� ���Ѵ�.
			Long letterPosAfterMoving = this->notepadForm->current->PreviousWord();
			//3.2.2 ���� ������ġ�� 0���� ū���� �ݺ��Ѵ�.
			while (currentLetterPos > letterPosAfterMoving)
			{
				//3.2.2.1 ���ڸ� �����.
				this->notepadForm->current->Remove(currentLetterPos - 1);
				currentLetterPos--;
			}
			//3.2.3 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.2.4 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->isDirty = true;
			//3.2.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
			UINT state = this->notepadForm->GetMenu()->
				GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
			//3.2.6 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
			if (state == MF_CHECKED)
			{
				//3.2.6.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
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
		//������ ������ �����ʹ�����
		Long startRowIndex = 0;//�����ϴ� ���� ��ġ
		Long startLetterIndex = 0;//�����ϴ� ���� ��ġ
		Long endRowIndex = 0;//������ ���� ��ġ
		Long endLetterIndex = 0;//������ ���� ��ġ
		Glyph* startRow = 0;//�����ϴ� ���� ��ġ
		//4.1 ������ ���۵Ǵ� �ٰ� ������ ������ ���� ������
		//(�� �� �������� ������ �Ǿ� �����Ƿ� ���� �������� �ʰ� ���ڵ鸸 ������)
		if (selectedStartRowPos == selectedEndRowPos)
		{
			//4.1.1 ������ ���������� ����Ǿ�����
			if (selectedStartLetterPos < selectedEndLetterPos)
			{
				//4.1.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
				startLetterIndex = selectedStartLetterPos;
				//4.1.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				endLetterIndex = selectedEndLetterPos;
				//4.1.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedStartRowPos;

			}
			//4.1.2 ������ �������� ����Ǿ�����
			else
			{
				//4.1.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				startLetterIndex = selectedEndLetterPos;
				//4.1.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
				endLetterIndex = selectedStartLetterPos;
				//4.1.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedEndRowPos;
			}
			//4.1.3 �����ϴ� ���� ���Ѵ�.
			startRow = this->notepadForm->note->GetAt(startRowIndex);
			//4.1.4 �����ϴ� ������ġ���� ������ ���ڱ��� �����.
			while (startLetterIndex < endLetterIndex)
			{
				//4.1.4.1 �ٿ��� ���ڸ� �����.
				startRow->Remove(startLetterIndex);
				//4.1.4.2 �ٿ��� ���ڰ� �������� ���� ������ �ٰ� �����ϴ� ������ ���� ���ڰ�
				//������ �����ϴ� ������ ��ġ�� �մ���� ���� �ǹǷ� ������ ������ ���� ���� ���ҽ�Ų��. 
				endLetterIndex--;
			}
		}
		//4.2 ������ ���۵Ǵ� �ٰ� ������ ������ ���� ���� �ٸ���
		//(������ ���� �ٿ� ���ļ� �Ǿ� �ֱ� ������ ���ڰ� �� ������ ���� ����������)
		else
		{
			//4.2.1 ������ ���������� ����Ǿ����� 
			if (selectedStartRowPos < selectedEndRowPos)
			{
				//4.2.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
				startLetterIndex = selectedStartLetterPos;
				//4.2.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				endLetterIndex = selectedEndLetterPos;
				//4.2.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedStartRowPos;
				//4.2.1.4 ������ ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
				endRowIndex = selectedEndRowPos;
			}
			//4.2.2 ������ �������� ����Ǿ�����
			else
			{
				//4.2.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				startLetterIndex = selectedEndLetterPos;
				//4.2.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
				endLetterIndex = selectedStartLetterPos;
				//4.2.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedEndRowPos;
				//4.2.2.4 ������ ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
				endRowIndex = selectedStartRowPos;
			}
			Glyph* endRow = 0;//������ ���� ��ġ
			Glyph* row = 0;//���� �ּҸ� ���� ����
			Long letterIndex = 0;//���� ��ġ
			//4.2.3 �����ϴ� ���� ���Ѵ�.
			startRow = this->notepadForm->note->GetAt(startRowIndex);
			//4.2.4 �����ϴ� ������ġ���� �����ϴ� ���� ������ ���ڱ��� �����.
			while (startLetterIndex < startRow->GetLength())
			{
				//4.2.4.1 �ٿ��� ���ڸ� �����.
				startRow->Remove(startLetterIndex);
			}
			//4.2.5 �����ϴ� ���� �����ٺ��� ������ �������� ���ڿ� ���� �����.
			Long nextRowIndex = startRowIndex + 1;
			while (nextRowIndex < endRowIndex)
			{
				//4.2.5.1 ���� ���Ѵ�.
				row = this->notepadForm->note->GetAt(nextRowIndex);
				//4.2.5.2 ������ġ�� ����ġ��Ų��.
				letterIndex = 0;
				//4.2.5.3 �ٿ��� ������ ���ڱ��� �ݺ��Ѵ�.
				while (letterIndex < row->GetLength())
				{
					//4.2.5.3.1 ���� ���ڸ� �����.
					row->Remove(letterIndex);
				}
				//4.2.5.4 ���� ���ڸ� �������⶧���� �޸��忡�� ���� �����.
				this->notepadForm->note->Remove(nextRowIndex);
				//4.2.5.5 ���� ������ ������ ������ ������ ���� ��ġ�� ��ĭ �մ������.
				endRowIndex--;
			}
			//4.2.6 ������ ���� ���Ѵ�.
			endRow = this->notepadForm->note->GetAt(endRowIndex);
			//4.2.7 ������ ���� ó������ ������ ���ڱ��� ���ڸ� �����.
			letterIndex = 0;
			while (letterIndex < endLetterIndex)
			{
				//4.2.7.1 ������ ���� ���ڸ� �����.
				endRow->Remove(letterIndex);
				//4.2.7.2 ������ ���� ù���ڸ� ����� ���� ���ں��� ������ ��ĭ��
				//������� ������ ������ ������ġ�� -1 ���ҽ�Ų��.
				endLetterIndex--;
			}
			//4.2.8 ������ ���� �����ϴ� �ٷ� Join��Ų��.
			endRow->Join(startRow);
			//4.2.9 ������ ���� �����ϴ� �ٷ� Join�Ǿ��� ������
			//������ ���� �޸��忡�� �����.
			this->notepadForm->note->Remove(endRowIndex);
			//4.2.10 ���� ���� ��ġ�� �����ϴ� ���� ��ġ�� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->
				GetAt(startRowIndex);
			//4.2.11 ���� ������ ��ġ�� �����ϴ� ������ ��ġ�� �����Ѵ�.
			this->notepadForm->current->Move(startLetterIndex);
		}
		//4.3 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//4.4 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->isDirty = true;
		//4.5 �ڵ� �� �ٲ��� �������̸�
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//4.6.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
		//4.7 �޸��忡�� ���õ� texts�� �� ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = false;
		//2.4 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartXPos = 0;
		//2.5 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartYPos = 0;
	}
}


//�Ҹ���
CtrlBackSpaceKeyAction::~CtrlBackSpaceKeyAction()
{

}