#include "CtrlRightArrowKeyAction.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "DummyRow.h"

//����Ʈ������
CtrlRightArrowKeyAction::CtrlRightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlRightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ������ ����ǰ� �ִ� ���̾�����
	if (this->notepadForm->isSelecting == true)
	{
		//1.1. ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
		this->notepadForm->selectingTexts->Undo();
		//1.2 ������ ���� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = false;
		//1.3 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartXPos = 0;
		//1.4 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartYPos = 0;
	}
	//2. �޸��忡�� ���� ���� ��ġ�� ���Ѵ�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//3. �޸��忡�� ���� ������ ��ġ�� ���Ѵ�.
	Long currentLetterIndex = this->notepadForm->note->GetAt(currentRowIndex)->GetCurrent();
	//4. �ڵ������� ���� ���� �ƴϸ�
	if (this->notepadForm->isRowAutoChanging == false)
	{
		//4.1 �̵��ϱ� ���� ���� ��ġ�� ���Ѵ�.
		Long previousRowIndex = currentRowIndex;
		//4.2 �̵��ϱ� ���� ������ ��ġ�� ���Ѵ�.
		Long previousLetterIndex = currentLetterIndex;
		//4.3 ��Ʈ���� �ܾ������ ���������� �̵��Ѵ�.
		currentLetterIndex = this->notepadForm->note->NextWord();
		//4.4 �̵��� �Ŀ� ���� ��ġ�� ���Ѵ�.
		currentRowIndex = this->notepadForm->note->GetCurrent();
		//4.5 �޸����� ���� ���� �̵��� ���� �ٷ� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	}
	//5. �ڵ������� ���� ���̸�
	else
	{
		//5.1 �޸����� ���� �ٿ��� ó�� ���ں��� ���� ���ڱ����� content�� ���Ѵ�.
		Long i = 0;
		Glyph* letter = 0;
		string letterContent = "";
		string currentLetters = "";
		while (i < currentLetterIndex)
		{
			//5.4.1 ���ڸ� ���Ѵ�.
			letter = this->notepadForm->current->GetAt(i);
			letterContent = letter->GetContent();
			//5.4.2 ���� content�� �����ش�.
			currentLetters += letterContent;
			i++;
		}
		//5.2 �޸����� ���� �ٿ��� ó�����ں��� ���� ���ڱ����� content�� ���̸� ���Ѵ�.
		Long currentLettersLength = currentLetters.length();
		//5.3 ���� ���� ���Ѵ�.
		Glyph* row = this->notepadForm->note->GetAt(currentRowIndex);
		//5.4 ���� ���� ��ü content�� ���Ѵ�.
		i = 0;
		string realRowContent = "";
		while (i < row->GetLength())
		{
			//5.4.1 ���ڸ� ���Ѵ�.
			letter = row->GetAt(i);
			letterContent = letter->GetContent();
			//5.4.2 ���� content�� �����ش�.
			realRowContent += letterContent;
			i++;
		}
		//5.5 ���� ���� ���Ѵ�.
		Long nextRowIndex = currentRowIndex + 1;
		Glyph* nextRow = this->notepadForm->note->GetAt(nextRowIndex);
		//5.6 ���� ���� ��ġ�� ���� �������� �������� �׸��� ���� ���� ��¥ ���� ���� �ݺ��Ѵ�.
		string dummyRowContent = "";
		while (nextRowIndex < this->notepadForm->note->GetLength() &&
			dynamic_cast<DummyRow*>(nextRow))
		{
			//5.6.1 ��¥���� content�� ���Ѵ�.
			i = 0;
			dummyRowContent = "";
			while (i < nextRow->GetLength())
			{
				//5.6.1.1 ���ڸ� ���Ѵ�.
				letter = nextRow->GetAt(i);
				letterContent = letter->GetContent();
				//5.6.1.2 ���� content�� �����ش�.
				dummyRowContent += letterContent;
				i++;
			}
			//5.6.2 ��¥���� content�� ��¥ ���� content�� �����ش�.
			realRowContent += dummyRowContent;
			//5.6.3 ���� �ٷ� �̵��Ѵ�.
			nextRowIndex++;
			//5.6.4 ���� ���� ���Ѵ�.
			nextRow = this->notepadForm->note->GetAt(nextRowIndex);
		}
		//5.7 ��¥ ���� ���� ��¥ ���� content ���̸� ���Ѵ�.
		Long rowContentLength = realRowContent.length();
		Long rowLastContentLength = rowContentLength - 1;
		i = currentLettersLength;
		Long j = 0;
		//5.8 ���� ���� ��ġ�� ���� ���� ������ ���� ��ġ��(���� ���� ��¥ �� �Ǵ� ��¥ �� ������
		//���� ���� ������ ��¥ ����. �׷��� ���� ���� ������ ���� ��ġ�� ������ ���� ��(��¥ ��)�� �̵���.)
		if (i == rowContentLength)
		{
			//5.8.1 ���� �ٷ� �̵��Ѵ�.
			currentRowIndex++;
			//5.8.2 ���� ���� ó��������ġ�� �̵��Ѵ�.
			currentLetterIndex = 0;
			//5.8.3 ���� ���Ѵ�.
			row = this->notepadForm->note->GetAt(currentRowIndex);
			//5.8.4 ���� content�� ���Ѵ�.
			j = 0;
			realRowContent = "";
			while (j < row->GetLength())
			{
				//5.8.4.1 ���ڸ� ���Ѵ�.
				letter = row->GetAt(j);
				letterContent = letter->GetContent();
				//5.8.4.2 ���� content�� �����ش�.
				realRowContent += letterContent;
				j++;
			}
			//5.8.5 ���� ���� ���Ѵ�.
			nextRowIndex = currentRowIndex + 1;
			nextRow = this->notepadForm->note->GetAt(nextRowIndex);
			//5.8.6 ���� ���� ��ġ�� ���� �������� �������� �׸��� ���� ���� ��¥ ���� ���� �ݺ��Ѵ�.
			dummyRowContent = "";
			while (nextRowIndex < this->notepadForm->note->GetLength() &&
				dynamic_cast<DummyRow*>(nextRow))
			{
				//5.8.6.1 ��¥���� content�� ���Ѵ�.
				j = 0;
				dummyRowContent = "";
				while (j < nextRow->GetLength())
				{
					//5.8.6.1.1 ���ڸ� ���Ѵ�.
					letter = nextRow->GetAt(j);
					letterContent = letter->GetContent();
					//5.8.6.1.2 ���� content�� �����ش�.
					dummyRowContent += letterContent;
					j++;
				}
				//5.8.8.2 ��¥���� content�� ��¥ ���� content�� �����ش�.
				realRowContent += dummyRowContent;
				//5.8.8.3 ���� �ٷ� �̵��Ѵ�.
				nextRowIndex++;
				//5.8.8.4 ���� ���� ���Ѵ�.
				nextRow = this->notepadForm->note->GetAt(nextRowIndex);
			}
			//5.8.7 ���� content ���̸� ���Ѵ�.
			rowContentLength = realRowContent.length();
			i = 0;
			letterContent = realRowContent[i];//�ѱ��ڸ� ���� ����
			//5.8.10 ���� ���ڰ� �ǹ��ڰų� �����̽������̸�
			if (letterContent == "\t" || letterContent == " ")
			{
				//5.8.10.1 ������ ���� ��ġ�� ���Ѵ�.
				rowLastContentLength = rowContentLength - 1;
				//5.8.10.2 ������ ������ġ���� �������� �׸��� �ǹ����ε��� �ݺ��Ѵ�.
				while (i < rowLastContentLength && letterContent == "\t")
				{
					//5.8.10.2.1 ���� ������ġ�� ������Ų��.
					currentLetterIndex++;
					i++;
					//5.8.10.2.2 ���ڸ� �����Ѵ�.
					letterContent = realRowContent[i];
				}
				//5.8.10.3 ������ ������ġ�� ���� �ǹ����̸�
				if (i == rowLastContentLength && letterContent == "\t")
				{
					//5.8.10.3.1 ���� ������ġ�� ������Ų��.
					currentLetterIndex++;
					i++;
				}
				//5.8.10.4 ������ ������ġ���� �������� �׸��� �����̽������ε��� �ݺ��Ѵ�.
				while (i < rowLastContentLength && letterContent == " ")
				{
					//5.8.10.4.1 ���� ������ġ�� ������Ų��.
					currentLetterIndex++;
					i++;
					//5.8.10.4.2 ���ڸ� �����Ѵ�.
					letterContent = realRowContent[i];
				}
				//5.8.10.5 ������ ������ġ�� ���� �����̽������̸�
				if (i == rowLastContentLength && letterContent == " ")
				{
					//5.8.10.5.1 ���� ������ġ�� ������Ų��.
					currentLetterIndex++;
					i++;
				}
			}
		}
		//5.9 �װ� �ƴϸ�
		else
		{
			//5.9.1 �ѱ��̸�
			if ((realRowContent[i] & 0x80))//�ѱ��̸�(2byte����)
			{
				//5.9.1.1 2byte�� �����Ѵ�.
				letterContent = realRowContent[i];
				i++;
				letterContent += realRowContent[i];
			}
			//5.9.2 �ѱ��� �ƴϸ�
			else
			{
				//5.9.2.1 1byte�� �����Ѵ�
				letterContent = realRowContent[i];
			}
			//5.9.3 ������ ������ġ���� �������� �׸��� �ǹ��ڿ� �����̽����ڰ� �ƴѵ��� �ݺ��Ѵ�.
			while (i < rowLastContentLength && letterContent != " " && letterContent != "\t")
			{
				//5.9.3.1 ���� ������ġ�� ������Ų��.
				currentLetterIndex++;
				i++;
				//5.9.3.2 �ٿ��� i��° ���� ���ڰ� �ѱ��̸�
				if ((realRowContent[i] & 0x80))//�ѱ��̸�(2byte����)
				{
					//5.9.3.2.1 2byte�� �����Ѵ�.
					letterContent = realRowContent[i];
					i++;
					letterContent += realRowContent[i];
				}
				//5.9.3.3 �ѱ��� �ƴϸ�
				else
				{
					//5.9.3.3.1 1byte�� �����Ѵ�
					letterContent = realRowContent[i];
				}
			}
			//5.9.4 ������ ������ġ�� ����, �ǹ��ڿ� �����̽� ���ڰ� �ƴϸ�
			if (i == rowLastContentLength && letterContent != " " && letterContent != "\t")
			{
				//5.9.4.1 ���� ������ġ�� ������Ų��.
				currentLetterIndex++;
				i++;
			}
			//5.9.5 ������ ������ġ���� �������� �׸��� �ǹ����ε��� �ݺ��Ѵ�.
			while (i < rowLastContentLength && letterContent == "\t")
			{
				//5.9.5.1 ���� ������ġ�� ������Ų��.
				currentLetterIndex++;
				i++;
				//5.9.5.2 ���ڸ� �����Ѵ�.
				letterContent = realRowContent[i];
			}
			//5.9.6 ������ ������ġ�� ���� �ǹ����̸�
			if (i == rowLastContentLength && letterContent == "\t")
			{
				//5.9.6.1 ���� ������ġ�� ������Ų��.
				currentLetterIndex++;
				i++;
			}
			//5.9.7 ������ ������ġ���� �������� �׸��� �����̽������ε��� �ݺ��Ѵ�.
			while (i < rowLastContentLength && letterContent == " ")
			{
				//5.9.7.1 ���� ������ġ�� ������Ų��.
				currentLetterIndex++;
				i++;
				//5.9.7.2 ���ڸ� �����Ѵ�.
				letterContent = realRowContent[i];
			}
			//5.9.8 ������ ������ġ�� ���� �����̽������̸�
			if (i == rowLastContentLength && letterContent == " ")
			{
				//5.9.8.1 ���� ������ġ�� ������Ų��.
				currentLetterIndex++;
				i++;
			}
		}
		//5.10 ���� ���� ���Ѵ�.
		Glyph* currentRow = this->notepadForm->note->GetAt(currentRowIndex);
		//5.11 ���� ���� content�� ���Ѵ�.
		j = 0;
		string currentRowContent = "";
		while (j < currentRow->GetLength())
		{
			//5.11.1 ���ڸ� ���Ѵ�.
			letter = currentRow->GetAt(j);
			letterContent = letter->GetContent();
			//5.11.2 ���� content�� �����ش�.
			currentRowContent += letterContent;
			j++;
		}
		//5.12 ���� ���� content�� length�� ���Ѵ�.
		Long currentRowContentLength = currentRowContent.length();
		//5.13 i�� ���� ���� content�� length���� ũ�ų� �������� �ݺ��Ѵ�.
		//�Ʊ� �տ��� �κ��ڵ������� ����ϴ� ��ó�� ��¥���� content���� ��¥���� content��
		//�߰��������� ��ǻ� ���� �޸��忡���� �ڵ������� �Ǿ� �ִ� �����̱� ������
		//�ڵ������� �� �޸��忡�� ã�� ���ڿ��� ã�Ƽ� ������ �ؾ��ϱ� ������ �Ʊ� ���ƴ� ������ 
		//��¥�ٵ��� �ٽ� ���������� ���ؼ� ��ģ �ٿ��� ���� ������ġ i�� ���ؼ� ���� ���� ��ġ��
		//�ٽ� �����ش�.
		string nextRowContent = "";
		while (i > currentRowContentLength)
		{
			//5.13.1 ���� ���� ��ġ�� ������Ų��.
			//�ڵ������� ��ҵ� ���¿��� �ش� ���ڿ��� ã�Ҵµ� ���������� ���� ���ڰ�
			//���� �������� ���� ���̺��� ��� ������ ���� �ٷ� �Ѿ�� �ٽ� ���ؾ��Ѵ�.
			currentRowIndex++;
			//5.13.2 ���� ������ġ�� �ڵ������� ��ҵ� ������ ������ġ�� ������ ���� ���� 
			//��ġ�� ���� ���� �������� ũ�� ������ �׸�ŭ ���� �ڵ������� �� ���·� �����Ѵ�
			//�տ��� currentLetterIndex++�� ���� 1�� ������ �����̱� ������ +1�� ���� �����൵��!
			currentLetterIndex -= currentRow->GetLength();
			//5.13.3 ���� ���� �������� ���Ѵ�.
			currentRow = this->notepadForm->note->GetAt(currentRowIndex);
			//5.13.4 ���� ���� content�� �����Ѵ�.
			j = 0;
			nextRowContent = "";
			while (j < currentRow->GetLength())
			{
				//5.13.4.1 ���ڸ� ���Ѵ�.
				letter = currentRow->GetAt(j);
				letterContent = letter->GetContent();
				//5.13.4.2 ���� content�� �����ش�.
				nextRowContent += letterContent;
				j++;
			}
			//5.13.5 ���� ���� content�� ���� ���� content�� �����ش�.(����)
			currentRowContent += nextRowContent;
			//5.13.6 ������ ���� content�� ���̸� ���Ѵ�.
			currentRowContentLength = currentRowContent.length();
		}
		//5.14 ���� ���� ��ġ�� ������ġ�� �ٽ� �������ش�.
		currentRowIndex = this->notepadForm->note->Move(currentRowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		currentLetterIndex = this->notepadForm->current->Move(currentLetterIndex);
	}
}

//�Ҹ���
CtrlRightArrowKeyAction::~CtrlRightArrowKeyAction()
{

}