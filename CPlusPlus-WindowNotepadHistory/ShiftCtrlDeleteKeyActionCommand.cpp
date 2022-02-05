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
		Glyph* row = 0;
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
			}
			//4.7.3 ���� ���� ��¥���̸�
			if (dynamic_cast<DummyRow*>(nextRow))
			{
				//4.7.3.1 ���� ���� ������ ������ġ�� ���Ѵ�.
				lastLetterPos = nextRow->GetLength();
				//4.7.3.2 ó�� �����̸�
				if (this->isRedone == false)
				{
					//4.7.3.2.1 ���� ������ġ�� ������ ������ġ���� �������� 
					//�׸��� �������� ��¥���ε��� �ݺ��Ѵ�
					while (nextRowLetterPos < lastLetterPos
						&& dynamic_cast<DummyRow*>(nextRow))
					{
						//4.7.3.2.1.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
						letter = nextRow->GetAt(nextRowLetterPos);
						//4.7.3.2.1.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
						this->glyph->Add(0, letter->Clone());
						//4.7.3.2.1.3 ���ڸ� �����.
						currentRow->Remove(nextRowLetterPos);
						lastLetterPos--;
					}
				}
				//4.7.3.3 �ٽ� �����̸�
				else
				{
					//4.7.3.3.1 ���� ������ġ�� ������ ������ġ���� �������� 
					//�׸��� �������� ��¥���ε��� �ݺ��Ѵ�
					while (nextRowLetterPos < lastLetterPos
						&& dynamic_cast<DummyRow*>(nextRow))
					{
						//4.7.3.3.1.1 ���ڸ� �����.
						currentRow->Remove(nextRowLetterPos);
						lastLetterPos--;
					}
				}
			}


			//4.6.2 ���� ���� ��¥���̸�
			if (dynamic_cast<DummyRow*>(nextRow))
			{
				//4.6.2.1 ó�� �����̸�
				if (this->isRedone == false)
				{
					//4.6.2.1.1 ���������ġ�� ������ ������ġ���� �������� 
					//�׸��� �������� ��¥���ε��� �ݺ��Ѵ�
					while (currentLetterPos < lastLetterPos 
						&& dynamic_cast<DummyRow*>(nextRow))
					{
						//4.2.3.2.1.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
						letter = nextRow->GetAt(currentLetterPos - 1);
						//4.2.3.2.1.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
						this->glyph->Add(0, letter->Clone());
						//4.2.3.2.1.3 ���ڸ� �����.
						currentRow->Remove(currentLetterPos - 1);
						currentLetterPos--;
					}
				}
				//4.2.3.2.2.2 �ٽ� �����̸�
				else
				{
					//4.2.3.2.2.2.1 ���� ������ġ�� 0���� �������� �ݺ��Ѵ�.
					while (currentLetterPos > 0 && dynamic_cast<DummyRow*>(currentRow))
					{
						//4.2.3.2.2.2.1.1 ���ڸ� �����.
						currentRow->Remove(currentLetterPos - 1);
						currentLetterPos--;
					}
				}
				//4.2.3.2.2.3 ���� �ּҸ� �����Ѵ�.
				row = currentRow;
				//4.2.3.2.2.4 ���� ���� ������ ������ġ�� �̵��Ѵ�.
				currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Last();
				currentRow = this->notepadForm->current;
			}



			Long i = 0;
			
			//4.4.1 ó�� ������ �Ǹ�
			if (this->isRedone == false)
			{
				//DummyRow�� �����ؼ� ���ڸ� ��� �װ� �����ؼ� this->glyph�� �ּҸ� �ű��,
				//���ڵ��� �ܾ������ ��������� �Űܼ� �����Ѵ�.
				//4.4.1.1 DummyRow�� �����Ѵ�.
				this->glyph = new DummyRow();
				//4.4.1.2 ���������ġ�� ������ ������ġ���� �������� �ݺ��Ѵ�.
				while (currentLetterPos < lastLetterPos)
				{
					//4.4.1.2.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
					letter = this->notepadForm->current->GetAt(currentLetterPos);
					//4.4.1.2.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
					this->glyph->Add(letter->Clone());
					//4.4.1.2.3 ���ڸ� �����.
					this->notepadForm->current->Remove(currentLetterPos);
					lastLetterPos--;
				}
			}
			//4.4.2 ó�� ������ �ƴϸ�
			else
			{
				//4.2.2.1 command�� ����Ǿ� �ִ� ���� ���ڰ����� ���Ѵ�.
				Long lengthOfCommandRow = this->glyph->GetLength();
				//4.2.2.2 currentLetterPos�� lengthOfCommandRow���� ũ�ų� ������
				if (currentLetterPos >= lengthOfCommandRow)
				{
					//4.2.2.2.1 i�� ���� ������ġ���� �������� �ݺ��Ѵ�.
					while (i < currentLetterPos)
					{
						//4.2.2.2.1.1 ���ڸ� �����.
						this->notepadForm->current->Remove(i);
						currentLetterPos--;
					}
				}
				//4.2.2.3 currentLetterPos�� lengthOfCommandRow���� ������
				else
				{
					//4.2.2.3.1 lengthOfCommandRow�� 0���� ū���� �ݺ��Ѵ�.
					while (lengthOfCommandRow > 0)
					{
						//4.2.2.3.1.1 i�� ���� ������ġ���� �������� �׸��� lengthOfCommandRow 
						//�� 0���� ū���� �ݺ��Ѵ�.
						i = 0;
						while (i < currentLetterPos && lengthOfCommandRow > 0)
						{
							//4.2.2.3.1.1.1 ���ڸ� �����.
							this->notepadForm->current->Remove(i);
							currentLetterPos--;
							lengthOfCommandRow--;
						}
						//4.2.2.3.1.2 lengthOfCommandRow�� 0���� ũ��
						if (lengthOfCommandRow > 0)
						{
							//4.2.2.3.1.2.1 ���� ���� ���� �ٷ� �̵��Ѵ�.
							currentRowPos = this->notepadForm->note->Previous();
							this->notepadForm->current = this->notepadForm->
								note->GetAt(currentRowPos);
							//4.2.2.3.1.2.2 ���� ���̸� currentLetterPos�� �����Ѵ�.
							currentLetterPos = this->notepadForm->current->GetLength();
						}
					}
				}
			}
			
			//3.4.4 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//3.4.4.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
			//3.4.5 Command�� ���� ������ ������ ǥ���Ѵ�.
			this->isDirty = true;
		}
		//3.5 Command�� ���� ������ ������
		if (this->isDirty == true)
		{
			//3.3.1 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.3.2 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->isDirty = true;
			//3.3.3 ���ڸ� ���� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
			this->rowIndex = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
			this->letterIndex = this->notepadForm->current->GetCurrent();
		}
	}
	//3. �޸��忡�� ���õ� texts�� ������
	else
	{
		//4.1 RemoveCommand�� �޼����� ������ ���ÿ����� �����.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
}

//�������
void ShiftCtrlDeleteKeyActionCommand::Unexecute()
{
	//1. ���� ���� ��ġ�� �̵���Ų��.(ĳ���� �ٸ� ���� ������ �� ���� ���ڰ� �������� ������)
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	//2. ���� ������ ��ġ�� �̵���Ų��.
	Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	//3. ���� �� ������ glyph�� ��(���๮��)�� �ƴϸ�
	if (dynamic_cast<DummyRow*>(this->glyph))
	{
		//3.1 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� ������
		if (currentLetterPos == this->notepadForm->current->GetLength())
		{
			//3.1.1 ���� ���� ���Ѵ�.
			Glyph* currentRow = this->notepadForm->current;
			//3.1.2 dummyRow�� ���� �����Ѵ�.
			Glyph* dummyRow = this->glyph->Clone();
			//3.1.3 dummyRow�� ���� �ٿ� ��ģ��.
			dummyRow->Join(currentRow);
			//3.1.4 dummyRow�� �Ҵ������Ѵ�.
			if (dummyRow != 0)
			{
				delete dummyRow;
			}
			//3.1.5 ���� ��ġ�� ���� �ٿ��� ���� ������ �̵���Ų��.
			currentRow->Move(currentLetterPos);
		}
		//3.2 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� �ٸ���
		else
		{
			//3.2.1 dummyRow�� ������ŭ �ݺ��Ѵ�.
			Glyph* letter = 0;
			Long i = this->glyph->GetLength() - 1;
			while (i >= 0)
			{
				//3.2.1.1 ���ڸ� ���Ѵ�.
				letter = this->glyph->GetAt(i);
				//3.2.1.2 ���� ���� ���� ��ġ�� dummyRow���� ���� ���縦 �� letter�� ���� �ִ´�.
				this->notepadForm->current->Add(currentLetterPos, letter->Clone());
				//3.2.1.3 i�� ���ҽ�Ų��.
				i--;
			}
			//3.2.2 ���� ��ġ�� ���������ش�.
			currentLetterPos = this->notepadForm->current->Move(currentLetterPos);
		}
	}
	//4. ���� �� ������ glyph�� ��(���๮��)�̸�
	else
	{
		//4.1 ������ ����� ���� ������ �Ҵ������Ѵ�.
		if (this->glyph != 0)
		{
			delete this->glyph;
		}
		//4.2. ���� �ٿ��� ���� ���� ���� ��ġ�� �ִ� ���ڵ��� ��� ���ο� ���� �����.
		this->glyph = this->notepadForm->current->Split(currentLetterPos);
		//4.3 ���� ���� ��ġ�� ��Ʈ�� ���� ����-1 �� ����(���� ���� ��ġ�� ������ ���̸�)
		if (currentRowPos == this->notepadForm->note->GetLength() - 1)
		{
			//4.3.1 ���ο� ���� ������ �� ������ �߰��Ѵ�.
			currentRowPos = this->notepadForm->note->Add(this->glyph->Clone());
		}
		//4.4 �װ� �ƴϸ�
		else
		{
			//4.4.1 ���ο� ���� ���� ���� ���� ��ġ�� ���� �ִ´�.
			currentRowPos = this->notepadForm->note->
				Add(currentRowPos + 1, this->glyph->Clone());
		}
		//4.5 ���� ���� ���� �����Ѵ�.(���� ������ ���� �ƴ϶� �и��� ���� ���� �ٷ� �Ѵ�.)
		currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//4.6 ���� ���� ���� ��ġ�� ���������� �̵���Ų��.
		this->notepadForm->current->Last();
		//4.7 �ڵ� �� �ٲ��� �������̸�
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//4.7.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
	//��������� ������
	//5. isComposing�� false�� �ٲ۴�.
	this->notepadForm->isComposing = false;
	//6. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//7. �޸��忡 ��������� ������ �����Ѵ�.
	this->notepadForm->isDirty = true;
	//8. ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
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