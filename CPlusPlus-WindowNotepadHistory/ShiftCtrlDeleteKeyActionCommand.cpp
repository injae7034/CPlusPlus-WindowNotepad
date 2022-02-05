#include "ShiftCtrlDeleteKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "DummyRow.h"
#include "RowAutoChange.h"

//����Ʈ������ ����
ShiftCtrlDeleteKeyActionCommand::ShiftCtrlDeleteKeyActionCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->glyph = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isDirty = false;//ó���� ������ ���� ��������� �����Ƿ� false�� ����Ʈ����.
}

//����
void ShiftCtrlDeleteKeyActionCommand::Execute()
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
	//3. OnCharCommand�� �ٽ� ����Ǹ�
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
	//4. �޸��忡�� ���õ� texts�� ������
	if (this->notepadForm->isSelecting == false)
	{
		//4.1 ���� ��Ʈ�� ������ ���� ��ġ�� ���Ѵ�.
		Long lastRowPos = this->notepadForm->note->GetLength() - 1;
		//4.2 ���� �ٿ��� ������ ������ ��ġ�� ���Ѵ�.
		Long lastLetterPos = this->notepadForm->current->GetLength();
		//4.3 ���� ���� ���Ѵ�.
		Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
		//4.4 ���� ���� ���� ���� ���Ѵ�.
		Glyph* nextRow = this->notepadForm->note->GetAt(currentRowPos + 1);
		//4.5 ���� ���� ��ġ�� ��Ʈ�� ������ ����ġ�̰�, ���� ���� ��ġ�� ������ �����̸�
		if (currentRowPos == lastRowPos && currentLetterPos == lastLetterPos)
		{
			//4.5.1 ó�� �����̸�
			if (this->isRedone == false)
			{
				//4.5.1.1 Command�� ���� ������ ������ ǥ���Ѵ�.
				this->isDirty = false;
			}
		}
		// ���� ���� ��ġ�� �������� �ƴϰ�, ���� ���� ��ġ�� �������̸� ���� ���� ���� �ٷ� ���Խ�Ų��.
		//4.6 ���� ���� ��ġ�� ��Ʈ�� ������ �� ��ġ���� �۰�, ���� ���� ��ġ�� �������̰�,
		//���� ���� ���� ���� ��¥ ���̸�
		else if (currentRowPos < lastRowPos && currentLetterPos == lastLetterPos
			&& !dynamic_cast<DummyRow*>(nextRow))
		{
			//4.6.1 ó�� ������ �Ǹ�
			if (this->isRedone == false)
			{
				//4.6.1.1 Row�� �����Ѵ�.
				this->glyph = new Row();
			}
			//4.6.2 ���� ���� ���� �ٿ� ��ģ��.
			nextRow->Join(currentRow);
			//4.6.3 Note���� ���� ���� �ּҸ� �����.
			this->notepadForm->note->Remove(currentRowPos + 1);
			//4.6.4 ���� ���� ���� ��ġ�� ������ �������̱� ������ lastLetterPos�� �Ű��ش�.
			currentLetterPos = this->notepadForm->current->Move(lastLetterPos);
			//4.6.5 Command�� ���� ������ ������ ǥ���Ѵ�.
			this->isDirty = true;
		}
		//4.7 �� �̿ܿ���
		else
		{
			Glyph* letter = 0;
			Long nextRowPos = currentRowPos + 1;
			Long nextRowLetterPos = 0;
			//4.7.1 ó�� �����̸�
			if (this->isRedone == false)
			{
				//4.7.1.1 DummyRow�� �����Ѵ�.
				this->glyph = new DummyRow();
				//4.7.1.2 Command�� ���� ������ ������ ǥ���Ѵ�.
				this->isDirty = true;
				//���� ���� ���ڵ��� �����.
				//4.7.1.3 ���������ġ�� ������ ������ġ���� �������� �ݺ��Ѵ�.
				while (currentLetterPos < lastLetterPos)
				{
					//4.7.1.3.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
					letter = currentRow->GetAt(currentLetterPos);
					//4.7.1.3.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
					this->glyph->Add(letter->Clone());
					//4.7.1.3.3 ���ڸ� �����.
					currentRow->Remove(currentLetterPos);
					lastLetterPos--;
				}
				//4.7.1.4 ���� ���� ��¥ ���ε��� �ݺ��Ѵ�. 
				while (dynamic_cast<DummyRow*>(nextRow))
				{
					//4.7.1.4.1 ���� ���� ������ ������ġ�� ���Ѵ�.
					lastLetterPos = nextRow->GetLength();
					//4.7.1.4.2 ���� ���� ������ġ�� �ʱ�ȭ�Ѵ�.
					nextRowLetterPos = 0;
					//4.7.1.4.3 ���� ���� ������ġ�� ������ ������ġ���� �������� �ݺ��Ѵ�.
					while (nextRowLetterPos < lastLetterPos)
					{
						//4.7.1.4.3.1.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
						letter = nextRow->GetAt(nextRowLetterPos);
						//4.7.1.4.3.1.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
						this->glyph->Add(letter->Clone());
						//4.7.1.4.3.1.3 ���ڸ� �����.
						nextRow->Remove(nextRowLetterPos);
						lastLetterPos--;
					}
					//4.7.1.4.4 ���ڰ� ���������� ������ Note���� ���� ���� �ּҸ� �����.
					this->notepadForm->note->Remove(nextRowPos);
					//4.7.1.4.5 ���� ���� �ּҸ� �����Ѵ�.
					nextRow = this->notepadForm->note->GetAt(nextRowPos);
				}
			}
			//4.7.2 �ٽ� �����̸�
			else
			{
				//���� ���� ���ڵ��� �����.
				//4.7.2.1 ���������ġ�� ������ ������ġ���� �������� �ݺ��Ѵ�.
				while (currentLetterPos < lastLetterPos)
				{
					//4.7.2.1.1 ���ڸ� �����.
					currentRow->Remove(currentLetterPos);
					lastLetterPos--;
				}
				//4.7.2.2 ���� ���� ��¥ ���ε��� �ݺ��Ѵ�. 
				while (dynamic_cast<DummyRow*>(nextRow))
				{
					//4.7.2.2.1 ���� ���� ������ ������ġ�� ���Ѵ�.
					lastLetterPos = nextRow->GetLength();
					//4.7.2.2.2 ���� ���� ������ġ�� �ʱ�ȭ�Ѵ�.
					nextRowLetterPos = 0;
					//4.7.2.2.3 ���� ���� ������ġ�� ������ ������ġ���� �������� �ݺ��Ѵ�.
					while (nextRowLetterPos < lastLetterPos)
					{
						//4.7.2.2.3.1 ���ڸ� �����.
						nextRow->Remove(nextRowLetterPos);
						lastLetterPos--;
					}
					//4.7.2.2.4 ���ڰ� ���������� ������ Note���� ���� ���� �ּҸ� �����.
					this->notepadForm->note->Remove(nextRowPos);
					//4.7.2.2.5 ���� ���� �ּҸ� �����Ѵ�.
					nextRow = this->notepadForm->note->GetAt(nextRowPos);
				}
			}
			//4.7.3 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//4.7.3.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
			//4.7.4 ĳ���� ��ġ�� �ٽ� �������ش�.
			//currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
			//this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//currentLetterPos = this->notepadForm->current->Last();
		}	
	}
	//5. �޸��忡�� ���õ� texts�� ������
	else
	{
		//5.1 RemoveCommand�� �޼����� ������ ���ÿ����� �����.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
	//6. Command�� ���� ������ ������
	if (this->isDirty == true)
	{
		//6.1 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//6.2 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->isDirty = true;
		//6.3 ���ڸ� ���� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
		//6.4 �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� �����ؾ���)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			Long changedRowPos = this->rowIndex;
			Long changedLetterPos = this->letterIndex;
			Long originRowPos = 0;
			Long originLetterPos = 0;
			//6.4.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			//6.4.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
			this->rowIndex = originRowPos;
			this->letterIndex = originLetterPos;
		}
	}
}

//�������
void ShiftCtrlDeleteKeyActionCommand::Unexecute()
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
	//5. ���� �� ������ glyph�� ��(���๮��)�� �ƴϸ�
	if (dynamic_cast<DummyRow*>(this->glyph))
	{
		//5.1 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� ������
		if (currentLetterPos == this->notepadForm->current->GetLength())
		{
			//5.1.1 ���� ���� ���Ѵ�.
			Glyph* currentRow = this->notepadForm->current;
			//5.1.2 dummyRow�� ���� �����Ѵ�.
			Glyph* dummyRow = this->glyph->Clone();
			//5.1.3 dummyRow�� ���� �ٿ� ��ģ��.
			dummyRow->Join(currentRow);
			//5.1.4 dummyRow�� �Ҵ������Ѵ�.
			if (dummyRow != 0)
			{
				delete dummyRow;
			}
			//5.1.5 ���� ��ġ�� ���� �ٿ��� ���� ������ �̵���Ų��.
			currentRow->Move(currentLetterPos);
		}
		//5.2 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� �ٸ���
		else
		{
			//5.2.1 dummyRow�� ������ŭ �ݺ��Ѵ�.
			Glyph* letter = 0;
			Long i = this->glyph->GetLength() - 1;//������ �迭��Һ��� ������ ������ ����� �ȴ�.
			Long distance = currentLetterPos;
			while (i >= 0)
			{
				//5.2.1.1 ���ڸ� ���Ѵ�.
				letter = this->glyph->GetAt(i);
				//5.2.1.2 ���� ���� ���� ��ġ�� dummyRow���� ���� ���縦 �� letter�� ���� �ִ´�.
				currentLetterPos = this->notepadForm->current->
					Add(currentLetterPos, letter->Clone());
				//5.2.1.3 �̵��� ��ġ�� ������Ų��.
				distance++;
				//5.2.1.3 i�� ������Ų��.
				i--;
			}
			//5.2.2 ���� ��ġ�� �������Ѵ�.
			this->notepadForm->current->Move(distance);
		}
		//OnSize�� ���� �ڵ����� ���� �ڵ������� �ǰ� ���� ��ġ�� ������ġ�� �ڵ����࿡ �°� �ٽ� �����ȴ�.
	}
	//6. ���� �� ������ glyph�� ��(���๮��)�̸�
	else
	{
		//6.1 ���� �ٿ��� ���� ���� ���� ��ġ�� �ִ� ���ڵ��� ��� ���ο� ���� �����.
		Glyph* row = this->notepadForm->current->Split(currentLetterPos);
		//6.2 ���� ���� ��ġ�� ��Ʈ�� ���� ����-1 �� ����(���� ���� ��ġ�� ������ ���̸�)
		if (currentRowPos == this->notepadForm->note->GetLength() - 1)
		{
			//6.2.1 ���ο� ���� ������ �� ������ �߰��Ѵ�.
			currentRowPos = this->notepadForm->note->Add(row);
		}
		//6.3 �װ� �ƴϸ�
		else
		{
			//6.3.1 ���ο� ���� ���� ���� ���� ��ġ�� ���� �ִ´�.
			currentRowPos = this->notepadForm->note->
				Add(currentRowPos + 1, row);
		}
		//6.4 �ڵ� �� �ٲ��� �������̸�
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//���� ���� �ڵ������� �ȵ��� ������ ���������� ���� �߰��� ���� �ٷ� �̵��ؼ� �ڵ������� ��Ų��
			//6.4.1 ���� ���� ���� �����Ѵ�.
			currentRowPos = this->notepadForm->note->Move(currentRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//6.4.2 ���� ���� ���� ��ġ�� ó������ �̵���Ų��.
			this->notepadForm->current->First();
			//6.4.3 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
		//6.5 �ڵ����� �Ŀ� ���������� �ٲ� ĳ���� ��ġ�� �ٽ� ���� ��ġ�� �̵���Ų��.
		currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Last();
	}
	//��������� ������
	//7. isComposing�� false�� �ٲ۴�.
	this->notepadForm->isComposing = false;
	//8. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//9. �޸��忡 ��������� ������ �����Ѵ�.
	this->notepadForm->isDirty = true;
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

//SetMacroEnd(������� �� �ٽý��� ��ũ����� �������� ����)
void ShiftCtrlDeleteKeyActionCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void ShiftCtrlDeleteKeyActionCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}


//SetRedone(�ٽ� �����̶�� ������)
void ShiftCtrlDeleteKeyActionCommand::SetRedone()
{
	this->isRedone = true;
}

//���� ��ġ ���ϱ�
Long ShiftCtrlDeleteKeyActionCommand::GetRowIndex()
{
	return this->rowIndex;
}
//���� ��ġ ���ϱ�
Long ShiftCtrlDeleteKeyActionCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//������� ������������ ���ϱ�
bool ShiftCtrlDeleteKeyActionCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool ShiftCtrlDeleteKeyActionCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//�ٽý������� ���� ���ϱ�
bool ShiftCtrlDeleteKeyActionCommand::IsRedone()
{
	return this->isRedone;
}

//��������� �ִ��� Ȯ�� ����
bool ShiftCtrlDeleteKeyActionCommand::IsDirty()
{
	return this->isDirty;
}

//�Ҹ��� ����
ShiftCtrlDeleteKeyActionCommand::~ShiftCtrlDeleteKeyActionCommand()
{
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}