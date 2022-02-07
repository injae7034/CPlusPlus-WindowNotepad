#include "OnReplaceAllButtonClickedCommand.h"
#include "CommandHistory.h"
#include "Row.h"
#include "RowAutoChange.h"
#include "DummyRow.h"
#include "TextExtent.h"
#include "SelectingTexts.h"
#include "ReplacingDialog.h"
#include "GlyphFinder.h"
#include "OnReplaceButtonClickedCommand.h"

//����Ʈ������
OnReplaceAllButtonClickedCommand::OnReplaceAllButtonClickedCommand(NotepadForm* notepadForm,
	Long undoListCapacity, Long redoListCapacity)
	:Command(notepadForm), 
	findingKeyword(((ReplacingDialog*)(this->notepadForm->findReplaceDialog))->findingKeyword),
	replacingKeyword(((ReplacingDialog*)(this->notepadForm->findReplaceDialog))->replacingKeyword),
	undoList(), redoList()
{
	this->matchCaseChecked = ((ReplacingDialog*)(this->notepadForm->findReplaceDialog))
		->matchCaseChecked;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->undoListCapacity = undoListCapacity;
	this->undoListLength = 0;
	this->redoListCapacity = redoListCapacity;
	this->redoListLength = 0;
}

//ó�� �� �ٽ� ����
void OnReplaceAllButtonClickedCommand::Execute()
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
		//1.5 �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
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
	bool isFounded = true;//ã�� �ܾ �߰��� �Ǿ����� �ƴ����� �Ǻ��� flag
	Command* command = 0;//OnRepalceButtonClickedCommand�� ���� ����
	Long i = 0;//�ݺ������

	//4. ó�� �����̸�
	if (this->isRedone == false)
	{
		//4.1 ��/�ҹ��� ������ �Ǿ� ������
		if (this->matchCaseChecked == BST_CHECKED)
		{
			//4.1.1 ã���� �ִ� ���� �ݺ��Ѵ�.
			while (isFounded == true)
			{
				//4.1.1.1 �Ʒ��� ã�⸦ �����Ѵ�.
				glyphFinder.FindDown(this->findingKeyword, &findingStartRowIndex,
					&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
				//4.1.1.2 ã�� �� ������
				if (findingStartRowIndex != findingEndRowIndex ||
					findingStartLetterIndex != findingEndLetterIndex)
				{
					//4.1.1.2.1 ������ ���۵Ǳ� ������ ������ ���۵��� ǥ���Ѵ�.
					this->notepadForm->isSelecting = true;
					//4.1.1.2.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
					this->notepadForm->selectedStartXPos = findingStartLetterIndex;
					//4.1.1.2.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
					this->notepadForm->selectedStartYPos = findingStartRowIndex;
					//4.1.2.2.4 ã�� ���ڸ� �����Ѵ�.
					this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
						findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
					//4.1.2.2.5 ĳ���� ��ġ�� �޸����� ã�� ���ڿ��� �ִ� ���� ã�� ���ڿ� ������ ������ġ�� �̵��Ѵ�.
					currentRowPos = this->notepadForm->note->Move(findingEndRowIndex);
					this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
					currentLetterPos = this->notepadForm->current->Move(findingEndLetterIndex);
					//4.1.2.2.6 OnReplaceButtonClikedCommand�� �����Ѵ�.
					command = new OnReplaceButtonClickedCommand(this->notepadForm);
					//4.1.2.2.7 undoList�� ��뷮�� �Ҵ緮���� ũ�ų� ������
					if (this->undoListLength >= this->undoListCapacity)
					{
						//4.1.2.2.7.1 undoList�� �Ҵ緮�� ������Ų��.
						this->undoListCapacity++;
					}
					//4.1.2.2.8 undoList�� ������ OnReplaceButtonClikedCommand�� �ִ´�.
					this->undoList.Push(command);
					//4.1.2.2.9 undoList�� ��뷮�� ������Ų��.
					this->undoListLength++;
					//4.1.2.2.10 OnReplaceButtonClikedCommand�� Execute�Ѵ�.
					command->Execute();
				}
				//4.1.1.3 ã�� �� ������
				else
				{
					//4.1.1.3.1 ã�� �� ���ٰ� ǥ���Ѵ�.
					isFounded = false;
				}
			}

		}
		//4.2 ��/�ҹ��� ������ �ȵǾ������
		else
		{
			//4.2.1 ã���� �ִ� ���� �ݺ��Ѵ�.
			while (isFounded == true)
			{
				//4.2.1.1 ��/�ҹ��� ���о��� �Ʒ��� ã�⸦ �����Ѵ�.
				glyphFinder.FindDownWithMatchCase(this->findingKeyword, &findingStartRowIndex,
					&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
				//4.2.1.2 ã�� �� ������
				if (findingStartRowIndex != findingEndRowIndex ||
					findingStartLetterIndex != findingEndLetterIndex)
				{
					//4.2.1.2.1 ������ ���۵Ǳ� ������ ������ ���۵��� ǥ���Ѵ�.
					this->notepadForm->isSelecting = true;
					//4.2.1.2.2 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
					this->notepadForm->selectedStartXPos = findingStartLetterIndex;
					//4.2.1.2.3 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
					this->notepadForm->selectedStartYPos = findingStartRowIndex;
					//4.2.2.2.4 ã�� ���ڸ� �����Ѵ�.
					this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
						findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
					//4.2.2.2.5 ĳ���� ��ġ�� �޸����� ã�� ���ڿ��� �ִ� ���� ã�� ���ڿ� ������ ������ġ�� �̵��Ѵ�.
					currentRowPos = this->notepadForm->note->Move(findingEndRowIndex);
					this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
					currentLetterPos = this->notepadForm->current->Move(findingEndLetterIndex);
					//4.2.2.2.6 OnReplaceButtonClikedCommand�� �����Ѵ�.
					command = new OnReplaceButtonClickedCommand(this->notepadForm);
					//4.2.2.2.7 undoList�� ��뷮�� �Ҵ緮���� ũ�ų� ������
					if (this->undoListLength >= this->undoListCapacity)
					{
						//4.2.2.2.7.1 undoList�� �Ҵ緮�� ������Ų��.
						this->undoListCapacity++;
					}
					//4.2.2.2.8 undoList�� ������ OnReplaceButtonClikedCommand�� �ִ´�.
					this->undoList.Push(command);
					//4.2.2.2.9 undoList�� ��뷮�� ������Ų��.
					this->undoListLength++;
					//4.2.2.2.10 OnReplaceButtonClikedCommand�� Execute�Ѵ�.
					command->Execute();
				}
				//4.2.1.3 ã�� �� ������
				else
				{
					//4.2.1.3.1 ã�� �� ���ٰ� ǥ���Ѵ�.
					isFounded = false;
				}
			}
		}
		//4.3 �޸��� ���� *�� �߰��Ѵ�.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//4.4 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->isDirty = true;
	}
	//5. �ٽ� �����̸�
	else
	{
		//5.1 redoList ��뷮��ŭ �ݺ��Ѵ�.
		i = 0;
		while (i < this->redoListLength)
		{
			//5.1.1 redoList���� ���� �ֱ� command�� ����.
			command = this->redoList.Pop();
			//5.1.2 redoList�� �Ҵ緮�� ���ҽ�Ų��.
			this->redoListCapacity--;
			//5.1.3 redoList�� ��뷮�� ���ҽ�Ų��.
			this->redoListLength--;
			//5.1.4 undoList�� ��뷮�� �Ҵ緮���� ũ�ų� ������
			if (this->undoListLength >= this->undoListCapacity)
			{
				//5.1.4.1 undoList�� �Ҵ緮�� ������Ų��.
				this->undoListCapacity++;
			}
			//5.1.5 undoList�� OnReplaceButtonClickedCommand�� �ִ´�.
			this->undoList.Push(command);
			//5.1.6 undoList�� ��뷮�� ������Ų��.
			this->undoListLength++;
			//5.1.7 ���� OnReplaceButtonClickedCommand�� Execute �Ǳ� ���� �ٽ� �����̶�� ǥ�ø� �Ѵ�.
			command->SetRedone();
			//5.1.8 OnReplaceButtonClickedCommand�� Execute�Ѵ�.
			command->Execute();
		}
	}
	//6. ���� ���� ��ġ�� ���� ��ġ�� ó������ �̵���Ų��.
	currentRowPos = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->First();
}

//�ؽ������������쿡 �ڵ������� ����� �ȵ�!
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
	//2. undoList ��뷮��ŭ �ݺ��Ѵ�.
	Long i = 0;
	Command* command = 0;//OnReplaceButtonClickedCommand�� ���� ����
	while (i < this->undoListLength)
	{
		//2.1 undoList���� ���� �ֱ� command�� ����.
		command = this->undoList.Pop();
		//2.2 undoList�� �Ҵ緮�� ���ҽ�Ų��.
		this->undoListCapacity--;
		//2.3 undoList�� ��뷮�� ���ҽ�Ų��.
		this->undoListLength--;
		//2.4 redoList�� ��뷮�� �Ҵ緮���� ũ�ų� ������
		if (this->redoListLength >= this->redoListCapacity)
		{
			//2.4.1 redoList�� �Ҵ緮�� ������Ų��.
			this->redoListCapacity++;
		}
		//2.5 redoList�� OnReplaceButtonClickedCommand�� �ִ´�.
		this->redoList.Push(command);
		//2.6 redoList�� ��뷮�� ������Ų��.
		this->redoListLength++;
		//2.7 OnReplaceButtonClickedCommand�� Unexecute�Ѵ�.
		command->Unexecute();
	}
	//OnReplaceButtonClickedCommand�� Unexecute�ϸ� ���ÿ����� �����Ǽ� ���ÿ����� ����� ������
	//���ÿ����� �ٽ� �����ش�.
	//3. ������ ����ǰ� �ִ� ���̾�����()
	if (this->notepadForm->isSelecting == true)
	{
		//3.1. ���õ� �ؽ�Ʈ�� ���������Ѵ�.(������ ������.)
		this->notepadForm->selectingTexts->Undo();
		//3.2 ������ ���� ���·� �ٲ۴�.
		this->notepadForm->isSelecting = false;
		//3.3 ������ ������ ������ ĳ���� x��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartXPos = 0;
		//3.4 ������ ������ ������ ĳ���� y��ǥ�� 0���� �����Ѵ�.
		this->notepadForm->selectedStartYPos = 0;
	}
	//4. ���� ���� ��ġ�� ���� ��ġ�� ���������� �̵���Ų��.
	Long currentRowPos = this->notepadForm->note->Last();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	Long currentLetterPos = this->notepadForm->current->Last();
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
	Command* command = 0;
	//1. UndoList�� �Ҵ��������ش�.
	while (this->undoList.IsEmpty() == false)
	{
		//1.1 undoList�� ����� �ּҰ� ����Ű�� ���� �Ҵ�� ������� �Ҵ��������ش�.
		command = this->undoList.Pop();
		if (command != 0)
		{
			delete command;
			command = 0;
		}
		//1.2 �Ҵ緮�� ���ҽ�Ų��.
		this->undoListCapacity--;
		//1.3 ��뷮�� ���ҽ�Ų��.
		this->undoListLength--;
	}
	//2. RedoList�� �Ҵ��������ش�.
	while (this->redoList.IsEmpty() == false)
	{
		//2.1 redoList�� ����� �ּҰ� ����Ű�� ���� �Ҵ�� ������� �Ҵ��������ش�.
		command = this->redoList.Pop();
		if (command != 0)
		{
			delete command;
			command = 0;
		}
		//2.2 �Ҵ緮�� ���ҽ�Ų��.
		this->redoListCapacity--;
		//2.3 ��뷮�� ���ҽ�Ų��.
		this->redoListLength--;
	}
}