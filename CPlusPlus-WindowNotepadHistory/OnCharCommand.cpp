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
}

//Execute ����
void OnCharCommand::Execute()
{
	this->notepadForm->commandHistory->current = this;

	//���ÿ����� ������ ���ÿ��� ����
	//1. �޸��忡�� ���õ� texts�� ������
	if (this->notepadForm->isSelecting == true)
	{
		//1.1 RemoveCommand�� �޼����� ������ ���ÿ����� �����.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
	//2. ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//�ٽ� �����̸� ĳ���� ��ġ�� ����
	//3. OnCharCommand�� �ٽ� ����Ǹ�
	if (this->isRedone == true)
	{
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
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
		//6.1 ������ ����� ���� �Ҵ������Ѵ�.
		if (this->glyph != 0)
		{
			delete this->glyph;
		}
		//6.2 ���� �ٿ��� ���� ���� ���� ��ġ�� �ִ� ���ڵ��� ��� ���ο� ���� ����� �����Ѵ�.
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
		//6.5 ���� ���� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//6.6 ���� ���� ���� ��ġ�� ó������ �̵���Ų��.
		this->notepadForm->current->First();
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
	//10.1 ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
#if 0
	//10. �ڵ����� ���� �ƴϸ�
	if (this->notepadForm->isRowAutoChanging == false)
	{
		//10.1 ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
	}
	//11. �ڵ����� ���̸�
	else
	{
#if 0
		//11.1 ���� ȭ���� ũ�⸦ ���Ѵ�.
		CRect rect;
		this->notepadForm->GetClientRect(&rect);
		//11.2 ���� ȭ���� ���� ���̸� ���Ѵ�.
		Long pageWidth = rect.Width();
		//11.3 ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//11.4 ���� ���� ���Ѵ�.
		Glyph* row = this->notepadForm->current;
		//11.5 ���� �ٿ��� ���� ���ڸ� �߰��ϱ� �� ��ġ������ contents ���α��̸� ���Ѵ�.
		Long rowTextWidth = this->notepadForm->textExtent->GetTextWidth
		(row->GetPartOfContent(currentLetterPos));
		//11.6 contents�� ���� ���̰� ȭ���� ���� ���̺��� ũ�ų� ������
		if (rowTextWidth >= pageWidth)
		{
			//11.6.1 command�� �� ��ġ�� ���� ���� ���� ���� �����Ѵ�.
			this->rowIndex = currentRowPos + 1;
			//11.6.2 command�� ���� ��ġ�� 1�� �����Ѵ�.
			this->letterIndex = 1;
		}
		//11.7 contents�� ���� ���̰� ȭ���� ���� ���̺��� ������
		else
		{
			//10.1 ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
			this->rowIndex = this->notepadForm->note->GetCurrent();
			this->letterIndex++;
		}
#endif

		//Long rowIndexBeforeRowAutoChange = 0;//�ڵ����� ���� ���� ��¥ ���� ��ġ
		//Long letterIndexBeforeRowAutoChange = 0;//�ڵ����� ���� ���� ��¥ ������ ��ġ
		//Long rowIndexAfterRowAutoChange = 0;//�ڵ����� �� ���� ��ġ
		//Long letterIndexAfterRowAutoChange = 0;//�ڵ� ���� �� ������ ��ġ
		
		Long realRowIndex = 0;//��¥ ���� ��ġ
		Long realLetterIndex = 0;//��¥ ���� ���� ��ġ
		//���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//���� ���Ѵ�.
		Glyph* row = this->notepadForm->current;
		//���� ��¥���� ���� �ݺ��Ѵ�. (��¥ ���� ��ġ ã��)
		Long i = currentRowPos;
		while (i >= 0 && dynamic_cast<DummyRow*>(row))
		{
			i--;
			row = this->notepadForm->note->GetAt(i);
		}
		if (i >= 0)
		{
			//��¥ ���� ��ġ�� �����Ѵ�.
			realRowIndex = i;
		}
		i = realRowIndex;
		Long length = 0;
		while (i < currentRowPos)
		{
			row = this->notepadForm->note->GetAt(i);
			length += (row->GetLength() - 1);
			i++;
		}
		length += currentLetterPos;
		realLetterIndex = length;
		this->rowIndex = realRowIndex;
		this->letterIndex = realLetterIndex;
#if 0
		//���� ��¥ �ٱ��� ���� ������ ����.
		i++;
		row = this->notepadForm->note->GetAt(i);
		while (i < this->notepadForm->note->GetLength() && dynamic_cast<DummyRow*>(row))
		{
			i++;
			row = this->notepadForm->note->GetAt(i);
		}
		//��¥ ���� ������ �ִ� ���� ������ ���Ѵ�.(�ڱ� �ڽ� + ��¥��)
		Long rowCount = i - realRowIndex;
		//��¥ ���� ������ ���Ѵ�.
		i = 0;
		Long j = realRowIndex;
		Long length = this->notepadForm->note->GetAt(j)->GetLength() - 1;
		Long realRowLength = length;
		while (i < rowCount)
		{
			j++;
			length = this->notepadForm->note->GetAt(j)->GetLength() - 1;
			realRowLength += length;
			i++;
		}
		realRowLength += 1;
#endif

	}
#endif
}

//Unexcute
void OnCharCommand::Unexecute()
{
	this->notepadForm->commandHistory->current = this;

	//4. ���� ���� ��ġ�� �̵���Ų��.(ĳ���� �ٸ� ���� ������ �� ���� ���ڰ� �������� ������)
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	//5. ���� ������ ��ġ�� �̵���Ų��.
	Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
#if 0
	//�ڵ����� �������̸�
	if (this->notepadForm->isRowAutoChanging == true)
	{
		RowAutoChange rowAutoChange(this->notepadForm);
		Long changedRowPos = 0;
		Long changedLetterPos = 0;
		Long originRowPos = this->rowIndex;
		Long originLetterPos = this->letterIndex;
		this->rowIndex = changedRowPos;
		this->letterIndex = changedLetterPos;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//5. ���� ������ ��ġ�� �̵���Ų��.
		currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	}
#endif
	//6. �Է¹��� ���ڰ� ���๮�ڰ� �ƴϸ�
	if (!dynamic_cast<Row*>(this->glyph))
	{
		//6.1 ���� ���ڸ� �����.
		this->notepadForm->current->Remove(currentLetterPos - 1);
		//6.2 �ڵ� �� �ٲ��� �������̸�
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//6.2.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
	//7. �Է¹��� ���ڰ� ���๮���̸�
	else
	{
		//7.1 ���� ���� ���Ѵ�.
		Glyph* currentRow = this->notepadForm->current;
		//7.2 ���� ���� ���� ���� ���Ѵ�.
		Glyph* previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
		//7.3 ���� ���� ���� ���� ������ ���� ��ġ�� ���Ѵ�.
		Long letterPos = previousRow->GetLength();
		//7.4 ���� ���� ���� �ٿ� ��ģ��.
		currentRow->Join(previousRow);
		//7.5 Note���� ���� ���� �ּҸ� �����.(������ ����� �ȵ�)
		this->notepadForm->note->Remove(currentRowPos);
		//7.6 ���� �������� ������ ���� ��ġ�� �ٽ� ���Ѵ�.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//7.7 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
		//���� ���� ������ ��ġ�� �̵���Ų��.
		Long index = this->notepadForm->current->Move(letterPos);
	}
	//8. �޸��� ���� *�� �߰��Ѵ�.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - �޸���";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//9. �޸��忡 ��������� ������ �����Ѵ�.
	this->notepadForm->isDirty = true;
	//10.1 ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
#if 0
	//10. �ڵ����� ���� �ƴϸ�
	if (this->notepadForm->isRowAutoChanging == false)
	{
		//10.1 ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
	}
	else
	{
		Long realRowIndex = 0;//��¥ ���� ��ġ
		Long realLetterIndex = 0;//��¥ ���� ���� ��ġ
		//���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//���� ���Ѵ�.
		Glyph* row = this->notepadForm->current;
		//���� ��¥���� ���� �ݺ��Ѵ�. (��¥ ���� ��ġ ã��)
		Long i = currentRowPos;
		while (i >= 0 && dynamic_cast<DummyRow*>(row))
		{
			i--;
			row = this->notepadForm->note->GetAt(i);
		}
		if (i >= 0)
		{
			//��¥ ���� ��ġ�� �����Ѵ�.
			realRowIndex = i;
		}
		i = realRowIndex;
		Long length = 0;
		while (i < currentRowPos)
		{
			row = this->notepadForm->note->GetAt(i);
			length += (row->GetLength() - 1);
			i++;
		}
		length += currentLetterPos;
		realLetterIndex = length;
		this->rowIndex = realRowIndex;
		this->letterIndex = realLetterIndex;
	}
#endif
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
//����ġ �����ϱ�
void OnCharCommand::SetRowIndex(Long rowIndex)
{
	this->rowIndex = rowIndex;
}
//������ġ �����ϱ�
void OnCharCommand::SetLetterIndex(Long letterIndex)
{
	this->letterIndex = letterIndex;
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
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}