#include "CtrlBackSpaceKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "DummyRow.h"
#include "RowAutoChange.h"

//����Ʈ������ ����
CtrlBackSpaceKeyActionCommand::CtrlBackSpaceKeyActionCommand(NotepadForm* notepadForm)
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
void CtrlBackSpaceKeyActionCommand::Execute()
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
	//3. CtrlBackSpaceKeyActionCommand�� �ٽ� ����Ǹ�
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
		//4.2 ���� ���� ��ġ�� 0�̰�, ���� ���� ��ġ�� 0�̸�
		if (currentRowPos == 0 && currentLetterPos == 0)
		{
			//4.2.1 ó�� �����̸�
			if (this->isRedone == false)
			{
				//4.2.1.1 Command�� ���� ������ ������ ǥ���Ѵ�.
				this->isDirty = false;
			}
		}
		//4.3 ���� ���� ��ġ�� 0���� ũ��, ���� ���� ��ġ�� 0�̰�, ���� ���� ��¥ ���̸�
		else if (currentRowPos > 0 && currentLetterPos == 0
			&& !dynamic_cast<DummyRow*>(currentRow))
		{
			//4.3.1 ó�� �����̸�
			if (this->isRedone == false)
			{
				//4.3.1.1 Row�� �����Ѵ�.
				this->glyph = new Row();
			}
			//4.3.2.2 ���� ���� ���� ���� ���Ѵ�.
			Glyph* previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
			//4.3.2.3 ���� ���� ���� ���� ������ ���� ��ġ�� ���Ѵ�.
			Long letterPos = previousRow->GetLength();
			//4.3.2.4 ���� ���� ���� �ٿ� ��ģ��.
			currentRow->Join(previousRow);
			//4.3.2.5 Note���� ���� ���� �ּҸ� �����.
			this->notepadForm->note->Remove(currentRowPos);
			//4.3.2.6 ���� ���� �������� ������ ���� ���� �����Ѵ�.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//4.3.2.7 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
			//���� ���� ������ ���� ���� ó�� ���̿� ��ġ�ϵ��� �����Ѵ�.
			currentLetterPos = this->notepadForm->current->Move(letterPos);
			//4.3.2.8 Command�� ���� ������ ������ ǥ���Ѵ�.
			this->isDirty = true;
		}
		//4.4 �� �̿ܿ���
		else
		{
			//4.4.1 �̵��ϱ� ���� ���� ���� ��ġ�� ���Ѵ�.
			Long previousRowIndex = this->notepadForm->note->GetCurrent();
			//4.4.2 �̵��ϱ� ���� ���� ������ ��ġ�� ���Ѵ�.
			Long previousLetterIndex = this->notepadForm->current->GetCurrent();
			//4.4.3 Ctrl�� ���� ���·� ������ش�.
			BYTE pKeys[256] = { 0, };
			pKeys[VK_CONTROL] = 0x80;
			SetKeyboardState(pKeys);
			//4.4.4 ���� �������� �ܾ������ �̵��Ѵ�.
			this->notepadForm->SendMessage(WM_KEYDOWN, VK_LEFT);
			//4.4.5 ���� �������� �ܾ������ �̵��� ������ ������ �ٽ� CtrlŰ�� �ȴ��� ���·� �ٲ��ش�.
			pKeys[VK_CONTROL] = -1;
			SetKeyboardState(pKeys);
			//4.4.6 ���� �������� �ܾ������ �̵��� ������ ���� ���� ��ġ�� ���Ѵ�.
			Long currentRowIndex = this->notepadForm->note->GetCurrent();
			Long currentLetterIndex = this->notepadForm->current->GetCurrent();
			Glyph* letter = 0;
			Long letterCount = 0;
			Glyph* row = 0;
			Glyph* nextRow = 0;
			Long i = 0;
			//4.4.7 ó�� �����̸�
			if (this->isRedone == false)
			{
				//4.4.7.1 DummyRow�� �����Ѵ�.
				this->glyph = new DummyRow();
				//4.4.7.2 Command�� ���� ������ ������ ǥ���Ѵ�.
				this->isDirty = true;
				//4.4.7.3 �̵��ϱ� ���� ���� ��ġ�� �̵��� ���� ���� ��ġ�� ���� ������
				if (previousRowIndex == currentRowIndex)
				{
					//4.4.7.3.1 ���� �������� �ܾ������ �̵��� ���� ������ġ�� 
					//�̵��ϱ� ���� ���� ��ġ���� �������� �ݺ��Ѵ�.
					while (currentLetterIndex < previousLetterIndex)
					{
						//4.4.7.3.1.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
						letter = this->notepadForm->current->GetAt(currentLetterIndex);
						//4.4.7.3.1.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
						this->glyph->Add(letter->Clone());
						//4.4.7.3.1.3 ���ڸ� �����.
						this->notepadForm->current->Remove(currentLetterIndex);
						previousLetterIndex--;
					}
				}
				//4.4.7.4 �̵��ϱ� ���� ���� ��ġ�� �̵��� ���� ���� ��ġ�� ���� �ٸ���
				else if (previousRowIndex > currentRowIndex)
				{
					//4.4.7.4.1 �̵��ϱ� ���� ���� ���Ѵ�.
					nextRow = this->notepadForm->note->GetAt(previousRowIndex);
					//4.4.7.4.2 �̵��ϱ� ���� ������ġ���� �ݺ��Ѵ�.
					i = 0;
					while (i < previousLetterIndex)
					{
						//4.4.7.4.2.1 ���ڸ� ���Ѵ�.
						letter = nextRow->GetAt(i);
						//4.4.7.4.2.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
						this->glyph->Add(letter->Clone());
						//4.4.7.4.2.3 ���ڸ� �����.
						nextRow->Remove(i);
						previousLetterIndex--;
					}
					//4.4.7.4.3 ���߿� ���� �ٿ� Join��Ű�� ���� ���� �ּҸ� �����Ѵ�.
					row = nextRow;
					//4.4.7.4.4 �̵��ϱ� ���� ���� �̵��� ���� ���� ��ġ���� ū ���� �ݺ��Ѵ�.
					previousRowIndex--;
					while (previousRowIndex > currentRowIndex)
					{
						//4.4.7.4.4.1 �̵��ϱ� ���� ���� ���Ѵ�.
						nextRow = this->notepadForm->note->GetAt(previousRowIndex);
						i = nextRow->GetLength() - 1;
						//4.4.7.4.4.2 �̵��ϱ� ���� ���� ������ŭ �ݺ��Ѵ�.
						while (i >= 0)
						{
							//4.4.7.4.4.2.1 ���ڸ� ���Ѵ�.
							letter = nextRow->GetAt(i);
							//4.4.7.4.4.2.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
							this->glyph->Add(0, letter->Clone());
							//4.4.7.4.4.2.3 ���ڸ� �����.
							nextRow->Remove(i);
							i--;
						}
						//4.4.7.4.4.3 ���ڸ� �������� ������ ���� �����.
						this->notepadForm->note->Remove(previousRowIndex);
						//4.4.7.4.4.4 ���� �ٷ� �̵��Ѵ�.
						previousRowIndex--;
					}
					//4.4.7.4.5 ���� ���� ���Ѵ�.
					currentRow = this->notepadForm->current;
					//4.4.7.4.6 ���� ���� �� ������ ������ ���� ���� ���ڵ��� �����.
					//���� ���� ���� �������� ���� ������ ��ġ�� ũ�ų� ���� ���� �ݺ��Ѵ�.
					letterCount = currentRow->GetLength() - 1;
					while (letterCount >= currentLetterIndex)
					{
						//4.4.7.4.6.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
						letter = currentRow->GetAt(letterCount);
						//4.4.7.4.6.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
						this->glyph->Add(0, letter->Clone());
						//4.4.7.4.6.3 ���ڸ� �����.
						currentRow->Remove(letterCount);
						letterCount--;
					}
					//4.4.7.4.7 ���� ���� ���� �ٿ� Join��Ų��.
					row->Join(currentRow);
					//4.4.7.4.8 ���� ���� ���� �ٿ� Join�Ǿ��� ������ �������� �ּҸ� ��Ʈ���� �����.
					this->notepadForm->note->Remove(currentRowIndex + 1);
					//4.4.7.4.9 ���� ������ ��ġ�� �ٽ� �̵������ش�.
					currentLetterIndex = currentRow->Move(currentLetterIndex);
				}
			}
			//4.4.8 ó�� ������ �ƴϸ�
			else
			{
				//4.4.8.1 �̵��ϱ� ���� ���� ��ġ�� �̵��� ���� ���� ��ġ�� ���� ������
				if (previousRowIndex == currentRowIndex)
				{
					//4.4.8.1.1 ���� �������� �ܾ������ �̵��� ���� ������ġ�� 
					//���� ���� ��ġ���� �������� �ݺ��Ѵ�.
					while (currentLetterIndex < previousLetterIndex)
					{
						//4.4.8.1.1.1 ���ڸ� �����.
						this->notepadForm->current->Remove(currentLetterIndex);
						previousLetterIndex--;
					}
				}
				//4.4.8.2 �̵��ϱ� ���� ���� ��ġ�� �̵��� ���� ���� ��ġ�� ���� �ٸ���
				else if (previousRowIndex > currentRowIndex)
				{
					//4.4.8.2.1 �̵��ϱ� ���� ���� ���Ѵ�.
					nextRow = this->notepadForm->note->GetAt(previousRowIndex);
					//4.4.8.2.2 �̵��ϱ� ���� ������ġ���� �ݺ��Ѵ�.
					i = 0;
					while (i < previousLetterIndex)
					{
						//4.4.8.2.2.1 ���ڸ� �����.
						nextRow->Remove(i);
						previousLetterIndex--;
					}
					//4.4.8.2.3 ���߿� ���� �ٿ� Join��Ű�� ���� ���� �ּҸ� �����Ѵ�.
					row = nextRow;
					//4.4.8.2.4 �̵��ϱ� ���� ���� �̵��� ���� ���� ��ġ���� ū ���� �ݺ��Ѵ�.
					previousRowIndex--;
					while (previousRowIndex > currentRowIndex)
					{
						//4.4.8.4.4.1 �̵��ϱ� ���� ���� ���Ѵ�.
						nextRow = this->notepadForm->note->GetAt(previousRowIndex);
						i = nextRow->GetLength() - 1;
						//4.4.8.4.4.2 �̵��ϱ� ���� ���� ������ŭ �ݺ��Ѵ�.
						while (i >= 0)
						{
							//4.4.8.4.4.2.1 ���ڸ� �����.
							nextRow->Remove(i);
							i--;
						}
						//4.4.8.4.4.3 ���ڸ� �������� ������ ���� �����.
						this->notepadForm->note->Remove(previousRowIndex);
						//4.4.8.4.4.4 ���� �ٷ� �̵��Ѵ�.
						previousRowIndex--;
					}
					//4.4.8.2.5 ���� ���� ���Ѵ�.
					currentRow = this->notepadForm->current;
					//4.4.8.2.6 ���� ���� �� ������ ������ ���� ���� ���ڵ��� �����.
					//���� ���� ���� �������� ���� ������ ��ġ�� ũ�ų� ���� ���� �ݺ��Ѵ�.
					letterCount = currentRow->GetLength() - 1;
					while (letterCount >= currentLetterIndex)
					{
						//4.4.8.2.6.1 ���ڸ� �����.
						currentRow->Remove(letterCount);
						letterCount--;
					}
					//4.4.8.2.7 ���� ���� ���� �ٿ� Join��Ų��.
					row->Join(currentRow);
					//4.4.8.2.8 ���� ���� ���� �ٿ� Join�Ǿ��� ������ �������� �ּҸ� ��Ʈ���� �����.
					this->notepadForm->note->Remove(currentRowIndex + 1);
					//4.4.8.2.9 ���� ������ ��ġ�� �ٽ� �̵������ش�.
					currentLetterIndex = currentRow->Move(currentLetterIndex);
				}
			}
		}
		//4.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//4.5.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
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
void CtrlBackSpaceKeyActionCommand::Unexecute()
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
				//5.2.1.4 i�� ������Ų��.
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
void CtrlBackSpaceKeyActionCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void CtrlBackSpaceKeyActionCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}


//SetRedone(�ٽ� �����̶�� ������)
void CtrlBackSpaceKeyActionCommand::SetRedone()
{
	this->isRedone = true;
}

//���� ��ġ ���ϱ�
Long CtrlBackSpaceKeyActionCommand::GetRowIndex()
{
	return this->rowIndex;
}
//���� ��ġ ���ϱ�
Long CtrlBackSpaceKeyActionCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//������� ������������ ���ϱ�
bool CtrlBackSpaceKeyActionCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool CtrlBackSpaceKeyActionCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//�ٽý������� ���� ���ϱ�
bool CtrlBackSpaceKeyActionCommand::IsRedone()
{
	return this->isRedone;
}

//��������� �ִ��� Ȯ�� ����
bool CtrlBackSpaceKeyActionCommand::IsDirty()
{
	return this->isDirty;
}

//�Ҹ��� ����
CtrlBackSpaceKeyActionCommand::~CtrlBackSpaceKeyActionCommand()
{
	//1. DummyRow�� Row�� �Ҵ������Ѵ�.
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}