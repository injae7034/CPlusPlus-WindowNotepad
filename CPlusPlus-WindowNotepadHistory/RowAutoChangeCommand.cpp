#include "RowAutoChangeCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "DummyRow.h"
#include "RowAutoChange.h"
#include "SelectingTexts.h"
#include "GlyphFinder.h"

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
		//5.1 ���� ȭ���� �ʺ� ���Ѵ�.
		CRect rect;
		this->notepadForm->GetClientRect(&rect);
		Long letterWidth = 0;
		string content;
		//5.2 GlyphFinder�� �����Ѵ�.
		GlyphFinder glyphFinder(this->notepadForm->note);
		//5.3 �̵��ϱ� ���� �ٰ� ������ ��ġ�� ���Ѵ�.
		Long previousRowIndex = this->notepadForm->note->GetCurrent();
		Long previousLetterIndex = this->notepadForm->current->GetCurrent();
		//5.4 �ǹ��ڸ� �Ʒ��� ã��� ã�´�.
		this->notepadForm->note->First();
		this->notepadForm->current = this->notepadForm->note->
			GetAt(this->notepadForm->note->GetCurrent());
		this->notepadForm->current->First();
		Long findingStartRowIndex = 0;
		Long findingStartLetterIndex = 0;
		Long findingEndRowIndex = 0;
		Long findingEndLetterIndex = 0;
		glyphFinder.FindDown("\t", &findingStartRowIndex, &findingStartLetterIndex,
			&findingEndRowIndex, &findingEndLetterIndex);
		//5.5 ã�� �� ������
		if (findingStartRowIndex != findingEndRowIndex ||
			findingStartLetterIndex != findingEndLetterIndex)
		{
			//5.5.1 �ǹ����� �ʺ� ���Ѵ�.
			letterWidth = this->notepadForm->textExtent->GetTextWidth("\t");
		}
		//5.6 ã�� �� ������
		else
		{
			//5.6.1 �ѱ��� �ʺ� ���Ѵ�.
			letterWidth = this->notepadForm->textExtent->GetTextWidth("��");
		}
		//5.7 ������ �ʺ� ȭ���� ũ�⺸�� ������
		if (letterWidth < rect.Width())
		{
			//5.7.1 �ڵ� �� �ٲ� �޴��� üũ���ش�.
			this->notepadForm->GetMenu()->
				CheckMenuItem(IDM_ROW_AUTOCHANGE, MF_CHECKED | MF_BYCOMMAND);
			//5.7.2 �ڵ������� �����Ѵ�.
			rowAutoChange.DoAllRows();
			this->notepadForm->isRowAutoChanging = true;
			//5.7.3 ĳ���� ���� ó������ ������.
			this->notepadForm->note->First();
			this->notepadForm->current = this->notepadForm->note->
				GetAt(this->notepadForm->note->GetCurrent());
			this->notepadForm->current->First();
		}
		//5.8 ������ �ʺ� ȭ���� ũ�⺸�� ũ�ų� ������
		else
		{
			//5.8.1 ������ �ٽ� �����϶�� �޼����ڽ��� ����Ѵ�.
			int messageBoxButton = this->notepadForm->MessageBox(
				"������ ���� ���̰� ���� ȭ���� ���� ���̺��� ��\n"
				"�ڵ� �� �ٲ��� �� �� �����ϴ�.\n"
				"ȭ���� ���̸� �ø��ų� �۲ÿ��� ���� ũ�⸦ ��������ּ���.",
				"�ڵ� �� �ٲ�", MB_OK);
			//5.8.2 ���� ��ġ�� �̵���Ų��.
			previousRowIndex = this->notepadForm->note->Move(previousRowIndex);
			this->notepadForm->current = this->notepadForm->note->GetAt(previousRowIndex);
			this->notepadForm->current->Move(previousLetterIndex);
		}
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
		//6.3 ĳ���� ���� ó������ ������.
		this->notepadForm->note->First();
		this->notepadForm->current = this->notepadForm->note->
			GetAt(this->notepadForm->note->GetCurrent());
		this->notepadForm->current->First();
	}
}


//�Ҹ���
RowAutoChangeCommand::~RowAutoChangeCommand()
{

}