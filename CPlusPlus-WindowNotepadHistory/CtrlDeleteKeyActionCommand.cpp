#include "CtrlDeleteKeyActionCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "DummyRow.h"
#include "RowAutoChange.h"
#include "Note.h"
#include "SelectingTexts.h"

//����Ʈ������ ����
CtrlDeleteKeyActionCommand::CtrlDeleteKeyActionCommand(NotepadForm* notepadForm)
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
	this->isSelectedTextsRemoved = false;//ó���� ������ ���� ���ÿ����� �����������Ƿ� false�� ����Ʈ��
}

//����
void CtrlDeleteKeyActionCommand::Execute()
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
	//3. CtrlDeleteKeyActionCommand�� �ٽ� ����Ǹ�
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
	//4. �޸��忡�� ���õ� texts�� ����, ���õ� ������ ����������
	if (this->notepadForm->isSelecting == false && this->isSelectedTextsRemoved == false)
	{
		//4.1 ���� ��Ʈ�� ������ ���� ��ġ�� ���Ѵ�.
		Long lastRowPos = this->notepadForm->note->GetLength() - 1;
		//4.2 ���� �ٿ��� ������ ������ ��ġ�� ���Ѵ�.
		Long lastLetterPos = this->notepadForm->current->GetLength();
		//4.3 ���� ���� ���Ѵ�.
		Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
		//4.4 ���� ���� ���� ���� ���Ѵ�.
		Glyph* nextRow = this->notepadForm->note->GetAt(currentRowPos + 1);
		//4.5 ���� ���� ��ġ�� ��Ʈ�� ������ ����ġ�̰�, ���� ���� ��ġ�� ������ �����̸�
		if (currentRowPos == lastRowPos && currentLetterPos == lastLetterPos)
		{
			//4.5.1 ó�� �����̸�
			if (this->isRedone == false)
			{
				//4.5.1.1 Command�� ���� ������ ������ ǥ���Ѵ�.
				this->isDirty = false;
			}
		}
		// ���� ���� ��ġ�� �������� �ƴϰ�, ���� ���� ��ġ�� �������̸� ���� ���� ���� �ٷ� ���Խ�Ų��.
		//4.6 ���� ���� ��ġ�� ��Ʈ�� ������ �� ��ġ���� �۰�, ���� ���� ��ġ�� �������̰�,
		//���� ���� ���� ���� ��¥ ���̸�
		else if (currentRowPos < lastRowPos && currentLetterPos == lastLetterPos
			&& !dynamic_cast<DummyRow*>(nextRow))
		{
			//4.6.1 ó�� ������ �Ǹ�
			if (this->isRedone == false)
			{
				//4.6.1.1 Row�� �����Ѵ�.
				this->glyph = new Row();
			}
			//4.6.2 ���� ���� ���� �ٿ� ��ģ��.
			nextRow->Join(currentRow);
			//4.6.3 Note���� ���� ���� �ּҸ� �����.
			this->notepadForm->note->Remove(currentRowPos + 1);
			//4.6.4 ���� ���� ���� ��ġ�� ������ �������̱� ������ lastLetterPos�� �Ű��ش�.
			currentLetterPos = this->notepadForm->current->Move(lastLetterPos);
			//4.6.5 Command�� ���� ������ ������ ǥ���Ѵ�.
			this->isDirty = true;
		}
		//4.7 �� �̿ܿ���
		else
		{
			//4.7.1 �̵��ϱ� ���� ���� ���� ��ġ�� ���Ѵ�.
			Long previousRowIndex = this->notepadForm->note->GetCurrent();
			//4.7.2 �̵��ϱ� ���� ���� ������ ��ġ�� ���Ѵ�.
			Long previousLetterIndex = this->notepadForm->current->GetCurrent();
			//4.7.3 �ڵ������� ���� ���� �ƴϸ�
			Long currentRowIndex = previousRowIndex;
			Long currentLetterIndex = previousLetterIndex;
			if (this->notepadForm->isRowAutoChanging == false)
			{
				//4.7.3.1 ��Ʈ���� �ܾ������ ���������� �̵��Ѵ�.
				currentLetterIndex = this->notepadForm->note->NextWord();
				//4.7.3.2 �̵��� �Ŀ� ���� ��ġ�� ���Ѵ�.
				currentRowIndex = this->notepadForm->note->GetCurrent();
				//4.7.3.3 �޸����� ���� ���� �̵��� ���� �ٷ� �����Ѵ�.
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			}
			//4.7.4 �ڵ������� ���� ���̸�
			else
			{
				//4.7.4.1 ��Ʈ���� �ڵ����� ���� ���� ���¿��� �ܾ������ ���������� �̵��Ѵ�.
				this->notepadForm->note->NextWordOnRowAutoChange(currentRowIndex, currentLetterIndex);
				//4.7.4.2 �̵��� �Ŀ� ���� ��ġ�� ���Ѵ�.
				currentRowIndex = this->notepadForm->note->GetCurrent();
				//4.7.4.3 �޸����� ���� ���� �̵��� ���� �ٷ� �����Ѵ�.
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			}
			//4.7.5 ������ �������� �ܾ������ �̵��� ������ ���� ���� ��ġ�� ���Ѵ�.
			currentRowIndex = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			currentLetterIndex = this->notepadForm->current->GetCurrent();
			Glyph* letter = 0;
			Long letterCount = 0;
			Glyph* row = 0;
			Glyph* previousRow = 0;
			Long i = 0;
			//4.7.7 ó�� �����̸�
			if (this->isRedone == false)
			{
				//4.7.7.1 DummyRow�� �����Ѵ�.
				this->glyph = new DummyRow();
				//4.7.7.2 Command�� ���� ������ ������ ǥ���Ѵ�.
				this->isDirty = true;
				//4.7.7.3 �̵��ϱ� ���� ���� ��ġ�� �̵��� ���� ���� ��ġ�� ���� ������
				if (previousRowIndex == currentRowIndex)
				{
					//4.7.7.3.1 �̵��ϱ� ���� ���� ��ġ�� ������ �������� �ܾ������ �̵��� ����
					//���� ���� ��ġ���� �������� �ݺ��Ѵ�.
					while (previousLetterIndex < currentLetterIndex)
					{
						//4.4.7.3.1.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
						letter = this->notepadForm->current->GetAt(previousLetterIndex);
						//4.4.7.3.1.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
						this->glyph->Add(letter->Clone());
						//4.4.7.3.1.3 ���ڸ� �����.
						this->notepadForm->current->Remove(previousLetterIndex);
						currentLetterIndex--;
					}
				}
				//4.4.7.4 �̵��ϱ� ���� ���� ��ġ�� �̵��� ���� ���� ��ġ�� ���� �ٸ���
				else if (previousRowIndex < currentRowIndex)
				{
					//4.4.7.4.1 �̵��ϱ� ���� ���� ���Ѵ�.
					previousRow = this->notepadForm->note->GetAt(previousRowIndex);
					//4.4.7.4.2 �̵��ϱ� ���� ������ġ���� ���� ���������� �����.
					while (previousLetterIndex < previousRow->GetLength())
					{
						//4.4.7.4.2.1 ���ڸ� ���Ѵ�.
						letter = previousRow->GetAt(previousLetterIndex);
						//4.4.7.4.2.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
						this->glyph->Add(letter->Clone());
						//4.4.7.4.2.3 ���ڸ� �����.
						previousRow->Remove(previousLetterIndex);
					}
					//4.4.7.4.3 �̵��ϱ� ���� ���� �̵��� ���� ���� ��ġ���� �������� �ݺ��Ѵ�.
					previousRowIndex++;
					while (previousRowIndex < currentRowIndex)
					{
						//4.4.7.4.3.1 �̵��ϱ� ���� ���� ���Ѵ�.
						previousRow = this->notepadForm->note->GetAt(previousRowIndex);
						i = 0;
						//4.4.7.4.3.2 �̵��ϱ� ���� ���� �������� �������� �ݺ��Ѵ�.
						while (i < previousRow->GetLength())
						{
							//4.4.7.4.3.2.1 ���ڸ� ���Ѵ�.
							letter = previousRow->GetAt(i);
							//4.4.7.4.3.2.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
							this->glyph->Add(letter->Clone());
							//4.4.7.4.4.2.3 ���ڸ� �����.
							previousRow->Remove(i);
						}
						//4.4.7.4.3.3 ���ڸ� �������� ������ ���� �����.
						this->notepadForm->note->Remove(previousRowIndex);
						//4.4.7.4.3.4 ���� ���� �������� ������ ���� ���� ��ġ�� ���ҽ����ش�.
						currentRowIndex--;
					}
					//4.4.7.4.4 ���� ���� ���Ѵ�.
					currentRowIndex = this->notepadForm->note->Move(currentRowIndex);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
					currentRow = this->notepadForm->current;
					//4.4.7.4.5 ���� ���� �� ������ ������ ���� ���� ���ڵ��� �����.
					//���� ���� ���� �������� �������� �ݺ��Ѵ�.
					i = 0;
					while (i < currentLetterIndex)
					{
						//4.4.7.4.5.1 ���ڸ� ����� ���� ���ڸ� ���Ѵ�.
						letter = currentRow->GetAt(i);
						//4.4.7.4.5.2 ���ڸ� ���� �����ؼ� DummyRow�� �����Ѵ�.
						this->glyph->Add(letter->Clone());
						//4.4.7.4.5.3 ���ڸ� �����.
						currentRow->Remove(i);
						currentLetterIndex--;
					}
					//4.4.7.4.6 ���� ���� ���ϰ� ���� ���� ���� �ٷ� �ٲ��ش�.
					currentRowIndex = this->notepadForm->note->Previous();
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
					previousRow = this->notepadForm->current;
					//4.4.7.4.7 currentRow�� previousRow�� Join��Ų��.
					currentRow->Join(previousRow);
					//4.4.7.4.8 currentRow�� �������� ������ ��Ʈ���� currentRow�� �ּҸ� �����.
					this->notepadForm->note->Remove(currentRowIndex + 1);//���� �� ��ġ+1
					//4.4.7.4.9 ���� ������ ��ġ�� �ٽ� �̵������ش�.
					currentLetterIndex = this->notepadForm->current->Move(previousLetterIndex);
				}
			}
			//4.7.8 ó�� ������ �ƴϸ�(�ٽ� �����̸�)
			else
			{
				//4.7.8.1 �̵��ϱ� ���� ���� ��ġ�� �̵��� ���� ���� ��ġ�� ���� ������
				if (previousRowIndex == currentRowIndex)
				{
					//4.7.8.1.1 �̵��ϱ� ���� ���� ��ġ�� ������ �������� �ܾ������ �̵��� ����
					//���� ���� ��ġ���� �������� �ݺ��Ѵ�.
					while (previousLetterIndex < currentLetterIndex)
					{
						//4.7.8.1.1.1 ���ڸ� �����.
						this->notepadForm->current->Remove(previousLetterIndex);
						currentLetterIndex--;
					}
				}
				//4.7.8.2 �̵��ϱ� ���� ���� ��ġ�� �̵��� ���� ���� ��ġ�� ���� �ٸ���
				else if (previousRowIndex < currentRowIndex)
				{
					//4.7.8.2.1 �̵��ϱ� ���� ���� ���Ѵ�.
					previousRow = this->notepadForm->note->GetAt(previousRowIndex);
					//4.7.8.2.2 �̵��ϱ� ���� ������ġ���� ���� ���������� �����.
					while (previousLetterIndex < previousRow->GetLength())
					{
						//4.7.7.8.2.2.1 ���ڸ� �����.
						previousRow->Remove(previousLetterIndex);
					}
					//4.7.8.2.3 �̵��ϱ� ���� ���� �̵��� ���� ���� ��ġ���� �������� �ݺ��Ѵ�.
					previousRowIndex++;
					while (previousRowIndex < currentRowIndex)
					{
						//4.7.8.2.3.1 �̵��ϱ� ���� ���� ���Ѵ�.
						previousRow = this->notepadForm->note->GetAt(previousRowIndex);
						i = 0;
						//4.7.8.2.3.2 �̵��ϱ� ���� ���� �������� �������� �ݺ��Ѵ�.
						while (i < previousRow->GetLength())
						{
							//4.7.8.2.3.2.1 ���ڸ� �����.
							previousRow->Remove(i);
						}
						//4.7.8.2.3.3 ���ڸ� �������� ������ ���� �����.
						this->notepadForm->note->Remove(previousRowIndex);
						//4.7.8.2.3.4 ���� ���� �������� ������ ���� ���� ��ġ�� ���ҽ����ش�.
						currentRowIndex--;
					}
					//4.7.8.2.4 ���� ���� ���Ѵ�.
					currentRowIndex = this->notepadForm->note->Move(currentRowIndex);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
					currentRow = this->notepadForm->current;
					//4.7.8.2.5 ���� ���� �� ������ ������ ���� ���� ���ڵ��� �����.
					//���� ���� ���� �������� �������� �ݺ��Ѵ�.
					i = 0;
					while (i < currentLetterIndex)
					{
						//4.7.8.2.5.1 ���ڸ� �����.
						currentRow->Remove(i);
						currentLetterIndex--;
					}
					//4.7.8.2.6 ���� ���� ���ϰ� ���� ���� ���� �ٷ� �ٲ��ش�.
					currentRowIndex = this->notepadForm->note->Previous();
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
					previousRow = this->notepadForm->current;
					//4.7.8.2.7 currentRow�� previousRow�� Join��Ų��.
					currentRow->Join(previousRow);
					//4.7.8.2.8 currentRow�� �������� ������ ��Ʈ���� currentRow�� �ּҸ� �����.
					this->notepadForm->note->Remove(currentRowIndex + 1);//���� �� ��ġ+1
					//4.7.8.2.9 ���� ������ ��ġ�� �ٽ� �̵������ش�.
					currentLetterIndex = this->notepadForm->current->Move(previousLetterIndex);
				}
			}
		}
		//4.8 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//4.8.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
	//5. �޸��忡�� ���õ� texts�� �ְų� �Ǵ� ���õ� ������ ��������
	else if (this->notepadForm->isSelecting == true || this->isSelectedTextsRemoved == true)
	{
		//5.1 �ٽ� �����̸�
		if (this->isRedone == true)
		{
			//5.1.1 notepadForm�� ������ ���۵Ǵ� ���� ��ġ�� ���� ��ġ�� �缳�����ش�. 
			this->notepadForm->selectedStartYPos = this->selectedStartYPos;
			this->notepadForm->selectedStartXPos = this->selectedStartXPos;
		}
		//5.2 ������ ���۵Ǵ� �ٰ� ���� ��ġ, ������ ������ �ٰ� ���� ��ġ�� �����Ѵ�.
		Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//������ ���۵Ǵ� ��
		Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//������ ���۵Ǵ� ����
		Long selectedEndRowPos = currentRowPos;//������ ������ ��
		Long selectedEndLetterPos = currentLetterPos;//������ ������ ����
		//5.3 ó�� �����̸�
		if (this->isRedone == false)
		{
			//5.3.1 content�� �����ϰ� �����.
			this->glyph = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
				selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
			//5.3.2 Command�� ���� ������ ������ ǥ���Ѵ�.
			this->isDirty = true;
		}
		//5.4 �ٽ� �����̸�
		else
		{
			//5.4.1 content�� �����.
			this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
				selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
		}
		//5.5 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.(note�ǿ���ȿ��� ���� ���� ��ġ�� ���� ��ġ��
		//������ ������ notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
		this->notepadForm->current = this->notepadForm->note->
			GetAt(this->notepadForm->note->GetCurrent());
		//5.6 �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//5.6.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
			//OnSize���� �κ��ڵ������� �ϵ��� �Ѵ�. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
		//5.6 �޸��忡�� ���õ� texts�� �� ������ ������ �޸��忡�� ������ �ȵ� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = false;
		//5.7 ���õ� texts�� ������ ������ command�� ���õ� ������ �����ٰ� ǥ���Ѵ�.
		this->isSelectedTextsRemoved = true;
		//5.8 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartXPos = 0;
		//5.9 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartYPos = 0;
		//5.10 �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
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
	}
	//7. ���ڸ� ���� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//8 �ڵ������� �������̸�(command�� �ٰ� ���� ��ġ�� �׻� ��¥ �ٰ� ���� ��ġ�� �����ؾ���)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//8.1 ����� ȭ�� ũ�⿡ �´� �ٰ� ĳ���� ��ġ�� ���Ѵ�.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//8.2 command�� ���ڸ� �Է��� �Ŀ� ���� ���� ��ġ�� ������ġ�� �ٽ� �����Ѵ�.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
	}
}

//�������
void CtrlDeleteKeyActionCommand::Unexecute()
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
	//5. ���� �� ������ glyph�� ��(���๮��)�� �ƴϸ�
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
			//5.1.5 ���� ��ġ�� ���� �ٿ��� ���� ������ �̵���Ų��.
			currentRow->Move(currentLetterPos);
		}
		//5.2 ���� ���� ���� ��ġ�� ���� ���� ���ڰ����� �ٸ���
		else
		{
			//5.2.1 dummyRow�� ������ŭ �ݺ��Ѵ�.
			Glyph* letter = 0;
			Long i = this->glyph->GetLength() - 1;
			while (i >= 0)
			{
				//5.2.1.1 ���ڸ� ���Ѵ�.
				letter = this->glyph->GetAt(i);
				//5.2.1.2 ���� ���� ���� ��ġ�� dummyRow���� ���� ���縦 �� letter�� ���� �ִ´�.
				this->notepadForm->current->Add(currentLetterPos, letter->Clone());
				//5.2.1.3 i�� ���ҽ�Ų��.
				i--;
			}
			//5.2.2 ���� ��ġ�� ���������ش�.
			currentLetterPos = this->notepadForm->current->Move(currentLetterPos);
		}
	}
	//6. ���� �� ������ glyph�� ��(���๮��)�̸�
	else if (dynamic_cast<Row*>(this->glyph))
	{
		//6.1 ������ ����� ���� ������ �Ҵ������Ѵ�.
		if (this->glyph != 0)
		{
			delete this->glyph;
		}
		//6.2. ���� �ٿ��� ���� ���� ���� ��ġ�� �ִ� ���ڵ��� ��� ���ο� ���� �����.
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
		//6.5 ���� ���� ���� �����Ѵ�.(���� ������ ���� �ƴ϶� �и��� ���� ���� �ٷ� �Ѵ�.)
		currentRowPos = this->notepadForm->note->Move(currentRowPos - 1);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//6.6 ���� ���� ���� ��ġ�� ���������� �̵���Ų��.
		this->notepadForm->current->Last();
		//6.7 �ڵ� �� �ٲ��� �������̸�
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//6.7.1 OnSize�� �޼����� ���� �ʱ� ������ OnSize�� ���� �޼����� ������
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
			currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
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
void CtrlDeleteKeyActionCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void CtrlDeleteKeyActionCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}


//SetRedone(�ٽ� �����̶�� ������)
void CtrlDeleteKeyActionCommand::SetRedone()
{
	this->isRedone = true;
}

//���� ��ġ ���ϱ�
Long CtrlDeleteKeyActionCommand::GetRowIndex()
{
	return this->rowIndex;
}
//���� ��ġ ���ϱ�
Long CtrlDeleteKeyActionCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//������ ���۵Ǵ� ������ ��ġ���ϱ�
Long CtrlDeleteKeyActionCommand::GetSelectedStartXPos()
{
	return this->selectedStartXPos;
}

//������ ���۵Ǵ� ���� ��ġ���ϱ�
Long CtrlDeleteKeyActionCommand::GetSelectedStartYPos()
{
	return this->selectedStartYPos;
}
//������� ������������ ���ϱ�
bool CtrlDeleteKeyActionCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool CtrlDeleteKeyActionCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//�ٽý������� ���� ���ϱ�
bool CtrlDeleteKeyActionCommand::IsRedone()
{
	return this->isRedone;
}
//��������� �ִ��� Ȯ�� ����
bool CtrlDeleteKeyActionCommand::IsDirty()
{
	return this->isDirty;
}
//���ÿ����� ���������� Ȯ�� ����
bool CtrlDeleteKeyActionCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}
//�Ҹ��� ����
CtrlDeleteKeyActionCommand::~CtrlDeleteKeyActionCommand()
{
	//1. DummyRow�� Row�� �Ҵ������Ѵ�.
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}