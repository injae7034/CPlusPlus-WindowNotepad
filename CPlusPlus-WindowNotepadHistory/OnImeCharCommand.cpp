#include "OnImeCharCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"

//����Ʈ������ ����
OnImeCharCommand::OnImeCharCommand(NotepadForm* notepadForm, WPARAM wParam,
	Long rowIndex, Long letterIndex)
	:Command(notepadForm)
{
	this->wParam = wParam;
	this->rowIndex = rowIndex;
	this->letterIndex = letterIndex;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
}

//����
void OnImeCharCommand::Execute()
{
	Long currentRowPos = 0;
	//�ٽ� �����̸� ĳ���� ��ġ�� ����
	//2. OnCompositionCommand�� �ٽ� ����Ǹ�
	if (this->isRedone == true)
	{
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//2.2.1 ���ڰ� ������ �� ĭ �̵��� �Ǵµ�, ���ڰ� ��ҵǾ��� ������ �� ĭ �������� �̵���Ų��.
		this->letterIndex--;
		this->notepadForm->current->Move(this->letterIndex);
	}
	//3. glyphCreator�� �����Ѵ�.
	GlyphCreator glyphCreator;
	WORD word = LOWORD(this->wParam);
	char koreanLetter[3];
	koreanLetter[0] = HIBYTE(word);
	koreanLetter[1] = LOBYTE(word);
	koreanLetter[2] = '\0';
	//4. doubleByteLetter�� �����Ѵ�.
	Glyph* doubleByteLetter = glyphCreator.Create((char*)koreanLetter);
	//5. ���� ���� ĳ���� ���� ��ġ�� ���Ѵ�.
	Long index = this->notepadForm->current->GetCurrent();
	//5. ó�� �����̸�
	if (this->isRedone == false)
	{
		//6. ���� ���� ĳ���� ���� ��ġ �ٷ� �տ� �ִ� ���� �ѱ��� �����.
		// �׷��� ���ؼ��� ĳ���� ���� ���� ��ġ�� 1������ ���� �־��ָ� �ȴ�.
		this->notepadForm->current->Remove(index - 1);
		//7. ���ŵ� current�� ��ġ�� index�� �����Ѵ�.
		index = this->notepadForm->current->GetCurrent();
	}
	
	//8. index�� ���� ���� length�� ������
	if (index == this->notepadForm->current->GetLength())
	{
		//8.1 ���� ���� ������ ���� �ڿ� ���ο� �ѱ��� �߰��Ѵ�.
		index = this->notepadForm->current->Add(doubleByteLetter);
	}
	//9. index�� ���� ���� length�� �ٸ���
	else
	{
		//9.1 ���� ���� index��°�� ���ο� �ѱ��� ���� ����.
		index = this->notepadForm->current->Add(index, doubleByteLetter);
	}
	//10. IsComposing�� false�� �ٲ۴�.
	this->notepadForm->isComposing = false;
	//11. ĳ���� ����Ǿ����� �˸���.
	this->notepadForm->Notify();
	//12. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//13. �޸��忡 ��������� ������ �����Ѵ�.
	this->notepadForm->isDirty = true;
	//14. �����Ѵ�.
	this->notepadForm->Invalidate(TRUE);

	//12. ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
}

//�������
void OnImeCharCommand::Unexecute()
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
void OnImeCharCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void OnImeCharCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}


//SetRedone(�ٽ� �����̶�� ������)
void OnImeCharCommand::SetRedone()
{
	this->isRedone = true;
}

//���� ��ġ ���ϱ�
Long OnImeCharCommand::GetRowIndex()
{
	return this->rowIndex;
}
//���� ��ġ ���ϱ�
Long OnImeCharCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//������� ������������ ���ϱ�
bool OnImeCharCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool OnImeCharCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//�ٽý������� ���� ���ϱ�
bool OnImeCharCommand::IsRedone()
{
	return this->isRedone;
}

//�Ҹ��� ����
OnImeCharCommand::~OnImeCharCommand()
{

}