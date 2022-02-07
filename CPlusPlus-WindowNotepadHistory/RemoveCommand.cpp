#include "RemoveCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"
#include "RowAutoChange.h"
#include "SelectingTexts.h"

//����Ʈ������
RemoveCommand::RemoveCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->glyph = 0;
	this->isRedone = false;
	this->startYPos = notepadForm->note->GetCurrent();
	this->startXPos = notepadForm->current->GetCurrent();
	this->selectedEndYPos = 0;
	this->selectedEndXPos = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isSelectedTextsRemoved = true;//������ ���ÿ����� ����� ������ default���� true
}

//ó�� ���� �� �ٽ� ����
void RemoveCommand::Execute()
{
	//1. RowAutoChange�� �����Ѵ�.
	RowAutoChange rowAutoChange(this->notepadForm);

	Long changedRowPos = 0;//�ڵ����� �� �� ��ġ
	Long changedLetterPos = 0;//�ڵ����� �� ���� ��ġ
	Long originRowPos = 0;//�ڵ����� �� �� ��ġ
	Long originLetterPos = 0;//�ڵ����� �� ���� ��ġ
	Long currentRowPos = 0;//���� ���� ��ġ
	Long currentLetterPos = 0;//���� ������ ��ġ
	Long selectedStartRowPos = 0;//������ ���۵Ǵ� �� ��ġ
	Long selectedStartLetterPos = 0;//������ ���۵Ǵ� ���� ��ġ
	Long selectedEndRowPos = 0;//������ ������ �� ��ġ
	Long selectedEndLetterPos = 0;//������ ������ ���� ��ġ

	//ó�������� �ƴϸ�
	if (this->isRedone == true)
	{
		//1. ������ ����ǰ� �ִ� ���̾�����
		if (this->notepadForm->isSelecting == true)
		{
			//1.1. ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
			this->notepadForm->selectingTexts->Undo();
			//1.2 ������ ���� ���·� �ٲ۴�.
			this->notepadForm->isSelecting = false;
			//1.3 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartXPos = 0;
			//1.4 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartYPos = 0;
		}
	}

	//2. RemoveCommand�� ó�� ����Ǹ�
	if (this->isRedone == false)
	{
		//2.1 ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
		currentRowPos = this->notepadForm->note->GetCurrent();
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//2.2 ������ ���۵Ǵ� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
		selectedStartRowPos = this->notepadForm->selectedStartYPos;
		this->startYPos = selectedStartRowPos;
		selectedStartLetterPos = this->notepadForm->selectedStartXPos;
		this->startXPos = selectedStartLetterPos;
		//2.3 ������ ������ ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
		selectedEndRowPos = currentRowPos;
		this->selectedEndYPos = selectedEndRowPos;
		selectedEndLetterPos = currentLetterPos;
		this->selectedEndXPos = selectedEndLetterPos;
		//2.4 ���õ� �ܾ�(ã�� �ܾ�)�� command�� �����ؼ� �����ϰ� �����.
		this->glyph = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
			selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		//2.5 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//2.6 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->isDirty = true;
	}
	//3. RemoveCommand�� �ٽ� ����Ǹ�
	else
	{
		//3.1 �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ��
		//����Ǿ� ����)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//3.1.1 ����� ȭ�� ũ�⿡ �´� ���� �ٰ� ���� ��ġ�� ���Ѵ�.
			changedRowPos = 0;
			changedLetterPos = 0;
			originRowPos = this->selectedEndYPos;
			originLetterPos = this->selectedEndXPos;
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//3.1.2 �ڵ������� �ݿ��ؼ� ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
			currentRowPos = this->notepadForm->note->Move(changedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
			//3.1.3 �ڵ������� �ݿ��ؼ� ������ ������ ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
			selectedEndRowPos = currentRowPos;
			selectedEndLetterPos = currentLetterPos;
			//3.1.4 ����� ȭ�� ũ�⿡ �´� ������ ���۵Ǵ� �ٰ� ���� ��ġ�� ���Ѵ�.
			changedRowPos = 0;
			changedLetterPos = 0;
			originRowPos = this->startYPos;
			originLetterPos = this->startXPos;
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//3.1.5 �ڵ������� �ݿ��ؼ� ������ ���۵Ǵ� �ٰ� ���� ��ġ�� �ٽ� �����Ѵ�.
			selectedStartRowPos = changedRowPos;//������ ���۵Ǵ� ��
			selectedStartLetterPos = changedLetterPos;//������ ���۵Ǵ� ����
		}
		//3.2 �ڵ����� ���� ���� �ƴϸ�
		else
		{
			//3.2.1 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
			currentRowPos = this->notepadForm->note->Move(this->selectedEndYPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->Move(this->selectedEndXPos);
			//3.2.2 ������ ���۵Ǵ� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
			selectedStartRowPos = this->startYPos;
			selectedStartLetterPos = this->startXPos;
			//3.2.3 ������ ������ ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
			selectedEndRowPos = currentRowPos;
			selectedEndLetterPos = currentLetterPos;
		}
		//3.3 ���õ� �ܾ�(ã�� �ܾ�)�� �����.
		this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
			selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
	}
	//4. ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�� ���� �ȿ��� 
	//���� ���� ��ġ�� ���� ��ġ�� ������ ������ notepadForm�� current(������)�� 
	//������ �� ��� notepadForm���� ���ش�.)
	currentRowPos = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//5. ���õ� �ܾ ������ ������ �ٽ� �����ϴ� ���� ��ġ�� ������ġ�� �����Ѵ�.
	this->startYPos = currentRowPos;
	this->startXPos = currentLetterPos;
	//6. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������(���ÿ����� ���� ��� ������ �ڵ������� �ٽ��������)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//6.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
		//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
		this->notepadForm->SendMessage(WM_SIZE);
		//6.2 ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ��
		//�ٲ� ���ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
		this->startYPos = this->notepadForm->note->GetCurrent();
		this->startXPos = this->notepadForm->current->GetCurrent();
		//6.3 ���� ���� startYPos�� startXPos�� ��ġ�� �ڵ������� ����� ������ ��ġ�̱�
		//������ �ڵ������� ���� ���¿��� �����ϱⰡ ���۵Ǵ� ��¥ ���� ��ġ�� ������ġ�� ���Ѵ�.
		changedRowPos = this->startYPos;
		changedLetterPos = this->startXPos;
		originRowPos = 0;
		originLetterPos = 0;
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		this->startYPos = originRowPos;
		this->startXPos = originLetterPos;
	}
	//7. �޸��忡�� ���õ� �ܾ ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
	this->notepadForm->isSelecting = false;
	//8. ������ ������ ������ ������ ���۵Ǵ� x��ǥ�� 0���� �����Ѵ�.
	this->notepadForm->selectedStartXPos = 0;
	//9. ������ ������ ������ ������ ���۵Ǵ� y��ǥ�� 0���� �����Ѵ�.
	this->notepadForm->selectedStartYPos = 0;
	//10. ���õ� ������ �������� ������ �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
}

//�������
void RemoveCommand::Unexecute()
{
	//1. ������ ����ǰ� �ִ� ���̾�����
	if (this->notepadForm->isSelecting == true)
	{
		//1.1. ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
		this->notepadForm->selectingTexts->Undo();
		//1.2 ������ ���� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = false;
		//1.3 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartXPos = 0;
		//1.4 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartYPos = 0;
	}
	//2. RowAutoChange�� �����Ѵ�.
	RowAutoChange rowAutoChange(this->notepadForm);

	Long changedRowPos = 0;//�ڵ����� �� �� ��ġ
	Long changedLetterPos = 0;//�ڵ����� �� ���� ��ġ
	Long originRowPos = 0;//�ڵ����� �� �� ��ġ
	Long originLetterPos = 0;//�ڵ����� �� ���� ��ġ
	Long currentRowPos = 0;//���� ���� ��ġ
	Long currentLetterPos = 0;//���� ������ ��ġ
	Long selectedStartRowPos = 0;//������ ���۵Ǵ� �� ��ġ
	Long selectedStartLetterPos = 0;//������ ���۵Ǵ� ���� ��ġ
	Long selectedEndRowPos = 0;//������ ������ �� ��ġ
	Long selectedEndLetterPos = 0;//������ ������ ���� ��ġ

	//3. �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� ����Ǿ� ����)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//3.1 �ڵ������� �ݿ��ؼ� �ٿ��ֱⰡ ���۵Ǵ� �ٰ� ���� ��ġ�� ���Ѵ�.
		changedRowPos = 0;
		changedLetterPos = 0;
		originRowPos = this->startYPos;
		originLetterPos = this->startXPos;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//3.2 �ڵ������� �ݿ��ؼ� ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		currentRowPos = this->notepadForm->note->Move(changedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
	}
	//4. �ڵ����� ���� ���� �ƴϸ�
	else
	{
		//4.1 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		currentRowPos = this->notepadForm->note->Move(this->startYPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->startXPos);
	}
	//5. �ٲ۴ܾ ������ ���� ���ÿ����� �ٽ� �����ϱ� ������ ���� �ٰ� ������ ��ġ��
	//���ÿ����� ���� �ٰ� ���� ��ġ�� �����Ѵ�.
	this->notepadForm->selectedStartYPos = currentRowPos;
	this->notepadForm->selectedStartXPos = currentLetterPos;
	//6. �޸��忡�� ���õ� texts�� �ٽ� �����ϱ� ������ �޸��忡�� ������ �� ���·� �ٲ۴�.
	this->notepadForm->isSelecting = true;
	//7. ���ÿ����� �ٽ� ����� ������ �����ϱ�, �߶󳻱�, ���� �޴��� Ȱ��ȭ ��Ų��.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	//8. ���ÿ����� �ٽ� �����ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = currentRowPos;
	//9. ���ÿ����� �ٽ� �����ϰ� ���� ���� ��ġ�� ��ȯ�޴´�.
	currentRowPos = this->notepadForm->note->
		InsertTexts(currentRowPos, currentLetterPos, this->glyph);
	//10. �޸����� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//11. �޸��忡 ���ÿ����� �����Ǿ��� ������ ������ ������ �ٰ� ������ġ�� �����Ѵ�.
	this->selectedEndYPos = currentRowPos;
	this->selectedEndXPos = currentLetterPos;
	//12. �ڵ������� �������̸� �ٿ����� �ٵ��� �ڵ���������ش�.
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//12.1 �κ��ڵ������� �Ѵ�.(������ ���� �����ϰ� �ڵ�������)
		Long endPastedRowPos = rowAutoChange.DoPartRows(previousRowIndex, currentRowPos);
		//12.2 ���ÿ����� ������ �ٷ� �̵���Ų��.
		//���ÿ����� ������ ���� OnSize���� �κ��ڵ������� �ؼ� ó���Ǳ� ������ ĳ���� ��ġ�� �������ָ� ��!
		currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		this->notepadForm->current->Move(currentLetterPos);
		//12.3 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
		//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. (������ ���� �ڵ������Ŵ)
		this->notepadForm->SendMessage(WM_SIZE);
		//12.4 �޸����� ���� ���� �����Ѵ�.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//12.5 �޸��忡 ���ÿ����� �����Ǿ���, �ڵ������� �ݿ��� ������ ������ �ٰ� ������ġ�� �����Ѵ�. 
		this->selectedEndYPos = currentRowPos;
		this->selectedEndXPos = currentLetterPos;
		//12.6 ���� ���� ������ ������ �ٰ� ���� ��ġ�� �ڵ������� ����� ������ ��ġ�̱� ������
		//�ڵ������� ���� ���¿��� �����ϱⰡ ������ ��¥ ���� ��ġ�� ������ġ�� ���Ѵ�.
		changedRowPos = this->selectedEndYPos;
		changedLetterPos = this->selectedEndXPos;
		originRowPos = 0;
		originLetterPos = 0;
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		this->selectedEndYPos = originRowPos;
		this->selectedEndXPos = originLetterPos;
	}
}

//�ٽý������� ���� ���ϱ�
bool RemoveCommand::IsRedone()
{
	return this->isRedone;
}

//�����ϴ� ���� ��ġ���ϱ�
Long RemoveCommand::GetStartYPos()
{
	return this->startYPos;
}
//�����ϴ� ������ ��ġ���ϱ�
Long RemoveCommand::GetStartXPos()
{
	return this->startXPos;
}
//������ ������ ���� ��ġ ���ϱ�
Long RemoveCommand::GetSelectedEndYPos()
{
	return this->selectedEndYPos;
}
//������ ������ ���� ��ġ ���ϱ�
Long RemoveCommand::GetSelectedEndXPos()
{
	return this->selectedEndXPos;
}

//SetMacroEnd(������� �� �ٽý��� ��ũ����� �������� ����)
void RemoveCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void RemoveCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//������� ������������ ���ϱ�
bool RemoveCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool RemoveCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}

//SetRedone(�ٽ� �����̶�� ������)
void RemoveCommand::SetRedone()
{
	this->isRedone = true;
}

//���ÿ����� ���������� Ȯ�� ����
bool RemoveCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}

Glyph* RemoveCommand::GetGlyph()
{
	return this->glyph;
}

//�Ҹ���
RemoveCommand::~RemoveCommand()
{
	//������ Note�� �Ҵ������Ѵ�.
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}