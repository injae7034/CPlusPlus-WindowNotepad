#include "OnCharCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"

//����Ʈ������ ����
OnCharCommand::OnCharCommand(NotepadForm* notepadForm, UINT nChar,
	Long rowIndex, Long letterIndex)
	:Command(notepadForm)
{
	this->nChar = nChar;
	this->rowIndex = rowIndex;
	this->letterIndex = letterIndex;
	this->startSplitIndex = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
}

//Execute ����
void OnCharCommand::Execute()
{
	//���ÿ����� ������ ���ÿ��� ����
	//1. �޸��忡�� ���õ� texts�� ������
	if (this->notepadForm->isSelecting == true)
	{
		//1.1 RemoveCommand�� �޼����� ������ ���ÿ����� �����.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
	Long currentRowPos = 0;

	//�ٽ� �����̸� ĳ���� ��ġ�� ����
	//2. OnCharCommand�� �ٽ� ����Ǹ�
	if (this->isRedone == true)
	{
		//2.1 OnCharCommand�� ���ڰ� ���๮���̸�
		if (this->nChar == '\n' || this->nChar == '\r')
		{
			//2.1.1 OnCharCommand�� ���� ��ġ�� �� �� �������� �̵��Ѵ�.(������ �� �� ���� ��ġ��
			//�� �� �����Ǿ���, ��ҵǾ��� ������ �ٽ� �����ϱ� ���� �� �� �������� �̵����Ѿ���)
			this->rowIndex--;
			currentRowPos = this->notepadForm->note->Move(this->rowIndex);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//2.1.2 ������ ��ҵǾ��� ������ �ٽ� ������ �Ǳ� ���� ĭ�� ���� ��ġ�� �̵���Ų��.
			this->letterIndex = this->notepadForm->current->Move(this->startSplitIndex);
		}
		//2.2 OnCharCommand�� ���ڰ� ���๮�ڰ� �ƴϸ�
		else
		{
			currentRowPos = this->notepadForm->note->Move(this->rowIndex);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//2.2.1 ���ڰ� ������ �� ĭ �̵��� �Ǵµ�, ���ڰ� ��ҵǾ��� ������ �� ĭ �������� �̵���Ų��.
			this->letterIndex--;
			this->notepadForm->current->Move(this->letterIndex);
		}
	}

	//���ڻ����ؼ� ��Ʈ�� �ٿ� �߰���
	//3. glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	//4. glyph�� �����Ѵ�.
	Glyph* glyph = glyphCreator.Create((char*)&this->nChar);
	Long letterIndex;
	Long rowIndex;
	//5. �Է¹��� ���ڰ� ���๮�ڰ� �ƴϸ�
	if (this->nChar != '\n' && this->nChar != '\r')
	{
		//5.1 ���� ���� ���� ��ġ�� ���Ѵ�.
		letterIndex = this->notepadForm->current->GetCurrent();
		//5.2 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� ������
		if (letterIndex == this->notepadForm->current->GetLength())
		{
			//5.2.1 ���� ���� ������ ���� �ڿ� ���ο� ���ڸ� �߰��Ѵ�.
			letterIndex = this->notepadForm->current->Add(glyph);
		}
		//5.3 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� �ٸ���
		else
		{
			//5.3.1 ���� ���� ���� ��ġ�� ���ڸ� ������ �߰��Ѵ�.
			letterIndex = this->notepadForm->current->Add(letterIndex, glyph);
		}
	}
	//6. �Է¹��� ���ڰ� ���๮���̸�
	else if (this->nChar == '\n' || this->nChar == '\r')
	{
		//6.1 ���� ���� ��ġ�� ���Ѵ�.
		rowIndex = this->notepadForm->note->GetCurrent();
		//6.2 ���� ���� ĳ���� ��ġ�� ���Ѵ�.
		letterIndex = this->notepadForm->current->GetCurrent();
		this->startSplitIndex = letterIndex;
		//6.3. ���� �ٿ��� ���� ���� ���� ��ġ�� �ִ� ���ڵ��� ��� ���ο� ���� �����.
		glyph = this->notepadForm->current->Split(letterIndex);
		//6.4 ���� ���� ��ġ�� ��Ʈ�� ���� ����-1 �� ����(���� ���� ��ġ�� ������ ���̸�)
		if (rowIndex == this->notepadForm->note->GetLength() - 1)
		{
			//6.4.1 ���ο� ���� ������ �� ������ �߰��Ѵ�.
			rowIndex = this->notepadForm->note->Add(glyph);
		}
		//6.5 �װ� �ƴϸ�
		else
		{
			//6.5.1 ���ο� ���� ���� ���� ���� ��ġ�� ���� �ִ´�.
			rowIndex = this->notepadForm->note->Add(rowIndex + 1, glyph);
		}
		//6.6 ���� ���� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
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
	//7. ĳ���� ��ġ�� ũ�Ⱑ ����Ǿ����� �˸���.
	this->notepadForm->Notify();
	//8. isComposing�� false�� �ٲ۴�.
	this->notepadForm->isComposing = false;
	//9. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//10. �޸��忡 ��������� ������ �����Ѵ�.
	this->notepadForm->isDirty = true;
	//11. �����Ѵ�.
	this->notepadForm->Invalidate(TRUE);
	//12. ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
}

//Unexcute
void OnCharCommand::Unexecute()
{
	//1. ���� ���� ��ġ�� �̵���Ų��.(ĳ���� �ٸ� ���� ������ �� ���� ���ڰ� �������� ������)
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	//2. ���� ������ ��ġ�� �̵���Ų��.
	Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
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
			//3.1.2 ���� ���� ���� ���� ���Ѵ�.
			previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
			//3.1.3 ���� ���� ���� ���� ������ ���� ��ġ�� ���Ѵ�.
			Long letterPos = previousRow->GetLength();
			//3.1.4 ���� ���� ���� �ٿ� ��ģ��.
			currentRow->Join(previousRow);
			//3.1.5 Note���� ���� ���� �ּҸ� �����.(������ ����� �ȵ�)
			this->notepadForm->note->Remove(currentRowPos);
			//3.1.6 ���� �������� ������ ���� ��ġ�� �ٽ� ���Ѵ�.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//3.1.7 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
			//���� ���� ������ ��ġ�� �̵���Ų��.
			Long index = this->notepadForm->current->Move(letterPos);
			//3.1.8 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.1.9 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->isDirty = true;
		}
		// ���� ���� ��ġ�� ó���� �ƴ� ��(���� ���� ó���̵� �ƴϵ� �������) ���� ���ڸ� �����.
		//3.2 ���� ���� ��ġ�� ó���� �ƴϸ�
		else if (currentLetterPos > 0)
		{
			//3.2.1 ���� ���ڸ� �����.
			this->notepadForm->current->Remove(currentLetterPos - 1);
			//3.2.2 �޸��� ���� *�� �߰��Ѵ�.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.2.3 �޸��忡 ��������� ������ �����Ѵ�.
			this->notepadForm->isDirty = true;
			//3.2.4 �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
			UINT state = this->notepadForm->GetMenu()->
				GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
			//3.2.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
			if (state == MF_CHECKED)
			{
				//3.2.5.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
		}
	}
}

//SetMacroEnd(������� �� �ٽý��� ��ũ����� �������� ����)
void OnCharCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void OnCharCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//SetRedone(�ٽ� �����̶�� ������)
void OnCharCommand::SetRedone()
{
	this->isRedone = true;
}

//���� ��ġ ���ϱ�
Long OnCharCommand::GetRowIndex()
{
	return this->rowIndex;
}
//���� ��ġ ���ϱ�
Long OnCharCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//������� ������������ ���ϱ�
bool OnCharCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool OnCharCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//�ٽý������� ���� ���ϱ�
bool OnCharCommand::IsRedone()
{
	return this->isRedone;
}

//�Ҹ��� ����
OnCharCommand::~OnCharCommand()
{

}