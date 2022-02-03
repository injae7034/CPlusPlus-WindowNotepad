#include "BackSpaceKeyAction.h"
#include "Glyph.h"

//����Ʈ������
BackSpaceKeyAction::BackSpaceKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void BackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���õ� ������ ���Ѵ�.
	Long startingRowPos = 0;
	Long startingLetterPos = 0;
	Long endingRowPos = 0;
	Long endingLetterPos = 0;
	this->notepadForm->note->CalculateSelectedRange(&startingRowPos,
		&startingLetterPos, &endingRowPos, &endingLetterPos);
	Long selectedRange = startingRowPos + startingLetterPos + endingRowPos + endingLetterPos;
	//2. ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	//3. ���� ������ ��ġ�� ���Ѵ�.
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//4. ���õ� texts�� �ϳ��� ������
	if (selectedRange == 0)
	{
		//���� ù ���̸� ���� ���� �� ����, ���� ù �ٿ��� ���� ��ġ�� ���� ó���� ������
		//�ƹ��͵� ���Ͼ. ���� ���� ��ġ�� ���� ó���� �ƴϰ�, ���� ���� ��ġ�� ���� ó���̸�
		//���� ���� ���� �ٿ� Join��Ų��.
		//4.1 ���� ���� ��ġ�� 0���� ũ��, ���� ���� ��ġ�� 0�̸�
		Glyph* currentRow = 0;
		Glyph* previousRow = 0;
		if (currentRowPos > 0 && currentLetterPos == 0)
		{
			//4.1.1 ���� ���� ���Ѵ�.
			currentRow = this->notepadForm->note->GetAt(currentRowPos);
			//4.1.2 ���� ���� ���� ���� ���Ѵ�.
			previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
			//2.3.3 ���� ���� ���� ���� ������ ���� ��ġ�� ���Ѵ�.
			Long letterPos = previousRow->GetLength();
			//4.1.3 ���� ���� ���� �ٿ� ��ģ��.
			currentRow->Join(previousRow);
			//4.1.4 Note���� ���� ���� �ּҸ� �����.
			this->notepadForm->note->Remove(currentRowPos);
			//4.1.5 ���� ���� �������� ������ ���� ���� �����Ѵ�.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//4.1.6 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
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
			//4.1.7 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//4.1.8 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->IsDirty = true;
		}
		// ���� ���� ��ġ�� ó���� �ƴ� ��(���� ���� ó���̵� �ƴϵ� �������) ���� ���ڸ� �����.
		//4.2 ���� ���� ��ġ�� ó���� �ƴϸ�
		else if (currentLetterPos > 0)
		{
			//4.2.1 ���� ���ڸ� �����.
			this->notepadForm->current->Remove(currentLetterPos - 1);
			//4.2.2 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//4.2.3 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->IsDirty = true;
			//4.2.4 �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
			UINT state = this->notepadForm->GetMenu()->
				GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
			//4.2.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
			if (state == MF_CHECKED)
			{
				//4.2.5.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
		}
	}
	//3. ���õ� texts�� ������
	else
	{
		//3.1 �޸��忡�� ���õ� texts�� ���� �����.
		Glyph* startingSelectedRow = 0;//������ ���۵Ǵ� ���� �ּҸ� ������ ����
		Glyph* letter = 0;//���� �ּҸ� ������ ����
		Glyph* row = 0;//�� �ּҸ� ������ ����
		Long startingLetterIndex = 0;//�ٿ��� �ݺ��� �����ϴ� ���� ��ġ
		Long endingLetterIndex = 0;//�ٿ��� �ݺ��� ������ ���� ��ġ
		Long lastLetterIndex = 0;//�ٿ��� ������ ������ ��ġ
		//3.1.1 ������ ���۵Ǵ� ���̶� ������ ������ ���̶� ������
		if (startingRowPos == endingRowPos)
		{	
			//3.1.1.1 ������ ���۵Ǵ� ���� ���Ѵ�.
			row = this->notepadForm->note->GetAt(startingRowPos);
			//3.1.1.2 ������ ���۵Ǵ� ���� ������ ������ġ�� ���Ѵ�.
			lastLetterIndex = row->GetLength() - 1;
			//3.1.1.3 ������ ���۵Ǵ� ���ں��� �����ؼ� ������ ������ ���ڱ��� ���ڸ� �����.
			startingLetterIndex = startingLetterPos;
			endingLetterIndex = endingLetterPos;
			while (startingLetterIndex <= endingLetterIndex)
			{
				//3.1.1.3.1 ���ڸ� �����.
				row->Remove(startingLetterIndex);
				//3.1.1.3.2 ������ ���� ��ġ�� ���ҽ�Ų��.
				endingLetterIndex--;
			}
			//3.1.1.4 ������ ���۵Ǵ� ���� ���ڰ� ������������
			//if (row->GetLength() == 0)
			//<����>�ٿ� �ִ� ���ڸ� �������� �ʾҴ��� �������� �ִ� ���ڸ� ��������
			//������ �������� �����ϴ� �ٿ� Join���Ѿ���!!!
			//3.1.1.4 ������ ���۵Ǵ� �ٿ��� ������ ������ ���� ��ġ�� ���� ������ �����̰�,
			//��������� ������ ���� ��ġ�� 0�̸�
			//(������ ���۵Ǵ� �ٿ��� ������ ��ġ�� �ִ� ���ڸ� ��������)
			if (endingLetterPos == lastLetterIndex && currentLetterPos == 0
				&& currentRowPos != startingRowPos)
			{
				//3.1.1.5 ������ ���۵Ǵ� ���� ���� ���� ���Ѵ�.
				row = this->notepadForm->note->GetAt(startingRowPos + 1);
				//3.1.1.6 ������ ���۵Ǵ� �� ������ ���� ������
				if (row != 0)
				{
					//3.1.1.6.1 ������ ���۵Ǵ� ���� ���Ѵ�.
					startingSelectedRow = this->notepadForm->note->GetAt(startingRowPos);
					//3.1.1.6.2 ������ ���۵Ǵ� ���� ���� ���� ������ ���۵Ǵ� �ٿ� Join��Ų��.
					row->Join(startingSelectedRow);
					//3.1.1.6.3 ������ ���۵Ǵ� ���� ���� ������ ������ ���۵Ǵ� �ٿ� 
					//Join�Ǿ��� ������ �޸��忡�� �ش� ���� �����.
					this->notepadForm->note->Remove(startingRowPos + 1);
				}
			}
		}
		//3.1.2 ������ ���۵Ǵ� ���̶� ������ ������ ���̶� �ٸ���
		//(������ ���۵Ǵ� ���� ��ġ���� ������ ������ ���� ��ġ�� �� ũ��
		else
		{
			//3.1.2.1 ������ ���۵Ǵ� �ٿ��� ������ ���۵Ǵ� ������ġ��
			//������ ���۵Ǵ� ���� ���ڰ������� �������� �ݺ��Ѵ�.
			//(������ ���۵Ǵ� �ٿ��� ���ڵ��� �����)
			//<����>������ ���۵Ǵ� �ٿ��� ���ڵ��� �� ������� �ְ� �� ������������ �ִ�
			//������ ���ڸ� �������ٰ� �ؼ� ������ ���۵Ǵ� ���� �޸��忡�� ����� �ȵ�. 
			//������ ������ ���� ������ ������ ���ڱ��� �� ��������� ������ ���۵Ǵ� �ٷ� 
			//Join�� ���Ѿ��ϱ� ������ ������ ���۵Ǵ� ���� ������ ������ �޸��忡�� ����� �ȵȴ�!!!
			startingLetterIndex = startingLetterPos;
			row = this->notepadForm->note->GetAt(startingRowPos);
			while (startingLetterIndex < row->GetLength())
			{
				//3.1.2.1.1 ���ڸ� �����.
				row->Remove(startingLetterIndex);
			}
			//������ ���۵Ǵ� ���� ���ڰ� ���������� �翬�� ���� �״�� �ֱ� ������ �����ٷ� �̵��ؾ��ϰ�,
			//���࿡ ������ ���۵Ǵ� ���� ���ڰ� ������������ �޸��忡�� ���� �������� �ʾұ� ������
			//�����ٷ� �̵��ؾ� �Ѵ�.
			//3.1.2.2 ���� ��ġ�� ������Ų��.(�����ϴ� ���� ���ڸ� �������⶧���� �����ٷ� �̵�)
			Long rowIndex = startingRowPos + 1;
			Long endingRowIndex = endingRowPos;
			//3.1.2.3 ���� ��ġ�� ������ ������ �ٺ��� �������� �ݺ��Ѵ�.
			while (rowIndex < endingRowIndex)
			{
				//3.1.2.3.1 ���� ���Ѵ�.
				row = this->notepadForm->note->GetAt(rowIndex);
				//3.1.2.3.2 ���� ��ġ�� 0���� �ʱ�ȭ ��Ų��.
				startingLetterIndex = 0;
				//3.1.2.3.3 ���� ��ġ�� ���� �������� �������� �ݺ��Ѵ�.
				while (startingLetterIndex < row->GetLength())
				{
					//3.1.2.3.3.1 ���ڸ� �����.
					row->Remove(startingLetterIndex);
				}
				//3.1.2.3.4 �ٿ� ��� ���ڸ� �������⶧���� �޸��忡�� �ش� ���� �����.
				this->notepadForm->note->Remove(rowIndex);
				//3.1.2.3.5 ������ ������ ���� �տ� �ִ� ���� �޸��忡�� �������� ������
				//�ڿ� �ٵ��� �� �پ� ������ �������. ���� ������ ������ ���� ��ġ��
				//���� ������ �̵��ϱ� ������ -1�� ���ҽ�Ų��.
				endingRowIndex--;
			}
			//3.1.2.4 ���� ���� ��ġ�� 0���� �ʱ�ȭ��Ų��.
			startingLetterIndex = 0;
			//3.1.2.5 ������ ������ �ٿ��� ������ ������ ���� ��ġ�� �����Ѵ�.
			endingLetterIndex = endingLetterPos;
			//3.1.2.6 ������ ������ ���� ���Ѵ�.
			row = this->notepadForm->note->GetAt(endingRowIndex);
			//3.1.2.7 ������ ������ ���� ������ ���� ��ġ�� ���Ѵ�.
			lastLetterIndex = row->GetLength() - 1;
			//3.1.2.8 ���� ��ġ�� ������ ������ �ٿ��� ������ ������ ���� ��ġ����
			//�۰ų� ���� ���� �ݺ��Ѵ�.
			while (startingLetterIndex <= endingLetterIndex)
			{
				//3.1.2.8.1 ���ڸ� �����.
				row->Remove(startingLetterIndex);
				//3.1.2.8.2 ������ ������ ���� ������ ���ڸ� ������ ������
				//������ ������ ���� ��ġ�� ������ ����(-1 ���ҽ�Ų��.)
				endingLetterIndex--;
			}

			//3.1.2.9 ������ ������ �ٿ��� ������ ������ ���� ��ġ�� ���� ������ �����̸�
			//(������ ������ �ٿ� ��� ���ڸ� ����������)
			//startingLetterIndex�� 0���� �����ϱ� ������(���� ù���ں��� �����ؼ� ����� ������) 
			//endingLetterPos == lastLetterIndex�̸� ���� ���ڸ� ��������̴�.
			//if (endingLetterPos == lastLetterIndex)
			//3.1.2.9 ���� ���ڰ����� 0�̸�(�ٿ� �ִ� ���ڰ� �� ����������)
			//���� ������ ������ġ�� �̿��ϴ°ź��� ���� ���ڰ����� �̿��ϴ°� �������̰� ��������!!
			//if (row->GetLength() == 0)
			//<����>�ٿ� �ִ� ���ڸ� �������� �ʾҴ��� �������� �ִ� ���ڸ� ��������
			//������ �������� �����ϴ� �ٿ� Join���Ѿ���!!!
			//3.1.2.9 ������ ���۵Ǵ� �ٿ��� ������ ������ ���� ��ġ�� ���� ������ �����̸�
			//(������ ���۵Ǵ� �ٿ��� ������ ��ġ�� �ִ� ���ڸ� ��������)

			//3.1.2.9 ������ ������ �ٿ��� ������ ������ ���� ��ġ�� ���� ������ �����̰�,
			//��������� ������ ���� ��ġ�� 0�̸�
			//(������ ���۵Ǵ� �ٿ��� ������ ��ġ�� �ִ� ���ڸ� ��������)
			if (endingLetterPos == lastLetterIndex && currentLetterPos == 0
				&& currentRowPos != startingRowPos)
			{
				//3.1.2.9.1 �ٿ� ��� ���ڸ� �������� ������ �޸��忡��
				//������ ������ ���� �����.
				this->notepadForm->note->Remove(endingRowIndex);
			}
			//3.1.2.10 ������ ���۵Ǿ��� ���� �ٽ� ���Ѵ�.
			startingSelectedRow = this->notepadForm->note->GetAt(startingRowPos);
			//3.1.2.11 ������ ������ ���� ���Ѵ�. ������ ������ ���� ���ڰ� ��� ������ ������ ����
			//�������ٸ� ������ ������ ���� ���� ���� �� �� ������ ������� ������ ������ ������
			//���� ��ġ endingRowIndex�� ������ ������ ������ ���� ���� ��ġ�� ���� �� �ִ�!
			//*������ ������ �� �Ǵ� ������ ������ ���� ���� ���� ���Ѵ�.
			row = this->notepadForm->note->GetAt(endingRowIndex);
			//3.1.2.12 ������ ������ ��(�Ǵ� ������ ������ ���� ������)�� ������ ���۵Ǿ��� �ٿ�
			//Join��Ų��.
			row->Join(startingSelectedRow);
			//3.1.2.13 ������ ������ ��(�Ǵ� ������ ������ ���� ������)�� ������ ���۵Ǿ��� �ٿ� 
			//Join�Ǿ��� ������ �޸��忡�� �ش� ���� �����.
			this->notepadForm->note->Remove(endingRowIndex);
		}
		//3.1.3 ���� ���� ��ġ�� �ٽ� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(startingRowPos);
		//3.1.4 ���� ���ڷ� �̵��Ѵ�.
		this->notepadForm->current->Move(startingLetterPos);
		//3.1.5 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//3.1.6 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->IsDirty = true;
		//3.1.7 �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
		UINT state = this->notepadForm->GetMenu()->
			GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
		//3.1.8 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (state == MF_CHECKED)
		{
			//3.1.8.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
}


//�Ҹ���
BackSpaceKeyAction::~BackSpaceKeyAction()
{

}