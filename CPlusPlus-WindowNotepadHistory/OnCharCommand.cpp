#include "OnCharCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "RowAutoChange.h"
#include "DummyRow.h"
#include "TextExtent.h"

//����Ʈ������ ����
OnCharCommand::OnCharCommand(NotepadForm* notepadForm, Glyph* glyph)
	:Command(notepadForm)
{
	this->glyph = glyph;//NotepadForm�� OnChar���� �������� Glyph�� ������.
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isSelectedTextsRemoved = false;//ó���� ������ ���� ���ÿ����� �����������Ƿ� false�� ����Ʈ��
}

//Execute ����
void OnCharCommand::Execute()
{
	//1. RowAutoChange�� �����Ѵ�.
	RowAutoChange rowAutoChange(this->notepadForm);
	Long changedRowPos = 0;
	Long changedLetterPos = 0;
	Long originRowPos = this->rowIndex;
	Long originLetterPos = this->letterIndex;
	//���ÿ����� ������ ���ÿ��� ����
	//2. �޸��忡�� ���õ� texts�� ������
	if (this->notepadForm->isSelecting == true)
	{
		//2.1 RemoveCommand�� �޼����� ������ ���ÿ����� �����.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
		//2.2 Command���� ���ÿ����� ���������� ǥ���Ѵ�.
		this->isSelectedTextsRemoved = true;
	}
	//3. ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//4. OnCharCommand�� �ٽ� ����Ǹ�
	if (this->isRedone == true)
	{
		//4.1 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
		//4.2 �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� ����Ǿ� ����)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//4.2.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//4.2.2 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
			currentRowPos = this->notepadForm->note->Move(changedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
		}
	}
	//NotepadForm�� OnChar���� ������ ���ڸ� �����ͼ� ��Ʈ�� �ٿ� �߰���
	//5. �Է¹��� ���ڰ� ���๮�ڰ� �ƴϸ�
	if (!dynamic_cast<Row*>(this->glyph))
	{
		//5.1 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� ������
		if (currentLetterPos == this->notepadForm->current->GetLength())
		{
			//5.1.1 ���� ���� ������ ���� �ڿ� ���ο� ���ڸ� �߰��Ѵ�.
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
		//5.2 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� �ٸ���
		else
		{
			//5.2.1 ���� ���� ���� ��ġ�� ���ڸ� ������ �߰��Ѵ�.
			currentLetterPos = this->notepadForm->current->
				Add(currentLetterPos, this->glyph->Clone());
		}
	}
	//6. �Է¹��� ���ڰ� ���๮���̸�
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

//Unexcute
void OnCharCommand::Unexecute()
{
	//1. ���õ� ������ ������ �ʾ�����
	//if (this->isSelectedTextsRemoved == false)
	//{
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
		//5. �Է¹��� ���ڰ� ���๮�ڰ� �ƴϸ�
		if (!dynamic_cast<Row*>(this->glyph))
		{
			//5.1 ���� ���ڸ� �����.
			this->notepadForm->current->Remove(currentLetterPos - 1);
			//5.2 �ڵ� �� �ٲ��� �������̸�
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//5.2.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
		}
		//6. �Է¹��� ���ڰ� ���๮���̸�
		else
		{
			//6.1 ���� ���� ���Ѵ�.
			Glyph* currentRow = this->notepadForm->current;
			//6.2 ���� ���� ���� ���� ���Ѵ�.
			Glyph* previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
			//6.3 ���� ���� ���� ���� ������ ���� ��ġ�� ���Ѵ�.
			Long letterPos = previousRow->GetLength();
			//6.4 ���� ���� ���� �ٿ� ��ģ��.
			currentRow->Join(previousRow);
			//6.5 Note���� ���� ���� �ּҸ� �����.(������ ����� �ȵ�)
			this->notepadForm->note->Remove(currentRowPos);
			//6.6 ���� �������� ������ ���� ��ġ�� �ٽ� ���Ѵ�.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//6.7 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
			//���� ���� ������ ��ġ�� �̵���Ų��.
			Long index = this->notepadForm->current->Move(letterPos);
		}
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
		//10. �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� �����ؾ���)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			Long changedRowPos = this->rowIndex;
			Long changedLetterPos = this->letterIndex;
			Long originRowPos = 0;
			Long originLetterPos = 0;
			//10.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			//10.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
			this->rowIndex = originRowPos;
			this->letterIndex = originLetterPos;
		}
	//}
	//2. ���ÿ����� ����������
	//else
	//{

	//}
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
//���ÿ����� ���������� Ȯ�� ����
bool OnCharCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}

//�Ҹ��� ����
OnCharCommand::~OnCharCommand()
{
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}