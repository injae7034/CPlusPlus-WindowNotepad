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
	//1. �޸��忡�� ���õ� texts�� ������
	if (this->notepadForm->isSelecting == true)
	{
		//1.1 RemoveCommand�� �޼����� ������ ���ÿ����� �����.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
	Long currentRowPos = 0;
	//2. OnCharCommand�� �ٽ� ������ �ƴϸ�(ó�� ������ �Ǹ�)
	if (this->isRedone == false)
	{
		//2.1 ���� ���� ��ġ�� ������ġ�� �̵���Ų��.(ĳ���� �ٸ� ���� ��ġ�� ��� ���ڸ� �� ������ �̵�!)
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		this->notepadForm->current->Move(this->letterIndex);
	}
	//3. OnCharCommand�� �ٽ� ����Ǹ�
	else
	{
		//3.1 OnCharCommand�� ���ڰ� ���๮���̸�
		if (this->nChar == '\n' || this->nChar == '\r')
		{
			//3.1 OnCharCommand�� ���� ��ġ�� �� �� �������� �̵��Ѵ�.
			this->rowIndex--;
			currentRowPos = this->notepadForm->note->Move(this->rowIndex);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			this->letterIndex = this->notepadForm->current->Move(this->startSplitIndex);
		}
		//3.2 ���๮�ڰ� �ƴϸ�
		else
		{
			currentRowPos = this->notepadForm->note->Move(this->rowIndex);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			this->letterIndex--;
			this->notepadForm->current->Move(this->letterIndex);
		}
	}
	//3. glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	//4. glyph�� �����Ѵ�.
	Glyph* glyph = glyphCreator.Create((char*)&this->nChar);
	Long letterIndex;
	Long rowIndex;
	//5. �Է¹��� ���ڰ� ���๮�ڰ� �ƴϸ�
	if (this->nChar != '\n' && this->nChar != '\r')
	{
		//5.1 ���� ���� ĳ���� ���� ��ġ�� ���Ѵ�.
		letterIndex = this->notepadForm->current->GetCurrent();
		//5.2 FileSaveCommand�� ���� ���� length�� ������
		if (letterIndex == this->notepadForm->current->GetLength())
		{
			//5.2.1 ���� ���� ������ ���� �ڿ� ���ο� ���ڸ� �߰��Ѵ�.
			letterIndex = this->notepadForm->current->Add(glyph);
		}
		//5.3 index�� ���� ���� length�� �ٸ���
		else
		{
			//5.3.1 ���� ���� index��°�� ���ο� ���ڸ� ���� ����.
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
		//6.3. ���� �ٿ��� ���� ĳ�� ���� ��ġ�� �ִ� ���ڵ��� �����.
		glyph = this->notepadForm->current->Split(letterIndex);
		//6.4 rowIndex�� ��Ʈ�� ���� ����-1 �� ����(���� ���� ��ġ�� ������ ���̸�)
		if (rowIndex == this->notepadForm->note->GetLength() - 1)
		{
			//6.4.1 ���ο� ���� ������ �� ������ �߰��Ѵ�.
			rowIndex = this->notepadForm->note->Add(glyph);
		}
		//6.5 �װ� �ƴϸ�
		else
		{
			//6.5.1 ���ο� ���� ���� ���� ���� ��ġ�� �����ִ´�.
			rowIndex = this->notepadForm->note->Add(rowIndex + 1, glyph);
		}
		//6.4 ���� ���� ��ġ�� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
		//6.5 ���� ���� ĳ���� ��ġ�� ó������ �̵���Ų��.
		this->notepadForm->current->First();
		//6.6 �ڵ� �� �ٲ��� �������̸�
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//6.6.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
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
	//1. ���� ���� ��ġ�� �̵���Ų��.
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	//2. ���� ������ ��ġ�� �̵���Ų��.
	//������� ���� ������ġ���� ĳ���� �ڿ� �־�� ������ֱ� ������ letterIndex + 1�� ���ش�.
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
			//3.1.5 Note���� ���� ���� �ּҸ� �����.
			this->notepadForm->note->Remove(currentRowPos);
			//3.1.6 ���� �������� ������ rowPosBeforeUndo�� �� �� �մ���.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//3.1.7 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
			//���� ���� ������ ���� ���� ó�� ���̿� ��ġ�ϵ��� �����Ѵ�.
			Long index = this->notepadForm->current->First();
			while (index < letterPos)
			{
				this->notepadForm->current->Next();
				index++;
				//�̷����ϸ� index�� ���� overflow�� ���� �ʱ� ������ �ݺ����� ��� �� ���Եǰ�,
				//�׷� �ᱹ�� ���ѹݺ��� �ȴ�.!!
				//index = this->current->Next();
			}
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

//SetMacroEnd
void OnCharCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void OnCharCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}


//SetRedone
void OnCharCommand::SetRedone()
{
	this->isRedone = true;
}

//�Ҹ��� ����
OnCharCommand::~OnCharCommand()
{

}