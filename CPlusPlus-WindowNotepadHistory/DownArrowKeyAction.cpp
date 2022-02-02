#include "DownArrowKeyAction.h"
#include "Glyph.h"
#include "TextExtent.h"

//디폴트생성자
DownArrowKeyAction::DownArrowKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void DownArrowKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 다음으로 이동하기 전에 캐럿의 현재 세로 위치를 저장한다.
	Long previousRowIndex = this->notepadForm->note->GetCurrent();
	//2. 다음으로 이동하기 전에 캐럿의 현재 가로 위치를 저장한다.
	Long previousCaretIndex = this->notepadForm->current->GetCurrent();
	//3. 캐럿의 현재 세로 위치를 다음 줄로 이동시킨다.
	Long currentRowIndex = this->notepadForm->note->Next();
	//4. 이동하기 전 캐럿의 세로 위치와 이동한 후 캐럿의 세로 위치가 서로 다르면(실질적으로 이동을 했으면)
	if (previousRowIndex != currentRowIndex)
	{
		//4.1 현재 줄을 이동한 후 캐럿의 세로 위치가 있는 줄로 변경한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		//4.2 이동하기 전 캐럿의 가로 위치가 0이 아니고 이동한 후 현재줄의 글자개수가 0이 아니면
		if (previousCaretIndex != 0 && this->notepadForm->current->GetLength() != 0)
		{
			//4.2.1 이동하기 전 줄의 텍스트 폭을 구한다.
			Long previousRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->note->GetAt(previousRowIndex)
					->GetPartOfContent(previousCaretIndex));
			//4.2.2 캐럿의 현재 가로 위치를 처음으로 이동시킨다.
			this->notepadForm->current->First();//캐럿의 가로 위치를 맨 처음(0)으로 보냄
			//4.2.3 첫번째 글자를 읽기 위해 캐럿을 1 증가시킨다.
			Long i = this->notepadForm->current->Next();
			//4.2.4 현재 줄의 택스트의 폭을 구한다.
			Long currentRowTextWidth = this->notepadForm->textExtent->
				GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			//4.2.5 i(캐럿의 현재 가로위치)가 length(현재줄의 글자개수)보다 작고
			//현재 줄의 텍스트 크기가 이동하기 전 줄의 텍스트 크기보다 작은동안 반복한다.
			while (i < this->notepadForm->current->GetLength()
				&& currentRowTextWidth < previousRowTextWidth)
			{
				//4.2.5.1 i(캐럿의 현재 가로위치)를 증가시킨다
				//(캐럿의 가로 위치를 다음 칸으로 이동시킨다.).
				i = this->notepadForm->current->Next();
				//4.2.5.2 현재 줄의 캐럿의 가로 위치까지의 택스트 폭을 구한다.
				currentRowTextWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetPartOfContent(i).c_str());
			}
			//4.2.6 현재 줄의 텍스트 폭에서 이전 줄의 텍스트 폭을 뺀다.
			Long difference = currentRowTextWidth - previousRowTextWidth;
			//4.2.7 차이가 0보다 크면(현재줄의 텍스트를 다 읽지 못한 경우)
			if (difference > 0)
			{
				//4.2.7.1 캐럿의 현재 가로위치 이전의 글자 폭을 구한다.
				Long letterWidth = this->notepadForm->textExtent->
					GetTextWidth(this->notepadForm->current->GetAt(i - 1)->GetContent());
				Long halfLetterSize = letterWidth / 2;
				//4.2.7.2 차이가 읽은 글자크기의 절반보다 같거나 크면
				if (difference >= halfLetterSize)
				{
					//4.2.7.2.1 캐럿의 현재 가로 위치를 이전으로 이동한다.
					this->notepadForm->current->Previous();
				}
			}
		}
		//4.3 캐럿의 이전 가로 위치가 0 또는 캐럿의 현재 줄의 글자개수가 0개이면
		else
		{
			//4.3.1 현재 캐럿의 가로 위치를 0으로 이동시킨다.
			this->notepadForm->current->First();
		}
	}
}

//소멸자
DownArrowKeyAction::~DownArrowKeyAction()
{

}