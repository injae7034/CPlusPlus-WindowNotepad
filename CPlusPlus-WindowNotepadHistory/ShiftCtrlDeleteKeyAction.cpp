#include "ShiftCtrlDeleteKeyAction.h"
#include "Glyph.h"

//디폴트생성자
ShiftCtrlDeleteKeyAction::ShiftCtrlDeleteKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm)
{

}

//Execute
void ShiftCtrlDeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//1. 현재 줄의 위치를 구한다.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	//2. 현재 글자의 위치를 구한다.
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//3. 메모장에서 선택된 texts가 없으면
	if (this->notepadForm->isSelecting == false)
	{
		//3.1 현재 노트의 마지막 줄의 위치를 구한다.
		Long lastRowPos = this->notepadForm->note->GetLength() - 1;
		//3.2 현재 줄에서 마지막 글자의 위치를 구한다.
		Long lastLetterPos = this->notepadForm->current->GetLength();
		//제일 마지막 줄이면 줄을 지울 수 없고, 마지막 줄에서 글자 위치가 마지막에 있으면 아무것도 안일어남
		// 현재 줄의 위치가 마지막이 아니고, 현재 글자 위치가 마지막이면 다음 줄을 현재 줄로 편입시킨다.
		//3.3 현재 줄의 위치가 노트의 마지막 줄 위치보다 작고, 현재 글자 위치가 마지막이면
		Glyph* currentRow = 0;
		Glyph* nextRow = 0;
		if (currentRowPos < lastRowPos && currentLetterPos == lastLetterPos)
		{
			//3.3.1 현재 줄을 구한다.
			currentRow = this->notepadForm->note->GetAt(currentRowPos);
			//3.3.2 현재 줄의 다음 줄을 구한다.
			nextRow = this->notepadForm->note->GetAt(currentRowPos + 1);
			//3.3.3 다음 줄을 현재 줄에 합친다.
			nextRow->Join(currentRow);
			//3.3.4 Note에서 다음 줄의 주소를 지운다.
			this->notepadForm->note->Remove(currentRowPos + 1);
			//3.3.5 현재 줄의 글자 위치가 지금은 마지막이기 때문에 변경해준다.
			Long index = this->notepadForm->current->First();
			while (index < lastLetterPos)
			{
				this->notepadForm->current->Next();
				index++;
				//이렇게하면 index는 절대 overflow가 되지 않기 때문에 반복문을 벗어날 수 없게되고,
				//그럼 결국에 무한반복이 된다.!!
				//index = this->current->Next();
			}
			//3.3.6 메모장 제목에 *를 추가한다.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - 메모장";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.3.7 메모장에 변경사항이 있음을 저장한다.
			this->notepadForm->isDirty = true;
		}
		// 현재 글자 위치가 마지막이 아닐 때(현재 줄이 마지막이든 아니든 상관없음)
		//현재 글자의 다음 글자부터 줄의 마지막 글자까지 다 지운다.
		//3.4 현재 글자 위치가 마지막이 아니면
		else if (currentLetterPos < lastLetterPos)
		{
			//3.4.1 마지막 글자위치가 현재글자위치보다 큰동안 반복한다.
			while (lastLetterPos > currentLetterPos)
			{
				//3.4.1.1 글자를 지운다.
				this->notepadForm->current->Remove(lastLetterPos - 1);
				lastLetterPos--;
			}
			//3.4.2 메모장 제목에 *를 추가한다.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - 메모장";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.4.3 메모장에 변경사항이 있음을 저장한다.
			this->notepadForm->isDirty = true;
			//3.4.4 자동 줄 바꿈 메뉴가 체크되었는지 확인한다.
			UINT state = this->notepadForm->GetMenu()->
				GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
			//3.4.5 자동 줄 바꿈 메뉴가 체크되어 있으면
			if (state == MF_CHECKED)
			{
				//3.4.5.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
				//OnSize에서 부분자동개행을 하도록 한다. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
		}
	}
	//4. 메모장에서 선택된 texts가 있으면
	else
	{
		Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//선택이 시작되는 줄
		Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//선택이 시작되는 글자
		Long selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
		Long selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
		//시작은 무조건 오른쪽방향임
		Long startRowIndex = 0;//시작하는 줄의 위치
		Long startLetterIndex = 0;//시작하는 글자 위치
		Long endRowIndex = 0;//끝나는 줄의 위치
		Long endLetterIndex = 0;//끝나는 글자 위치
		Glyph* startRow = 0;//시작하는 줄의 위치
		//4.1 선택이 시작되는 줄과 선택이 끝나는 줄이 같으면
		//(한 줄 내에서만 선택이 되어 있으므로 줄을 지워지지 않고 글자들만 지워짐)
		if (selectedStartRowPos == selectedEndRowPos)
		{
			//4.1.1 선택이 오른쪽으로 진행되었으면
			if (selectedStartLetterPos < selectedEndLetterPos)
			{
				//4.1.1.1 시작하는 글자위치를 선택이 시작되는 글자위치로 정한다.
				startLetterIndex = selectedStartLetterPos;
				//4.1.1.2 끝나는 글자위치를 선택이 끝나는 글자위치로 정한다.
				endLetterIndex = selectedEndLetterPos;
				//4.1.1.3 시작하는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
				startRowIndex = selectedStartRowPos;

			}
			//4.1.2 선택이 왼쪽으로 진행되었으면
			else
			{
				//4.1.2.1 시작하는 글자위치를 선택이 끝나는 글자위치로 정한다.
				startLetterIndex = selectedEndLetterPos;
				//4.1.2.2 끝나는 글자위치를 선택이 시작하는 글자위치로 정한다.
				endLetterIndex = selectedStartLetterPos;
				//4.1.2.3 시작하는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
				startRowIndex = selectedEndRowPos;
			}
			//4.1.3 시작하는 줄을 구한다.
			startRow = this->notepadForm->note->GetAt(startRowIndex);
			//4.1.4 시작하는 글자위치부터 끝나는 글자까지 지운다.
			while (startLetterIndex < endLetterIndex)
			{
				//4.1.4.1 줄에서 글자를 지운다.
				startRow->Remove(startLetterIndex);
				//4.1.4.2 줄에서 글자가 지워지면 줄의 개수가 줄고 시작하는 글자의 다음 글자가
				//선택이 시작하는 글자의 위치로 앞당겨져 오게 되므로 선택이 끝나는 줄의 값을 감소시킨다. 
				endLetterIndex--;
			}
		}
		//4.2 선택이 시작되는 줄과 선택이 끝나는 줄이 서로 다르면
		//(선택이 여러 줄에 걸쳐서 되어 있기 때문에 글자가 다 지워진 줄은 지워져야함)
		else
		{
			//4.2.1 선택이 오른쪽으로 진행되었으면 
			if (selectedStartRowPos < selectedEndRowPos)
			{
				//4.2.1.1 시작하는 글자위치를 선택이 시작되는 글자위치로 정한다.
				startLetterIndex = selectedStartLetterPos;
				//4.2.1.2 끝나는 글자위치를 선택이 끝나는 글자위치로 정한다.
				endLetterIndex = selectedEndLetterPos;
				//4.2.1.3 시작하는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
				startRowIndex = selectedStartRowPos;
				//4.2.1.4 끝나는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
				endRowIndex = selectedEndRowPos;
			}
			//4.2.2 선택이 왼쪽으로 진행되었으면
			else
			{
				//4.2.2.1 시작하는 글자위치를 선택이 끝나는 글자위치로 정한다.
				startLetterIndex = selectedEndLetterPos;
				//4.2.2.2 끝나는 글자위치를 선택이 시작하는 글자위치로 정한다.
				endLetterIndex = selectedStartLetterPos;
				//4.2.2.3 시작하는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
				startRowIndex = selectedEndRowPos;
				//4.2.2.4 끝나는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
				endRowIndex = selectedStartRowPos;
			}
			Glyph* endRow = 0;//끝나는 줄의 위치
			Glyph* row = 0;//줄의 주소를 담을 공간
			Long letterIndex = 0;//글자 위치
			//4.2.3 시작하는 줄을 구한다.
			startRow = this->notepadForm->note->GetAt(startRowIndex);
			//4.2.4 시작하는 글자위치부터 시작하는 줄의 마지막 글자까지 지운다.
			while (startLetterIndex < startRow->GetLength())
			{
				//4.2.4.1 줄에서 글자를 지운다.
				startRow->Remove(startLetterIndex);
			}
			//4.2.5 시작하는 줄의 다음줄부터 끝나는 줄전까지 글자와 줄을 지운다.
			Long nextRowIndex = startRowIndex + 1;
			while (nextRowIndex < endRowIndex)
			{
				//4.2.5.1 줄을 구한다.
				row = this->notepadForm->note->GetAt(nextRowIndex);
				//4.2.5.2 글자위치를 원위치시킨다.
				letterIndex = 0;
				//4.2.5.3 줄에서 마지막 글자까지 반복한다.
				while (letterIndex < row->GetLength())
				{
					//4.2.5.3.1 줄의 글자를 지운다.
					row->Remove(letterIndex);
				}
				//4.2.5.4 줄의 글자를 다지웠기때문에 메모장에서 줄을 지운다.
				this->notepadForm->note->Remove(nextRowIndex);
				//4.2.5.5 줄을 지웠기 때문에 선택이 끝나는 줄의 위치가 한칸 앞당겨진다.
				endRowIndex--;
			}
			//4.2.6 끝나는 줄을 구한다.
			endRow = this->notepadForm->note->GetAt(endRowIndex);
			//4.2.7 끝나는 줄의 처음부터 끝나는 글자까지 글자를 지운다.
			letterIndex = 0;
			while (letterIndex < endLetterIndex)
			{
				//4.2.7.1 끝나는 줄의 글자를 지운다.
				endRow->Remove(letterIndex);
				//4.2.7.2 끝나는 줄의 첫글자를 지우면 다음 글자부터 앞으로 한칸씩
				//당겨지기 때문에 끝나는 글자위치를 -1 감소시킨다.
				endLetterIndex--;
			}
			//4.2.8 끝나는 줄을 시작하는 줄로 Join시킨다.
			endRow->Join(startRow);
			//4.2.9 끝나는 줄이 시작하는 줄로 Join되었기 때문에
			//끝나는 줄을 메모장에서 지운다.
			this->notepadForm->note->Remove(endRowIndex);
			//4.2.10 현재 줄의 위치를 시작하는 줄의 위치로 변경한다.
			this->notepadForm->current = this->notepadForm->note->
				GetAt(startRowIndex);
			//4.2.11 현재 글자의 위치를 시작하는 글자의 위치로 변경한다.
			this->notepadForm->current->Move(startLetterIndex);
		}
		//4.3 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//4.4 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->isDirty = true;
		//4.5 자동 줄 바꿈 메뉴가 체크되었는지 확인한다.
		UINT state = this->notepadForm->GetMenu()->
			GetMenuState(IDM_ROW_AUTOCHANGE, MF_BYCOMMAND);
		//4.6 자동 줄 바꿈 메뉴가 체크되어 있으면
		if (state == MF_CHECKED)
		{
			//4.6.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
		//4.7 메모장에서 선택된 texts를 다 지웠기 때문에 메모장에서 선택이 안된 상태로 바꾼다.
		this->notepadForm->isSelecting = false;
	}
}


//소멸자
ShiftCtrlDeleteKeyAction::~ShiftCtrlDeleteKeyAction()
{

}