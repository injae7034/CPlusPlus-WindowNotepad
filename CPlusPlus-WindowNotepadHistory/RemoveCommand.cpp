#include "RemoveCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"
#include "RowAutoChange.h"
#include "SelectingTexts.h"

//����Ʈ������
RemoveCommand::RemoveCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->glyph = 0;
	this->isRedone = false;
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->selectedStartXPos = 0;
	this->selectedStartYPos = 0;
}

//ó�� ���� �� �ٽ� ����
void RemoveCommand::Execute()
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
	//�ٽ� �����̸�
	if (this->IsRedone() == true)
	{
		this->notepadForm->selectedStartYPos = this->selectedStartYPos;
		this->notepadForm->selectedStartXPos = this->selectedStartXPos;
	}
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
	Glyph* letter = 0;//������ �ּҸ� ���� ����
	Long nextRowIndex = 0;//���� ���� �ּ���ġ
	Glyph* newRow = 0;//���� ���� �����Ǵ� ���� �ּҸ� ���� ����
	//4. ������ ���۵Ǵ� �ٰ� ������ ������ ���� ������
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
		//4.1.4 ó�� ������ �Ǹ�
		if (this->isRedone == false)
		{
			//4.1.4.1 Note�� �����Ѵ�.
			this->glyph = new Note();
			//4.1.4.2 ������ startRow�� ��¥ ���̸�
			if (!dynamic_cast<DummyRow*>(startRow))
			{
				//4.1.4.2.1 Row�� �����Ѵ�.
				newRow = new Row();
			}
			//4.1.4.3 ������ startRow�� ��¥ ���̸�
			else
			{
				//4.1.4.3.1 DummyRow�� �����Ѵ�.
				newRow = new DummyRow();
			}
			//4.1.4.4 ���� ������ Row�� command�� Note�� �߰��Ѵ�.
			this->glyph->Add(newRow);
			//4.1.4.5 �����ϴ� ������ġ���� ������ ���ڱ��� �����.
			while (startLetterIndex < endLetterIndex)
			{
				//4.1.4.5.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
				letter = startRow->GetAt(startLetterIndex);
				//4.1.4.5.2 ���ڸ� ���� �����ؼ� ���� ������ �ٿ� �����Ѵ�.
				newRow->Add(letter->Clone());
				//4.1.4.5.3 �ٿ��� ���ڸ� �����.
				startRow->Remove(startLetterIndex);
				//4.1.4.5.4 �ٿ��� ���ڰ� �������� ���� ������ �ٰ� �����ϴ� ������ ���� ���ڰ�
				//������ �����ϴ� ������ ��ġ�� �մ���� ���� �ǹǷ� ������ ������ ���� ���� ���ҽ�Ų��. 
				endLetterIndex--;
			}
		}
		//4.1.5 �ٽ� �����̸�
		else
		{
			//4.1.5.1 �����ϴ� ������ġ���� ������ ���ڱ��� �����.
			while (startLetterIndex < endLetterIndex)
			{
				//4.1.5.1.1 �ٿ��� ���ڸ� �����.
				startRow->Remove(startLetterIndex);
				//4.1.5.1.2 �ٿ��� ���ڰ� �������� ���� ������ �ٰ� �����ϴ� ������ ���� ���ڰ�
				//������ �����ϴ� ������ ��ġ�� �մ���� ���� �ǹǷ� ������ ������ ���� ���� ���ҽ�Ų��. 
				endLetterIndex--;
			}
		}
	}
	//5. ������ ���۵Ǵ� �ٰ� ������ ������ ���� ���� �ٸ���
	//(������ ���� �ٿ� ���ļ� �Ǿ� �ֱ� ������ ���ڰ� �� ������ ���� ����������)
	else
	{
		//5.1 ������ ���������� ����Ǿ����� 
		if (selectedStartRowPos < selectedEndRowPos)
		{
			//5.1.1 �����ϴ� ������ġ�� ������ ���۵Ǵ� ������ġ�� ���Ѵ�.
			startLetterIndex = selectedStartLetterPos;
			//5.1.2 ������ ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			endLetterIndex = selectedEndLetterPos;
			//5.1.3 �����ϴ� ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedStartRowPos;
			//5.1.4 ������ ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			endRowIndex = selectedEndRowPos;
		}
		//5.2 ������ �������� ����Ǿ�����
		else
		{
			//5.2.1 �����ϴ� ������ġ�� ������ ������ ������ġ�� ���Ѵ�.
			startLetterIndex = selectedEndLetterPos;
			//5.2.2 ������ ������ġ�� ������ �����ϴ� ������ġ�� ���Ѵ�.
			endLetterIndex = selectedStartLetterPos;
			//5.2.3 �����ϴ� ���� ��ġ�� ������ ������ ���� ��ġ�� ���Ѵ�.
			startRowIndex = selectedEndRowPos;
			//5.2.4 ������ ���� ��ġ�� ������ �����ϴ� ���� ��ġ�� ���Ѵ�.
			endRowIndex = selectedStartRowPos;
		}
		Glyph* endRow = 0;//������ ���� ��ġ
		Glyph* row = 0;//���� �ּҸ� ���� ����
		Long letterIndex = 0;//���� ��ġ
		//5.3 �����ϴ� ���� ���Ѵ�.
		startRow = this->notepadForm->note->GetAt(startRowIndex);
		//5.4 ó�� �����̸�
		if (this->isRedone == false)
		{
			//5.4.1 Note�� �����Ѵ�.
			this->glyph = new Note();
			//5.4.2 ������ startRow�� ��¥ ���̸�
			if (!dynamic_cast<DummyRow*>(startRow))
			{
				//5.4.2.1 Row�� �����Ѵ�.
				newRow = new Row();
			}
			//5.4.3 ������ startRow�� ��¥ ���̸�
			else
			{
				//5.4.3.1 DummyRow�� �����Ѵ�.
				newRow = new DummyRow();
			}
			//5.4.4 ���� ������ Row�� command�� Note�� �߰��Ѵ�.
			this->glyph->Add(newRow);
			//5.4.5 �����ϴ� ������ġ���� �����ϴ� ���� ������ ���ڱ��� �����.
			while (startLetterIndex < startRow->GetLength())
			{
				//5.4.5.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
				letter = startRow->GetAt(startLetterIndex);
				//5.4.5.2 ���ڸ� ���� �����ؼ� ���� ������ �ٿ� �����Ѵ�.
				newRow->Add(letter->Clone());
				//5.4.5.3 �ٿ��� ���ڸ� �����.
				startRow->Remove(startLetterIndex);
			}
			//5.4.6 �����ϴ� ���� �����ٺ��� ������ �������� ���ڿ� ���� �����.
			nextRowIndex = startRowIndex + 1;
			while (nextRowIndex < endRowIndex)
			{
				//5.4.6.1 ���� ���Ѵ�.
				row = this->notepadForm->note->GetAt(nextRowIndex);
				//5.4.6.2 ������ ���� ��¥ ���̸�
				if (!dynamic_cast<DummyRow*>(row))
				{
					//5.4.6.2.1 Row�� �����Ѵ�.
					newRow = new Row();
				}
				//5.4.6.3 ������ ���� ��¥ ���̸�
				else
				{
					//5.4.6.3.1 DummyRow�� �����Ѵ�.
					newRow = new DummyRow();
				}
				//5.4.6.4 ���� ������ Row�� command�� Note�� �߰��Ѵ�.
				this->glyph->Add(newRow);
				//5.4.6.5 ������ġ�� ����ġ��Ų��.
				letterIndex = 0;
				//5.4.6.6 �ٿ��� ������ ���ڱ��� �ݺ��Ѵ�.
				while (letterIndex < row->GetLength())
				{
					//5.4.6.6.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
					letter = row->GetAt(letterIndex);
					//5.4.6.6.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
					newRow->Add(letter->Clone());
					//5.4.6.6.3 ���� ���ڸ� �����.
					row->Remove(letterIndex);
				}
				//5.4.6.7 ���� ���ڸ� �������⶧���� �޸��忡�� ���� �����.
				this->notepadForm->note->Remove(nextRowIndex);
				//5.4.6.8 ���� ������ ������ ������ ������ ���� ��ġ�� ��ĭ �մ������.
				endRowIndex--;
			}
			//5.4.7 ������ ���� ���Ѵ�.
			endRow = this->notepadForm->note->GetAt(endRowIndex);
			//5.4.8 ������ ���� ��¥ ���̸�
			if (!dynamic_cast<DummyRow*>(endRow))
			{
				//5.4.8.1 Row�� �����Ѵ�.
				newRow = new Row();
			}
			//5.4.9 ������ ���� ��¥ ���̸�
			else
			{
				//5.4.9.1 DummyRow�� �����Ѵ�.
				newRow = new DummyRow();
			}
			//5.4.10 ���� ������ Row�� command�� Note�� �߰��Ѵ�.
			this->glyph->Add(newRow);
			//5.4.11 ������ ���� ó������ ������ ���ڱ��� ���ڸ� �����.
			letterIndex = 0;
			while (letterIndex < endLetterIndex)
			{
				//5.4.11.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
				letter = endRow->GetAt(letterIndex);
				//5.4.11.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
				newRow->Add(letter->Clone());
				//5.4.11.3 ������ ���� ���ڸ� �����.
				endRow->Remove(letterIndex);
				//5.4.11.4 ������ ���� ù���ڸ� ����� ���� ���ں��� ������ ��ĭ��
				//������� ������ ������ ������ġ�� -1 ���ҽ�Ų��.
				endLetterIndex--;
			}
		}
		//5.5 �ٽ� �����̸�
		else
		{
			//5.5.1 �����ϴ� ������ġ���� �����ϴ� ���� ������ ���ڱ��� �����.
			while (startLetterIndex < startRow->GetLength())
			{
				//5.5.1.1 �ٿ��� ���ڸ� �����.
				startRow->Remove(startLetterIndex);
			}
			//5.5.2 �����ϴ� ���� �����ٺ��� ������ �������� ���ڿ� ���� �����.
			nextRowIndex = startRowIndex + 1;
			while (nextRowIndex < endRowIndex)
			{
				//5.5.2.1 ���� ���Ѵ�.
				row = this->notepadForm->note->GetAt(nextRowIndex);
				//5.5.2.2 ������ġ�� ����ġ��Ų��.
				letterIndex = 0;
				//5.5.2.3 �ٿ��� ������ ���ڱ��� �ݺ��Ѵ�.
				while (letterIndex < row->GetLength())
				{
					//5.5.2.3.1 ���� ���ڸ� �����.
					row->Remove(letterIndex);
				}
				//5.5.2.4 ���� ���ڸ� �������⶧���� �޸��忡�� ���� �����.
				this->notepadForm->note->Remove(nextRowIndex);
				//5.5.2.5 ���� ������ ������ ������ ������ ���� ��ġ�� ��ĭ �մ������.
				endRowIndex--;
			}
			//5.5.3 ������ ���� ���Ѵ�.
			endRow = this->notepadForm->note->GetAt(endRowIndex);
			//5.5.4 ������ ���� ó������ ������ ���ڱ��� ���ڸ� �����.
			letterIndex = 0;
			while (letterIndex < endLetterIndex)
			{
				//5.5.4.1 ������ ���� ���ڸ� �����.
				endRow->Remove(letterIndex);
				//5.5.4.2 ������ ���� ù���ڸ� ����� ���� ���ں��� ������ ��ĭ��
				//������� ������ ������ ������ġ�� -1 ���ҽ�Ų��.
				endLetterIndex--;
			}
		}
		//5.6 ������ ���� �����ϴ� �ٷ� Join��Ų��.
		endRow->Join(startRow);
		//5.7 ������ ���� �����ϴ� �ٷ� Join�Ǿ��� ������
		//������ ���� �޸��忡�� �����.
		this->notepadForm->note->Remove(endRowIndex);
		//5.8 ���� ���� ��ġ�� �����ϴ� ���� ��ġ�� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->
			GetAt(startRowIndex);
		//5.9 ���� ������ ��ġ�� �����ϴ� ������ ��ġ�� �����Ѵ�.
		this->notepadForm->current->Move(startLetterIndex);
	}
	//6. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//7. �޸��忡 ��������� ������ �����Ѵ�.
	this->notepadForm->isDirty = true;
	//8. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//8.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
		//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
		this->notepadForm->SendMessage(WM_SIZE);
	}
	//9. �޸��忡�� ���õ� texts�� �� ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
	this->notepadForm->isSelecting = false;
	//10. ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
	this->notepadForm->selectedStartXPos = 0;
	//11. ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
	this->notepadForm->selectedStartYPos = 0;
	//12. �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	//13. ���ڸ� ���� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//14. �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� �����ؾ���)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//14.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//14.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
	}
}

//�������
void RemoveCommand::Unexecute()
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

#if 0
	//Join�� ��Ű�� ���ÿ����� ������ ���ڰ� ������ ���� ���������� �߰��Ǳ� ������ DummyRow�� �����ؼ�
	//Join�� ��Ű�� �ȵȴ�. �츮�� ���ϴ� ����� ���ڵ�� ���ڵ���̿� ���ڵ��� ������ �Ǿ�� �ϱ� ������
	//letter������ �ݺ������� ������ ������ ��������Ѵ�.
	//9. command�� ������ �ִ� contents�� command�� ������ �ִ� ���� ���� ���ڵ� ��ġ�� ���Խ�Ų��.
	Long i = this->glyph->GetLength() - 1;
	Glyph* letter = 0;
	Long distanceMove = 0;
	while (i >= 0)
	{
		letter = this->glyph->GetAt(i);
		currentLetterPos = currentRow->Add(currentLetterPos, letter->Clone());
		distanceMove++;
		i--;
	}
	//10. �޸��忡 ���ڰ� �߰��Ǿ��� ������ ������ġ�� �ٽ� �������ش�.
	currentLetterPos += distanceMove;
	currentLetterPos = this->notepadForm->current->Move(currentLetterPos);
#endif
#if 0
	//6. command�� contents�� �����Ѵ�.
	Glyph* dummyRow = this->glyph->Clone();
	//7. ���� �ٿ� dummyRow�� Join��Ų��.
	dummyRow->Join(currentRow);
#endif

	//11. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
	//if (this->notepadForm->isRowAutoChanging == true)
	//{
		//11.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
		//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
	//	this->notepadForm->SendMessage(WM_SIZE);
	//}

	
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

//�ٽý������� ���� ���ϱ�
bool RemoveCommand::IsRedone()
{
	return this->isRedone;
}

//���� ��ġ ���ϱ�
Long RemoveCommand::GetRowIndex()
{
	return this->rowIndex;
}
//���� ��ġ ���ϱ�
Long RemoveCommand::GetLetterIndex()
{
	return this->letterIndex;
}

//������ ���۵Ǵ� ������ ��ġ���ϱ�
Long RemoveCommand::GetSelectedStartXPos()
{
	return this->selectedStartXPos;
}

//������ ���۵Ǵ� ���� ��ġ���ϱ�
Long RemoveCommand::GetSelectedStartYPos()
{
	return this->selectedStartYPos;
}

//SetMacroEnd(������� �� �ٽý��� ��ũ����� �������� ����)
void RemoveCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void RemoveCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//������� ������������ ���ϱ�
bool RemoveCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool RemoveCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}


//SetRedone(�ٽ� �����̶�� ������)
void RemoveCommand::SetRedone()
{
	this->isRedone = true;
}

//�Ҹ���
RemoveCommand::~RemoveCommand()
{
	//������ Note�� �Ҵ������Ѵ�.
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}