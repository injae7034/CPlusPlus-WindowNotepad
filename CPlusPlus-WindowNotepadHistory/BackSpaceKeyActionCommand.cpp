#include "BackSpaceKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "RowAutoChange.h"
#include "DummyRow.h"
#include "Note.h"
#include "SelectingTexts.h"

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
	this->isDirty = false;//ó���� ������ ���� ��������� �����Ƿ� false�� ����Ʈ����.
	this->selectedStartXPos = 0;
	this->selectedStartYPos = 0;
}

//����
void BackSpaceKeyActionCommand::Execute()
{
	//1. RowAutoChange�� �����Ѵ�.
	RowAutoChange rowAutoChange(this->notepadForm);
	Long changedRowPos = 0;
	Long changedLetterPos = 0;
	Long originRowPos = this->rowIndex;
	Long originLetterPos = this->letterIndex;
	//2. ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//3. BackSpaceKeyActionCommand�� �ٽ� ����Ǹ�
	if (this->isRedone == true)
	{
		//3.1 ���� ���� ��ġ�� ������ġ�� ���������ش�.
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
		//3.2 �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� ����Ǿ� ����)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//3.2.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//3.2.2 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
			currentRowPos = this->notepadForm->note->Move(changedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
		}
	}
	//4. �޸��忡�� ���õ� texts�� ������
	if (this->notepadForm->isSelecting == false)
	{
		//4.1 ���� ���� ���Ѵ�.
		Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
		Glyph* letter = 0;
		//4.2 ���� ���� ��ġ�� 0�̰�, ���� ���� ��ġ�� 0�̸�
		if (currentRowPos == 0 && currentLetterPos == 0)
		{
			//4.2.1 ó�� �����̸�
			if (this->isRedone == false)
			{
				//4.2.1.1 Command�� ���� ������ ������ ǥ���Ѵ�.
				this->isDirty = false;
			}
		}
		//4.3 ���� ���� ��ġ�� 0���� ũ��, ���� ���� ��ġ�� 0�̸�
		else if (currentRowPos > 0 && currentLetterPos == 0)
		{
			//4.3.1 ���� ���� ��¥ ���̸�
			if (!dynamic_cast<DummyRow*>(currentRow))
			{
				//4.3.1.1 ó�� �����̸�
				if (this->isRedone == false)
				{
					//4.3.1.1.1 Row�� �����Ѵ�.
					this->glyph = new Row();
				}
				//4.3.1.2 ���� ���� ���� ���� ���Ѵ�.
				Glyph* previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
				//4.3.1.3 ���� ���� ���� ���� ������ ���� ��ġ�� ���Ѵ�.
				Long letterPos = previousRow->GetLength();
				//4.3.1.4 ���� ���� ���� �ٿ� ��ģ��.
				currentRow->Join(previousRow);
				//4.3.1.5 Note���� ���� ���� �ּҸ� �����.
				this->notepadForm->note->Remove(currentRowPos);
				//4.3.1.6 ���� ���� �������� ������ ���� ���� �����Ѵ�.
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				//4.3.1.7 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
				//���� ���� ������ ���� ���� ó�� ���̿� ��ġ�ϵ��� �����Ѵ�.
				currentLetterPos = this->notepadForm->current->Move(letterPos);
			}
			//4.3.2 ���� ���� ��¥���̸�
			else
			{
				//4.3.2.1 ���� �ٷ� �̵��Ѵ�.
				currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				//4.3.2.2 ó�� ������ �Ǹ�
				if (this->isRedone == false)
				{
					//4.3.2.2.1 ���� ���ڸ� ���Ѵ�.
					letter = this->notepadForm->current->
						GetAt(this->notepadForm->current->GetLength() - 1);
					//4.3.2.2.2 ���� ���ڸ� ���� �����ؼ� �����Ѵ�.
					this->glyph = letter->Clone();
				}
				//4.3.2.3 ���� ���� ������ ���ڸ� �����.
				this->notepadForm->current->Remove(this->notepadForm->current->GetLength() - 1);
				//4.3.2.4 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				//��¥���� �ִٴ°� �ڵ������� �������̶�� �ǹ���.
				this->notepadForm->SendMessage(WM_SIZE);
			}
			///4.3.3 Command�� ���� ������ ������ ǥ���Ѵ�.
			this->isDirty = true;
		}
		//4.4 �� �̿ܿ���
		else
		{
			//4.4.1 ó�� ������ �Ǹ�
			if (this->isRedone == false)
			{
				//4.4.1.1 ���� ���ڸ� ���Ѵ�.
				letter = this->notepadForm->current->GetAt(currentLetterPos - 1);
				//4.4.1.2 ���� ���ڸ� ���� �����ؼ� �����Ѵ�.
				this->glyph = letter->Clone();
			}
			//4.4.2 ���� ���ڸ� �����.
			this->notepadForm->current->Remove(currentLetterPos - 1);
			//4.4.3 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//4.4.3.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
			///4.4.4 Command�� ���� ������ ������ ǥ���Ѵ�.
			this->isDirty = true;
		}
	}
	//5. �޸��忡�� ���õ� texts�� ������
	else
	{
		//5.1 �ٽ� �����̸�
		if (this->isRedone == true)
		{
			//5.1.1 notepadForm�� ������ ���۵Ǵ� ���� ��ġ�� ���� ��ġ�� �缳�����ش�. 
			this->notepadForm->selectedStartYPos = this->selectedStartYPos;
			this->notepadForm->selectedStartXPos = this->selectedStartXPos;
		}
		Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//������ ���۵Ǵ� ��
		Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//������ ���۵Ǵ� ����
		Long selectedEndRowPos = currentRowPos;//������ ������ ��
		Long selectedEndLetterPos = currentLetterPos;//������ ������ ����
		//5.2 ó�� �����̸�
		if (this->isRedone == false)
		{
			//5.2.1 content�� �����ϰ� �����.
			this->glyph = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
				selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		}
		//5.3 �ٽ� �����̸�
		else
		{
			//5.3.1 content�� �����.
			this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
				selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		}
		//5.4 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�ǿ���ȿ��� ���� ���� ��ġ�� ���� ��ġ��
		//������ ������ notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
		this->notepadForm->current = this->notepadForm->note->
			GetAt(this->notepadForm->note->GetCurrent());
		//5.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//5.5.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
		//5.6 �޸��忡�� ���õ� texts�� �� ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = false;
		//5.7 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartXPos = 0;
		//5.8 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartYPos = 0;
		//5.9 �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		//5.10 Command�� ���� ������ ������ ǥ���Ѵ�.
		this->isDirty = true;
	}
	//6. Command�� ���� ������ ������
	if (this->isDirty == true)
	{
		//6.1 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//6.2 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->isDirty = true;
		//6.3 ���ڸ� ���� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
		//6.4 �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� �����ؾ���)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			Long changedRowPos = this->rowIndex;
			Long changedLetterPos = this->letterIndex;
			Long originRowPos = 0;
			Long originLetterPos = 0;
			//6.4.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			//6.4.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
			this->rowIndex = originRowPos;
			this->letterIndex = originLetterPos;
		}
	}
}

//�������
void BackSpaceKeyActionCommand::Unexecute()
{
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
	//5. ���� �� ������ glyph�� ��(���๮��)�� �ƴϰ� ���ÿ���(Note)�� �ƴϸ�
	if (!dynamic_cast<Row*>(this->glyph) && !dynamic_cast<Note*>(this->glyph))
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
		//5.3 ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		//currentRowPos = this->notepadForm->note->Move(currentRowPos + 1);
		//this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//currentLetterPos = this->notepadForm->current->First();
	}
	//6. ���� �� ������ glyph�� ��(���๮��)�̸�
	else if(dynamic_cast<Row*>(this->glyph))
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
	//7. ���� �� ������ glyph�� ��Ʈ(���ÿ���)�̸�
	else if (dynamic_cast<Note*>(this->glyph))
	{
		//7.1 ������ ����ǰ� �ִ� ���̾�����
		if (this->notepadForm->isSelecting == true)
		{
			//7.1.1 ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
			this->notepadForm->selectingTexts->Undo();
			//7.1.2 ������ ���� ���·� �ٲ۴�.
			this->notepadForm->isSelecting = false;
			//7.1.3 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartXPos = 0;
			//7.1.4 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartYPos = 0;
		}
		//7.2 �޸��忡�� ���õ� texts�� �ٽ� �����ϱ� ������ �޸��忡�� ������ �� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = true;
		//7.3 ������ �ٽ� �����߱� ������ ĳ���� x��ǥ�� command�� ����� ���� ���� ��ġ�� �����Ѵ�.
		this->notepadForm->selectedStartXPos = currentLetterPos;
		this->selectedStartXPos = this->notepadForm->selectedStartXPos;
		//7.4 ������ �ٽ� �����߱� ������ ĳ���� y��ǥ�� command�� ����� ���� ���� ��ġ�� �����Ѵ�.
		this->notepadForm->selectedStartYPos = currentRowPos;
		this->selectedStartYPos = this->notepadForm->selectedStartYPos;
		//7.5 ���� ��ġ�� command�� ������ �ִ� content�� �����Ѵ�.
		Long rowIndex = this->notepadForm->note->
			InsertTexts(currentRowPos, currentLetterPos, this->glyph);
		//7.6 �޸����� ���� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
		//7.7 �ڵ������� �������̸� �ٿ����� �ٵ��� �ڵ���������ش�.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//7.7.1 �ڵ�����Ŭ������ �����Ѵ�.
			RowAutoChange rowAutoChange(this->notepadForm);
			//7.7.2 �κ��ڵ������� �Ѵ�.
			Long endPastedRowPos = rowAutoChange.DoPartRows(currentRowPos, rowIndex);
			//7.7.3 �ٿ��ֱⰡ ������ �ٷ� �̵���Ų��.
			//�ٿ��ֱⰡ ������ ���� OnSize���� �κ��ڵ������� �ؼ� ó���Ǳ� ������ ĳ���� ��ġ�� �������ָ� ��!
			this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current->Move(currentLetterPos);
		}
		//7.8 ���ÿ����� �ٽ� ����� ������ �����ϱ�, �߶󳻱�, ���� �޴��� Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	}
	//8. ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//9. �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� �����ؾ���)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//9.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//9.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
	}
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

//������ ���۵Ǵ� ������ ��ġ���ϱ�
Long BackSpaceKeyActionCommand::GetSelectedStartXPos()
{
	return this->selectedStartXPos;
}

//������ ���۵Ǵ� ���� ��ġ���ϱ�
Long BackSpaceKeyActionCommand::GetSelectedStartYPos()
{
	return this->selectedStartYPos;
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

//��������� �ִ��� Ȯ�� ����
bool BackSpaceKeyActionCommand::IsDirty()
{
	return this->isDirty;
}

//�Ҹ��� ����
BackSpaceKeyActionCommand::~BackSpaceKeyActionCommand()
{
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}