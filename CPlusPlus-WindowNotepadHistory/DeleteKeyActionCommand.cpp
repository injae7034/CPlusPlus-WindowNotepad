#include "DeleteKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"

//����Ʈ������ ����
DeleteKeyActionCommand::DeleteKeyActionCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->glyph = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
}

//����
void DeleteKeyActionCommand::Execute()
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
			//3.3.3 ���� ���� ���� ���� ���� �����Ѵ�.
			this->glyph = nextRow->Clone();
			//3.3.4 ���� ���� ���� �ٿ� ��ģ��.
			nextRow->Join(currentRow);
			//3.3.5 Note���� ���� ���� �ּҸ� �����.
			this->notepadForm->note->Remove(currentRowPos + 1);
			//3.3.6 ���� ���� ���� ��ġ�� ������ �������̱� ������ lastLetterPos�� �Ű��ش�.
			currentLetterPos = this->notepadForm->current->Move(lastLetterPos);
		}
		// ���� ���� ��ġ�� �������� �ƴ� ��(���� ���� �������̵� �ƴϵ� �������)
		//���� ������ ���� ���ڸ� �����.
		//3.4 ���� ���� ��ġ�� �������� �ƴϸ�
		else if (currentLetterPos < lastLetterPos)
		{
			//3.4.1 ���� ������ ���� ���ڸ� ����� ���� ���� ���ڸ� ���Ѵ�.
			Glyph* letter = this->notepadForm->current->GetAt(currentLetterPos);
			//3.4.2 ���� ������ ���� ���ڸ� ���� �����ؼ� �����Ѵ�.
			this->glyph = letter->Clone();
			//3.4.3 ���� ������ ���� ���ڸ� �����.
			this->notepadForm->current->Remove(currentLetterPos);
			//3.4.4 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//3.4.4.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
		}
		//3.5 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//3.6 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->isDirty = true;
		//3.7 ���ڸ� ���� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
	}
	//4. �޸��忡�� ���õ� texts�� ������
	else
	{
		//4.1 RemoveCommand�� �޼����� ������ ���ÿ����� �����.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
}

//�������
void DeleteKeyActionCommand::Unexecute()
{
	//1. ���� ���� ��ġ�� �̵���Ų��.(ĳ���� �ٸ� ���� ������ �� ���� ���ڰ� �������� ������)
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	//2. ���� ������ ��ġ�� �̵���Ų��.
	Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	//3. ���� �� ������ glyph�� ��(���๮��)�� �ƴϸ�
	if (!dynamic_cast<Row*>(this->glyph))
	{
		//3.1 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� ������
		if (currentLetterPos == this->notepadForm->current->GetLength())
		{
			//3.1.1 ���� ���� ������ ���� �ڿ� ���ο� ���ڸ� �߰��Ѵ�.
			//���⼭ command�� ������ �ִ� glyph�� ���� �����ؼ� �ٿ� �߰����ش�.
			//Command�� glyph�� note�ʹ� ���� ������ �־�� ���߿� ������ �ȳ��� �Ҵ������ϱ� ���ϴ�.
			//�׸��� glyph�� ���� ������ command�̱� ������ note�� ���� ���縦 �ؼ� �������°� ������ �´�.
			//�׷� ���� note�� �Ҵ������� �� �ڱⰡ ���� ���縦 �� glyph�� �Ҵ������ϰ�,
			//command�� �Ҵ������� �� �ڱⰡ ������ �ִ� glyph�� �Ҵ����� �ϱ� ������ ��� �Ҵ���������
			//���� �Ӹ� ���� ���� ����, ������� command�� glyph�� �����ϱ� ���ϱ� ������ 
			//command�� note�ʹ� ������ �ڱ⸸�� glyph�� ������ �־���Ѵ�.
			//�ƴϸ� ���߿� Backspace�� ������� �ٽ� ���ڸ� �Է��� �� PushUndo���� ������ ����.!
			currentLetterPos = this->notepadForm->current->Add(this->glyph->Clone());
		}
		//3.2 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� �ٸ���
		else
		{
			//3.2.1 ���� ���� ���� ��ġ�� ���ڸ� ������ �߰��Ѵ�.
			currentLetterPos = this->notepadForm->current->
				Add(currentLetterPos, this->glyph->Clone());
		}
		//3.3 ������ġ�� ���� ���ں��� �տ� ��ġ�ϰ� �����Ѵ�.
		currentLetterPos = this->notepadForm->current->Previous();
	}
	//4. ���� �� ������ glyph�� ��(���๮��)�̸�
	else
	{
		//4.1. ���� �ٿ��� ���� ���� ���� ��ġ�� �ִ� ���ڵ��� ��� ���ο� ���� �����.
		this->glyph = this->notepadForm->current->Split(currentLetterPos);
		//4.2 ���� ���� ��ġ�� ��Ʈ�� ���� ����-1 �� ����(���� ���� ��ġ�� ������ ���̸�)
		if (currentRowPos == this->notepadForm->note->GetLength() - 1)
		{
			//4.2.1 ���ο� ���� ������ �� ������ �߰��Ѵ�.
			currentRowPos = this->notepadForm->note->Add(this->glyph->Clone());
		}
		//4.3 �װ� �ƴϸ�
		else
		{
			//4.3.1 ���ο� ���� ���� ���� ���� ��ġ�� ���� �ִ´�.
			currentRowPos = this->notepadForm->note->
				Add(currentRowPos + 1, this->glyph->Clone());
		}
		//4.4 ���� ���� ���� �����Ѵ�.(���� ������ ���� �ƴ϶� �и��� ���� ���� �ٷ� �Ѵ�.)
		currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//4.5 ���� ���� ���� ��ġ�� ���������� �̵���Ų��.
		this->notepadForm->current->Last();
		//4.6 �ڵ� �� �ٲ��� �������̸�
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//4.6.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
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
void DeleteKeyActionCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void DeleteKeyActionCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//SetRedone(�ٽ� �����̶�� ������)
void DeleteKeyActionCommand::SetRedone()
{
	this->isRedone = true;
}

//���� ��ġ ���ϱ�
Long DeleteKeyActionCommand::GetRowIndex()
{
	return this->rowIndex;
}
//���� ��ġ ���ϱ�
Long DeleteKeyActionCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//������� ������������ ���ϱ�
bool DeleteKeyActionCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool DeleteKeyActionCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//�ٽý������� ���� ���ϱ�
bool DeleteKeyActionCommand::IsRedone()
{
	return this->isRedone;
}

//�Ҹ��� ����
DeleteKeyActionCommand::~DeleteKeyActionCommand()
{

}