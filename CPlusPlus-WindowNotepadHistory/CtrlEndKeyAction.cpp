#include "CtrlEndKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//����Ʈ������
CtrlEndKeyAction::CtrlEndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void CtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. �޸����� ���� ���������� ĳ���� ���� ��ġ�� �̵���Ų��.
	Long lastIndex = this->notepadForm->note->Last();
	//2. �޸����� ���� ���� �����Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(lastIndex);
	//3. ���� ���� �� ���������� ĳ���� ���� ��ġ�� �̵���Ų��.
	this->notepadForm->current->Last();
	//4.3.3 ���� �ٱ��� �� ���� ���̸� ���Ѵ�.(ĳ���� ���� ��ġ�� ���Ѵ�.)
	Long caretYPos = this->notepadForm->textExtent->GetHeight() * (lastIndex + 1);
	//4. ������ũ���� ���������� �̵������ش�.
	this->notepadForm->scrollController->scroll[1]->Move(caretYPos);
	//5. ����ũ�ѹ��� ����ũ���� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_VERT,
		this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	//6. ���� ȭ���� ���� ���̸� ���Ѵ�.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	Long pageWidth = rect.Width();
	//7. ���� ���� ĳ���� ���� ��ġ�� ���Ѵ�.
	Long caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
		current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
	//8. ����ũ���� �̵��ؾ��� ������ ���Ѵ�.
	Long currentScrollPos = caretXPos - pageWidth;
	//9. ĳ���� �̵��� ������ �ʺ�ŭ ����ũ���� �̵������ش�.
	this->notepadForm->scrollController->scroll[0]->Move(currentScrollPos + 2);
	//10. ����ũ�ѹ��� ����ũ���� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
}

//�Ҹ���
CtrlEndKeyAction::~CtrlEndKeyAction()
{

}