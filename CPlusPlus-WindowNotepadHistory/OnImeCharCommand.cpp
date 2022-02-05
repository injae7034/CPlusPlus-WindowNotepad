#include "OnImeCharCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"

//����Ʈ������ ����
OnImeCharCommand::OnImeCharCommand(NotepadForm* notepadForm, Glyph* glyph)
	:Command(notepadForm)
{
	this->glyph = glyph;
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
}

//����
void OnImeCharCommand::Execute()
{
	//1. ���� ���� ���� ��ġ�� ���Ѵ�.
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//�ٽ� �����̸� ĳ���� ��ġ�� ����
	//2. OnCompositionCommand�� �ٽ� ����Ǹ�
	if (this->isRedone == true)
	{
		Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	}
	//3. ó�� �����̸�(�տ� �������� �ѱ��� �������ϰ�, �ٽ� �����̸� �տ� �������� �ѱ��� ����!)
	else
	{
		//3.1 ���� ���� ĳ���� ���� ��ġ �ٷ� �տ� �ִ� ���� �ѱ��� �����.
		// �׷��� ���ؼ��� ĳ���� ���� ���� ��ġ�� 1������ ���� �־��ָ� �ȴ�.
		this->notepadForm->current->Remove(currentLetterPos - 1);
		//3.2 ���ŵ� current�� ��ġ�� index�� �����Ѵ�.
		currentLetterPos = this->notepadForm->current->GetCurrent();
	}
	//4. currentRowPos�� ���� ���� length�� ������
	if (currentLetterPos == this->notepadForm->current->GetLength())
	{
		//4.1 ���� ���� ������ ���� �ڿ� ���ο� �ѱ��� �߰��Ѵ�.
		currentLetterPos = this->notepadForm->current->Add(this->glyph->Clone());
	}
	//5. currentRowPos�� ���� ���� length�� �ٸ���
	else
	{
		//5.1 ���� ���� currentRowPos��°�� ���ο� �ѱ��� ���� ����.
		currentLetterPos = this->notepadForm->current->Add(currentLetterPos, 
			this->glyph->Clone());
	}
	//6. IsComposing�� false�� �ٲ۴�.
	this->notepadForm->isComposing = false;
	//7. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//8. �޸��忡 ��������� ������ �����Ѵ�.
	this->notepadForm->isDirty = true;
	//9. ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
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
	//3. ���� ���ڸ� �����.
	this->notepadForm->current->Remove(currentLetterPos - 1);
	//4. �ڵ� �� �ٲ��� �������̸�
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//4.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
		//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
		this->notepadForm->SendMessage(WM_SIZE);
	}
	//5. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//6. �޸��忡 ��������� ������ �����Ѵ�.
	this->notepadForm->isDirty = true;
	//7. ���ڸ� ���� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
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
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}