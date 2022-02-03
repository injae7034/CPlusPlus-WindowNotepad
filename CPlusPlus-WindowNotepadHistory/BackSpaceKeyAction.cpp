#include "BackSpaceKeyAction.h"
#include "Glyph.h"

//디폴트생성자
BackSpaceKeyAction::BackSpaceKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void BackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 선택된 범위를 구한다.
	Long startingRowPos = 0;
	Long startingLetterPos = 0;
	Long endingRowPos = 0;
	Long endingLetterPos = 0;
	this->notepadForm->note->CalculateSelectedRange(&startingRowPos,
		&startingLetterPos, &endingRowPos, &endingLetterPos);
	Long selectedRange = startingRowPos + startingLetterPos + endingRowPos + endingLetterPos;
	//2. 현재 줄의 위치를 구한다.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	//3. 현재 글자의 위치를 구한다.
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//4. 선택된 texts가 하나도 없으면
	if (selectedRange == 0)
	{
		//제일 첫 줄이면 줄을 지울 수 없고, 제일 첫 줄에서 글자 위치가 제일 처음에 있으면
		//아무것도 안일어남. 현재 줄의 위치가 제일 처음이 아니고, 현재 글자 위치가 제일 처음이면
		//현재 줄을 이전 줄에 Join시킨다.
		//4.1 현재 줄의 위치가 0보다 크고, 현재 글자 위치가 0이면
		Glyph* currentRow = 0;
		Glyph* previousRow = 0;
		if (currentRowPos > 0 && currentLetterPos == 0)
		{
			//4.1.1 현재 줄을 구한다.
			currentRow = this->notepadForm->note->GetAt(currentRowPos);
			//4.1.2 현재 줄의 이전 줄을 구한다.
			previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
			//2.3.3 현재 줄의 이전 줄의 마지막 글자 위치를 구한다.
			Long letterPos = previousRow->GetLength();
			//4.1.3 현재 줄을 이전 줄에 합친다.
			currentRow->Join(previousRow);
			//4.1.4 Note에서 현재 줄의 주소를 지운다.
			this->notepadForm->note->Remove(currentRowPos);
			//4.1.5 현재 줄이 지워졌기 때문에 현재 줄을 변경한다.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			//4.1.6 현재 줄의 글자 위치가 지금은 마지막이기 때문에 변경해준다.
			//이전 줄의 마지막 현재 줄의 처음 사이에 위치하도록 조정한다.
			Long index = this->notepadForm->current->First();
			while (index < letterPos)
			{
				this->notepadForm->current->Next();
				index++;
				//이렇게하면 index는 절대 overflow가 되지 않기 때문에 반복문을 벗어날 수 없게되고,
				//그럼 결국에 무한반복이 된다.!!
				//index = this->current->Next();
			}
			//4.1.7 메모장 제목에 *를 추가한다.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - 메모장";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//4.1.8 메모장에 변경사항이 있음을 저장한다.
			this->notepadForm->IsDirty = true;
		}
		// 현재 글자 위치가 처음이 아닐 때(현재 줄이 처음이든 아니든 상관없음) 현재 글자를 지운다.
		//4.2 현재 글자 위치가 처음이 아니면
		else if (currentLetterPos > 0)
		{
			//4.2.1 현재 글자를 지운다.
			this->notepadForm->current->Remove(currentLetterPos - 1);
			//4.2.2 메모장 제목에 *를 추가한다.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - 메모장";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//4.2.3 메모장에 변경사항이 있음을 저장한다.
			this->notepadForm->IsDirty = true;
			//4.2.4 자동 줄 바꿈 메뉴가 체크되었는지 확인한다.
			UINT state = this->notepadForm->GetMenu()->
				GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
			//4.2.5 자동 줄 바꿈 메뉴가 체크되어 있으면
			if (state == MF_CHECKED)
			{
				//4.2.5.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
				//OnSize에서 부분자동개행을 하도록 한다. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
		}
	}
	//3. 선택된 texts가 있으면
	else
	{
		//3.1 메모장에서 선택된 texts를 전부 지운다.
		Glyph* startingSelectedRow = 0;//선택이 시작되는 줄의 주소를 저장할 공간
		Glyph* letter = 0;//글자 주소를 저장할 공간
		Glyph* row = 0;//줄 주소를 저장할 공간
		Long startingLetterIndex = 0;//줄에서 반복을 시작하는 글자 위치
		Long endingLetterIndex = 0;//줄에서 반복이 끝나는 글자 위치
		Long lastLetterIndex = 0;//줄에서 마지막 글자의 위치
		//3.1.1 선택이 시작되는 줄이랑 선택이 끝나는 줄이랑 같으면
		if (startingRowPos == endingRowPos)
		{	
			//3.1.1.1 선택이 시작되는 줄을 구한다.
			row = this->notepadForm->note->GetAt(startingRowPos);
			//3.1.1.2 선택이 시작되는 줄의 마지막 글자위치를 구한다.
			lastLetterIndex = row->GetLength() - 1;
			//3.1.1.3 선택이 시작되는 글자부터 시작해서 선택이 끝나는 글자까지 글자를 지운다.
			startingLetterIndex = startingLetterPos;
			endingLetterIndex = endingLetterPos;
			while (startingLetterIndex <= endingLetterIndex)
			{
				//3.1.1.3.1 글자를 지운다.
				row->Remove(startingLetterIndex);
				//3.1.1.3.2 끝나는 글자 위치를 감소시킨다.
				endingLetterIndex--;
			}
			//3.1.1.4 선택이 시작되는 줄의 글자가 다지워졌으면
			//if (row->GetLength() == 0)
			//<주의>줄에 있는 글자를 다지우지 않았더라도 마지막에 있는 글자를 지웠으면
			//무조건 다음줄을 시작하는 줄에 Join시켜야함!!!
			//3.1.1.4 선택이 시작되는 줄에서 선택이 끝나는 글자 위치가 줄의 마지막 글자이고,
			//지우기전에 글자의 현재 위치가 0이면
			//(선택이 시작되는 줄에서 마지막 위치에 있는 글자를 지웠으면)
			if (endingLetterPos == lastLetterIndex && currentLetterPos == 0
				&& currentRowPos != startingRowPos)
			{
				//3.1.1.5 선택이 시작되는 줄의 다음 줄을 구한다.
				row = this->notepadForm->note->GetAt(startingRowPos + 1);
				//3.1.1.6 선택이 시작되는 줄 다음에 줄이 있으면
				if (row != 0)
				{
					//3.1.1.6.1 선택이 시작되는 줄을 구한다.
					startingSelectedRow = this->notepadForm->note->GetAt(startingRowPos);
					//3.1.1.6.2 선택이 시작되는 줄의 다음 줄을 선택이 시작되는 줄에 Join시킨다.
					row->Join(startingSelectedRow);
					//3.1.1.6.3 선택이 시작되는 줄의 다음 줄이이 선택이 시작되는 줄에 
					//Join되었기 때문에 메모장에서 해당 줄을 지운다.
					this->notepadForm->note->Remove(startingRowPos + 1);
				}
			}
		}
		//3.1.2 선택이 시작되는 줄이랑 선택이 끝나는 줄이랑 다르면
		//(선택이 시작되는 줄의 위치보다 선택이 끝나는 줄의 위치가 더 크면
		else
		{
			//3.1.2.1 선택이 시작되는 줄에서 선택이 시작되는 글자위치가
			//선택이 시작되는 줄의 글자개수보다 작은동안 반복한다.
			//(선택이 시작되는 줄에서 글자들을 지운다)
			//<주의>선택이 시작되는 줄에서 글자들을 다 지울수도 있고 다 안지워질수도 있다
			//하지만 글자를 다지웠다고 해서 선택이 시작되는 줄을 메모장에서 지우면 안됨. 
			//선택이 끝나는 줄의 선택이 끝나는 글자까지 다 지운다음에 선택이 시작되는 줄로 
			//Join을 시켜야하기 때문에 선택이 시작되는 줄은 내용이 없더라도 메모장에서 지우면 안된다!!!
			startingLetterIndex = startingLetterPos;
			row = this->notepadForm->note->GetAt(startingRowPos);
			while (startingLetterIndex < row->GetLength())
			{
				//3.1.2.1.1 글자를 지운다.
				row->Remove(startingLetterIndex);
			}
			//선택이 시작되는 줄의 글자가 남아있으면 당연히 줄이 그대로 있기 때문에 다음줄로 이동해야하고,
			//만약에 선택이 시작되는 줄의 글자가 다지웠졌더라도 메모장에서 줄을 지우지는 않았기 때문에
			//다음줄로 이동해야 한다.
			//3.1.2.2 줄의 위치를 증가시킨다.(시작하는 줄의 글자를 다지웠기때문에 다음줄로 이동)
			Long rowIndex = startingRowPos + 1;
			Long endingRowIndex = endingRowPos;
			//3.1.2.3 줄의 위치가 선택이 끝나는 줄보다 작은동안 반복한다.
			while (rowIndex < endingRowIndex)
			{
				//3.1.2.3.1 줄을 구한다.
				row = this->notepadForm->note->GetAt(rowIndex);
				//3.1.2.3.2 글자 위치를 0으로 초기화 시킨다.
				startingLetterIndex = 0;
				//3.1.2.3.3 글자 위치가 줄의 개수보다 작은동안 반복한다.
				while (startingLetterIndex < row->GetLength())
				{
					//3.1.2.3.3.1 글자를 지운다.
					row->Remove(startingLetterIndex);
				}
				//3.1.2.3.4 줄에 담긴 글자를 다지웠기때문에 메모장에서 해당 줄을 지운다.
				this->notepadForm->note->Remove(rowIndex);
				//3.1.2.3.5 선택이 끝나는 줄의 앞에 있는 줄이 메모장에서 지워졌기 때문에
				//뒤에 줄들을 한 줄씩 앞으로 당겨진다. 따라서 선택이 끝나는 줄의 위치도
				//한줄 앞으로 이동하기 때문에 -1을 감소시킨다.
				endingRowIndex--;
			}
			//3.1.2.4 시작 글자 위치를 0으로 초기화시킨다.
			startingLetterIndex = 0;
			//3.1.2.5 선택이 끝나는 줄에서 선택이 끝나는 줄의 위치를 대입한다.
			endingLetterIndex = endingLetterPos;
			//3.1.2.6 선택이 끝나는 줄을 구한다.
			row = this->notepadForm->note->GetAt(endingRowIndex);
			//3.1.2.7 선택이 끝나는 줄의 마지막 글자 위치를 구한다.
			lastLetterIndex = row->GetLength() - 1;
			//3.1.2.8 글자 위치가 선택이 끝나는 줄에서 선택이 끝나는 글자 위치보다
			//작거나 같은 동안 반복한다.
			while (startingLetterIndex <= endingLetterIndex)
			{
				//3.1.2.8.1 글자를 지운다.
				row->Remove(startingLetterIndex);
				//3.1.2.8.2 선택이 끝나는 글자 이전의 글자를 지웠기 떄문에
				//선택이 끝나는 글자 위치를 앞으로 당긴다(-1 감소시킨다.)
				endingLetterIndex--;
			}

			//3.1.2.9 선택이 끝나는 줄에서 선택이 끝나는 글자 위치가 줄의 마지막 글자이면
			//(선택이 끝나는 줄에 담긴 글자를 다지웠으면)
			//startingLetterIndex가 0부터 시작하기 때문에(줄의 첫글자부터 시작해서 지우기 때문애) 
			//endingLetterPos == lastLetterIndex이면 줄의 글자를 다지운것이다.
			//if (endingLetterPos == lastLetterIndex)
			//3.1.2.9 줄의 글자개수가 0이면(줄에 있는 글자가 다 지워졌으면)
			//줄의 마지막 글자위치를 이용하는거보다 줄의 글자개수를 이용하는게 더논리적이고 직관적임!!
			//if (row->GetLength() == 0)
			//<주의>줄에 있는 글자를 다지우지 않았더라도 마지막에 있는 글자를 지웠으면
			//무조건 다음줄을 시작하는 줄에 Join시켜야함!!!
			//3.1.2.9 선택이 시작되는 줄에서 선택이 끝나는 글자 위치가 줄의 마지막 글자이면
			//(선택이 시작되는 줄에서 마지막 위치에 있는 글자를 지웠으면)

			//3.1.2.9 선택이 끝나는 줄에서 선택이 끝나는 글자 위치가 줄의 마지막 글자이고,
			//지우기전에 글자의 현재 위치가 0이면
			//(선택이 시작되는 줄에서 마지막 위치에 있는 글자를 지웠으면)
			if (endingLetterPos == lastLetterIndex && currentLetterPos == 0
				&& currentRowPos != startingRowPos)
			{
				//3.1.2.9.1 줄에 담긴 글자를 다지웠기 때문에 메모장에서
				//선택이 끝나는 줄을 지운다.
				this->notepadForm->note->Remove(endingRowIndex);
			}
			//3.1.2.10 선택이 시작되었던 줄을 다시 구한다.
			startingSelectedRow = this->notepadForm->note->GetAt(startingRowPos);
			//3.1.2.11 선택이 끝나는 줄을 구한다. 선택이 끝나는 줄의 글자가 없어서 선택이 끝나는 줄이
			//지워졌다면 선택이 끝나는 줄의 다음 줄이 한 줄 앞으로 당겨지기 때문에 선택이 끝나는
			//줄의 위치 endingRowIndex를 넣으면 선택이 끝나는 줄의 다음 위치를 구할 수 있다!
			//*선택이 끝나는 줄 또는 선택이 끝나는 줄의 다음 줄을 구한다.
			row = this->notepadForm->note->GetAt(endingRowIndex);
			//3.1.2.12 선택이 끝나는 줄(또는 선택이 끝나는 줄의 다음줄)을 선택이 시작되었던 줄에
			//Join시킨다.
			row->Join(startingSelectedRow);
			//3.1.2.13 선택이 끝나는 줄(또는 선택이 끝나는 줄의 다음줄)이 선택이 시작되었던 줄에 
			//Join되었기 때문에 메모장에서 해당 줄을 지운다.
			this->notepadForm->note->Remove(endingRowIndex);
		}
		//3.1.3 현재 줄의 위치를 다시 조정한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(startingRowPos);
		//3.1.4 현재 글자로 이동한다.
		this->notepadForm->current->Move(startingLetterPos);
		//3.1.5 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//3.1.6 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->IsDirty = true;
		//3.1.7 자동 줄 바꿈 메뉴가 체크되었는지 확인한다.
		UINT state = this->notepadForm->GetMenu()->
			GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
		//3.1.8 자동 줄 바꿈 메뉴가 체크되어 있으면
		if (state == MF_CHECKED)
		{
			//3.1.8.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
}


//소멸자
BackSpaceKeyAction::~BackSpaceKeyAction()
{

}