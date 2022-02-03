#include "RightArrowKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"
#include "SelectText.h"

//����Ʈ������
RightArrowKeyAction::RightArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void RightArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �������� �̵��ϱ� ���� ������ ��ġ�� �����Ѵ�.
	Long previousLetterIndex = this->notepadForm->current->GetCurrent();
	//2. �������� �̵��ϱ� ���� ���� ��ġ�� �����Ѵ�.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//3. �������� �̵��ϰ� ���� ������ ��ġ�� ��ȯ�޴´�.
	Long currentLetterIndex = this->notepadForm->current->Next();
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	//4. ĳ���� �̵��� ��������(ĳ���� ���� �������� ��ġ�ϰ� �־��� ������)
	if (previousLetterIndex == currentLetterIndex)
	{
		//4.1 ���� �ٷ� �̵���Ų��.
		currentRowIndex = this->notepadForm->note->Next();
		//4.2 ���� �̵��� �־�����(�̵��ϱ� ���� ���� ��Ʈ�� ������ ���� �ƴϸ�)
		if (previousRowIndex != currentRowIndex)
		{
			//4.2.1 ���� ���� �������� �̵��� �ٷ� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
			//4.2.2 ĳ���� ���� ��ġ�� ó������ ������.(���� ���� ���� �ٷ� �Ű����� ������
			//ĳ���� ���� ���� ������ ��ġ���� ���� ���� ó�� ��ġ�� �̵��Ѵ�.)
			currentLetterIndex = this->notepadForm->current->First();
		}
	}
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
}

//�Ҹ���
RightArrowKeyAction::~RightArrowKeyAction()
{

}

