#include "RowAutoChangeCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "DummyRow.h"
#include "RowAutoChange.h"

//����Ʈ������
RowAutoChangeCommand::RowAutoChangeCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	
}

//Execute
void RowAutoChangeCommand::Execute()
{
	//1. RowAutoChange�� �����Ѵ�.(���� �Ҵ��ϸ� ���߿� ���� �Ҵ������� �������
	//�׷��� �ּҾ��� ���ÿ� �Ҵ��ϸ� �� �Լ� ������ ����Ǹ� �ڵ����� ���� �����.)
	//���⼭�� �� ���ÿ����� RowAutoChange�� ������ ���� ���Ѱ��̱� ������ ���ÿ� �Ҵ��ϴ°� ȿ������!
	RowAutoChange rowAutoChange(this->notepadForm);
	//2. �ڵ� �� �ٲ� �޴��� üũ�Ǿ����� Ȯ���Ѵ�.
	UINT state = this->notepadForm->GetMenu()->
		GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
	//3. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ���� ������
	if (state == MF_UNCHECKED)
	{
		//3.1 �ڵ� �� �ٲ� �޴��� üũ���ش�.
		this->notepadForm->GetMenu()->
			CheckMenuItem(IDM_ROW_AUTOCHANGE, MF_CHECKED | MF_BYCOMMAND);
		//3.2 �ڵ������� �����Ѵ�.
		rowAutoChange.Do();
	}
	//3. �ڵ� �� �ٲ� �޴��� üũ�Ǿ� ������
	else
	{
		//3.1 �ڵ� �� �ٲ� �޴��� �ִ� üũ�� ���ش�.	
		this->notepadForm->GetMenu()->
			CheckMenuItem(IDM_ROW_AUTOCHANGE, MF_UNCHECKED | MF_BYCOMMAND);
		//3.2 �ڵ������� ����Ѵ�.
		rowAutoChange.Undo();
	}
	//4. ĳ���� ���� ó������ ������.
	this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->
		GetAt(this->notepadForm->note->GetCurrent());
	this->notepadForm->current->First();
	//5. ĳ���� ��ġ�� ����Ǿ����� �˸���.
	this->notepadForm->Notify();
	//6. ��������� �����Ѵ�.
	this->notepadForm->Invalidate(TRUE);
}


//�Ҹ���
RowAutoChangeCommand::~RowAutoChangeCommand()
{

}