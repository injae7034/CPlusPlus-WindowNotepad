#include "OnReplaceButtonClickedCommand.h"
#include "CommandHistory.h"
#include "Row.h"
#include "RowAutoChange.h"
#include "DummyRow.h"
#include "TextExtent.h"
#include "SelectingTexts.h"
#include "ReplacingDialog.h"

//����Ʈ������
OnReplaceButtonClickedCommand::OnReplaceButtonClickedCommand(NotepadForm* notepadForm)
	:Command(notepadForm), 
	replacingKeyword(((ReplacingDialog*)(this->notepadForm->findReplaceDialog))->replacingKeyword)
{
	this->note = 0;//���õ� texts�� ������ ����� ���� ������ ����
	this->startYPos = notepadForm->selectedStartYPos;
	this->startXPos = notepadForm->selectedStartXPos;
	this->selectedEndYPos = notepadForm->note->GetCurrent();
	this->selectedEndXPos = notepadForm->current->GetCurrent();
	this->pastingEndYPos = 0;
	this->pastingEndXPos = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isSelectedTextsRemoved = true;//������ ���ÿ����� ����� ������ default���� true
}

//ó�� �� �ٽ� ����(���ÿ����� �׻� �ִ� ���¿��� �����)
void OnReplaceButtonClickedCommand::Execute()
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

	//2. OnReplaceButtonClickedCommand�� ó�� ����Ǹ�
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
		this->note = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
			selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		//2.5 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//2.6 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->isDirty = true;
		//2.7 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�� ���� �ȿ��� ���� ���� ��ġ�� ���� 
		//��ġ�� ������ ������ notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//2.8 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������(���ÿ����� ���� ��� ������ �ڵ������� �ٽ��������)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//2.8.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
			//2.8.2 �ڵ������� ���� ������ �����ϱⰡ ���۵Ǵ� ��¥ ���� ��ġ�� ������ġ�� ���Ѵ�.
			changedRowPos = this->startYPos;
			changedLetterPos = this->startXPos;
			originRowPos = 0;
			originLetterPos = 0;
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			this->startYPos = originRowPos;
			this->startXPos = originLetterPos;
			//2.8.3 �ڵ������� ���� ������ �����ϱⰡ ������ ��¥ ���� ��ġ�� ������ġ�� ���Ѵ�.
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
	//3. OnReplaceButtonClickedCommand�� �ٽ� ����Ǹ�
	else
	{
		//3.1 �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� ����Ǿ� ����)
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
		//3.4 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�� ���� �ȿ��� ���� ���� ��ġ�� ���� 
		//��ġ�� ������ ������ notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//3.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������(���ÿ����� ���� ��� ������ �ڵ������� �ٽ��������)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//3.5.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
	//4. �޸��忡�� ���õ� �ܾ ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
	this->notepadForm->isSelecting = false;
	//5. ������ ������ ������ ������ ���۵Ǵ� x��ǥ�� 0���� �����Ѵ�.
	this->notepadForm->selectedStartXPos = 0;
	//6. ������ ������ ������ ������ ���۵Ǵ� y��ǥ�� 0���� �����Ѵ�.
	this->notepadForm->selectedStartYPos = 0;
	//7. ���õ� ������ �������� ������ �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	//8. ���� ���� ���� ���� ��ġ���� �ٲ� �ܾ �����Ѵ�.
	this->notepadForm->current->AddWord(this->replacingKeyword);
	//9. ������ ������ ������ ���� ������ ��ġ�� �ٽ� �������ش�.
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//10. ���ڸ� �߰��� �Ŀ� �ٿ��ֱⰡ ������ ���� ��ġ�� ������ġ�� �����Ѵ�.
	this->pastingEndYPos = this->notepadForm->note->GetCurrent();
	this->pastingEndXPos = this->notepadForm->current->GetCurrent();
	//11. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������(�ٲ� �ܾ �߰���� ������ �ڵ������� �ٽ��������)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//11.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
		//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
		this->notepadForm->SendMessage(WM_SIZE);
		//11.2 �ڵ������� ���� ������ �ٿ��ֱⰡ ������ ��¥ ���� ��ġ�� ������ġ�� ���Ѵ�.
		changedRowPos = this->pastingEndYPos;
		changedLetterPos = this->pastingEndXPos;
		originRowPos = 0;
		originLetterPos = 0;
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		this->pastingEndYPos = originRowPos;
		this->pastingEndXPos = originLetterPos;
	}
}

//�������
void OnReplaceButtonClickedCommand::Unexecute()
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
		//3.1 �ڵ������� �ݿ��ؼ� �ٿ��ֱⰡ ������(����) �ٰ� ���� ��ġ�� ���Ѵ�.
		changedRowPos = 0;
		changedLetterPos = 0;
		originRowPos = this->pastingEndYPos;
		originLetterPos = this->pastingEndXPos;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//3.2 �ڵ������� �ݿ��ؼ� ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		currentRowPos = this->notepadForm->note->Move(changedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
		//3.3 �ڵ������� �ݿ��ؼ� �ٿ��ֱⰡ ������ ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		selectedEndRowPos = currentRowPos;
		selectedEndLetterPos = currentLetterPos;
		//3.4 �ڵ������� �ݿ��ؼ� �ٿ��ֱⰡ ���۵Ǵ� �ٰ� ���� ��ġ�� ���Ѵ�.
		changedRowPos = 0;
		changedLetterPos = 0;
		originRowPos = this->startYPos;
		originLetterPos = this->startXPos;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//3.5 �ڵ������� �ݿ��ؼ� �ٿ��ֱⰡ ���۵Ǵ� �ٰ� ���� ��ġ�� �ٽ� �����Ѵ�.
		selectedStartRowPos = changedRowPos;//������ ���۵Ǵ� ��
		selectedStartLetterPos = changedLetterPos;//������ ���۵Ǵ� ����
	}
	//4. �ڵ����� ���� ���� �ƴϸ�
	else
	{
		//4.1 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		currentRowPos = this->notepadForm->note->Move(this->pastingEndYPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->pastingEndXPos);
		//4.2 �ٿ��ֱⰡ ���۵Ǵ� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		selectedStartRowPos = this->startYPos;
		selectedStartLetterPos = this->startXPos;
		//4.3 �ٿ��ֱⰡ ������ ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
		selectedEndRowPos = currentRowPos;
		selectedEndLetterPos = currentLetterPos;
	}
	//5. Execute���� �ٿ����� �ٲ� �ܾ �����.
	this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
		selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
	//6. ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�ǿ���ȿ��� ���� ���� ��ġ�� ���� ��ġ��
	//������ ������ notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
	currentRowPos = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//7. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//7.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
		//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
		this->notepadForm->SendMessage(WM_SIZE);
	}

	this->notepadForm->selectedStartYPos = currentRowPos;
	this->notepadForm->selectedStartXPos = currentLetterPos;
	//11. �޸��忡�� ���õ� texts�� �ٽ� �����ϱ� ������ �޸��忡�� ������ �� ���·� �ٲ۴�.
	this->notepadForm->isSelecting = true;
	//12. ���ÿ����� �ٽ� ����� ������ �����ϱ�, �߶󳻱�, ���� �޴��� Ȱ��ȭ ��Ų��.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);

	//8. �ٲ� �ܾ ���� ��ġ�� �Ʊ� ������ ���ÿ��� content�� �����Ѵ�.
	Long rowIndex = this->notepadForm->note->
		InsertTexts(currentRowPos, currentLetterPos, this->note);
	//9. �޸����� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
	//10. �ڵ������� �������̸� �ٿ����� �ٵ��� �ڵ���������ش�.
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//10.1 �κ��ڵ������� �Ѵ�.
		Long endPastedRowPos = rowAutoChange.DoPartRows(currentRowPos, rowIndex);
		//10.2 �ٿ��ֱⰡ ������ �ٷ� �̵���Ų��.
		//�ٿ��ֱⰡ ������ ���� OnSize���� �κ��ڵ������� �ؼ� ó���Ǳ� ������ ĳ���� ��ġ�� �������ָ� ��!
		currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		this->notepadForm->current->Move(currentLetterPos);
	}
}

//�ٽý������� ���� ���ϱ�
bool OnReplaceButtonClickedCommand::IsRedone()
{
	return this->isRedone;
}

//���� ���� ��ġ���ϱ�
Long OnReplaceButtonClickedCommand::GetStartYPos()
{
	return this->startYPos;
}
//���� ������ ��ġ���ϱ�
Long OnReplaceButtonClickedCommand::GetStartXPos()
{
	return this->startXPos;
}
//������ ������ ���� ��ġ ���ϱ�
Long OnReplaceButtonClickedCommand::GetSelectedEndYPos()
{
	return this->selectedEndYPos;
}
//������ ������ ���� ��ġ ���ϱ�
Long OnReplaceButtonClickedCommand::GetSelectedEndXPos()
{
	return this->selectedEndXPos;
}
//�ٿ��ֱⰡ ������ ���� ��ġ���ϱ�
Long OnReplaceButtonClickedCommand::GetPastingEndYPos()
{
	return this->pastingEndYPos;
}
//�ٿ��ֱⰡ ������ ������ ��ġ���ϱ�
Long OnReplaceButtonClickedCommand::GetPastingEndXPos()
{
	return this->pastingEndXPos;
}

//SetMacroEnd(������� �� �ٽý��� ��ũ����� �������� ����)
void OnReplaceButtonClickedCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void OnReplaceButtonClickedCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//������� ������������ ���ϱ�
bool OnReplaceButtonClickedCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool OnReplaceButtonClickedCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}

//SetRedone(�ٽ� �����̶�� ������)
void OnReplaceButtonClickedCommand::SetRedone()
{
	this->isRedone = true;
}

//���ÿ����� ���������� Ȯ�� ����
bool OnReplaceButtonClickedCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}

//�Ҹ���
OnReplaceButtonClickedCommand::~OnReplaceButtonClickedCommand()
{
	if (this->note != 0)
	{
		delete this->note;
	}
}