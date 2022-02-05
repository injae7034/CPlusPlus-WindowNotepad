#include "CtrlDeleteKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "DummyRow.h"

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
	//1. ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//2. OnCharCommand�� �ٽ� ����Ǹ�
	if (this->isRedone == true)
	{
		//2.1 ���� ���� ��ġ�� ������ġ�� ���������ش�.
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	}
	//3. �޸��忡�� ���õ� texts�� ������
	if (this->notepadForm->isSelecting == false)
	{
		//3.1 ���� ��Ʈ�� ������ ���� ��ġ�� ���Ѵ�.
		Long lastRowPos = this->notepadForm->note->GetLength() - 1;
		//3.2 ���� �ٿ��� ������ ������ ��ġ�� ���Ѵ�.
		Long lastLetterPos = this->notepadForm->current->GetLength();
		//���� ������ ���̸� ���� ���� �� ����, ������ �ٿ��� ���� ��ġ�� �������� ������ �ƹ��͵� ���Ͼ
		// ���� ���� ��ġ�� �������� �ƴϰ�, ���� ���� ��ġ�� �������̸� ���� ���� ���� �ٷ� ���Խ�Ų��.
		//3.3 ���� ���� ��ġ�� ��Ʈ�� ������ �� ��ġ���� �۰�, ���� ���� ��ġ�� �������̸�
		if (currentRowPos < lastRowPos && currentLetterPos == lastLetterPos)
		{
			//3.3.1 ���� ���� ���Ѵ�.
			Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
			//3.3.2 ���� ���� ���� ���� ���Ѵ�.
			Glyph* nextRow = this->notepadForm->note->GetAt(currentRowPos + 1);
			//3.3.3 ������ ����� ���� ������ �Ҵ������Ѵ�.
			if (this->glyph != 0)
			{
				delete this->glyph;
			}
			//3.3.4 ���� ���� ���� ���� ���� �����ؼ� �����Ѵ�.
			this->glyph = nextRow->Clone();
			//3.3.5 ���� ���� ���� �ٿ� ��ģ��.
			nextRow->Join(currentRow);
			//3.3.6 Note���� ���� ���� �ּҸ� �����.
			this->notepadForm->note->Remove(currentRowPos + 1);
			//3.3.7 ���� ���� ���� ��ġ�� ������ �������̱� ������ lastLetterPos�� �Ű��ش�.
			currentLetterPos = this->notepadForm->current->Move(lastLetterPos);
			//3.3.8 Command�� ���� ������ ������ ǥ���Ѵ�.
			this->isDirty = true;
		}
		// ���� ���� ��ġ�� �������� �ƴ� ��(���� ���� �������̵� �ƴϵ� �������)
		//���� ������ ���� ���ڸ� �����.
		//3.4 ���� ���� ��ġ�� �������� �ƴϸ�
		else if (currentLetterPos < lastLetterPos)
		{
			//3.4.1 ������ ����� ���� ������ �Ҵ������Ѵ�.
			if (this->glyph != 0)
			{
				delete this->glyph;
			}
			//DummyRow�� �����ؼ� ���ڸ� ��� �װ� �����ؼ� this->glyph�� �ּҸ� �ű��,
			//���ڵ��� �ܾ������ ��������� �Űܼ� �����Ѵ�.
			//3.4.2 DummyRow�� �����Ѵ�.
			this->glyph = new DummyRow();
			//3.4.3 ������ �������� �ܾ������ �̵��� ���� ������ġ�� ���Ѵ�.
			Long letterPosAfterMoving = this->notepadForm->current->NextWord();
			//3.4.4 ������ �������� �ܾ������ �̵��� ���� ������ġ�� ���� ���� ��ġ���� �������� �ݺ��Ѵ�.
			Glyph* letter = 0;
			while (currentLetterPos < letterPosAfterMoving)
			{
				//3.4.4.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
				letter = this->notepadForm->current->GetAt(currentLetterPos);
				//3.4.4.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
				this->glyph->Add(letter->Clone());
				//3.4.4.3 ���ڸ� �����.
				this->notepadForm->current->Remove(currentLetterPos);
				letterPosAfterMoving--;
			}
			//3.4.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//3.4.5.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
			//3.1.10 Command�� ���� ������ ������ ǥ���Ѵ�.
			this->isDirty = true;
		}
		//3.5 Command�� ���� ������ ������
		if (this->isDirty == true)
		{
			//3.5.1 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.5.2 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->isDirty = true;
			//3.5.3 ���ڸ� ���� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
			this->rowIndex = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
			this->letterIndex = this->notepadForm->current->GetCurrent();
		}
	}
	//4. �޸��忡�� ���õ� texts�� ������
	else
	{
		//4.1 RemoveCommand�� �޼����� ������ ���ÿ����� �����.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
}

//�������
void CtrlDeleteKeyActionCommand::Unexecute()
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