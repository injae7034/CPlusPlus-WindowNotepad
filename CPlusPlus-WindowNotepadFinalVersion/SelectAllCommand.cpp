#include "SelectAllCommand.h"
#include "Glyph.h"
#include "SelectingTexts.h"
#include "NotepadForm.h"

//����Ʈ������
SelectAllCommand::SelectAllCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void SelectAllCommand::Execute()
{
	//1. �޸����� ��Ʈ���� ���� ������ ���Ѵ�.
	Long rowCountOfNote = this->notepadForm->note->GetLength();
	//2. �޸����� ��Ʈ�� ������ ���� ���� ������ ���Ѵ�.
	Long letterCountOfFirstRow = this->notepadForm->note->GetAt(rowCountOfNote - 1)->GetLength();
	//3. �޸����� ��Ʈ�� ���� ������ �ϳ��ִµ� �� ���� ���ڰ� �ϳ��� ���� ��찡 �ƴ϶��
	//���ڰ� �ϳ��� ���� �ٸ� 2���̻� �ִ� �����ʹ� ������ �����ϱ� ������ ����, �߶󳻱�, ������ �����ϴ�.
	if (rowCountOfNote != 1 || letterCountOfFirstRow != 0)
	{
		//3.1 ������ �ִ� ���� ������ �����Ѵ�.
		this->notepadForm->selectingTexts->Undo();
		//3.2 ������ �ٷ� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
		Long previousRowIndex = 0;
		//3.3 ���������� ���ڷ� �̵��ϱ� ���� ������ ��ġ�� �����Ѵ�.
		Long previousLetterIndex = 0;
		//3.4 ������ ����ǰ� �ִ� ������ ���¸� �ٲ۴�.
		this->notepadForm->isSelecting = true;
		//3.5 ������ ���۵Ǵ� ĳ���� x��ǥ�� �����Ѵ�.
		this->notepadForm->selectedStartXPos = previousLetterIndex;
		//3.6 ������ ���۵Ǵ� ĳ���� y��ǥ�� �����Ѵ�.
		this->notepadForm->selectedStartYPos = previousRowIndex;
		//3.7 �޸����� ���� ������ �ٷ� �̵��Ѵ�.
		Long currentRowIndex = this->notepadForm->note->Last();
		//3.8 �޸����� ���� ���� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		//3.9 ���� ���� ������ ���ڷ� �̵��Ѵ�.
		Long currentLetterIndex = this->notepadForm->current->Last();
		//3.10 ��ü ���ڸ� �����Ѵ�.
		this->notepadForm->selectingTexts->DoNext(previousRowIndex, previousLetterIndex,
			currentRowIndex, currentLetterIndex);
		//3.11 �޸��� texts ��ü�� ���õǾ��� ������ �����ϱ�� �߶󳻱�, ������ Ȱ��ȭ �����ش�.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	}
}

//�Ҹ���
SelectAllCommand::~SelectAllCommand()
{

}