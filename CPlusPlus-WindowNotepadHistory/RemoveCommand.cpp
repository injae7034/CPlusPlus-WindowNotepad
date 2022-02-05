#include "RemoveCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"
#include "RowAutoChange.h"

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
			//4.1.4.1 DummyRow�� �����Ѵ�.
			this->glyph = new DummyRow();
			//4.1.4.2 �����ϴ� ������ġ���� ������ ���ڱ��� �����.
			while (startLetterIndex < endLetterIndex)
			{
				//4.1.4.2.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
				letter = this->notepadForm->current->GetAt(startLetterIndex);
				//4.1.4.2.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
				this->glyph->Add(letter->Clone());
				//4.1.4.2.3 �ٿ��� ���ڸ� �����.
				startRow->Remove(startLetterIndex);
				//4.1.4.2.4 �ٿ��� ���ڰ� �������� ���� ������ �ٰ� �����ϴ� ������ ���� ���ڰ�
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
			//5.4.1 DummyRow�� �����Ѵ�.
			this->glyph = new DummyRow();
			//5.4.2 �����ϴ� ������ġ���� �����ϴ� ���� ������ ���ڱ��� �����.
			while (startLetterIndex < startRow->GetLength())
			{
				//5.4.2.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
				letter = this->notepadForm->current->GetAt(startLetterIndex);
				//5.4.2.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
				this->glyph->Add(letter->Clone());
				//5.4.2.3 �ٿ��� ���ڸ� �����.
				startRow->Remove(startLetterIndex);
			}
			//5.4.3 �����ϴ� ���� �����ٺ��� ������ �������� ���ڿ� ���� �����.
			nextRowIndex = startRowIndex + 1;
			while (nextRowIndex < endRowIndex)
			{
				//5.4.3.1 ���� ���Ѵ�.
				row = this->notepadForm->note->GetAt(nextRowIndex);
				//5.4.3.2 ������ġ�� ����ġ��Ų��.
				letterIndex = 0;
				//5.4.3.3 �ٿ��� ������ ���ڱ��� �ݺ��Ѵ�.
				while (letterIndex < row->GetLength())
				{
					//5.4.3.3.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
					letter = this->notepadForm->current->GetAt(startLetterIndex);
					//5.4.3.3.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
					this->glyph->Add(letter->Clone());
					//5.4.3.3.3 ���� ���ڸ� �����.
					row->Remove(letterIndex);
				}
				//5.4.3.4 ���� ���ڸ� �������⶧���� �޸��忡�� ���� �����.
				this->notepadForm->note->Remove(nextRowIndex);
				//5.4.3.5 ���� ������ ������ ������ ������ ���� ��ġ�� ��ĭ �մ������.
				endRowIndex--;
			}
			//5.4.4 ������ ���� ���Ѵ�.
			endRow = this->notepadForm->note->GetAt(endRowIndex);
			//5.4.5 ������ ���� ó������ ������ ���ڱ��� ���ڸ� �����.
			letterIndex = 0;
			while (letterIndex < endLetterIndex)
			{
				//5.4.5.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
				letter = this->notepadForm->current->GetAt(startLetterIndex);
				//5.4.5.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
				this->glyph->Add(letter->Clone());
				//5.4.5.3 ������ ���� ���ڸ� �����.
				endRow->Remove(letterIndex);
				//5.4.5.4 ������ ���� ù���ڸ� ����� ���� ���ں��� ������ ��ĭ��
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
	//5. ���� ���� ���Ѵ�.
	Glyph* currentRow = this->notepadForm->current;
	//6. command�� contents�� �����Ѵ�.
	Glyph* dummyRow = this->glyph->Clone();
	//7. ���� �ٿ� dummyRow�� Join��Ų��.
	dummyRow->Join(currentRow);
	//8. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//8.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
		//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
		this->notepadForm->SendMessage(WM_SIZE);
	}

	//9. �޸��忡�� ���õ� texts�� �� ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
	this->notepadForm->isSelecting = true;
	//10. ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
	this->notepadForm->selectedStartXPos = 0;
	//11. ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
	this->notepadForm->selectedStartYPos = 0;
	//12. �����ϱ�, �߶󳻱�, ���� �޴��� Ȱ��ȭ ��Ų��.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	//13. ���ڸ� ���� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();

	//10. ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//11. �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� �����ؾ���)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//11.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//11.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
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
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}