#include "ShiftCtrlBackSpaceKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "DummyRow.h"
#include "RowAutoChange.h"

//����Ʈ������ ����
ShiftCtrlBackSpaceKeyActionCommand::ShiftCtrlBackSpaceKeyActionCommand(NotepadForm* notepadForm)
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
void ShiftCtrlBackSpaceKeyActionCommand::Execute()
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
	//3. ShiftCtrlBackSpaceKeyActionCommand�� �ٽ� ����Ǹ�
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
		//4.1 ���� ���� ���Ѵ�.
		Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
		Glyph* row = 0;
		//4.2 ó�� �����̸�
		//if (this->isRedone == false)
		//{
			//4.2.1 ���� ���� ��ġ�� 0�̰�, ���� ���� ��ġ�� 0�̸�
			if (currentRowPos == 0 && currentLetterPos == 0)
			{
				//4.2.1.1 ó�� �����̸�
				if (this->isRedone == false)
				{
					//4.2.1.1.1 Command�� ���� ������ ������ ǥ���Ѵ�.
					this->isDirty = false;
				}
			}
			//4.2.2 ���� ���� ��ġ�� 0���� ũ��, ���� ���� ��ġ�� 0�̰�, ���� ���� ��¥ ���̸�
			else if (currentRowPos > 0 && currentLetterPos == 0
				&& !dynamic_cast<DummyRow*>(currentRow))
			{
				//4.2.2.1 ó�� �����̸�
				if (this->isRedone == false)
				{
					//4.2.2.1.1 Row�� �����Ѵ�.
					this->glyph = new Row();
				}
				//4.2.2.2 ���� ���� ���� ���� ���Ѵ�.
				Glyph* previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
				//4.2.2.3 ���� ���� ���� ���� ������ ���� ��ġ�� ���Ѵ�.
				Long letterPos = previousRow->GetLength();
				//4.2.2.4 ���� ���� ���� �ٿ� ��ģ��.
				currentRow->Join(previousRow);
				//4.2.2.5 Note���� ���� ���� �ּҸ� �����.
				this->notepadForm->note->Remove(currentRowPos);
				//4.2.2.6 ���� ���� �������� ������ ���� ���� �����Ѵ�.
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				//4.2.2.7 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
				//���� ���� ������ ���� ���� ó�� ���̿� ��ġ�ϵ��� �����Ѵ�.
				currentLetterPos = this->notepadForm->current->Move(letterPos);
				//4.2.2.8 Command�� ���� ������ ������ ǥ���Ѵ�.
				this->isDirty = true;
			}
			//4.2.3 �� �̿ܿ���
			else
			{
				Glyph* letter = 0;
				//4.2.3.1 ó�� �����̸�
				if (this->isRedone == false)
				{
					//4.2.3.1.1 DummyRow�� �����Ѵ�.
					this->glyph = new DummyRow();
					//4.2.3.1.2 Command�� ���� ������ ������ ǥ���Ѵ�.
					this->isDirty = true;
				}
				//4.2.3.2 ���� ���� ��¥���̸�
				if (dynamic_cast<DummyRow*>(currentRow))
				{
					//4.2.3.2.2.1 ó�� �����̸�
					if (this->isRedone == false)
					{
						//4.2.3.2.1 ���� ������ġ�� 0���� �������� �ݺ��Ѵ�.
						while (currentLetterPos > 0)
						{
							//4.2.3.2.1.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
							letter = currentRow->GetAt(currentLetterPos - 1);
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
						while (currentLetterPos > 0)
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
				//4.2.3.3 ó�� �����̸�
				if (this->isRedone == false)
				{
					//4.2.3.3.1 ��¥ ���ε��� �ݺ��Ѵ�. 
					while (dynamic_cast<DummyRow*>(currentRow))
					{
						//4.2.3.3.1.1 ���� ������ġ�� 0���� �������� �ݺ��Ѵ�.
						while (currentLetterPos > 0)
						{
							//4.2.3.3.1.1.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
							letter = currentRow->GetAt(currentLetterPos - 1);
							//4.2.3.3.1.1.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
							this->glyph->Add(0, letter->Clone());
							//4.2.3.3.1.1.3 ���ڸ� �����.
							currentRow->Remove(currentLetterPos - 1);
							currentLetterPos--;
						}
						//4.2.3.3.1.2 ���ڰ� ���������� ������ Note���� ���� ���� �ּҸ� �����.
						this->notepadForm->note->Remove(currentRowPos);
						//4.2.3.3.1.3 ���� ���� ������ ������ġ�� �̵��Ѵ�.
						currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
						this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
						currentLetterPos = this->notepadForm->current->Last();
						currentRow = this->notepadForm->current;
					}
					//4.2.3.2 ��¥ ���� ���ڸ� �� ����� ���� �ݺ��Ѵ�.
					while (currentLetterPos > 0)
					{
						//4.2.3.2.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
						letter = currentRow->GetAt(currentLetterPos - 1);
						//4.2.3.2.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
						this->glyph->Add(0, letter->Clone());
						//4.2.3.2.3 ���ڸ� �����.
						currentRow->Remove(currentLetterPos - 1);
						currentLetterPos--;
					}
				}
				//4.2.3.4 �ٽ� �����̸�
				else
				{
					//4.2.3.4.1 ��¥ ���ε��� �ݺ��Ѵ�. 
					while (dynamic_cast<DummyRow*>(currentRow))
					{
						//4.2.3.4.1.1 ���� ������ġ�� 0���� �������� �ݺ��Ѵ�.
						while (currentLetterPos > 0)
						{
							//4.2.3.4.1.1.1 ���ڸ� �����.
							currentRow->Remove(currentLetterPos - 1);
							currentLetterPos--;
						}
						//4.2.3.4.1.2 ���ڰ� ���������� ������ Note���� ���� ���� �ּҸ� �����.
						this->notepadForm->note->Remove(currentRowPos);
						//4.2.3.4.1.3 ���� ���� ������ ������ġ�� �̵��Ѵ�.
						currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
						this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
						currentLetterPos = this->notepadForm->current->Last();
						currentRow = this->notepadForm->current;
					}
					//4.2.3.4.2 ��¥ ���� ���ڸ� �� ����� ���� �ݺ��Ѵ�.
					while (currentLetterPos > 0)
					{
						//4.2.3.4.2.1 ���ڸ� �����.
						currentRow->Remove(currentLetterPos - 1);
						currentLetterPos--;
					}
				}
				//4.2.3.5 �ٿ� ���ڰ� ����������
				if (row != 0)
				{
					//4.2.3.5.1 ���ڰ� �����ִ� ���� ���� �ٿ� ��ģ��.
					row->Join(currentRow);
					//4.2.3.5.2 Note���� ���ڰ� �����ִ� ���� �ּҸ� �����.
					this->notepadForm->note->Remove(currentRowPos + 1);
					//4.2.3.5.3 �����ٿ��� ĳ���� ���� ó������ ������.
					currentRow->First();
				}	

#if 0
				//4.2.3.5 ��¥ ���ε��� �ݺ��Ѵ�. 
				while (dynamic_cast<DummyRow*>(currentRow))
				{
					//4.2.3.3.1 ���� ������ġ�� 0���� �������� �ݺ��Ѵ�.
					while (currentLetterPos > 0)
					{
						//4.2.3.3.1.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
						letter = currentRow->GetAt(currentLetterPos - 1);
						//4.2.3.3.1.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
						this->glyph->Add(0, letter->Clone());
						//4.2.3.3.1.3 ���ڸ� �����.
						currentRow->Remove(currentLetterPos - 1);
						currentLetterPos--;
					}
					//4.2.3.3.2 ���ڰ� ���������� ������ Note���� ���� ���� �ּҸ� �����.
					this->notepadForm->note->Remove(currentRowPos);
					//4.2.3.3.3 ���� ���� ������ ������ġ�� �̵��Ѵ�.
					currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
					currentLetterPos = this->notepadForm->current->Last();
					currentRow = this->notepadForm->current;
				}
				//4.2.3.4 ��¥ ���� ���ڸ� �� ����� ���� �ݺ��Ѵ�.
				while (currentLetterPos > 0)
				{
					//4.2.3.4.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
					letter = currentRow->GetAt(currentLetterPos - 1);
					//4.2.3.4.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
					this->glyph->Add(0, letter->Clone());
					//4.2.3.4.3 ���ڸ� �����.
					currentRow->Remove(currentLetterPos - 1);
					currentLetterPos--;
				}
				//4.2.3.5 �ٿ� ���ڰ� ����������
				if (row != 0)
				{
					//4.2.3.5.1 ���ڰ� �����ִ� ���� ���� �ٿ� ��ģ��.
					row->Join(currentRow);
					//4.2.3.5.2 Note���� ���ڰ� �����ִ� ���� �ּҸ� �����.
					this->notepadForm->note->Remove(currentRowPos + 1);
					//4.2.3.5.3 �����ٿ��� ĳ���� ���� ó������ ������.
					currentRow->First();
				}
				//4.2.3.6 Command�� ���� ������ ������ ǥ���Ѵ�.
				this->isDirty = true;
#endif
			}
		//}
			//�������� �������� ��¥���̵� ��¥ ���̵� ������ġ 0���� ����� ������ ȭ���� Ŭ �� �� �� ����Ͱ�
			//ȭ���� ���� �� �� �� ����°� ���̰� ���� contents�� length��ŭ �ݺ������� ���ȴµ�
			//������ ���� ���� ��¥ ���̸� ������ġ 0���� ���߰�, ��¥ ���̸� ��¥ ���� ������ġ 0����
			//������⶧���� contentsLength�� �̿��� �ʿ䰡 ����, ó�������Ҷ��� �������� �����ϴ°�
			//����� ������ ������. �׷��� �Ʒ� �ٽ� ���� logic�� ���� �ʿ����.
#if 0
		//4.3 �ٽ� �����̸�
		else
		{
			//4.3.1 command�� ����Ǿ� �ִ� ���� ���ڰ����� ���Ѵ�.
			Long lengthOfCommandRow = this->glyph->GetLength();
			//4.3.2 currentLetterPos�� lengthOfCommandRow���� ũ�ų� ������
			if (currentLetterPos >= lengthOfCommandRow)
			{
				//4.3.2.1 lengthOfCommandRow�� 0���� ū���� �ݺ��Ѵ�.
				while (lengthOfCommandRow > 0)
				{
					//4.3.2.1.1 ���ڸ� �����.
					currentRow->Remove(currentLetterPos - 1);
					currentLetterPos--;
					lengthOfCommandRow--;
				}
			}
			//4.3.3 currentLetterPos�� lengthOfCommandRow���� ������
			else
			{
				//4.3.3.1 currentLetterPos�� 0���� ū ���� �ݺ��Ѵ�.
				while (currentLetterPos > 0)
				{
					//4.3.3.1.1 ���ڸ� �����.
					currentRow->Remove(currentLetterPos - 1);
					currentLetterPos--;
					lengthOfCommandRow--;
				}
				//4.3.3.2 ���� �ּҸ� �����Ѵ�.
				row = currentRow;
				//4.3.3.3 ���� ���� ������ ������ġ�� �̵��Ѵ�.
				currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Last();
				currentRow = this->notepadForm->current;
				//4.3.3.4 lengthOfCommandRow�� 0���� ū���� �ݺ��Ѵ�.
				while (lengthOfCommandRow > 0)
				{
					//4.3.3.4.1 currentLetterPos�� 0���� ū ���� �׸��� 
					//lengthOfCommandRow�� 0���� ū���� �ݺ��Ѵ�.
					while (currentLetterPos > 0 && lengthOfCommandRow > 0)
					{
						//4.3.3.4.1.1 ���ڸ� �����.
						currentRow->Remove(currentLetterPos - 1);
						currentLetterPos--;
						lengthOfCommandRow--;
					}
					//4.2.3.4.2 lengthOfCommandRow�� 0���� ũ��
					if (lengthOfCommandRow > 0)
					{
						//4.2.3.4.2.1  Note���� ���� ���� �ּҸ� �����.
						this->notepadForm->note->Remove(currentRowPos);
						//4.3.3.4.2.2 ���� ���� ������ ������ġ�� �̵��Ѵ�.
						currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
						this->notepadForm->current = this->notepadForm->note
							->GetAt(currentRowPos);
						currentLetterPos = this->notepadForm->current->Last();
						currentRow = this->notepadForm->current;
					}
				}
				//4.3.3.5 ���ڰ� �����ִ� ���� ���� �ٿ� ��ģ��.
				row->Join(currentRow);
				//4.3.3.6 Note���� ���ڰ� �����ִ� ���� �ּҸ� �����.
				this->notepadForm->note->Remove(currentRowPos + 1);
				//4.3.3.7 �����ٿ��� ĳ���� ���� ó������ ������.
				currentRow->First();
			}
		}
#endif
		//4.4 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//4.4.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
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
void ShiftCtrlBackSpaceKeyActionCommand::Unexecute()
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
			//5.1.5 ���� ��ġ�� ���� �ٿ��� ���� ���������� ������.
			currentLetterPos = currentRow->Last();
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
		//6.4 ���� ���� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//6.5 ���� ���� ���� ��ġ�� ó������ �̵���Ų��.
		this->notepadForm->current->First();
		//6.6 �ڵ� �� �ٲ��� �������̸�
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//6.6.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
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
void ShiftCtrlBackSpaceKeyActionCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void ShiftCtrlBackSpaceKeyActionCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}


//SetRedone(�ٽ� �����̶�� ������)
void ShiftCtrlBackSpaceKeyActionCommand::SetRedone()
{
	this->isRedone = true;
}

//���� ��ġ ���ϱ�
Long ShiftCtrlBackSpaceKeyActionCommand::GetRowIndex()
{
	return this->rowIndex;
}
//���� ��ġ ���ϱ�
Long ShiftCtrlBackSpaceKeyActionCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//������� ������������ ���ϱ�
bool ShiftCtrlBackSpaceKeyActionCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool ShiftCtrlBackSpaceKeyActionCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//�ٽý������� ���� ���ϱ�
bool ShiftCtrlBackSpaceKeyActionCommand::IsRedone()
{
	return this->isRedone;
}

//��������� �ִ��� Ȯ�� ����
bool ShiftCtrlBackSpaceKeyActionCommand::IsDirty()
{
	return this->isDirty;
}

//�Ҹ��� ����
ShiftCtrlBackSpaceKeyActionCommand::~ShiftCtrlBackSpaceKeyActionCommand()
{
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}