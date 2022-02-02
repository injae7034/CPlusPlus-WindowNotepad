#include "EndKeyAction.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "TextExtent.h"

//����Ʈ������
EndKeyAction::EndKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void EndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. ���� ȭ���� ���� ���̸� ���Ѵ�.
	CRect rect;
	this->notepadForm->GetClientRect(&rect);
	Long pageWidth = rect.Width();
	//2. ���� ���� �� ���������� ĳ���� ���� ��ġ�� �̵���Ų��.
	this->notepadForm->current->Last();
	//3. ���� ���� ĳ���� ���� ��ġ�� ���Ѵ�.
	Long caretXPos = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
		current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
	//4. ��ũ���� �̵��ؾ��� ������ ���Ѵ�.
	Long currentScrollPos = caretXPos - pageWidth;
	//5. ĳ���� �̵��� ������ �ʺ�ŭ ����ũ���� �̵������ش�.
	this->notepadForm->scrollController->scroll[0]->Move(currentScrollPos);
	//6. ����ũ�ѹ��� ����ũ���� �̵���Ų��.
	this->notepadForm->SetScrollPos(SB_HORZ,
		this->notepadForm->scrollController->scroll[0]->GetCurrentPos());
}

//�Ҹ���
EndKeyAction::~EndKeyAction()
{

}