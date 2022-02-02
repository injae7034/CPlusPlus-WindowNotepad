#include "OnImeCharCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "RowAutoChange.h"
#include "SelectingTexts.h"

//����Ʈ������ ����
OnImeCharCommand::OnImeCharCommand(NotepadForm* notepadForm, Glyph* glyph)
	:Command(notepadForm)
{
	this->note = notepadForm->GetRemovedSelectedTexts();
	this->glyph = glyph;
	this->startYPos = notepadForm->note->GetCurrent();
	this->startXPos = notepadForm->current->GetCurrent();
	this->selectedEndYPos = 0;
	this->selectedEndXPos = 0;
	this->pastingEndYPos = 0;
	this->pastingEndXPos = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isSelectedTextsRemoved = false;//ó���� ������ ���� ���ÿ����� �����������Ƿ� false�� ����Ʈ��
}

//����
void OnImeCharCommand::Execute()
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

	//2. OnImeCharCommand�� �ٽ� ����Ǹ�
	if (this->isRedone == true)
	{
		//2.1 �ѱ��� �Է��� �� �޸��忡�� ���õ� ������ ��������
		if (this->note != 0)
		{
			//2.1.1 �ڵ������� ���� ���̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ��
			//����Ǿ� ����)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//2.1.1.1 ����� ȭ�� ũ�⿡ �´� ���� �ٰ� ���� ��ġ�� ���Ѵ�.
				changedRowPos = 0;
				changedLetterPos = 0;
				originRowPos = this->selectedEndYPos;
				originLetterPos = this->selectedEndXPos;
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//2.1.1.2 �ڵ������� �ݿ��ؼ� ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
				currentRowPos = this->notepadForm->note->Move(changedRowPos);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
				//2.1.1.3 �ڵ������� �ݿ��ؼ� ������ ������ ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
				selectedEndRowPos = currentRowPos;
				selectedEndLetterPos = currentLetterPos;
				//2.1.1.4 ����� ȭ�� ũ�⿡ �´� ������ ���۵Ǵ� �ٰ� ���� ��ġ�� ���Ѵ�.
				changedRowPos = 0;
				changedLetterPos = 0;
				originRowPos = this->startYPos;
				originLetterPos = this->startXPos;
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//2.1.1.5 �ڵ������� �ݿ��ؼ� ������ ���۵Ǵ� �ٰ� ���� ��ġ�� �ٽ� �����Ѵ�.
				selectedStartRowPos = changedRowPos;//������ ���۵Ǵ� ��
				selectedStartLetterPos = changedLetterPos;//������ ���۵Ǵ� ����
			}
			//2.1.2 �ڵ����� ���� ���� �ƴϸ�
			else
			{
				//2.1.2.1 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
				currentRowPos = this->notepadForm->note->Move(this->selectedEndYPos);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Move(this->selectedEndXPos);
				//2.1.2.2 ������ ���۵Ǵ� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
				selectedStartRowPos = this->startYPos;
				selectedStartLetterPos = this->startXPos;
				//2.1.2.3 ������ ������ ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
				selectedEndRowPos = currentRowPos;
				selectedEndLetterPos = currentLetterPos;
			}
			//2.1.3 ���õ� �ܾ�(ã�� �ܾ�)�� �����.
			this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
				selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
			//2.1.4 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�� ���� �ȿ��� ���� ���� ��ġ�� ���� 
			//��ġ�� ������ ������ notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//2.1.5 ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ�� �ٲ� ��
			//�ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
			this->startYPos = this->notepadForm->note->GetCurrent();
			this->startXPos = this->notepadForm->current->GetCurrent();
			//2.1.6 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������(���ÿ����� ���� ��� ������ �ڵ������� �ٽ��������)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//2.1.6.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
				//2.1.6.2 ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ��
				//�ٲ� �� �ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
				this->startYPos = this->notepadForm->note->GetCurrent();
				this->startXPos = this->notepadForm->current->GetCurrent();
				//2.1.6.3 ���� ���� startYPos�� startXPos�� ��ġ�� �ڵ������� ����� ������ ��ġ�̱�
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
			//2.1.7 �޸��忡�� ���õ� texts�� �� ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
			this->notepadForm->isSelecting = false;
			//2.1.8 ���õ� texts�� ������ ������ command�� ���õ� ������ �����ٰ� ǥ���Ѵ�.
			this->isSelectedTextsRemoved = true;
			//2.1.9 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartXPos = 0;
			//2.1.10 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
			this->notepadForm->selectedStartYPos = 0;
			//2.1.11 �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		//2.2 �ѱ��� �Է��� �� �޸��忡�� ���ÿ����� ����������
		else
		{
			//2.2.1 �ڵ������� �������̸�(command�� �����ϴ� �ٰ� ���� ��ġ�� 
			//�׻� ��¥ �ٰ� ���� ��ġ�� ����Ǿ� �ֱ� ������ �ڵ����࿡ �°� �ٲ������)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//2.2.1.1 �ڵ������� �ݿ��Ͽ� ���۵Ǵ� �ٰ� ���� ��ġ�� ���Ѵ�.
				changedRowPos = 0;
				changedLetterPos = 0;
				originRowPos = this->startYPos;
				originLetterPos = this->startXPos;
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//2.2.1.2 �ڵ������� �ݿ��ؼ� ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
				currentRowPos = this->notepadForm->note->Move(changedRowPos);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
			}
			//2.2.2 �ڵ����� �������� �ƴϸ�
			else
			{
				//2.2.2.1 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
				currentRowPos = this->notepadForm->note->Move(this->startYPos);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Move(this->startXPos);
			}
		}
	}
	//3. ó�� �����̸�(�տ� �������� �ѱ��� �������ϰ�, �ٽ� �����̸� �տ� �������� �ѱ��� ����!)
	else
	{
		//3.1 ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//3.2 ���� ���� ĳ���� ���� ��ġ �ٷ� �տ� �ִ� ���� �ѱ��� �����.
		//�׷��� ���ؼ��� ĳ���� ���� ���� ��ġ�� 1������ ���� �־��ָ� �ȴ�.
		//�̿ϼ��� �ѱ��� ����� �ٷ� �ٽ� �ϼ��� �ѱ��� �ֱ� ������ �̿ϼ��� �ѱ��� ���� ����
		//�ڵ������� ������ ���� �ʿ䰡 ����, �ϼ��� �ѱ��� �ְ� ������ �ڵ������� ���� ���� �ʿ䰡 ����
		//�ֳ��ϸ� �̿ϼ� ���� ��ġ�� �ϼ����ڷ� �ٲ������ϱ� ���� ���̿��� ������ ���� ������!
		this->notepadForm->current->Remove(currentLetterPos - 1);
		//3.3 ������ ������ ������ ���� ������ ��ġ�� �ٽ� ���Ѵ�.
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//3.4 ���õ� �ܾ ������ ������ �ٽ� �����ϴ� ���� ��ġ�� ������ġ�� �����Ѵ�.
		this->startYPos = currentRowPos;
		this->startXPos = currentLetterPos;
		//3.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������(���ÿ����� ���� ��� ������ �ڵ������� �ٽ��������)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//3.5.1 ���� ���� startYPos�� startXPos�� ��ġ�� �ڵ������� ����� ������ ��ġ�̱�
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
		//3.6 �ѱ��� �Է��� �� �޸��忡�� ���õ� ������ ��������
		if (this->note != 0)
		{
			//3.6.1 ���õ� texts�� ������ ������ command�� ���õ� ������ �����ٰ� ǥ���Ѵ�.
			this->isSelectedTextsRemoved = true;
		}
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
	//6. �޸����� ���� ���� ��ġ�� �ٽ� ���Ѵ�.
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//7. �޸��忡 �ٿ��־��� ������ �ٿ��ֱⰡ ������ �ٰ� ������ġ�� �����Ѵ�.
	this->pastingEndYPos = currentRowPos;
	this->pastingEndXPos = currentLetterPos;
	//8. �ڵ������� �������̸�
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//8.1 ���ڰ� �߰��� ���� �ڵ���������ش�.
		//this->notepadForm->SendMessage(WM_SIZE);
		//8.2 �ڵ����� �� �޸����� ���� ���� �����Ѵ�.
		//currentRowPos = this->notepadForm->note->GetCurrent();
		//this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//currentLetterPos = this->notepadForm->current->GetCurrent();
		//8.3 �޸��忡 ���ÿ����� �����Ǿ���, �ڵ������� �ݿ��� ������ ������ �ٰ� ������ġ�� �����Ѵ�. 
		//this->pastingEndYPos = currentRowPos;
		//this->pastingEndXPos = currentLetterPos;
		//8.4 ���� ���� ������ ������ �ٰ� ���� ��ġ�� �ڵ������� ����� ������ ��ġ�̱� ������
		//�ڵ������� ���� ���¿��� �����ϱⰡ ������ ��¥ ���� ��ġ�� ������ġ�� ���Ѵ�.
		changedRowPos = this->pastingEndYPos;
		changedLetterPos = this->pastingEndXPos;
		originRowPos = 0;
		originLetterPos = 0;
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		this->pastingEndYPos = originRowPos;
		this->pastingEndXPos = originLetterPos;
	}
	//9. ó�� �����̸�
	if (this->isRedone == false)
	{
		//9.1 IsComposing�� false�� �ٲ۴�.
		this->notepadForm->isComposing = false;
		//9.2 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//9.3 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->isDirty = true;
	}
}

//�������
void OnImeCharCommand::Unexecute()
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
	//5. ���� ���ڸ� �����.
	this->notepadForm->current->Remove(currentLetterPos - 1);
	//8. ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�ǿ���ȿ��� ���� ���� ��ġ�� ���� ��ġ��
	//������ ������ notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
	currentRowPos = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//9. ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ�� �ٲ� ��
	//�ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
	this->startYPos = this->notepadForm->note->GetCurrent();
	this->startXPos = this->notepadForm->current->GetCurrent();
	//10. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//10.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
		//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
		this->notepadForm->SendMessage(WM_SIZE);
		//10.2 ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ�� �ٲ� ��
		//�ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
		this->startYPos = this->notepadForm->note->GetCurrent();
		this->startXPos = this->notepadForm->current->GetCurrent();
		//10.3 ���� ���� startYPos�� startXPos�� ��ġ�� �ڵ������� ����� ������ ��ġ�̱� ������
		//�ڵ������� ���� ���¿��� �����ϱⰡ ���۵Ǵ� ��¥ ���� ��ġ�� ������ġ�� ���Ѵ�.
		changedRowPos = this->startYPos;
		changedLetterPos = this->startXPos;
		originRowPos = 0;
		originLetterPos = 0;
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		this->startYPos = originRowPos;
		this->startXPos = originLetterPos;
	}
	//7. �ѱ��� �Է��� �� �޸��忡�� ���õ� ������ ��������
	if (this->note != 0)
	{
		//11.1 �ٲ۴ܾ ������ ���� ���ÿ����� �ٽ� �����ϱ� ������ ���� �ٰ� ������ ��ġ��
		//���ÿ����� ���� �ٰ� ���� ��ġ�� �����Ѵ�.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		this->notepadForm->selectedStartYPos = currentRowPos;
		this->notepadForm->selectedStartXPos = currentLetterPos;
		//11.2 �޸��忡�� ���õ� texts�� �ٽ� �����ϱ� ������ �޸��忡�� ������ �� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = true;
		//11.3 ���ÿ����� �ٽ� ����� ������ �����ϱ�, �߶󳻱�, ���� �޴��� Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
		//11.4 ���ÿ����� �ٽ� �����ϱ� ���� ���� ��ġ�� �����Ѵ�.
		Long previousRowIndex = currentRowPos;
		//11.5 ���ÿ����� �ٽ� �����ϰ� ���� ���� ��ġ�� ��ȯ�޴´�.
		currentRowPos = this->notepadForm->note->
			InsertTexts(currentRowPos, currentLetterPos, this->note);
		//11.6 �޸����� ���� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//11.7 �޸��忡 ���ÿ����� �����Ǿ��� ������ ������ ������ �ٰ� ������ġ�� �����Ѵ�.
		this->selectedEndYPos = currentRowPos;
		this->selectedEndXPos = currentLetterPos;
		//11.8 �ڵ������� �������̸� �ٿ����� �ٵ��� �ڵ���������ش�.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//11.8.1 �κ��ڵ������� �Ѵ�.(������ ���� �����ϰ� �ڵ�������)
			Long endPastedRowPos = rowAutoChange.DoPartRows(previousRowIndex, currentRowPos);
			//11.8.2 ���ÿ����� ������ �ٷ� �̵���Ų��.
			//���ÿ����� ������ ���� OnSize���� �κ��ڵ������� �ؼ� ó���Ǳ� ������ ĳ���� ��ġ�� �������ָ� ��!
			currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			this->notepadForm->current->Move(currentLetterPos);
			//11.8.3 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. (������ ���� �ڵ������Ŵ)
			this->notepadForm->SendMessage(WM_SIZE);
			//11.8.4 �޸����� ���� ���� �����Ѵ�.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//11.8.5 �޸��忡 ���ÿ����� �����Ǿ���, �ڵ������� �ݿ��� ������ ������ �ٰ� ������ġ�� �����Ѵ�. 
			this->selectedEndYPos = currentRowPos;
			this->selectedEndXPos = currentLetterPos;
			//11.8.6 ���� ���� ������ ������ �ٰ� ���� ��ġ�� �ڵ������� ����� ������ ��ġ�̱� ������
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

//�����ϴ� ���� ��ġ���ϱ�
Long OnImeCharCommand::GetStartYPos()
{
	return this->startYPos;
}
//�����ϴ� ������ ��ġ���ϱ�
Long OnImeCharCommand::GetStartXPos()
{
	return this->startXPos;
}
//������ ������ ���� ��ġ ���ϱ�
Long OnImeCharCommand::GetSelectedEndYPos()
{
	return this->selectedEndYPos;
}
//������ ������ ���� ��ġ ���ϱ�
Long OnImeCharCommand::GetSelectedEndXPos()
{
	return this->selectedEndXPos;
}
//�ٿ��ֱⰡ ������ ���� ��ġ���ϱ�
Long OnImeCharCommand::GetPastingEndYPos()
{
	return this->pastingEndYPos;
}
//�ٿ��ֱⰡ ������ ������ ��ġ���ϱ�
Long OnImeCharCommand::GetPastingEndXPos()
{
	return this->pastingEndXPos;
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
//���ÿ����� ���������� Ȯ�� ����
bool OnImeCharCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}


Glyph* OnImeCharCommand::GetGlyph()
{
	return this->glyph;
}

Glyph* OnImeCharCommand::GetNote()
{
	return this->note;
}

//�Ҹ��� ����
OnImeCharCommand::~OnImeCharCommand()
{
	if (this->note != 0)
	{
		delete this->note;
	}
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}