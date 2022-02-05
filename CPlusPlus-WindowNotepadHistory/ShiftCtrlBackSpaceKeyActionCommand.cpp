#include "ShiftCtrlBackSpaceKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "DummyRow.h"

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
		//���� ù ���̸� ���� ���� �� ����, ���� ù �ٿ��� ���� ��ġ�� ���� ó���� ������
		//�ƹ��͵� ���Ͼ. ���� ���� ��ġ�� ���� ó���� �ƴϰ�, ���� ���� ��ġ�� ���� ó���̸�
		//���� ���� ���� �ٿ� Join��Ų��.
		//3.1 ���� ���� ��ġ�� 0���� ũ��, ���� ���� ��ġ�� 0�̸�
		Glyph* currentRow = 0;
		Glyph* previousRow = 0;
		if (currentRowPos > 0 && currentLetterPos == 0)
		{
			//3.1.1 ���� ���� ���Ѵ�.
			currentRow = this->notepadForm->note->GetAt(currentRowPos);
			//3.1.2 ������ ����� ���� ������ �Ҵ������Ѵ�.
			if (this->glyph != 0)
			{
				delete this->glyph;
			}
			//3.1.3 ���� ���� ���� �����Ѵ�.
			this->glyph = currentRow->Clone();
			//3.1.4 ���� ���� ���� ���� ���Ѵ�.
			previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
			//3.1.5 ���� ���� ���� ���� ������ ���� ��ġ�� ���Ѵ�.
			Long letterPos = previousRow->GetLength();
			//3.1.6 ���� ���� ���� �ٿ� ��ģ��.
			currentRow->Join(previousRow);
			//3.1.7 Note���� ���� ���� �ּҸ� �����.
			this->notepadForm->note->Remove(currentRowPos);
			//3.1.8 ���� ���� �������� ������ ���� ���� �����Ѵ�.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//3.1.9 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
			//���� ���� ������ ���� ���� ó�� ���̿� ��ġ�ϵ��� �����Ѵ�.
			Long index = this->notepadForm->current->Move(letterPos);
			//3.1.10 Command�� ���� ������ ������ ǥ���Ѵ�.
			this->isDirty = true;
		}
		// ���� ���� ��ġ�� ó���� �ƴ� ��(���� ���� ó���̵� �ƴϵ� �������) ���� ���ڸ� �����.
		//2.2 ���� ���� ��ġ�� ó���� �ƴϸ�
		else if (currentLetterPos > 0)
		{
			//2.2.1 ������ ����� ���� ������ �Ҵ������Ѵ�.
			if (this->glyph != 0)
			{
				delete this->glyph;
			}
			//DummyRow�� �����ؼ� ���ڸ� ��� �װ� �����ؼ� this->glyph�� �ּҸ� �ű��,
			//���ڵ��� �ܾ������ ��������� �Űܼ� �����Ѵ�.
			//2.2.2 DummyRow�� �����Ѵ�.
			this->glyph = new DummyRow();
			//2.2.3 i�� ���� ������ġ���� �������� �ݺ��Ѵ�.
			Long i = 0;
			Glyph* letter = 0;
			while (i < currentLetterPos)
			{
				//2.2.3.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
				letter = this->notepadForm->current->GetAt(i);
				//2.2.3.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
				this->glyph->Add(letter->Clone());
				//2.2.3.3 ���ڸ� �����.
				this->notepadForm->current->Remove(i);
				currentLetterPos--;
			}
			//2.2.4 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//2.2.4.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
			//3.2.6 Command�� ���� ������ ������ ǥ���Ѵ�.
			this->isDirty = true;
		}
		//3.3 Command�� ���� ������ ������
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
void ShiftCtrlBackSpaceKeyActionCommand::Unexecute()
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
			//3.1.5 ���� ��ġ�� ���� �ٿ��� ���� ���������� ������.
			currentLetterPos = currentRow->Last();
		}
		//3.2 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� �ٸ���
		else
		{
			//3.2.1 dummyRow�� ������ŭ �ݺ��Ѵ�.
			Glyph* letter = 0;
			Long i = this->glyph->GetLength() - 1;//������ �迭��Һ��� ������ ������ ����� �ȴ�.
			Long distance = currentLetterPos;
			while (i >= 0)
			{
				//3.2.1.1 ���ڸ� ���Ѵ�.
				letter = this->glyph->GetAt(i);
				//3.2.1.2 ���� ���� ���� ��ġ�� dummyRow���� ���� ���縦 �� letter�� ���� �ִ´�.
				currentLetterPos = this->notepadForm->current->
					Add(currentLetterPos, letter->Clone());
				//3.2.1.3 �̵��� ��ġ�� ������Ų��.
				distance++;
				//3.2.1.3 i�� ������Ų��.
				i--;
			}
			//3.2.2 ���� ��ġ�� �������Ѵ�.
			this->notepadForm->current->Move(distance);
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
		//4.2 ���� �ٿ��� ���� ���� ���� ��ġ�� �ִ� ���ڵ��� ��� ���ο� ���� �����.
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
		//4.5 ���� ���� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//4.6 ���� ���� ���� ��ġ�� ó������ �̵���Ų��.
		this->notepadForm->current->First();
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