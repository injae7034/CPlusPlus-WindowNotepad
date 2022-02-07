#include "ShiftCtrlBackSpaceKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "DummyRow.h"
#include "RowAutoChange.h"
#include "Note.h"
#include "SelectingTexts.h"

//����Ʈ������ ����
ShiftCtrlBackSpaceKeyActionCommand::ShiftCtrlBackSpaceKeyActionCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->startYPos = notepadForm->note->GetCurrent();
	this->startXPos = notepadForm->current->GetCurrent();
	this->selectedEndYPos = 0;
	this->selectedEndXPos = 0;
	this->glyph = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isDirty = false;//ó���� ������ ���� ��������� �����Ƿ� false�� ����Ʈ����.
	this->isSelectedTextsRemoved = false;//ó���� ������ ���� ���ÿ����� �����������Ƿ� false�� ����Ʈ��
}

//����
void ShiftCtrlBackSpaceKeyActionCommand::Execute()
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

	//2. �޸��忡�� ���õ� texts�� ����, ���õ� ������ ����������
	if (this->notepadForm->isSelecting == false && this->isSelectedTextsRemoved == false)
	{
		//2.1 ó�� �����̸�
		if (this->isRedone == false)
		{
			//2.1.1 ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
			currentRowPos = this->notepadForm->note->GetCurrent();
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//2.1.2 ���� ���� ��ġ�� ������ġ�� �����ϴ� �ٰ� ���� ��ġ�� �����Ѵ�.
			this->startYPos = currentRowPos;
			this->startXPos = currentLetterPos;
			//2.1.3 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������(��¥ ���� ��ġ�� ������ġ�� �����Ѵ�.)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//2.1.3.1 ���� ���� startYPos�� startXPos�� ��ġ�� �ڵ������� ����� ������ ��ġ�̱�
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
		}
		//2.2 �ٽ� �����̸�
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
		//2.3 ���� ���� ���Ѵ�.
		Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
		Glyph* row = 0;
		//2.4 ���� ���� ��ġ�� 0�̰�, ���� ���� ��ġ�� 0�̸�
		if (currentRowPos == 0 && currentLetterPos == 0)
		{
			//2.4.1 ó�� �����̸�
			if (this->isRedone == false)
			{
				//2.4.1.1 Command�� ���� ������ ������ ǥ���Ѵ�.
				this->isDirty = false;
			}
		}
		//2.5 ���� ���� ��ġ�� 0���� ũ��, ���� ���� ��ġ�� 0�̰�, ���� ���� ��¥ ���̸�
		else if (currentRowPos > 0 && currentLetterPos == 0
			&& !dynamic_cast<DummyRow*>(currentRow))
		{
			//2.5.1 ó�� �����̸�
			if (this->isRedone == false)
			{
				//2.5.1.1 Row�� �����Ѵ�.
				this->glyph = new Row();
			}
			//2.5.2 ���� ���� ���� ���� ���Ѵ�.
			Glyph* previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
			//2.5.3 ���� ���� ���� ���� ������ ���� ��ġ�� ���Ѵ�.
			Long letterPos = previousRow->GetLength();
			//2.5.4 ���� ���� ���� �ٿ� ��ģ��.
			currentRow->Join(previousRow);
			//2.5.5 Note���� ���� ���� �ּҸ� �����.
			this->notepadForm->note->Remove(currentRowPos);
			//2.5.6 ���� ���� �������� ������ ���� ���� �����Ѵ�.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//2.5.7 ���� ���� ���� ��ġ�� ������ �������̱� ������ �������ش�.
			//���� ���� ������ ���� ���� ó�� ���̿� ��ġ�ϵ��� �����Ѵ�.
			currentLetterPos = this->notepadForm->current->Move(letterPos);
			//2.5.8 Command�� ���� ������ ������ ǥ���Ѵ�.
			this->isDirty = true;
		}
		//2.6 �� �̿ܿ���
		else
		{
			Glyph* letter = 0;
			//2.6.1 ó�� �����̸�
			if (this->isRedone == false)
			{
				//2.6.1.1 DummyRow�� �����Ѵ�.
				this->glyph = new DummyRow();
				//2.6.1.2 Command�� ���� ������ ������ ǥ���Ѵ�.
				this->isDirty = true;
			}
			//2.6.2 ���� ���� ��¥���̸�
			if (dynamic_cast<DummyRow*>(currentRow))
			{
				//2.6.2.1 ó�� �����̸�
				if (this->isRedone == false)
				{
					//2.6.2.1.1 ���� ������ġ�� 0���� �������� �ݺ��Ѵ�.
					while (currentLetterPos > 0)
					{
						//2.6.2.1.1.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
						letter = currentRow->GetAt(currentLetterPos - 1);
						//2.6.2.1.1.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
						this->glyph->Add(0, letter->Clone());
						//2.6.2.1.1.3 ���ڸ� �����.
						currentRow->Remove(currentLetterPos - 1);
						currentLetterPos--;
					}
				}
				//2.6.2.2 �ٽ� �����̸�
				else
				{
					//2.6.2.2.1 ���� ������ġ�� 0���� �������� �ݺ��Ѵ�.
					while (currentLetterPos > 0)
					{
						//2.6.2.2.1.1 ���ڸ� �����.
						currentRow->Remove(currentLetterPos - 1);
						currentLetterPos--;
					}
				}
				//2.6.2.3 ���� �ּҸ� �����Ѵ�.
				row = currentRow;
				//2.6.2.4 ���� ���� ������ ������ġ�� �̵��Ѵ�.
				currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Last();
				currentRow = this->notepadForm->current;
			}
			//2.6.3 ó�� �����̸�
			if (this->isRedone == false)
			{
				//2.6.3.1 ��¥ ���ε��� �ݺ��Ѵ�. 
				while (dynamic_cast<DummyRow*>(currentRow))
				{
					//2.6.3.1.1 ���� ������ġ�� 0���� �������� �ݺ��Ѵ�.
					while (currentLetterPos > 0)
					{
						//2.6.3.1.1.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
						letter = currentRow->GetAt(currentLetterPos - 1);
						//2.6.3.1.1.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
						this->glyph->Add(0, letter->Clone());
						//2.6.3.1.1.3 ���ڸ� �����.
						currentRow->Remove(currentLetterPos - 1);
						currentLetterPos--;
					}
					//2.6.3.1.2 ���ڰ� ���������� ������ Note���� ���� ���� �ּҸ� �����.
					this->notepadForm->note->Remove(currentRowPos);
					//2.6.3.1.3 ���� ���� ������ ������ġ�� �̵��Ѵ�.
					currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
					currentLetterPos = this->notepadForm->current->Last();
					currentRow = this->notepadForm->current;
				}
				//2.6.3.2 ��¥ ���� ���ڸ� �� ����� ���� �ݺ��Ѵ�.
				while (currentLetterPos > 0)
				{
					//2.6.3.2.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
					letter = currentRow->GetAt(currentLetterPos - 1);
					//2.6.3.2.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
					this->glyph->Add(0, letter->Clone());
					//2.6.3.2.3 ���ڸ� �����.
					currentRow->Remove(currentLetterPos - 1);
					currentLetterPos--;
				}
			}
			//4.4.6 �ٽ� �����̸�
			else
			{
				//4.4.6.1 ��¥ ���ε��� �ݺ��Ѵ�. 
				while (dynamic_cast<DummyRow*>(currentRow))
				{
					//4.4.6.1.1 ���� ������ġ�� 0���� �������� �ݺ��Ѵ�.
					while (currentLetterPos > 0)
					{
						//4.4.6.1.1.1 ���ڸ� �����.
						currentRow->Remove(currentLetterPos - 1);
						currentLetterPos--;
					}
					//4.4.6.1.2 ���ڰ� ���������� ������ Note���� ���� ���� �ּҸ� �����.
					this->notepadForm->note->Remove(currentRowPos);
					//4.4.6.1.3 ���� ���� ������ ������ġ�� �̵��Ѵ�.
					currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
					currentLetterPos = this->notepadForm->current->Last();
					currentRow = this->notepadForm->current;
				}
				//4.4.6.2 ��¥ ���� ���ڸ� �� ����� ���� �ݺ��Ѵ�.
				while (currentLetterPos > 0)
				{
					//4.4.6.2.1 ���ڸ� �����.
					currentRow->Remove(currentLetterPos - 1);
					currentLetterPos--;
				}
			}
			//4.4.7 �ٿ� ���ڰ� ����������
			if (row != 0)
			{
				//4.4.7.1 ���ڰ� �����ִ� ���� ���� �ٿ� ��ģ��.
				row->Join(currentRow);
				//4.4.7.2 Note���� ���ڰ� �����ִ� ���� �ּҸ� �����.
				this->notepadForm->note->Remove(currentRowPos + 1);
				//4.4.7.3 �����ٿ��� ĳ���� ���� ó������ ������.
				currentRow->First();
			}
			//2.6.7.2 Command�� ���� ������ ������ ǥ���Ѵ�.
			this->isDirty = true;
		}	
		//2.7 ��������� ������
		if (this->isDirty == true)
		{
			//2.7.1 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�� ���� �ȿ��� 
			//���� ���� ��ġ�� ���� ��ġ�� ������ ������ notepadForm�� current(������)�� 
			//������ �� ��� notepadForm���� ���ش�.)
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//2.7.2 ���ڳ� ���� ������ ������ �ٽ� �����ϴ� ���� ��ġ�� ������ġ�� �����Ѵ�.
			this->startYPos = currentRowPos;
			this->startXPos = currentLetterPos;
			//2.7.3 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������(���ÿ����� ���� ��� ������ �ڵ������� �ٽ��������)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//2.7.3.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
				//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
				this->notepadForm->SendMessage(WM_SIZE);
				//2.7.3.2 ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ��
				//�ٲ� ���ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
				this->startYPos = this->notepadForm->note->GetCurrent();
				this->startXPos = this->notepadForm->current->GetCurrent();
				//2.7.3.3 ���� ���� startYPos�� startXPos�� ��ġ�� �ڵ������� ����� ������ ��ġ�̱�
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
		}
	}
	//3. �޸��忡�� ���õ� texts�� �ְų� �Ǵ� ���õ� ������ ��������
	else if (this->notepadForm->isSelecting == true || this->isSelectedTextsRemoved == true)
	{
		//3.1 OnCharCommand�� ó�� ����Ǹ�
		if (this->isRedone == false)
		{
			//3.1.1 ���� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
			currentRowPos = this->notepadForm->note->GetCurrent();
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//3.1.2 ������ ���۵Ǵ� ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
			selectedStartRowPos = this->notepadForm->selectedStartYPos;
			this->startYPos = selectedStartRowPos;
			selectedStartLetterPos = this->notepadForm->selectedStartXPos;
			this->startXPos = selectedStartLetterPos;
			//3.1.3 ������ ������ ���� ��ġ�� ���� ��ġ�� ���Ѵ�.
			selectedEndRowPos = currentRowPos;
			this->selectedEndYPos = selectedEndRowPos;
			selectedEndLetterPos = currentLetterPos;
			this->selectedEndXPos = selectedEndLetterPos;
			//3.1.4 ���õ� �ܾ�(ã�� �ܾ�)�� command�� �����ؼ� �����ϰ� �����.
			this->glyph = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
				selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		}
		//3.2 OnCharCommand�� �ٽ� ����Ǹ�
		else
		{
			//3.2.1 �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ��
			//����Ǿ� ����)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//3.2.1.1 ����� ȭ�� ũ�⿡ �´� ���� �ٰ� ���� ��ġ�� ���Ѵ�.
				changedRowPos = 0;
				changedLetterPos = 0;
				originRowPos = this->selectedEndYPos;
				originLetterPos = this->selectedEndXPos;
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//3.2.1.2 �ڵ������� �ݿ��ؼ� ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
				currentRowPos = this->notepadForm->note->Move(changedRowPos);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
				//3.2.1.3 �ڵ������� �ݿ��ؼ� ������ ������ ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
				selectedEndRowPos = currentRowPos;
				selectedEndLetterPos = currentLetterPos;
				//3.2.1.4 ����� ȭ�� ũ�⿡ �´� ������ ���۵Ǵ� �ٰ� ���� ��ġ�� ���Ѵ�.
				changedRowPos = 0;
				changedLetterPos = 0;
				originRowPos = this->startYPos;
				originLetterPos = this->startXPos;
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//3.2.1.5 �ڵ������� �ݿ��ؼ� ������ ���۵Ǵ� �ٰ� ���� ��ġ�� �ٽ� �����Ѵ�.
				selectedStartRowPos = changedRowPos;//������ ���۵Ǵ� ��
				selectedStartLetterPos = changedLetterPos;//������ ���۵Ǵ� ����
			}
			//3.2.2 �ڵ����� ���� ���� �ƴϸ�
			else
			{
				//3.2.2.1 ���� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
				currentRowPos = this->notepadForm->note->Move(this->selectedEndYPos);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Move(this->selectedEndXPos);
				//3.2.2.2 ������ ���۵Ǵ� ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
				selectedStartRowPos = this->startYPos;
				selectedStartLetterPos = this->startXPos;
				//3.2.2.3 ������ ������ ���� ��ġ�� ���� ��ġ�� �ٽ� �����Ѵ�.
				selectedEndRowPos = currentRowPos;
				selectedEndLetterPos = currentLetterPos;
			}
			//3.2.3 ���õ� �ܾ�(ã�� �ܾ�)�� �����.
			this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
				selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		}
		//3.3 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�� ���� �ȿ��� 
		//���� ���� ��ġ�� ���� ��ġ�� ������ ������ notepadForm�� current(������)�� 
		//������ �� ��� notepadForm���� ���ش�.)
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//3.4 ���õ� �ܾ ������ ������ �ٽ� �����ϴ� ���� ��ġ�� ������ġ�� �����Ѵ�.
		this->startYPos = currentRowPos;
		this->startXPos = currentLetterPos;
		//3.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������(���ÿ����� ���� ��� ������ �ڵ������� �ٽ��������)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//3.5.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
			//3.5.2 ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ��
			//�ٲ� ���ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
			this->startYPos = this->notepadForm->note->GetCurrent();
			this->startXPos = this->notepadForm->current->GetCurrent();
			//3.5.3 ���� ���� startYPos�� startXPos�� ��ġ�� �ڵ������� ����� ������ ��ġ�̱�
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
		//3.6 �޸��忡�� ���õ� �ܾ ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = false;
		//3.7 ������ ������ ������ ������ ���۵Ǵ� x��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartXPos = 0;
		//3.8 ������ ������ ������ ������ ���۵Ǵ� y��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartYPos = 0;
		//3.9 ���õ� ������ �������� ������ �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		//3.10 command�� ���ÿ����� �����ٰ� ǥ���Ѵ�.
		this->isSelectedTextsRemoved = true;
		//3.11 Command�� ���� ������ ������ ǥ���Ѵ�.
		this->isDirty = true;
	}
	//4. Command�� ���� ������ ������
	if (this->isDirty == true)
	{
		//4.1 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//4.2 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->isDirty = true;
	}
}

//�������
void ShiftCtrlBackSpaceKeyActionCommand::Unexecute()
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
	//5. ���� �� ������ glyph�� ��¥���̸�
	if (dynamic_cast<DummyRow*>(this->glyph))
	{
		//5.1 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� ������
		if (currentLetterPos == this->notepadForm->current->GetLength())
		{
			//5.1.1 ���� ���� ���Ѵ�.
			Glyph* currentRow = this->notepadForm->current;
			//5.1.2 dummyRow�� ���� �����Ѵ�.
			Glyph* dummyRow = this->glyph->Clone();
			//5.1.3 dummyRow�� ���� �ٿ� ��ģ��.
			dummyRow->Join(currentRow);
			//5.1.4 dummyRow�� �Ҵ������Ѵ�.
			if (dummyRow != 0)
			{
				delete dummyRow;
			}
			//5.1.5 ���� ��ġ�� ���� �ٿ��� ���� ���������� ������.
			currentLetterPos = currentRow->Last();
		}
		//5.2 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� �ٸ���
		else
		{
			//5.2.1 dummyRow�� ������ŭ �ݺ��Ѵ�.
			Glyph* letter = 0;
			Long i = this->glyph->GetLength() - 1;//������ �迭��Һ��� ������ ������ ����� �ȴ�.
			Long distance = currentLetterPos;
			while (i >= 0)
			{
				//5.2.1.1 ���ڸ� ���Ѵ�.
				letter = this->glyph->GetAt(i);
				//5.2.1.2 ���� ���� ���� ��ġ�� dummyRow���� ���� ���縦 �� letter�� ���� �ִ´�.
				currentLetterPos = this->notepadForm->current->
					Add(currentLetterPos, letter->Clone());
				//5.2.1.3 �̵��� ��ġ�� ������Ų��.
				distance++;
				//5.2.1.3 i�� ������Ų��.
				i--;
			}
			//5.2.2 ���� ��ġ�� �������Ѵ�.
			this->notepadForm->current->Move(distance);
		}
		//5.3 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�ǿ���ȿ��� ���� ���� ��ġ�� ���� ��ġ��
		//������ ������ notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//5.4 ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ�� �ٲ� ��
		//�ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
		this->startYPos = currentRowPos;
		this->startXPos = currentLetterPos;
		//5.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//5.5.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
			//5.5.2 ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ�� �ٲ� ��
			//�ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
			this->startYPos = this->notepadForm->note->GetCurrent();
			this->startXPos = this->notepadForm->current->GetCurrent();
			//5.5.3 ���� ���� startYPos�� startXPos�� ��ġ�� �ڵ������� ����� ������ ��ġ�̱� ������
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
	}
	//6. ���� �� ������ glyph�� ��(���๮��)�̸�
	else if (dynamic_cast<Row*>(this->glyph))
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
		//5.3 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�ǿ���ȿ��� ���� ���� ��ġ�� ���� ��ġ��
		//������ ������ notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//6.5 ���� ���� ���� ��ġ�� ó������ �̵���Ų��.
		currentLetterPos = this->notepadForm->current->First();
		//5.4 ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ�� �ٲ� ��
		//�ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
		this->startYPos = currentRowPos;
		this->startXPos = currentLetterPos;
		//5.5 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//5.5.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
			//5.5.2 ���ÿ����� �����Ǿ��� ������ �ڵ������� ������ ���� ���� ��ġ�� ���� ��ġ�� �ٲ� ��
			//�ֱ� ������ �����ϴ� ���� ��ġ�� ���� ���� ��ġ�� ������ġ�� �簻��������Ѵ�.
			this->startYPos = this->notepadForm->note->GetCurrent();
			this->startXPos = this->notepadForm->current->GetCurrent();
			//5.5.3 ���� ���� startYPos�� startXPos�� ��ġ�� �ڵ������� ����� ������ ��ġ�̱� ������
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
	}
	//7. ���� �� ������ glyph�� ��Ʈ(���ÿ���)�̸�
	else if (dynamic_cast<Note*>(this->glyph))
	{
		//7.1 �ٲ۴ܾ ������ ���� ���ÿ����� �ٽ� �����ϱ� ������ ���� �ٰ� ������ ��ġ��
		//���ÿ����� ���� �ٰ� ���� ��ġ�� �����Ѵ�.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		this->notepadForm->selectedStartYPos = currentRowPos;
		this->notepadForm->selectedStartXPos = currentLetterPos;
		//7.2 �޸��忡�� ���õ� texts�� �ٽ� �����ϱ� ������ �޸��忡�� ������ �� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = true;
		//7.3 ���ÿ����� �ٽ� ����� ������ �����ϱ�, �߶󳻱�, ���� �޴��� Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
		//7.4 ���ÿ����� �ٽ� �����ϱ� ���� ���� ��ġ�� �����Ѵ�.
		Long previousRowIndex = currentRowPos;
		//7.5 ���ÿ����� �ٽ� �����ϰ� ���� ���� ��ġ�� ��ȯ�޴´�.
		currentRowPos = this->notepadForm->note->
			InsertTexts(currentRowPos, currentLetterPos, this->glyph);
		//7.6 �޸����� ���� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//7.7 �޸��忡 ���ÿ����� �����Ǿ��� ������ ������ ������ �ٰ� ������ġ�� �����Ѵ�.
		this->selectedEndYPos = currentRowPos;
		this->selectedEndXPos = currentLetterPos;
		//7.8 �ڵ������� �������̸� �ٿ����� �ٵ��� �ڵ���������ش�.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//7.8.1 �κ��ڵ������� �Ѵ�.(������ ���� �����ϰ� �ڵ�������)
			Long endPastedRowPos = rowAutoChange.DoPartRows(previousRowIndex, currentRowPos);
			//7.8.2 ���ÿ����� ������ �ٷ� �̵���Ų��.
			//���ÿ����� ������ ���� OnSize���� �κ��ڵ������� �ؼ� ó���Ǳ� ������ ĳ���� ��ġ�� �������ָ� ��!
			currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			this->notepadForm->current->Move(currentLetterPos);
			//7.8.3 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. (������ ���� �ڵ������Ŵ)
			this->notepadForm->SendMessage(WM_SIZE);
			//7.8.4 �޸����� ���� ���� �����Ѵ�.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//7.8.5 �޸��忡 ���ÿ����� �����Ǿ���, �ڵ������� �ݿ��� ������ ������ �ٰ� ������ġ�� �����Ѵ�. 
			this->selectedEndYPos = currentRowPos;
			this->selectedEndXPos = currentLetterPos;
			//7.8.6 ���� ���� ������ ������ �ٰ� ���� ��ġ�� �ڵ������� ����� ������ ��ġ�̱� ������
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
void ShiftCtrlBackSpaceKeyActionCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void ShiftCtrlBackSpaceKeyActionCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}


//SetRedone(�ٽ� �����̶�� ������)
void ShiftCtrlBackSpaceKeyActionCommand::SetRedone()
{
	this->isRedone = true;
}

//�����ϴ� ���� ��ġ���ϱ�
Long ShiftCtrlBackSpaceKeyActionCommand::GetStartYPos()
{
	return this->startYPos;
}
//�����ϴ� ������ ��ġ���ϱ�
Long ShiftCtrlBackSpaceKeyActionCommand::GetStartXPos()
{
	return this->startXPos;
}
//������ ������ ���� ��ġ ���ϱ�
Long ShiftCtrlBackSpaceKeyActionCommand::GetSelectedEndYPos()
{
	return this->selectedEndYPos;
}
//������ ������ ���� ��ġ ���ϱ�
Long ShiftCtrlBackSpaceKeyActionCommand::GetSelectedEndXPos()
{
	return this->selectedEndXPos;
}
//������� ������������ ���ϱ�
bool ShiftCtrlBackSpaceKeyActionCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool ShiftCtrlBackSpaceKeyActionCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//�ٽý������� ���� ���ϱ�
bool ShiftCtrlBackSpaceKeyActionCommand::IsRedone()
{
	return this->isRedone;
}

//��������� �ִ��� Ȯ�� ����
bool ShiftCtrlBackSpaceKeyActionCommand::IsDirty()
{
	return this->isDirty;
}

//���ÿ����� ���������� Ȯ�� ����
bool ShiftCtrlBackSpaceKeyActionCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}


Glyph* ShiftCtrlBackSpaceKeyActionCommand::GetGlyph()
{
	return this->glyph;
}

//�Ҹ��� ����
ShiftCtrlBackSpaceKeyActionCommand::~ShiftCtrlBackSpaceKeyActionCommand()
{
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}