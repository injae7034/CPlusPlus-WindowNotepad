#include "CtrlDeleteKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "DummyRow.h"
#include "RowAutoChange.h"

//����Ʈ������ ����
CtrlDeleteKeyActionCommand::CtrlDeleteKeyActionCommand(NotepadForm* notepadForm)
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
void CtrlDeleteKeyActionCommand::Execute()
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
	//3. CtrlDeleteKeyActionCommand�� �ٽ� ����Ǹ�
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
		Glyph* letter = 0;
		//4.1 ���� ��Ʈ�� ������ ���� ��ġ�� ���Ѵ�.
		Long lastRowPos = this->notepadForm->note->GetLength() - 1;
		//4.2 ���� �ٿ��� ������ ������ ��ġ�� ���Ѵ�.
		Long lastLetterPos = this->notepadForm->current->GetLength();
		//���� ������ ���̸� ���� ���� �� ����, ������ �ٿ��� ���� ��ġ�� �������� ������ �ƹ��͵� ���Ͼ
		// ���� ���� ��ġ�� �������� �ƴϰ�, ���� ���� ��ġ�� �������̸� ���� ���� ���� �ٷ� ���Խ�Ų��.
		//4.3 ���� ���� ��ġ�� ��Ʈ�� ������ �� ��ġ���� �۰�, ���� ���� ��ġ�� �������̸�
		if (currentRowPos < lastRowPos && currentLetterPos == lastLetterPos)
		{
			//4.3.1 ���� ���� ���� ���� ���Ѵ�.
			Glyph* nextRow = this->notepadForm->note->GetAt(currentRowPos + 1);
			//4.3.2 ���� ���� ���� ���� ��¥���� �ƴϸ�(��¥ ���̸�)
			if (!dynamic_cast<DummyRow*>(nextRow))
			{
				//4.3.2.1 ó�� ������ �Ǹ�
				if (this->isRedone == false)
				{
					//4.3.2.1.1 Row�� �����Ѵ�.
					this->glyph = new Row();
				}
				//4.3.2.2 ���� ���� ���Ѵ�.
				Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
				//4.3.2.3 ���� ���� ���� �ٿ� ��ģ��.
				nextRow->Join(currentRow);
				//4.3.2.4 Note���� ���� ���� �ּҸ� �����.
				this->notepadForm->note->Remove(currentRowPos + 1);
				//4.3.2.5 ���� ���� �������� ������ ���� ���� �����Ѵ�.
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				//4.3.2.6 ���� ���� ���� ��ġ�� ������ ���� �������̱� ������
				//������ ���� ������ lastLetterPos�� �Ű��ش�.
				currentLetterPos = this->notepadForm->current->Move(lastLetterPos);
			}
			//4.1.3 ���� ���� ���� ���� ��¥���̸�
			else
			{
				//4.1.3.1 ���� ���� ù���ڷ� �̵��Ѵ�.
				currentRowPos = this->notepadForm->note->Move(currentRowPos + 1);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->First();
				//4.1.3.2 ó�� ������ �Ǹ�
				if (this->isRedone == false)
				{
					//4.1.3.2.1 ���� ���ڸ� ���Ѵ�.
					letter = this->notepadForm->current->GetAt(currentLetterPos);
					//4.1.3.2.2 ���� ���ڸ� ���� �����ؼ� �����Ѵ�.
					this->glyph = letter->Clone();
				}
				//4.1.3.3 ���� ���� ù��° ���ڸ� �����.
				this->notepadForm->current->Remove(currentLetterPos);
				//4.1.3.4 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				//��¥���� �ִٴ°� �ڵ������� �������̶�� �ǹ���.
				this->notepadForm->SendMessage(WM_SIZE);
				//4.1.3.5 ���� ��ġ�� �ٽ� ���� �� ������ ���ڷ� �������ش�.
				currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Last();
			}
			//4.1.4 Command�� ���� ������ ������ ǥ���Ѵ�.
			this->isDirty = true;
		}
		// ���� ���� ��ġ�� �������� �ƴ� ��(���� ���� �������̵� �ƴϵ� �������)
		//���� ������ ���� ���ڸ� �����.
		//4.4 ���� ���� ��ġ�� �������� �ƴϸ�
		else if (currentLetterPos < lastLetterPos)
		{
			//4.4.1 ������ ����� ���� ������ �Ҵ������Ѵ�.
			if (this->glyph != 0)
			{
				delete this->glyph;
			}
			//DummyRow�� �����ؼ� ���ڸ� ��� �װ� �����ؼ� this->glyph�� �ּҸ� �ű��,
			//���ڵ��� �ܾ������ ��������� �Űܼ� �����Ѵ�.
			//4.4.2 DummyRow�� �����Ѵ�.
			this->glyph = new DummyRow();
			//4.4.3 ������ �������� �ܾ������ �̵��� ���� ������ġ�� ���Ѵ�.
			Long letterPosAfterMoving = this->notepadForm->current->NextWord();
			//4.4.4 ������ �������� �ܾ������ �̵��� ���� ������ġ�� ���� ���� ��ġ���� �������� �ݺ��Ѵ�.
			Glyph* letter = 0;
			while (currentLetterPos < letterPosAfterMoving)
			{
				//4.4.4.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
				letter = this->notepadForm->current->GetAt(currentLetterPos);
				//4.4.4.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
				this->glyph->Add(letter->Clone());
				//4.4.4.3 ���ڸ� �����.
				this->notepadForm->current->Remove(currentLetterPos);
				letterPosAfterMoving--;
			}
			//4.4.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//4.4.5.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
			//4.1.10 Command�� ���� ������ ������ ǥ���Ѵ�.
			this->isDirty = true;
		}
		//4.5 Command�� ���� ������ ������
		if (this->isDirty == true)
		{
			//4.5.1 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//4.5.2 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->isDirty = true;
			//4.5.3 ���ڸ� ���� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
			this->rowIndex = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
			this->letterIndex = this->notepadForm->current->GetCurrent();
			//4.5.4 �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� �����ؾ���)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				Long changedRowPos = this->rowIndex;
				Long changedLetterPos = this->letterIndex;
				Long originRowPos = 0;
				Long originLetterPos = 0;
				//4.5.4.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
				rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
					&originRowPos);
				//4.5.4.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
				this->rowIndex = originRowPos;
				this->letterIndex = originLetterPos;
			}
		}
	}
	//5. �޸��忡�� ���õ� texts�� ������
	else
	{
		//5.1 RemoveCommand�� �޼����� ������ ���ÿ����� �����.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
}

//�������
void CtrlDeleteKeyActionCommand::Unexecute()
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
			Long i = this->glyph->GetLength() - 1;
			while (i >= 0)
			{
				//5.2.1.1 ���ڸ� ���Ѵ�.
				letter = this->glyph->GetAt(i);
				//5.2.1.2 ���� ���� ���� ��ġ�� dummyRow���� ���� ���縦 �� letter�� ���� �ִ´�.
				this->notepadForm->current->Add(currentLetterPos, letter->Clone());
				//5.2.1.3 i�� ���ҽ�Ų��.
				i--;
			}
			//5.2.2 ���� ��ġ�� ���������ش�.
			currentLetterPos = this->notepadForm->current->Move(currentLetterPos);
		}
	}
	//6. ���� �� ������ glyph�� ��(���๮��)�̸�
	else
	{
		//6.1 ������ ����� ���� ������ �Ҵ������Ѵ�.
		if (this->glyph != 0)
		{
			delete this->glyph;
		}
		//6.2. ���� �ٿ��� ���� ���� ���� ��ġ�� �ִ� ���ڵ��� ��� ���ο� ���� �����.
		this->glyph = this->notepadForm->current->Split(currentLetterPos);
		//6.3 ���� ���� ��ġ�� ��Ʈ�� ���� ����-1 �� ����(���� ���� ��ġ�� ������ ���̸�)
		if (currentRowPos == this->notepadForm->note->GetLength() - 1)
		{
			//6.3.1 ���ο� ���� ������ �� ������ �߰��Ѵ�.
			currentRowPos = this->notepadForm->note->Add(this->glyph->Clone());
		}
		//6.4 �װ� �ƴϸ�
		else
		{
			//6.4.1 ���ο� ���� ���� ���� ���� ��ġ�� ���� �ִ´�.
			currentRowPos = this->notepadForm->note->
				Add(currentRowPos + 1, this->glyph->Clone());
		}
		//6.5 ���� ���� ���� �����Ѵ�.(���� ������ ���� �ƴ϶� �и��� ���� ���� �ٷ� �Ѵ�.)
		currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//6.6 ���� ���� ���� ��ġ�� ���������� �̵���Ų��.
		this->notepadForm->current->Last();
		//6.7 �ڵ� �� �ٲ��� �������̸�
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//6.7.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
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
void CtrlDeleteKeyActionCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void CtrlDeleteKeyActionCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}


//SetRedone(�ٽ� �����̶�� ������)
void CtrlDeleteKeyActionCommand::SetRedone()
{
	this->isRedone = true;
}

//���� ��ġ ���ϱ�
Long CtrlDeleteKeyActionCommand::GetRowIndex()
{
	return this->rowIndex;
}
//���� ��ġ ���ϱ�
Long CtrlDeleteKeyActionCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//������� ������������ ���ϱ�
bool CtrlDeleteKeyActionCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool CtrlDeleteKeyActionCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//�ٽý������� ���� ���ϱ�
bool CtrlDeleteKeyActionCommand::IsRedone()
{
	return this->isRedone;
}
//��������� �ִ��� Ȯ�� ����
bool CtrlDeleteKeyActionCommand::IsDirty()
{
	return this->isDirty;
}
//�Ҹ��� ����
CtrlDeleteKeyActionCommand::~CtrlDeleteKeyActionCommand()
{
	//1. DummyRow�� Row�� �Ҵ������Ѵ�.
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}