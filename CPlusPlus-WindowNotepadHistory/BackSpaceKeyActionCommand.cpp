#include "BackSpaceKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"

//����Ʈ������ ����
BackSpaceKeyActionCommand::BackSpaceKeyActionCommand(NotepadForm* notepadForm)
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
void BackSpaceKeyActionCommand::Execute()
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
	//2. �޸��忡�� ���õ� texts�� ������
	if (this->notepadForm->isSelecting == false)
	{
		//���� ù ���̸� ���� ���� �� ����, ���� ù �ٿ��� ���� ��ġ�� ���� ó���� ������
		//�ƹ��͵� ���Ͼ. ���� ���� ��ġ�� ���� ó���� �ƴϰ�, ���� ���� ��ġ�� ���� ó���̸�
		//���� ���� ���� �ٿ� Join��Ų��.
		//2.1 ���� ���� ��ġ�� 0���� ũ��, ���� ���� ��ġ�� 0�̸�
		Glyph* currentRow = 0;
		Glyph* previousRow = 0;
		if (currentRowPos > 0 && currentLetterPos == 0)
		{
			//2.1.1 ���� ���� ���Ѵ�.
			currentRow = this->notepadForm->note->GetAt(currentRowPos);
			//2.1.2 ���� ���� ���� �����Ѵ�.
			this->glyph = currentRow->Clone();
			//2.1.3 ���� ���� ���� ���� ���Ѵ�.
			previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
			//2.1.4 ���� ���� ���� ���� ������ ���� ��ġ�� ���Ѵ�.
			Long letterPos = previousRow->GetLength();
			//2.1.5 ���� ���� ���� �ٿ� ��ģ��.
			currentRow->Join(previousRow);
			//2.1.6 Note���� ���� ���� �ּҸ� �����.
			this->notepadForm->note->Remove(currentRowPos);
			//2.1.7 ���� ���� �������� ������ ���� ���� �����Ѵ�.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//2.1.8 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
			//���� ���� ������ ���� ���� ó�� ���̿� ��ġ�ϵ��� �����Ѵ�.
			Long index = this->notepadForm->current->Move(letterPos);
			
		}
		// ���� ���� ��ġ�� ó���� �ƴ� ��(���� ���� ó���̵� �ƴϵ� �������) ���� ���ڸ� �����.
		//2.2 ���� ���� ��ġ�� ó���� �ƴϸ�
		else if (currentLetterPos > 0)
		{
			//2.2.1 ���� ���ڸ� ����� ���� ���� ���ڸ� ���Ѵ�.
			Glyph* letter = this->notepadForm->current->GetAt(currentLetterPos - 1);
			//2.2.2 ���� ���ڸ� ���� �����ؼ� �����Ѵ�.
			this->glyph = letter->Clone();
			//2.2.3 ���� ���ڸ� �����.
			this->notepadForm->current->Remove(currentLetterPos - 1);
			//2.2.4 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//2.2.4.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
		}
		//2.3 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//2.4 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->isDirty = true;
		//2.5 ���ڸ� ���� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
	}
	//3. �޸��忡�� ���õ� texts�� ������
	else
	{
		//4.1 RemoveCommand�� �޼����� ������ ���ÿ����� �����.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
}

//�������
void BackSpaceKeyActionCommand::Unexecute()
{
	//1. ���� ���� ��ġ�� �̵���Ų��.(ĳ���� �ٸ� ���� ������ �� ���� ���ڰ� �������� ������)
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	//2. ���� ������ ��ġ�� �̵���Ų��.
	Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex);

	//NotepadForm�� OnChar���� ������ ���ڸ� �����ͼ� ��Ʈ�� �ٿ� �߰���
	//5. �Է¹��� ���ڰ� ���๮�ڰ� �ƴϸ�
	if (!dynamic_cast<Row*>(this->glyph))
	{
		//5.2 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� ������
		if (currentLetterPos == this->notepadForm->current->GetLength())
		{
			//5.2.1 ���� ���� ������ ���� �ڿ� ���ο� ���ڸ� �߰��Ѵ�.
			//���⼭ command�� ������ �ִ� glyph�� ���� �����ؼ� �ٿ� �߰����ش�.
			//Command�� glyph�� note�ʹ� ���� ������ �־�� ���߿� ������ �ȳ��� �Ҵ������ϱ� ���ϴ�.
			//�׸��� glyph�� ���� ������ command�̱� ������ note�� ���� ���縦 �ؼ� �������°� ���� �´�.
			//�׷� ���� note�� �Ҵ������� �� �ڱⰡ ���� ���縦 �� glyph�� �Ҵ������ϰ�,
			//command�� �Ҵ������� �� �ڱⰡ ������ �ִ� glyph�� �Ҵ����� �ϱ� ������ ��� �Ҵ���������
			//���� �Ӹ� ���� ���� ����, ������� command�� glyph�� �����ϱ� ���ϱ� ������ 
			//command�� note�ʹ� ������ �ڱ⸸�� glyph�� ������ �־���Ѵ�.
			//�ƴϸ� ���߿� Backspace�� ������� �ٽ� ���ڸ� �Է��� �� PushUndo���� ������ ����.!
			currentLetterPos = this->notepadForm->current->Add(this->glyph->Clone());
		}
		//5.3 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� �ٸ���
		else
		{
			//5.3.1 ���� ���� ���� ��ġ�� ���ڸ� ������ �߰��Ѵ�.
			currentLetterPos = this->notepadForm->current->
				Add(currentLetterPos, this->glyph->Clone());
		}
	}
	//6. �Է¹��� ���ڰ� ���๮���̸�
	else
	{
		//6.1 ���� ���� ��ġ�� ���Ѵ�.
		currentRowPos = this->notepadForm->note->GetCurrent();
		//6.2 ���� ���� ĳ���� ��ġ�� ���Ѵ�.
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//6.3. ���� �ٿ��� ���� ���� ���� ��ġ�� �ִ� ���ڵ��� ��� ���ο� ���� �����.
		this->glyph = this->notepadForm->current->Split(currentLetterPos);
		//6.4 ���� ���� ��ġ�� ��Ʈ�� ���� ����-1 �� ����(���� ���� ��ġ�� ������ ���̸�)
		if (currentRowPos == this->notepadForm->note->GetLength() - 1)
		{
			//6.4.1 ���ο� ���� ������ �� ������ �߰��Ѵ�.
			currentRowPos = this->notepadForm->note->Add(this->glyph->Clone());
		}
		//6.5 �װ� �ƴϸ�
		else
		{
			//6.5.1 ���ο� ���� ���� ���� ���� ��ġ�� ���� �ִ´�.
			currentRowPos = this->notepadForm->note->
				Add(currentRowPos + 1, this->glyph->Clone());
		}
		//6.6 ���� ���� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//6.7 ���� ���� ���� ��ġ�� ó������ �̵���Ų��.
		this->notepadForm->current->First();
		//7.8 �ڵ� �� �ٲ��� �������̸�
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//6.8.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
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
}

//SetMacroEnd(������� �� �ٽý��� ��ũ����� �������� ����)
void BackSpaceKeyActionCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void BackSpaceKeyActionCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}


//SetRedone(�ٽ� �����̶�� ������)
void BackSpaceKeyActionCommand::SetRedone()
{
	this->isRedone = true;
}

//���� ��ġ ���ϱ�
Long BackSpaceKeyActionCommand::GetRowIndex()
{
	return this->rowIndex;
}
//���� ��ġ ���ϱ�
Long BackSpaceKeyActionCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//������� ������������ ���ϱ�
bool BackSpaceKeyActionCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool BackSpaceKeyActionCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//�ٽý������� ���� ���ϱ�
bool BackSpaceKeyActionCommand::IsRedone()
{
	return this->isRedone;
}

//�Ҹ��� ����
BackSpaceKeyActionCommand::~BackSpaceKeyActionCommand()
{

}