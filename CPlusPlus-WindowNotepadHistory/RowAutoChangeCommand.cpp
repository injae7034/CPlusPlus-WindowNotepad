#include "RowAutoChangeCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "TextExtent.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "DummyRow.h"

//디폴트생성자
RowAutoChangeCommand::RowAutoChangeCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void RowAutoChangeCommand::Execute()
{
	Long letterIndex = 0;
	Long rowTextWidth = 0;
	Glyph* glyph = 0;
	Long rowIndex = 0;
	Glyph* row = 0;
	Glyph* previousRow = 0;
	//1. 자동 줄 바꿈 메뉴가 체크되었는지 확인한다.
	UINT state = this->notepadForm->GetMenu()->
		GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
	//2. 자동 줄 바꿈 메뉴가 체크되어 있지 않으면
	if (state == MF_UNCHECKED)
	{
		//2.1 자동 줄 바꿈 메뉴를 체크해준다.
		this->notepadForm->GetMenu()->
			CheckMenuItem(IDM_ROW_AUTOCHANGE, MF_CHECKED | MF_BYCOMMAND);
		//2.2 Note의 총 줄의 개수보다 작은동안 반복한다.
		while (rowIndex < this->notepadForm->note->GetLength())
		{
			//2.2.1 메모장에서 rowIndex번째 줄을 구한다.
			row = this->notepadForm->note->GetAt(rowIndex);
			//2.2.2 letterIndex를 원위치시킨다.
			letterIndex = 0;
			//2.2.3 letterIndex를 증가시킨다.
			letterIndex++;
			//2.2.4 rowIndex번째 줄에서 letterIndex까지 텍스트의 가로길이를 측정한다.
			rowTextWidth = this->notepadForm->textExtent->GetTextWidth
				(row->GetPartOfContent(letterIndex).c_str());
			//2.2.5 letterIndex가 rowIndex번째 줄의 총글자 개수보다 작은동안 
			//그리고 rowIndex번째 줄의 가로길이가 현재화면의 가로길이보다 작은동안 반복한다.
			while (letterIndex < row->GetLength() &&
				rowTextWidth <= this->notepadForm->scrollController->
				scroll[0]->GetPageSize())
			{
				//2.2.5.1 letterIndex를 증가시킨다.
				letterIndex++;
				//2.2.5.2 증가된 letterIndex까지의 가로 길이를 측정한다.
				rowTextWidth = this->notepadForm->textExtent->GetTextWidth
				(row->GetPartOfContent(letterIndex));
			}
			//2.2.6 rowIndex번째 줄의 가로 길이가 현재 화면의 가로 길이보다 크면
			if (rowTextWidth > this->notepadForm->scrollController->
				scroll[0]->GetPageSize())
			{
				//2.2.6.1 letterIndex까지의 길이가 현재화면의 가로 길이(cx)보다 크기 때문에 
				//이 선택문에 들어왔다. 그래서 캐럿이 이전으로 한 칸 이동을 해서 길이를 재면
				//현재화면의 가로 길이(cx)보다 작다. 캐럿(letterIndex)은 다음 글자를 적을 위치를
				//반영하기 때문에 항상 현재 글자보다 한칸 앞서 있다
				//그래서 letterIndex-1에서 split을 해야 화면을 넘는 글자를 다음 줄로 보낼 수 있다.
				letterIndex--;
				//2.2.6.2 rowIndex번째 줄의 가로 길이가 현재화면의 가로 길이보다 커진 시점의
				//글자부터 rowIndex번째 줄에서 letterIndex 다음 위치에 있는 글자들을 나눈다.
				//(DummyRow생성)
				glyph = row->Split(letterIndex, true);
				//2.2.6.3 새로운 줄을 rowIndex번째 줄의 다음 위치에 끼워넣는다.
				rowIndex = this->notepadForm->note->Add(rowIndex + 1, glyph);
			}
			//2.2.7 letterIndex가 rowIndex번째 줄의 총글자 개수보다 크거나 같으면
			else if (letterIndex >= row->GetLength())
			{
				//2.2.7.1 다음 줄로 이동한다.
				rowIndex++;
			}
		}
	}
	//3. 자동 줄 바꿈 메뉴가 체크되어 있으면
	else
	{
		//3.1 자동 줄 바꿈 메뉴에 있는 체크를 없앤다.	
		this->notepadForm->GetMenu()->
			CheckMenuItem(IDM_ROW_AUTOCHANGE, MF_UNCHECKED | MF_BYCOMMAND);
		//3.2 Note의 총 줄의 개수보다 작은동안 반복한다.
		while (rowIndex < this->notepadForm->note->GetLength())
		{
			//3.2.1 메모장에서 rowIndex번째 줄을 구한다.
			row = this->notepadForm->note->GetAt(rowIndex);
			//3.2.2 DummyRow이면
			if (dynamic_cast<DummyRow*>(row))
			{
				//3.2.2.1 DummyRow 이전 줄(Row)을 구한다.
				previousRow = this->notepadForm->note->GetAt(rowIndex - 1);
				//3.2.2.2 DummyRow를 이전 줄(Row)에 합친다.
				row->Join(previousRow);
				//3.2.2.3 Note에서 DummyRow의 주소를 지운다.
				this->notepadForm->note->Remove(rowIndex);
			}
			//3.2.3 DummyRow가 아니면
			else
			{
				//3.2.3.1 다음 줄로 이동한다.
				rowIndex++;
			}
		}
	}
	//4. 캐럿을 제일 처음으로 보낸다.
	this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->
		GetAt(this->notepadForm->note->GetCurrent());
	this->notepadForm->current->First();
	//5. 수직스크롤을 제일 처음으로 보낸다.
	//this->notepadForm->scrollController->scroll[1]->First();
	//6. 수직스크롤바의 Thumb를 수직스크롤의 현재위치로 이동시킨다.
	//this->notepadForm->SetScrollPos(SB_VERT,
	//	this->notepadForm->scrollController->scroll[1]->GetCurrentPos());
	//7. 스크롤의 위치가 변경되었음을 알린다.
	this->notepadForm->Notify();
	//8. 변경사항을 갱신한다.
	this->notepadForm->Invalidate(TRUE);
}


//소멸자
RowAutoChangeCommand::~RowAutoChangeCommand()
{

}