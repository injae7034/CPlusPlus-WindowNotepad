#include "RowAutoChangeCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "DummyRow.h"
#include "RowAutoChange.h"
#include "SelectingTexts.h"

//����Ʈ������
RowAutoChangeCommand::RowAutoChangeCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	
}

//Execute
void RowAutoChangeCommand::Execute()
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
	//2. ������ texts���� �� �������������
	if (this->notepadForm->isSelecting == false)
	{
		//2.1 �����ϱ�, �߶󳻱�, ���� �޴��� ��Ȱ��ȭ ��Ų��.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	//3. RowAutoChange�� �����Ѵ�.(���� �Ҵ��ϸ� ���߿� ���� �Ҵ������� �������
	//�׷��� �ּҾ��� ���ÿ� �Ҵ��ϸ� �� �Լ� ������ ����Ǹ� �ڵ����� ���� �����.)
	//���⼭�� �� ���ÿ����� RowAutoChange�� ������ ���� ���Ѱ��̱� ������ ���ÿ� �Ҵ��ϴ°� ȿ������!
	RowAutoChange rowAutoChange(this->notepadForm);
	//4. �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
	UINT state = this->notepadForm->GetMenu()->
		GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
	//5. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ���� ������
	if (state == MF_UNCHECKED)
	{
		//5.1 �ڵ� �� �ٲ� �޴��� üũ���ش�.
		this->notepadForm->GetMenu()->
			CheckMenuItem(IDM_ROW_AUTOCHANGE, MF_CHECKED | MF_BYCOMMAND);
		//5.2 �ڵ������� �����Ѵ�.
		rowAutoChange.DoAllRows();
		this->notepadForm->isRowAutoChanging = true;
	}
	//6. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
	else
	{
		//6.1 �ڵ� �� �ٲ� �޴��� �ִ� üũ�� ���ش�.	
		this->notepadForm->GetMenu()->
			CheckMenuItem(IDM_ROW_AUTOCHANGE, MF_UNCHECKED | MF_BYCOMMAND);
		//6.2 �ڵ������� ����Ѵ�.
		rowAutoChange.UndoAllRows();
		this->notepadForm->isRowAutoChanging = false;
	}
	//7. ĳ���� ���� ó������ ������.
	this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->
		GetAt(this->notepadForm->note->GetCurrent());
	this->notepadForm->current->First();
}


//�Ҹ���
RowAutoChangeCommand::~RowAutoChangeCommand()
{

}