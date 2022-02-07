#include "OnReplaceAllButtonClickedCommand.h"
#include "CommandHistory.h"
#include "Row.h"
#include "RowAutoChange.h"
#include "DummyRow.h"
#include "TextExtent.h"
#include "SelectingTexts.h"
#include "ReplacingDialog.h"
#include "GlyphFinder.h"

//����Ʈ������
OnReplaceAllButtonClickedCommand::OnReplaceAllButtonClickedCommand(NotepadForm* notepadForm)
	:Command(notepadForm), 
	findingKeyword(((ReplacingDialog*)(this->notepadForm->findReplaceDialog))->findingKeyword),
	replacingKeyword(((ReplacingDialog*)(this->notepadForm->findReplaceDialog))->replacingKeyword)
{
	this->matchCaseChecked = ((ReplacingDialog*)(this->notepadForm->findReplaceDialog))
		->matchCaseChecked;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
}

//ó�� �� �ٽ� ����
void OnReplaceAllButtonClickedCommand::Execute()
{
	//1. ���� ���� ��ġ�� ���� ��ġ�� ó������ �̵���Ų��.
	Long currentRowPos = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	Long currentLetterPos = this->notepadForm->current->First();
	//2. GlyphFinder�� �����Ѵ�.
	GlyphFinder glyphFinder(this->notepadForm->note);

	Long findingStartRowIndex = 0;//ã�� �ܾ��� ���� ����ġ
	Long findingStartLetterIndex = 0;//ã�� �ܾ��� ���� ������ġ
	Long findingEndRowIndex = 0;//ã�� �ܾ��� �� ����ġ
	Long findingEndLetterIndex = 0;//ã�� �ܾ��� �� ������ġ
	Long selectedStartRowPos = 0;//������ ���۵Ǵ� ���� ��ġ
	Long selectedStartLetterPos = 0;//������ ���۵Ǵ� ���� ��ġ
	Long selectedEndRowPos = 0;//������ ������ �� ��ġ
	Long selectedEndLetterPos = 0;//������ ������ ���� ��ġ
	bool isFounded = true;//ã�� �ܾ �߰��� �Ǿ����� �ƴ����� �Ǻ��� flag
	//3. ��/�ҹ��� ������ �Ǿ� ������
	if (this->matchCaseChecked == BST_CHECKED)
	{
		//3.1 ã���� �ִ� ���� �ݺ��Ѵ�.
		while (isFounded == true)
		{
			//3.1.1 �Ʒ��� ã�⸦ �����Ѵ�.
			glyphFinder.FindDown(this->findingKeyword, &findingStartRowIndex,
				&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
			//3.1.2 ã�� �� ������
			if (findingStartRowIndex != findingEndRowIndex ||
				findingStartLetterIndex != findingEndLetterIndex)
			{
				//3.1.2.1 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
				this->notepadForm->selectedStartXPos = findingStartLetterIndex;
				//3.1.2.2 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
				this->notepadForm->selectedStartYPos = findingStartRowIndex;
				//3.1.2.3 ã�� ���ڸ� �����Ѵ�.
				this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
					findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
				//3.1.2.4 ĳ���� ��ġ�� �޸����� ã�� ���ڿ��� �ִ� ���� ã�� ���ڿ� ������ ������ġ�� �̵��Ѵ�.
				currentRowPos = this->notepadForm->note->Move(findingEndRowIndex);
				this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
				currentLetterPos = this->notepadForm->current->Move(findingEndLetterIndex);
				//3.1.2.5 ������ ���۵Ǵ� �ٰ� ���� ��ġ, ������ ������ �ٰ� ���� ��ġ�� �����Ѵ�.
				selectedStartRowPos = this->notepadForm->selectedStartYPos;//������ ���۵Ǵ� ��
				selectedStartLetterPos = this->notepadForm->selectedStartXPos;//������ ���۵Ǵ� ����
				selectedEndRowPos = currentRowPos;//������ ������ ��
				selectedEndLetterPos = currentLetterPos;//������ ������ ����
				//3.1.2.6 �ܾ �����.
				this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
				//3.1.2.7 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.
				//(note�ǿ���ȿ��� ���� ���� ��ġ�� ���� ��ġ�� ������ ������ 
				//notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//3.1.2.8 ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
				this->notepadForm->selectingTexts->Undo();
				//3.1.2.9 ������ ���� ���·� �ٲ۴�.
				this->notepadForm->isSelecting = false;
				//3.1.2.10 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
				this->notepadForm->selectedStartXPos = 0;
				//3.1.2.11 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
				this->notepadForm->selectedStartYPos = 0;
				//3.1.2.12 ���� �ٿ��� �ܾ������ �߰��Ѵ�.
				this->notepadForm->current->AddWord(this->replacingKeyword);
				//3.1.2.13 �ڵ������� �������̸� �ܾ �߰��ϰ� �ڵ���������ش�.
				if (this->notepadForm->isRowAutoChanging == true)
				{
					this->notepadForm->SendMessage(WM_SIZE);
				}
			}
			//3.1.3 ã�� �� ������
			else
			{
				isFounded = false;
			}
		}
	}
	//4. ��/�ҹ��� ������ �ȵǾ������
	else
	{
		//4.1 ã���� �ִ� ���� �ݺ��Ѵ�.
		while (isFounded == true)
		{
			//4.1.1 ��/�ҹ��� ���о��� �Ʒ��� ã�⸦ �����Ѵ�.
			glyphFinder.FindDownWithMatchCase(this->findingKeyword, &findingStartRowIndex,
				&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
			//4.1.2 ã�� �� ������
			if (findingStartRowIndex != findingEndRowIndex ||
				findingStartLetterIndex != findingEndLetterIndex)
			{
				//4.1.2.1 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
				this->notepadForm->selectedStartXPos = findingStartLetterIndex;
				//4.1.2.2 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
				this->notepadForm->selectedStartYPos = findingStartRowIndex;
				//4.1.2.3 ã�� ���ڸ� �����Ѵ�.
				this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
					findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
				//4.1.2.4 ĳ���� ��ġ�� �޸����� ã�� ���ڿ��� �ִ� ���� ã�� ���ڿ� ������ ������ġ�� �̵��Ѵ�.
				currentRowPos = this->notepadForm->note->Move(findingEndRowIndex);
				this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
				currentLetterPos = this->notepadForm->current->Move(findingEndLetterIndex);
				//4.1.2.5 ������ ���۵Ǵ� �ٰ� ���� ��ġ, ������ ������ �ٰ� ���� ��ġ�� �����Ѵ�.
				selectedStartRowPos = this->notepadForm->selectedStartYPos;//������ ���۵Ǵ� ��
				selectedStartLetterPos = this->notepadForm->selectedStartXPos;//������ ���۵Ǵ� ����
				selectedEndRowPos = currentRowPos;//������ ������ ��
				selectedEndLetterPos = currentLetterPos;//������ ������ ����
				//4.1.2.6 content�� �����.
				this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
				//4.1.2.7 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.
				//(note�ǿ���ȿ��� ���� ���� ��ġ�� ���� ��ġ�� ������ ������ 
				//notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//4.1.2.8 ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
				this->notepadForm->selectingTexts->Undo();
				//4.1.2.9 ������ ���� ���·� �ٲ۴�.
				this->notepadForm->isSelecting = false;
				//4.1.2.10 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
				this->notepadForm->selectedStartXPos = 0;
				//4.1.2.11 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
				this->notepadForm->selectedStartYPos = 0;
				//4.1.2.12 ���� �ٿ��� �ܾ������ �߰��Ѵ�.
				this->notepadForm->current->AddWord(this->replacingKeyword);
				//4.1.2.13 �ڵ������� �������̸� �ܾ �߰��ϰ� �ڵ���������ش�.
				if (this->notepadForm->isRowAutoChanging == true)
				{
					this->notepadForm->SendMessage(WM_SIZE);
				}
			}
			//4.1.3 ã�� �� ������
			else
			{
				isFounded = false;
			}
		}
	}
	//5. ó�� �����̸�
	if (this->isRedone == false)
	{
		//5.1 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//5.2 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->isDirty = true;
	}
	//6. ���� ���� ��ġ�� ���� ��ġ�� ó������ �̵���Ų��.
	currentRowPos = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->First();
}

//�������
void OnReplaceAllButtonClickedCommand::Unexecute()
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
	//2. ���� ���� ��ġ�� ���� ��ġ�� ó������ �̵���Ų��.
	Long currentRowPos = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	Long currentLetterPos = this->notepadForm->current->First();
	//3. GlyphFinder�� �����Ѵ�.
	GlyphFinder glyphFinder(this->notepadForm->note);

	Long findingStartRowIndex = 0;//ã�� �ܾ��� ���� ����ġ
	Long findingStartLetterIndex = 0;//ã�� �ܾ��� ���� ������ġ
	Long findingEndRowIndex = 0;//ã�� �ܾ��� �� ����ġ
	Long findingEndLetterIndex = 0;//ã�� �ܾ��� �� ������ġ
	Long selectedStartRowPos = 0;//������ ���۵Ǵ� ���� ��ġ
	Long selectedStartLetterPos = 0;//������ ���۵Ǵ� ���� ��ġ
	Long selectedEndRowPos = 0;//������ ������ �� ��ġ
	Long selectedEndLetterPos = 0;//������ ������ ���� ��ġ
	bool isFounded = true;//ã�� �ܾ �߰��� �Ǿ����� �ƴ����� �Ǻ��� flag
	//4. ��/�ҹ��� ������ �Ǿ� ������
	if (this->matchCaseChecked == BST_CHECKED)
	{
		//4.1 ã���� �ִ� ���� �ݺ��Ѵ�.
		while (isFounded == true)
		{
			//4.1.1 �Ʒ��� ã�⸦ �����Ѵ�.
			glyphFinder.FindDown(this->replacingKeyword, &findingStartRowIndex,
				&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
			//4.1.2 ã�� �� ������
			if (findingStartRowIndex != findingEndRowIndex ||
				findingStartLetterIndex != findingEndLetterIndex)
			{
				//4.1.2.1 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
				this->notepadForm->selectedStartXPos = findingStartLetterIndex;
				//4.1.2.2 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
				this->notepadForm->selectedStartYPos = findingStartRowIndex;
				//4.1.2.3 ã�� ���ڸ� �����Ѵ�.
				this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
					findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
				//4.1.2.4 ĳ���� ��ġ�� �޸����� ã�� ���ڿ��� �ִ� ���� ã�� ���ڿ� ������ ������ġ�� �̵��Ѵ�.
				currentRowPos = this->notepadForm->note->Move(findingEndRowIndex);
				this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
				currentLetterPos = this->notepadForm->current->Move(findingEndLetterIndex);
				//4.1.2.5 ������ ���۵Ǵ� �ٰ� ���� ��ġ, ������ ������ �ٰ� ���� ��ġ�� �����Ѵ�.
				selectedStartRowPos = this->notepadForm->selectedStartYPos;//������ ���۵Ǵ� ��
				selectedStartLetterPos = this->notepadForm->selectedStartXPos;//������ ���۵Ǵ� ����
				selectedEndRowPos = currentRowPos;//������ ������ ��
				selectedEndLetterPos = currentLetterPos;//������ ������ ����
				//4.1.2.6 �ܾ �����.
				this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
				//4.1.2.7 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.
				//(note�ǿ���ȿ��� ���� ���� ��ġ�� ���� ��ġ�� ������ ������ 
				//notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//4.1.2.8 ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
				this->notepadForm->selectingTexts->Undo();
				//4.1.2.9 ������ ���� ���·� �ٲ۴�.
				this->notepadForm->isSelecting = false;
				//4.1.2.10 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
				this->notepadForm->selectedStartXPos = 0;
				//4.1.2.11 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
				this->notepadForm->selectedStartYPos = 0;
				//4.1.2.12 ���� �ٿ��� �ܾ������ �߰��Ѵ�.
				this->notepadForm->current->AddWord(this->findingKeyword);
				//4.1.2.13 �ڵ������� �������̸� �ܾ �߰��ϰ� �ڵ���������ش�.
				if (this->notepadForm->isRowAutoChanging == true)
				{
					this->notepadForm->SendMessage(WM_SIZE);
				}
			}
			//4.1.3 ã�� �� ������
			else
			{
				isFounded = false;
			}
		}
	}
	//5. ��/�ҹ��� ������ �ȵǾ������
	else
	{
		//5.1 ã���� �ִ� ���� �ݺ��Ѵ�.
		while (isFounded == true)
		{
			//5.1.1 ��/�ҹ��� ���о��� �Ʒ��� ã�⸦ �����Ѵ�.
			glyphFinder.FindDownWithMatchCase(this->replacingKeyword, &findingStartRowIndex,
				&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
			//5.1.2 ã�� �� ������
			if (findingStartRowIndex != findingEndRowIndex ||
				findingStartLetterIndex != findingEndLetterIndex)
			{
				//5.1.2.1 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
				this->notepadForm->selectedStartXPos = findingStartLetterIndex;
				//5.1.2.2 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
				this->notepadForm->selectedStartYPos = findingStartRowIndex;
				//5.1.2.3 ã�� ���ڸ� �����Ѵ�.
				this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
					findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
				//5.1.2.4 ĳ���� ��ġ�� �޸����� ã�� ���ڿ��� �ִ� ���� ã�� ���ڿ� ������ ������ġ�� �̵��Ѵ�.
				currentRowPos = this->notepadForm->note->Move(findingEndRowIndex);
				this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
				currentLetterPos = this->notepadForm->current->Move(findingEndLetterIndex);
				//5.1.2.5 ������ ���۵Ǵ� �ٰ� ���� ��ġ, ������ ������ �ٰ� ���� ��ġ�� �����Ѵ�.
				selectedStartRowPos = this->notepadForm->selectedStartYPos;//������ ���۵Ǵ� ��
				selectedStartLetterPos = this->notepadForm->selectedStartXPos;//������ ���۵Ǵ� ����
				selectedEndRowPos = currentRowPos;//������ ������ ��
				selectedEndLetterPos = currentLetterPos;//������ ������ ����
				//5.1.2.6 content�� �����.
				this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
				//5.1.2.7 ������ ������ ������ ���� ���� ��ġ�� �ٽ� �������ش�.
				//(note�ǿ���ȿ��� ���� ���� ��ġ�� ���� ��ġ�� ������ ������ 
				//notepadForm�� current(������)�� ������ �� ��� notepadForm���� ���ش�.)
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//5.1.2.8 ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
				this->notepadForm->selectingTexts->Undo();
				//5.1.2.9 ������ ���� ���·� �ٲ۴�.
				this->notepadForm->isSelecting = false;
				//5.1.2.10 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
				this->notepadForm->selectedStartXPos = 0;
				//5.1.2.11 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
				this->notepadForm->selectedStartYPos = 0;
				//5.1.2.12 ���� �ٿ��� �ܾ������ �߰��Ѵ�.
				this->notepadForm->current->AddWord(this->findingKeyword);
				//5.1.2.13 �ڵ������� �������̸� �ܾ �߰��ϰ� �ڵ���������ش�.
				if (this->notepadForm->isRowAutoChanging == true)
				{
					this->notepadForm->SendMessage(WM_SIZE);
				}
			}
			//5.1.3 ã�� �� ������
			else
			{
				isFounded = false;
			}
		}
	}
	//6. ���� ���� ��ġ�� ���� ��ġ�� ���������� �̵���Ų��.
	currentRowPos = this->notepadForm->note->Last();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->Last();
}

//�ٽý������� ���� ���ϱ�
bool OnReplaceAllButtonClickedCommand::IsRedone()
{
	return this->isRedone;
}

//SetMacroEnd(������� �� �ٽý��� ��ũ����� �������� ����)
void OnReplaceAllButtonClickedCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void OnReplaceAllButtonClickedCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//������� ������������ ���ϱ�
bool OnReplaceAllButtonClickedCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//�ٽý��� ������������ ���ϱ� 
bool OnReplaceAllButtonClickedCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}

//SetRedone(�ٽ� �����̶�� ������)
void OnReplaceAllButtonClickedCommand::SetRedone()
{
	this->isRedone = true;
}

//�Ҹ���
OnReplaceAllButtonClickedCommand::~OnReplaceAllButtonClickedCommand()
{
	
}