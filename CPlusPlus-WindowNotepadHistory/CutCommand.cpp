#include "CutCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"
#include "RowAutoChange.h"
#include "SelectingTexts.h"

//����Ʈ������
CutCommand::CutCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->glyph = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->selectedStartXPos = 0;
	this->selectedStartYPos = 0;
	this->isSelectedTextsRemoved = false;//ó���� ������ ���� ���ÿ����� �����������Ƿ� false�� ����Ʈ��
}

//Execute
void CutCommand::Execute()
{
	//������ ���� ���̸�(���õ� ������ ������ �߶󳻱⸦ ���� ����)
	if (this->notepadForm->isSelecting == true)
	{
		//1. RowAutoChange�� �����Ѵ�.
		RowAutoChange rowAutoChange(this->notepadForm);
		Long changedRowPos = 0;
		Long changedLetterPos = 0;
		Long originRowPos = this->rowIndex;
		Long originLetterPos = this->letterIndex;
		//2. ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
		Long currentRowPos = this->notepadForm->note->GetCurrent();
		Long currentLetterPos = this->notepadForm->current->GetCurrent();
		//3. RemoveCommand�� �ٽ� ����Ǹ�
		if (this->isRedone == true)
		{
			//3.1 ���� ���� ��ġ�� ������ġ�� ���������ش�.
			currentRowPos = this->notepadForm->note->Move(this->rowIndex);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
			//3.2 �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� ����Ǿ� ����)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//3.2.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//3.2.2 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
				currentRowPos = this->notepadForm->note->Move(changedRowPos);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
			}
		}
		//4. �ٽ� �����̸�
		if (this->isRedone == true)
		{
			this->notepadForm->selectedStartYPos = this->selectedStartYPos;
			this->notepadForm->selectedStartXPos = this->selectedStartXPos;
		}
		//5. ������ ���۵Ǵ� �ٰ� ���� ��ġ, ������ ������ �ٰ� ���� ��ġ�� �����Ѵ�.
		Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//������ ���۵Ǵ� ��
		Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//������ ���۵Ǵ� ����
		Long selectedEndRowPos = currentRowPos;//������ ������ ��
		Long selectedEndLetterPos = currentLetterPos;//������ ������ ����
		//6. ������ ����� ������� ������ ����(����)���� �����ϴ� �ٰ� ����, ������ �ٰ� ������ġ�� ���Ѵ�.
		Long startRowIndex = 0;//�����ϴ� ���� ��ġ
		Long startLetterIndex = 0;//�����ϴ� ���� ��ġ
		Long endRowIndex = 0;//������ ���� ��ġ
		Long endLetterIndex = 0;//������ ���� ��ġ
		Long copyLetterIndex = 0;//Ŭ�����忡 ����Ǵ� ���ڵ��� ��ġ�� ������ ���
		Long i = 0;//�ݺ������
		Glyph* startRow = 0;//�����ϴ� ���� ��ġ
		Glyph* letter = 0;
		Glyph* newRow = 0;//���� ���� �����Ǵ� ���� �ּҸ� ���� ����
		//7. ������ ���۵Ǵ� �ٰ� ������ ������ ���� ������(���簡 �� �� ������ �̷�������)
		if (selectedStartRowPos == selectedEndRowPos)
		{
			//7.1.1 ������ ���������� ����Ǿ�����
			if (selectedStartLetterPos < selectedEndLetterPos)
			{
				//7.1.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
				startLetterIndex = selectedStartLetterPos;
				//7.1.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				endLetterIndex = selectedEndLetterPos;
				//7.1.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedStartRowPos;

			}
			//7.1.2 ������ �������� ����Ǿ�����
			else
			{
				//7.1.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				startLetterIndex = selectedEndLetterPos;
				//7.1.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
				endLetterIndex = selectedStartLetterPos;
				//7.1.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedEndRowPos;
			}
			//7.1.3 �����ϴ� ���� ���Ѵ�.
			startRow = this->notepadForm->note->GetAt(startRowIndex);
			//7.1.4 ó�� �����̸�
			if (this->isRedone == false)
			{
				//7.1.4.1 Note�� �����Ѵ�.
				this->glyph = new Note();
				//7.1.4.2 ������ startRow�� ��¥ ���̸�
				if (!dynamic_cast<DummyRow*>(startRow))
				{
					//7.1.4.2.1 Row�� �����Ѵ�.
					newRow = new Row();
				}
				//7.1.4.3 ������ startRow�� ��¥ ���̸�
				else
				{
					//7.1.4.3.1 DummyRow�� �����Ѵ�.
					newRow = new DummyRow();
				}
				//7.1.4.4 ���� ������ Row�� command�� Note�� �߰��Ѵ�.
				this->glyph->Add(newRow);
				//7.1.4.5 �����ϴ� ������ġ���� ������ ���ڱ��� �����.
				while (startLetterIndex < endLetterIndex)
				{
					//7.1.4.5.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
					letter = startRow->GetAt(startLetterIndex);
					//7.1.4.5.2 ���ڸ� ���� �����ؼ� ���� ������ �ٿ� �����Ѵ�.
					newRow->Add(letter->Clone());
					//7.1.4.5.3 �ٿ��� ���ڸ� �����.
					startRow->Remove(startLetterIndex);
					//7.1.4.5.4 �ٿ��� ���ڰ� �������� ���� ������ �ٰ� �����ϴ� ������ ���� ���ڰ�
					//������ �����ϴ� ������ ��ġ�� �մ���� ���� �ǹǷ� ������ ������ ���� ���� ���ҽ�Ų��. 
					endLetterIndex--;
				}
			}
			//7.1.5 �ٽ� �����̸�
			else
			{
				//7.1.5.1 �����ϴ� ������ġ���� ������ ���ڱ��� �����.
				while (startLetterIndex < endLetterIndex)
				{
					//7.1.5.1.1 �ٿ��� ���ڸ� �����.
					startRow->Remove(startLetterIndex);
					//7.1.5.1.2 �ٿ��� ���ڰ� �������� ���� ������ �ٰ� �����ϴ� ������ ���� ���ڰ�
					//������ �����ϴ� ������ ��ġ�� �մ���� ���� �ǹǷ� ������ ������ ���� ���� ���ҽ�Ų��. 
					endLetterIndex--;
				}
			}
		}
		//8. ������ �����ϴ� �ٰ� ������ ������ ���� ���� �ٸ���(������ �����ٿ� �������� �Ǿ� ������)
		else
		{
			//8.1 ������ ���������� ����Ǿ����� 
			if (selectedStartRowPos < selectedEndRowPos)
			{
				//8.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
				startLetterIndex = selectedStartLetterPos;
				//8.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				endLetterIndex = selectedEndLetterPos;
				//8.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedStartRowPos;
				//8.1.4 ������ ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
				endRowIndex = selectedEndRowPos;
			}
			//8.2 ������ �������� ����Ǿ�����
			else
			{
				//8.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
				startLetterIndex = selectedEndLetterPos;
				//8.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
				endLetterIndex = selectedStartLetterPos;
				//8.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
				startRowIndex = selectedEndRowPos;
				//8.2.4 ������ ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
				endRowIndex = selectedStartRowPos;
			}
			Glyph* endRow = 0;//������ ���� ��ġ
			Glyph* row = 0;//���� �ּҸ� ���� ����
			Long letterIndex = 0;//���� ��ġ
			Long nextRowIndex = 0;
			//8.3 �����ϴ� ���� ���Ѵ�.
			startRow = this->notepadForm->note->GetAt(startRowIndex);
			//8.4 ó�� �����̸�
			if (this->isRedone == false)
			{
				//8.4.1 Note�� �����Ѵ�.
				this->glyph = new Note();
				//8.4.2 ������ startRow�� ��¥ ���̸�
				if (!dynamic_cast<DummyRow*>(startRow))
				{
					//8.4.2.1 Row�� �����Ѵ�.
					newRow = new Row();
				}
				//8.4.3 ������ startRow�� ��¥ ���̸�
				else
				{
					//8.4.3.1 DummyRow�� �����Ѵ�.
					newRow = new DummyRow();
				}
				//8.4.4 ���� ������ Row�� command�� Note�� �߰��Ѵ�.
				this->glyph->Add(newRow);
				//8.4.5 �����ϴ� ������ġ���� �����ϴ� ���� ������ ���ڱ��� �����.
				while (startLetterIndex < startRow->GetLength())
				{
					//8.4.5.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
					letter = startRow->GetAt(startLetterIndex);
					//8.4.5.2 ���ڸ� ���� �����ؼ� ���� ������ �ٿ� �����Ѵ�.
					newRow->Add(letter->Clone());
					//8.4.5.3 �ٿ��� ���ڸ� �����.
					startRow->Remove(startLetterIndex);
				}
				//8.4.6 �����ϴ� ���� �����ٺ��� ������ �������� ���ڿ� ���� �����.
				nextRowIndex = startRowIndex + 1;
				while (nextRowIndex < endRowIndex)
				{
					//8.4.6.1 ���� ���Ѵ�.
					row = this->notepadForm->note->GetAt(nextRowIndex);
					//8.4.6.2 ������ ���� ��¥ ���̸�
					if (!dynamic_cast<DummyRow*>(row))
					{
						//8.4.6.2.1 Row�� �����Ѵ�.
						newRow = new Row();
					}
					//8.4.6.3 ������ ���� ��¥ ���̸�
					else
					{
						//8.4.6.3.1 DummyRow�� �����Ѵ�.
						newRow = new DummyRow();
					}
					//8.4.6.4 ���� ������ Row�� command�� Note�� �߰��Ѵ�.
					this->glyph->Add(newRow);
					//8.4.6.5 ������ġ�� ����ġ��Ų��.
					letterIndex = 0;
					//8.4.6.6 �ٿ��� ������ ���ڱ��� �ݺ��Ѵ�.
					while (letterIndex < row->GetLength())
					{
						//8.4.6.6.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
						letter = row->GetAt(letterIndex);
						//8.4.6.6.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
						newRow->Add(letter->Clone());
						//8.4.6.6.3 ���� ���ڸ� �����.
						row->Remove(letterIndex);
					}
					//8.4.6.7 ���� ���ڸ� �������⶧���� �޸��忡�� ���� �����.
					this->notepadForm->note->Remove(nextRowIndex);
					//8.4.6.8 ���� ������ ������ ������ ������ ���� ��ġ�� ��ĭ �մ������.
					endRowIndex--;
				}
				//8.4.7 ������ ���� ���Ѵ�.
				endRow = this->notepadForm->note->GetAt(endRowIndex);
				//8.4.8 ������ ���� ��¥ ���̸�
				if (!dynamic_cast<DummyRow*>(endRow))
				{
					//8.4.8.1 Row�� �����Ѵ�.
					newRow = new Row();
				}
				//8.4.9 ������ ���� ��¥ ���̸�
				else
				{
					//8.4.9.1 DummyRow�� �����Ѵ�.
					newRow = new DummyRow();
				}
				//8.4.10 ���� ������ Row�� command�� Note�� �߰��Ѵ�.
				this->glyph->Add(newRow);
				//8.4.11 ������ ���� ó������ ������ ���ڱ��� ���ڸ� �����.
				letterIndex = 0;
				while (letterIndex < endLetterIndex)
				{
					//8.4.11.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
					letter = endRow->GetAt(letterIndex);
					//8.4.11.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
					newRow->Add(letter->Clone());
					//8.4.11.3 ������ ���� ���ڸ� �����.
					endRow->Remove(letterIndex);
					//8.4.11.4 ������ ���� ù���ڸ� ����� ���� ���ں��� ������ ��ĭ��
					//������� ������ ������ ������ġ�� -1 ���ҽ�Ų��.
					endLetterIndex--;
				}
			}
			//8.5 �ٽ� �����̸�
			else
			{
				//8.5.1 �����ϴ� ������ġ���� �����ϴ� ���� ������ ���ڱ��� �����.
				while (startLetterIndex < startRow->GetLength())
				{
					//8.5.1.1 �ٿ��� ���ڸ� �����.
					startRow->Remove(startLetterIndex);
				}
				//8.5.2 �����ϴ� ���� �����ٺ��� ������ �������� ���ڿ� ���� �����.
				nextRowIndex = startRowIndex + 1;
				while (nextRowIndex < endRowIndex)
				{
					//8.5.2.1 ���� ���Ѵ�.
					row = this->notepadForm->note->GetAt(nextRowIndex);
					//8.5.2.2 ������ġ�� ����ġ��Ų��.
					letterIndex = 0;
					//8.5.2.3 �ٿ��� ������ ���ڱ��� �ݺ��Ѵ�.
					while (letterIndex < row->GetLength())
					{
						//8.5.2.3.1 ���� ���ڸ� �����.
						row->Remove(letterIndex);
					}
					//8.5.2.4 ���� ���ڸ� �������⶧���� �޸��忡�� ���� �����.
					this->notepadForm->note->Remove(nextRowIndex);
					//8.5.2.5 ���� ������ ������ ������ ������ ���� ��ġ�� ��ĭ �մ������.
					endRowIndex--;
				}
				//8.5.3 ������ ���� ���Ѵ�.
				endRow = this->notepadForm->note->GetAt(endRowIndex);
				//8.5.4 ������ ���� ó������ ������ ���ڱ��� ���ڸ� �����.
				letterIndex = 0;
				while (letterIndex < endLetterIndex)
				{
					//8.5.4.1 ������ ���� ���ڸ� �����.
					endRow->Remove(letterIndex);
					//8.5.4.2 ������ ���� ù���ڸ� ����� ���� ���ں��� ������ ��ĭ��
					//������� ������ ������ ������ġ�� -1 ���ҽ�Ų��.
					endLetterIndex--;
				}
			}
			//8.6 ������ ���� �����ϴ� �ٷ� Join��Ų��.
			endRow->Join(startRow);
			//8.7 ������ ���� �����ϴ� �ٷ� Join�Ǿ��� ������
			//������ ���� �޸��忡�� �����.
			this->notepadForm->note->Remove(endRowIndex);
			//8.8 ���� ���� ��ġ�� �����ϴ� ���� ��ġ�� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->
				GetAt(startRowIndex);
			//8.9 ���� ������ ��ġ�� �����ϴ� ������ ��ġ�� �����Ѵ�.
			this->notepadForm->current->Move(startLetterIndex);
		}
		//9. ó�� �����̸�
		if (this->isRedone == false)
		{
			//9.1 notepadForm�� clipboard�� command�� ���� ������ Note(������ ����)�� �߰������ش�.
			Long noteIndex = this->notepadForm->clipboard->Add(this->glyph->Clone());
			//9.2 ������ texts�� ����� ������ �ٿ��ֱ� �޴��� Ȱ��ȭ�����ش�.
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
			//����Ŭ�����忡 ������ ������ �ܺ�Ŭ������� �ű��
			//9.3 notepadForm�� clipboard�� ���� ������ Note(������ ����)�� �� ���̸� ���Ѵ�.
			CString content = this->notepadForm->clipboard->
				GetAt(noteIndex)->GetContent().c_str();
			//������ ���ڿ��� ���̸� ���Ѵ�. ('\0'���� ������ ũ��)
			int contentLength = content.GetLength() + 1;
			//9.4 �ܺ� Ŭ�����尡 ��������
			//Ŭ������� ���� ���α׷����� ������ �� �ִ� ����̱� ������ � ���α׷�(������)�� Ŭ�����带
			//����ϴ��� �����ϰ� ����ϴ� ���� ����
			if (this->notepadForm->OpenClipboard() != 0)
			{
				//9.4.1 notepadForm�� clipboard�� ���� ������ Note(������ ����)�� Ŭ�����忡 �ű� ������ ����
				//Ŭ�����忡 ���ڿ��� �����ϱ� ���ؼ��� ���ڿ��� ������ �޸𸮸� Ŭ������� �����ؾ� �մϴ�.
				//���� ���ڿ��� ������ �޸𸮸� ���� ������ �ϴµ�, �� �޸� ������ �ڵ��� ����ؾ� �ϱ�
				//������ HeapAlloc, malloc ���� �� �Լ��� ����� �Ұ����մϴ�.�׷��� �Ʒ��� �ڵ� �� ������
				//����ϴ� GlobalAlloc �Լ��� ����ؼ� �޸𸮸� �Ҵ��ؾ� �ϰ�, Ŭ������� �ٸ� ���α׷���
				//�����ϴ� �����̱� ������ �� �޸� �Ӽ��� GMEM_DDESHARE�� GMEM_MOVEABLE�� �߰��Ѵ�.
				//(�� �޸𸮴� Ŭ������� ���޵Ǳ� ������ �ڽ��� ���α׷����� �����ϸ� �ȵȴ�)
				HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, contentLength);
				//�̷��� �Ҵ�� �޸𸮴� �ڵ� ���� ��ȯ�ϱ� ������ �� �޸𸮿� ���ڿ��� �����ϱ� ���ؼ��� ���
				//������ �ּҸ� ���� �Ѵ�. ���� GlobalLock �Լ��� ����Ͽ� �ڽ��� �� �޸𸮸� ����ϰڴٰ�
				//�����ϰ� ��� ������ �ּҸ� ���� �Ѵ�.�̶�, �����ؾ� �� ���� Ŭ�����忡�� ����ϴ�
				//���ڿ��� ASCII �������� �����Ǿ�� �ϱ� ������ �����ڵ带 ����(MFC ���α׷����� �⺻������
				//(�����ڵ� ������)�ϴ� ���α׷������� ASCII �������� ���ڿ��� �����ؼ� ����ؾ� �Ѵ�.
				char* pchData = (char*)GlobalLock(hClipboardData);
				if (pchData != NULL)
				{
					// �Ҵ�� �޸� ������ ������ ���ڿ��� �����Ѵ�.
					memcpy(pchData, content, contentLength);
					// ���ڿ��� �����ϱ� ���ؼ� Lock �ߴ� �޸𸮸� �����Ѵ�. 
					// Ŭ�����带 ����. 
					GlobalUnlock(hClipboardData);
					// Ŭ�����忡 ����� ���� ���ڿ��� �����Ѵ�. 
					//�ٸ� ���α׷��� Ŭ�����带 ����ϰų� �ٸ� ���ڿ��� Ŭ�����忡 ����Ǿ� ���� �� �ֱ�
					//������ EmptyClipboard �Լ��� ����ؼ� Ŭ�����忡 �����͸� �����ϱ� ���� �غ�
					EmptyClipboard();
					// Ŭ������� ���ڿ��� �����ϰ�, Ŭ�����忡 �����ϰ��� �ϴ� ������ ���ڿ����� �˷��ش�.
					SetClipboardData(CF_TEXT, hClipboardData);
					// Ŭ�����带 �ݴ´�.
					CloseClipboard();
				}
			}
		}
		//10. �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//11. �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->isDirty = true;
		//12. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//12.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
		//13. �޸��忡�� ���õ� texts�� �� ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = false;
		//14. ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartXPos = 0;
		//15. ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartYPos = 0;
		//16. �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		//17. ���ڸ� ���� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
		//18. �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� �����ؾ���)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			Long changedRowPos = this->rowIndex;
			Long changedLetterPos = this->letterIndex;
			Long originRowPos = 0;
			Long originLetterPos = 0;
			//18.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			//18.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
			this->rowIndex = originRowPos;
			this->letterIndex = originLetterPos;
		}
	}
}

//�������
void CutCommand::Unexecute()
{
	//�߶� content�� ������
	if (this->glyph != 0)
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
		//1. RowAutoChange�� �����Ѵ�.
		RowAutoChange rowAutoChange(this->notepadForm);
		Long changedRowPos = 0;
		Long changedLetterPos = 0;
		Long originRowPos = this->rowIndex;
		Long originLetterPos = this->letterIndex;
		//2. ���� ���� ��ġ�� �̵���Ų��.(ĳ���� �ٸ� ���� ������ �� ���� ���ڰ� �������� ������)
		Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//3. ���� ������ ��ġ�� �̵���Ų��.
		Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
		//4. �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� ����Ǿ� ����)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//4.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//4.2 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
			currentRowPos = this->notepadForm->note->Move(changedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
		}
		//5. �޸��忡�� ���õ� texts�� �ٽ� �����ϱ� ������ �޸��忡�� ������ �� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = true;
		//6. ������ �ٽ� �����߱� ������ ĳ���� x��ǥ�� command�� ����� ���� ���� ��ġ�� �����Ѵ�.
		this->notepadForm->selectedStartXPos = currentLetterPos;
		this->selectedStartXPos = this->notepadForm->selectedStartXPos;
		//7. ������ �ٽ� �����߱� ������ ĳ���� y��ǥ�� command�� ����� ���� ���� ��ġ�� �����Ѵ�.
		this->notepadForm->selectedStartYPos = currentRowPos;
		this->selectedStartYPos = this->notepadForm->selectedStartYPos;
		//8. �޸����� ���� ���� ���Ѵ�.
		Glyph* currentRow = this->notepadForm->current;
		//9. �޸����� ���� ������ġ�� ���� ���ڰ������� ������
		//�ڿ� split�� ���ڰ� ������ split�ϰ� ������ ���ڶ� �ڿ� ���ڰ� ������ split���� �ʴ´�.
		Glyph* splitedRow = 0;
		bool isSplited = false;
		if (currentLetterPos < currentRow->GetLength())
		{
			//9.1 �޸����� ���� �ٿ��� ���� ������ġ �������� split��Ų��.(splitedRow�� ���� �Ҵ��)
			splitedRow = currentRow->Split(currentLetterPos);
			//9.2 split�� �Ǿ��� ������ isSplited�� true�� �ٲ��ش�.
			isSplited = true;
		}
		//10. command�� ���� ��Ʈ�� ù��° ���� ���Ѵ�.
		Long firstCopyRowPos = 0;
		Glyph* firstCopyRow = this->glyph->GetAt(firstCopyRowPos);
		Glyph* letter = 0;//���ڸ� ���� ����
		//11. command�� ���� ��Ʈ�� ù��° ���� �������� �������� �ݺ��Ѵ�.
		Long letterIndex = 0;
		while (letterIndex < firstCopyRow->GetLength())
		{
			//11.1 command�� ���� ��Ʈ�� ù��° ���� ���ڸ� ���Ѵ�.
			letter = firstCopyRow->GetAt(letterIndex);
			//11.2 ���ڸ� �޸����� ���� �ٿ� �߰��Ѵ�.(���� ����)
			currentRow->Add(letter->Clone());
			//11.3 ���� ���ڷ� �̵��Ѵ�.
			letterIndex++;
		}
		//12. command�� ���� ��Ʈ�� ù��° �ٿ��� ���� �ٷ� �̵��Ѵ�.
		Long nextCopyRowPos = firstCopyRowPos + 1;
		Glyph* copyRow = firstCopyRow;//ù��° ���� ����Ʈ
		Long i = currentRowPos;
		//13. Ŭ�������� ���� ��Ʈ(������ ����)�� �������ٱ��� �ݺ��Ѵ�.
		while (nextCopyRowPos < this->glyph->GetLength())
		{
			//13.1 ������ ���� ���Ѵ�.
			copyRow = this->glyph->GetAt(nextCopyRowPos);
			//13.2 �޸����� ���� ���� ���� �ٷ� �̵��Ѵ�.
			i++;
			//13.3 ���� ���� �޸����� ���� ���� ��ġ �������� �����ִ´�.(���� ����)
			i = this->notepadForm->note->Add(i, copyRow->Clone());
			//13.4 ������ ��Ʈ���� ���� �ٷ� �̵��Ѵ�.
			nextCopyRowPos++;
		}
		//14. �޸��忡�� ���� ���� ���Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(i);
		//15. �޸��忡�� ���� ������ġ�� ���Ѵ�.
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//16. �޸��忡�� �Ʊ� split�ߴ� ���ڵ��� ������
		if (isSplited == true)
		{
			//16.1 split�� ���� �޸����� ���� �ٿ� Join��Ų��.
			splitedRow->Join(this->notepadForm->current);
			//16.2 splitedRow�� �Ҵ����� �����ش�.
			if (splitedRow != 0)
			{
				delete splitedRow;
			}
			//16.3 �޸��忡�� ���� ������ġ�� �ٽ� �������ش�.
			this->notepadForm->current->Move(currentLetterPos);
		}
		//17. �ڵ������� �������̸� �ٿ����� �ٵ��� �ڵ���������ش�.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//17.1 �ڵ�����Ŭ������ �����Ѵ�.
			RowAutoChange rowAutoChange(this->notepadForm);
			//17.2 �κ��ڵ������� �Ѵ�.
			Long endPastedRowPos = rowAutoChange.DoPartRows(currentRowPos, i);
			//17.3 �ٿ��ֱⰡ ������ �ٷ� �̵���Ų��.
			//�ٿ��ֱⰡ ������ ���� OnSize���� �κ��ڵ������� �ؼ� ó���Ǳ� ������ ĳ���� ��ġ�� �������ָ� ��!
			this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current->Move(currentLetterPos);
		}
		//12. ���ÿ����� �ٽ� ����� ������ �����ϱ�, �߶󳻱�, ���� �޴��� Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
		//13. ���ڰ� �ٽ� �߰��Ǿ��� ������ ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();

		//14. ���ڸ� �Է��� �Ŀ� command�� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
		//15. �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� �����ؾ���)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			Long changedRowPos = this->rowIndex;
			Long changedLetterPos = this->letterIndex;
			Long originRowPos = 0;
			Long originLetterPos = 0;
			//15.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			//15.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
			this->rowIndex = originRowPos;
			this->letterIndex = originLetterPos;
		}
	}
}

//�ٽý������� ���� ���ϱ�
bool CutCommand::IsRedone()
{
	return this->isRedone;
}

//���� ��ġ ���ϱ�
Long CutCommand::GetRowIndex()
{
	return this->rowIndex;
}
//���� ��ġ ���ϱ�
Long CutCommand::GetLetterIndex()
{
	return this->letterIndex;
}

//������ ���۵Ǵ� ������ ��ġ���ϱ�
Long CutCommand::GetSelectedStartXPos()
{
	return this->selectedStartXPos;
}

//������ ���۵Ǵ� ���� ��ġ���ϱ�
Long CutCommand::GetSelectedStartYPos()
{
	return this->selectedStartYPos;
}

//SetMacroEnd(������� �� �ٽý��� ��ũ����� �������� ����)
void CutCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void CutCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//������� ������������ ���ϱ�
bool CutCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool CutCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}

//SetRedone(�ٽ� �����̶�� ������)
void CutCommand::SetRedone()
{
	this->isRedone = true;
}

//���ÿ����� ���������� Ȯ�� ����
bool CutCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}

//�Ҹ���
CutCommand::~CutCommand()
{
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}