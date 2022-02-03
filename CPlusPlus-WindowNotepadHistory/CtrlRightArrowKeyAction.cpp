#include "CtrlRightArrowKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"
#include "SelectText.h"

//����Ʈ������
CtrlRightArrowKeyAction::CtrlRightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlRightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �̵��ϱ� ���� ���� ��ġ�� ���Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. �̵��ϱ� ���� ������ ��ġ�� ���Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//3. ��Ʈ���� �ܾ������ ���������� �̵��Ѵ�.
	Long currentLetterIndex = this->notepadForm->note->NextWord();
	//4. �̵��� �Ŀ� ���� ��ġ�� ���Ѵ�.
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//5. ���� ���� �̵��� ���� �ٷ� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//6. �ٿ��� ĳ���� �ܾ������ �̵��Ѵ�.
	this->notepadForm->current->Move(currentLetterIndex);
	//5. ȭ�鿡�� ���ÿ��θ� Ȯ���ϱ� ���� SelectText�� �����Ѵ�.
	SelectText selectText(this->notepadForm);
	//6. ShiftŰ�� ���������� �ȴ��������� ���¸� üũ�ؼ� �����Ѵ�.
	Long ctrlPressedCheck = GetKeyState(VK_SHIFT);
	//7. ShiftŰ�� ������������
	if (ctrlPressedCheck & 0x8000)
	{
		//7.1 �ؽ�Ʈ�� �����Ѵ�.
		selectText.Do(previousRowIndex, previousLetterIndex, currentRowIndex,
			currentLetterIndex);
	}
	//8. ShiftŰ�� �ȴ�����������
	else
	{
		//8.1 �ؽ�Ʈ�� ���������Ѵ�.
		selectText.Undo();
	}
#if 0
	if (currentLetterIndex > 0)
	{
		//6.1 ShiftŰ�� ���������� �ȴ��������� ���¸� üũ�ؼ� �����Ѵ�.
		Long ctrlPressedCheck = GetKeyState(VK_SHIFT);
		
		//6.2 ShiftŰ�� ������������
		if (ctrlPressedCheck & 0x8000)
		{
			
		}
	}
#endif
}

//�Ҹ���
CtrlRightArrowKeyAction::~CtrlRightArrowKeyAction()
{

}