#include "CutCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"
#include "RowAutoChange.h"
#include "SelectingTexts.h"

//디폴트생성자
CutCommand::CutCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->glyph = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->selectedStartXPos = 0;
	this->selectedStartYPos = 0;
	this->isSelectedTextsRemoved = false;//처음에 생성될 때는 선택영역이 안지워졌으므로 false가 디폴트값
}

//Execute
void CutCommand::Execute()
{
	//선택이 진행 중이면(선택된 영역이 없으면 잘라내기를 하지 않음)
	if (this->notepadForm->isSelecting == true)
	{
		//1. RowAutoChange를 생성한다.
		RowAutoChange rowAutoChange(this->notepadForm);
		Long changedRowPos = 0;
		Long changedLetterPos = 0;
		Long originRowPos = this->rowIndex;
		Long originLetterPos = this->letterIndex;
		//2. 현재 줄의 위치와 글자 위치를 구한다.
		Long currentRowPos = this->notepadForm->note->GetCurrent();
		Long currentLetterPos = this->notepadForm->current->GetCurrent();
		//3. RemoveCommand가 다시 실행되면
		if (this->isRedone == true)
		{
			//3.1 현재 줄의 위치와 글자위치를 재조정해준다.
			currentRowPos = this->notepadForm->note->Move(this->rowIndex);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
			//3.2 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치가 저장되어 있음)
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//3.2.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
				rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
					&changedRowPos);
				//3.2.2 현재 줄의 위치와 글자 위치를 다시 조정한다.
				currentRowPos = this->notepadForm->note->Move(changedRowPos);
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
			}
		}
		//4. 다시 실행이면
		if (this->isRedone == true)
		{
			this->notepadForm->selectedStartYPos = this->selectedStartYPos;
			this->notepadForm->selectedStartXPos = this->selectedStartXPos;
		}
		//5. 선택이 시작되는 줄과 글자 위치, 선택이 끝나는 줄과 글자 위치를 저장한다.
		Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//선택이 시작되는 줄
		Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//선택이 시작되는 글자
		Long selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
		Long selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
		//6. 선택의 방향과 상관없이 오른쪽 방향(다음)으로 시작하는 줄과 글자, 끝나는 줄과 글자위치를 정한다.
		Long startRowIndex = 0;//시작하는 줄의 위치
		Long startLetterIndex = 0;//시작하는 글자 위치
		Long endRowIndex = 0;//끝나는 줄의 위치
		Long endLetterIndex = 0;//끝나는 글자 위치
		Long copyLetterIndex = 0;//클립보드에 복사되는 글자들의 위치를 저장할 장소
		Long i = 0;//반복제어변수
		Glyph* startRow = 0;//시작하는 줄의 위치
		Glyph* letter = 0;
		Glyph* newRow = 0;//힙에 새로 생성되는 줄의 주소를 담을 공간
		//7. 선택이 시작되는 줄과 선택이 끝나는 줄이 같으면(복사가 한 줄 내에서 이뤄졌으면)
		if (selectedStartRowPos == selectedEndRowPos)
		{
			//7.1.1 선택이 오른쪽으로 진행되었으면
			if (selectedStartLetterPos < selectedEndLetterPos)
			{
				//7.1.1.1 시작하는 글자위치를 선택이 시작되는 글자위치로 정한다.
				startLetterIndex = selectedStartLetterPos;
				//7.1.1.2 끝나는 글자위치를 선택이 끝나는 글자위치로 정한다.
				endLetterIndex = selectedEndLetterPos;
				//7.1.1.3 시작하는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
				startRowIndex = selectedStartRowPos;

			}
			//7.1.2 선택이 왼쪽으로 진행되었으면
			else
			{
				//7.1.2.1 시작하는 글자위치를 선택이 끝나는 글자위치로 정한다.
				startLetterIndex = selectedEndLetterPos;
				//7.1.2.2 끝나는 글자위치를 선택이 시작하는 글자위치로 정한다.
				endLetterIndex = selectedStartLetterPos;
				//7.1.2.3 시작하는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
				startRowIndex = selectedEndRowPos;
			}
			//7.1.3 시작하는 줄을 구한다.
			startRow = this->notepadForm->note->GetAt(startRowIndex);
			//7.1.4 처음 실행이면
			if (this->isRedone == false)
			{
				//7.1.4.1 Note를 생성한다.
				this->glyph = new Note();
				//7.1.4.2 복사할 startRow가 진짜 줄이면
				if (!dynamic_cast<DummyRow*>(startRow))
				{
					//7.1.4.2.1 Row를 생성한다.
					newRow = new Row();
				}
				//7.1.4.3 복사할 startRow가 가짜 줄이면
				else
				{
					//7.1.4.3.1 DummyRow를 생성한다.
					newRow = new DummyRow();
				}
				//7.1.4.4 새로 생성한 Row를 command의 Note에 추가한다.
				this->glyph->Add(newRow);
				//7.1.4.5 시작하는 글자위치부터 끝나는 글자까지 지운다.
				while (startLetterIndex < endLetterIndex)
				{
					//7.1.4.5.1 글자를 지우기 전에 글자를 구한다.
					letter = startRow->GetAt(startLetterIndex);
					//7.1.4.5.2 글자를 깊은 복사해서 새로 생성한 줄에 저장한다.
					newRow->Add(letter->Clone());
					//7.1.4.5.3 줄에서 글자를 지운다.
					startRow->Remove(startLetterIndex);
					//7.1.4.5.4 줄에서 글자가 지워지면 줄의 개수가 줄고 시작하는 글자의 다음 글자가
					//선택이 시작하는 글자의 위치로 앞당겨져 오게 되므로 선택이 끝나는 줄의 값을 감소시킨다. 
					endLetterIndex--;
				}
			}
			//7.1.5 다시 실행이면
			else
			{
				//7.1.5.1 시작하는 글자위치부터 끝나는 글자까지 지운다.
				while (startLetterIndex < endLetterIndex)
				{
					//7.1.5.1.1 줄에서 글자를 지운다.
					startRow->Remove(startLetterIndex);
					//7.1.5.1.2 줄에서 글자가 지워지면 줄의 개수가 줄고 시작하는 글자의 다음 글자가
					//선택이 시작하는 글자의 위치로 앞당겨져 오게 되므로 선택이 끝나는 줄의 값을 감소시킨다. 
					endLetterIndex--;
				}
			}
		}
		//8. 선택이 시작하는 줄과 선택이 끝나는 줄이 서로 다르면(선택이 여러줄에 걸쳐져서 되어 있으면)
		else
		{
			//8.1 선택이 오른쪽으로 진행되었으면 
			if (selectedStartRowPos < selectedEndRowPos)
			{
				//8.1.1 시작하는 글자위치를 선택이 시작되는 글자위치로 정한다.
				startLetterIndex = selectedStartLetterPos;
				//8.1.2 끝나는 글자위치를 선택이 끝나는 글자위치로 정한다.
				endLetterIndex = selectedEndLetterPos;
				//8.1.3 시작하는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
				startRowIndex = selectedStartRowPos;
				//8.1.4 끝나는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
				endRowIndex = selectedEndRowPos;
			}
			//8.2 선택이 왼쪽으로 진행되었으면
			else
			{
				//8.2.1 시작하는 글자위치를 선택이 끝나는 글자위치로 정한다.
				startLetterIndex = selectedEndLetterPos;
				//8.2.2 끝나는 글자위치를 선택이 시작하는 글자위치로 정한다.
				endLetterIndex = selectedStartLetterPos;
				//8.2.3 시작하는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
				startRowIndex = selectedEndRowPos;
				//8.2.4 끝나는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
				endRowIndex = selectedStartRowPos;
			}
			Glyph* endRow = 0;//끝나는 줄의 위치
			Glyph* row = 0;//줄의 주소를 담을 공간
			Long letterIndex = 0;//글자 위치
			Long nextRowIndex = 0;
			//8.3 시작하는 줄을 구한다.
			startRow = this->notepadForm->note->GetAt(startRowIndex);
			//8.4 처음 실행이면
			if (this->isRedone == false)
			{
				//8.4.1 Note를 생성한다.
				this->glyph = new Note();
				//8.4.2 복사할 startRow가 진짜 줄이면
				if (!dynamic_cast<DummyRow*>(startRow))
				{
					//8.4.2.1 Row를 생성한다.
					newRow = new Row();
				}
				//8.4.3 복사할 startRow가 가짜 줄이면
				else
				{
					//8.4.3.1 DummyRow를 생성한다.
					newRow = new DummyRow();
				}
				//8.4.4 새로 생성한 Row를 command의 Note에 추가한다.
				this->glyph->Add(newRow);
				//8.4.5 시작하는 글자위치부터 시작하는 줄의 마지막 글자까지 지운다.
				while (startLetterIndex < startRow->GetLength())
				{
					//8.4.5.1 글자를 지우기 전에 글자를 구한다.
					letter = startRow->GetAt(startLetterIndex);
					//8.4.5.2 글자를 깊은 복사해서 새로 생성한 줄에 저장한다.
					newRow->Add(letter->Clone());
					//8.4.5.3 줄에서 글자를 지운다.
					startRow->Remove(startLetterIndex);
				}
				//8.4.6 시작하는 줄의 다음줄부터 끝나는 줄전까지 글자와 줄을 지운다.
				nextRowIndex = startRowIndex + 1;
				while (nextRowIndex < endRowIndex)
				{
					//8.4.6.1 줄을 구한다.
					row = this->notepadForm->note->GetAt(nextRowIndex);
					//8.4.6.2 복사할 줄이 진짜 줄이면
					if (!dynamic_cast<DummyRow*>(row))
					{
						//8.4.6.2.1 Row를 생성한다.
						newRow = new Row();
					}
					//8.4.6.3 복사할 줄이 가짜 줄이면
					else
					{
						//8.4.6.3.1 DummyRow를 생성한다.
						newRow = new DummyRow();
					}
					//8.4.6.4 새로 생성한 Row를 command의 Note에 추가한다.
					this->glyph->Add(newRow);
					//8.4.6.5 글자위치를 원위치시킨다.
					letterIndex = 0;
					//8.4.6.6 줄에서 마지막 글자까지 반복한다.
					while (letterIndex < row->GetLength())
					{
						//8.4.6.6.1 글자를 지우기 전에 글자를 구한다.
						letter = row->GetAt(letterIndex);
						//8.4.6.6.2 글자를 깊은 복사해서 DummyRow에 저장한다.
						newRow->Add(letter->Clone());
						//8.4.6.6.3 줄의 글자를 지운다.
						row->Remove(letterIndex);
					}
					//8.4.6.7 줄의 글자를 다지웠기때문에 메모장에서 줄을 지운다.
					this->notepadForm->note->Remove(nextRowIndex);
					//8.4.6.8 줄을 지웠기 때문에 선택이 끝나는 줄의 위치가 한칸 앞당겨진다.
					endRowIndex--;
				}
				//8.4.7 끝나는 줄을 구한다.
				endRow = this->notepadForm->note->GetAt(endRowIndex);
				//8.4.8 복사할 줄이 진짜 줄이면
				if (!dynamic_cast<DummyRow*>(endRow))
				{
					//8.4.8.1 Row를 생성한다.
					newRow = new Row();
				}
				//8.4.9 복사할 줄이 가짜 줄이면
				else
				{
					//8.4.9.1 DummyRow를 생성한다.
					newRow = new DummyRow();
				}
				//8.4.10 새로 생성한 Row를 command의 Note에 추가한다.
				this->glyph->Add(newRow);
				//8.4.11 끝나는 줄의 처음부터 끝나는 글자까지 글자를 지운다.
				letterIndex = 0;
				while (letterIndex < endLetterIndex)
				{
					//8.4.11.1 글자를 지우기 전에 글자를 구한다.
					letter = endRow->GetAt(letterIndex);
					//8.4.11.2 글자를 깊은 복사해서 DummyRow에 저장한다.
					newRow->Add(letter->Clone());
					//8.4.11.3 끝나는 줄의 글자를 지운다.
					endRow->Remove(letterIndex);
					//8.4.11.4 끝나는 줄의 첫글자를 지우면 다음 글자부터 앞으로 한칸씩
					//당겨지기 때문에 끝나는 글자위치를 -1 감소시킨다.
					endLetterIndex--;
				}
			}
			//8.5 다시 실행이면
			else
			{
				//8.5.1 시작하는 글자위치부터 시작하는 줄의 마지막 글자까지 지운다.
				while (startLetterIndex < startRow->GetLength())
				{
					//8.5.1.1 줄에서 글자를 지운다.
					startRow->Remove(startLetterIndex);
				}
				//8.5.2 시작하는 줄의 다음줄부터 끝나는 줄전까지 글자와 줄을 지운다.
				nextRowIndex = startRowIndex + 1;
				while (nextRowIndex < endRowIndex)
				{
					//8.5.2.1 줄을 구한다.
					row = this->notepadForm->note->GetAt(nextRowIndex);
					//8.5.2.2 글자위치를 원위치시킨다.
					letterIndex = 0;
					//8.5.2.3 줄에서 마지막 글자까지 반복한다.
					while (letterIndex < row->GetLength())
					{
						//8.5.2.3.1 줄의 글자를 지운다.
						row->Remove(letterIndex);
					}
					//8.5.2.4 줄의 글자를 다지웠기때문에 메모장에서 줄을 지운다.
					this->notepadForm->note->Remove(nextRowIndex);
					//8.5.2.5 줄을 지웠기 때문에 선택이 끝나는 줄의 위치가 한칸 앞당겨진다.
					endRowIndex--;
				}
				//8.5.3 끝나는 줄을 구한다.
				endRow = this->notepadForm->note->GetAt(endRowIndex);
				//8.5.4 끝나는 줄의 처음부터 끝나는 글자까지 글자를 지운다.
				letterIndex = 0;
				while (letterIndex < endLetterIndex)
				{
					//8.5.4.1 끝나는 줄의 글자를 지운다.
					endRow->Remove(letterIndex);
					//8.5.4.2 끝나는 줄의 첫글자를 지우면 다음 글자부터 앞으로 한칸씩
					//당겨지기 때문에 끝나는 글자위치를 -1 감소시킨다.
					endLetterIndex--;
				}
			}
			//8.6 끝나는 줄을 시작하는 줄로 Join시킨다.
			endRow->Join(startRow);
			//8.7 끝나는 줄이 시작하는 줄로 Join되었기 때문에
			//끝나는 줄을 메모장에서 지운다.
			this->notepadForm->note->Remove(endRowIndex);
			//8.8 현재 줄의 위치를 시작하는 줄의 위치로 변경한다.
			this->notepadForm->current = this->notepadForm->note->
				GetAt(startRowIndex);
			//8.9 현재 글자의 위치를 시작하는 글자의 위치로 변경한다.
			this->notepadForm->current->Move(startLetterIndex);
		}
		//9. 처음 실행이면
		if (this->isRedone == false)
		{
			//9.1 notepadForm의 clipboard에 command의 새로 생성한 Note(복사한 내용)를 추가시켜준다.
			Long noteIndex = this->notepadForm->clipboard->Add(this->glyph->Clone());
			//9.2 복사한 texts가 생겼기 때문에 붙여넣기 메뉴를 활성화시켜준다.
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);
			//내부클립보드에 복사한 내용을 외부클립보드로 옮기기
			//9.3 notepadForm의 clipboard에 새로 생성한 Note(복사한 내용)와 그 길이를 구한다.
			CString content = this->notepadForm->clipboard->
				GetAt(noteIndex)->GetContent().c_str();
			//저장할 문자열의 길이를 구한다. ('\0'까지 포함한 크기)
			int contentLength = content.GetLength() + 1;
			//9.4 외부 클립보드가 열렸으면
			//클립보드는 여러 프로그램들이 공유할 수 있는 기능이기 때문에 어떤 프로그램(윈도우)이 클립보드를
			//사용하는지 지정하고 사용하는 것이 좋다
			if (this->notepadForm->OpenClipboard() != 0)
			{
				//9.4.1 notepadForm의 clipboard에 새로 생성한 Note(복사한 내용)을 클립보드에 옮길 공간을 생성
				//클립보드에 문자열을 복사하기 위해서는 문자열을 저장한 메모리를 클립보드로 전달해야 합니다.
				//따라서 문자열을 저장할 메모리를 먼저 만들어야 하는데, 이 메모리 형식이 핸들을 사용해야 하기
				//때문에 HeapAlloc, malloc 같은 힙 함수는 사용이 불가능합니다.그래서 아래와 핸들 값 형식을
				//사용하는 GlobalAlloc 함수를 사용해서 메모리를 할당해야 하고, 클립보드는 다른 프로그램과
				//공유하는 형식이기 때문에 이 메모리 속성에 GMEM_DDESHARE와 GMEM_MOVEABLE를 추가한다.
				//(이 메모리는 클립보드로 전달되기 때문에 자신의 프로그램에서 해제하면 안된다)
				HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, contentLength);
				//이렇게 할당된 메모리는 핸들 값을 반환하기 때문에 이 메모리에 문자열을 복사하기 위해서는 사용
				//가능한 주소를 얻어야 한다. 따라서 GlobalLock 함수를 사용하여 자신이 이 메모리를 사용하겠다고
				//설정하고 사용 가능한 주소를 얻어야 한다.이때, 주의해야 할 점은 클립보드에서 사용하는
				//문자열이 ASCII 형식으로 구성되어야 하기 때문에 유니코드를 지원(MFC 프로그램들은 기본적으로
				//(유니코드 형식임)하는 프로그램에서는 ASCII 형식으로 문자열을 변경해서 사용해야 한다.
				char* pchData = (char*)GlobalLock(hClipboardData);
				if (pchData != NULL)
				{
					// 할당된 메모리 영역에 삽입할 문자열을 복사한다.
					memcpy(pchData, content, contentLength);
					// 문자열을 복사하기 위해서 Lock 했던 메모리를 해제한다. 
					// 클립보드를 연다. 
					GlobalUnlock(hClipboardData);
					// 클립보드에 저장된 기존 문자열을 삭제한다. 
					//다른 프로그램이 클립보드를 사용하거나 다른 문자열이 클립보드에 저장되어 있을 수 있기
					//때문에 EmptyClipboard 함수를 사용해서 클립보드에 데이터를 저장하기 위한 준비
					EmptyClipboard();
					// 클립보드로 문자열을 복사하고, 클립보드에 저장하고자 하는 정보가 문자열임을 알려준다.
					SetClipboardData(CF_TEXT, hClipboardData);
					// 클립보드를 닫는다.
					CloseClipboard();
				}
			}
		}
		//10. 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//11. 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->isDirty = true;
		//12. 자동 줄 바꿈 메뉴가 체크되어 있으면
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//12.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
		//13. 메모장에서 선택된 texts를 다 지웠기 때문에 메모장에서 선택이 안된 상태로 바꾼다.
		this->notepadForm->isSelecting = false;
		//14. 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
		this->notepadForm->selectedStartXPos = 0;
		//15. 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
		this->notepadForm->selectedStartYPos = 0;
		//16. 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		//17. 글자를 지운 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
		//18. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			Long changedRowPos = this->rowIndex;
			Long changedLetterPos = this->letterIndex;
			Long originRowPos = 0;
			Long originLetterPos = 0;
			//18.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			//18.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
			this->rowIndex = originRowPos;
			this->letterIndex = originLetterPos;
		}
	}
}

//실행취소
void CutCommand::Unexecute()
{
	//잘라낸 content가 있으면
	if (this->glyph != 0)
	{
		//1. 선택이 진행되고 있는 중이었으면
		if (this->notepadForm->isSelecting == true)
		{
			//1.1. 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
			this->notepadForm->selectingTexts->Undo();
			//1.2 선택이 끝난 상태로 바꾼다.
			this->notepadForm->isSelecting = false;
			//1.3 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
			this->notepadForm->selectedStartXPos = 0;
			//1.4 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
			this->notepadForm->selectedStartYPos = 0;
		}
		//1. RowAutoChange를 생성한다.
		RowAutoChange rowAutoChange(this->notepadForm);
		Long changedRowPos = 0;
		Long changedLetterPos = 0;
		Long originRowPos = this->rowIndex;
		Long originLetterPos = this->letterIndex;
		//2. 현재 줄의 위치를 이동시킨다.(캐럿이 다른 곳에 있으면 그 곳에 글자가 지워지기 때문에)
		Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//3. 현재 글자의 위치를 이동시킨다.
		Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
		//4. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치가 저장되어 있음)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//4.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//4.2 현재 줄의 위치와 글자 위치를 다시 조정한다.
			currentRowPos = this->notepadForm->note->Move(changedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
		}
		//5. 메모장에서 선택된 texts를 다시 복구하기 때문에 메모장에서 선택이 된 상태로 바꾼다.
		this->notepadForm->isSelecting = true;
		//6. 선택이 다시 시작했기 때문에 캐럿의 x좌표를 command에 저장된 현재 글자 위치로 저장한다.
		this->notepadForm->selectedStartXPos = currentLetterPos;
		this->selectedStartXPos = this->notepadForm->selectedStartXPos;
		//7. 선택이 다시 시작했기 때문에 캐럿의 y좌표를 command에 저장된 현재 줄의 위치로 저장한다.
		this->notepadForm->selectedStartYPos = currentRowPos;
		this->selectedStartYPos = this->notepadForm->selectedStartYPos;
		//8. 메모장의 현재 줄을 구한다.
		Glyph* currentRow = this->notepadForm->current;
		//9. 메모장의 현재 글자위치가 줄의 글자개수보다 작으면
		//뒤에 split할 글자가 있으면 split하고 마지막 글자라서 뒤에 글자가 없으면 split하지 않는다.
		Glyph* splitedRow = 0;
		bool isSplited = false;
		if (currentLetterPos < currentRow->GetLength())
		{
			//9.1 메모장의 현재 줄에서 현재 글자위치 다음부터 split시킨다.(splitedRow가 힙에 할당됨)
			splitedRow = currentRow->Split(currentLetterPos);
			//9.2 split이 되었기 때문에 isSplited을 true로 바꿔준다.
			isSplited = true;
		}
		//10. command의 현재 노트의 첫번째 줄을 구한다.
		Long firstCopyRowPos = 0;
		Glyph* firstCopyRow = this->glyph->GetAt(firstCopyRowPos);
		Glyph* letter = 0;//글자를 담을 공간
		//11. command의 현재 노트의 첫번째 줄의 개수보다 작은동안 반복한다.
		Long letterIndex = 0;
		while (letterIndex < firstCopyRow->GetLength())
		{
			//11.1 command의 현재 노트의 첫번째 줄의 글자를 구한다.
			letter = firstCopyRow->GetAt(letterIndex);
			//11.2 글자를 메모장의 현재 줄에 추가한다.(깊은 복사)
			currentRow->Add(letter->Clone());
			//11.3 다음 글자로 이동한다.
			letterIndex++;
		}
		//12. command의 현재 노트의 첫번째 줄에서 다음 줄로 이동한다.
		Long nextCopyRowPos = firstCopyRowPos + 1;
		Glyph* copyRow = firstCopyRow;//첫번째 줄이 디폴트
		Long i = currentRowPos;
		//13. 클립보드의 현재 노트(복사한 내용)의 마지막줄까지 반복한다.
		while (nextCopyRowPos < this->glyph->GetLength())
		{
			//13.1 복사한 줄을 구한다.
			copyRow = this->glyph->GetAt(nextCopyRowPos);
			//13.2 메모장의 현재 줄의 다음 줄로 이동한다.
			i++;
			//13.3 구한 줄을 메모장의 현재 줄의 위치 다음부터 끼워넣는다.(깊은 복사)
			i = this->notepadForm->note->Add(i, copyRow->Clone());
			//13.4 복사한 노트에서 다음 줄로 이동한다.
			nextCopyRowPos++;
		}
		//14. 메모장에서 현재 줄을 구한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(i);
		//15. 메모장에서 현재 글자위치를 구한다.
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//16. 메모장에서 아까 split했던 글자들이 있으면
		if (isSplited == true)
		{
			//16.1 split한 줄을 메모장의 현재 줄에 Join시킨다.
			splitedRow->Join(this->notepadForm->current);
			//16.2 splitedRow를 할당해제 시켜준다.
			if (splitedRow != 0)
			{
				delete splitedRow;
			}
			//16.3 메모장에서 현재 글자위치를 다시 조정해준다.
			this->notepadForm->current->Move(currentLetterPos);
		}
		//17. 자동개행이 진행중이면 붙여넣은 줄들을 자동개행시켜준다.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//17.1 자동개행클래스를 생성한다.
			RowAutoChange rowAutoChange(this->notepadForm);
			//17.2 부분자동개행을 한다.
			Long endPastedRowPos = rowAutoChange.DoPartRows(currentRowPos, i);
			//17.3 붙여넣기가 끝나는 줄로 이동시킨다.
			//붙여넣기가 끝나는 줄은 OnSize에서 부분자동개행을 해서 처리되기 때문에 캐럿의 위치만 조정해주면 됨!
			this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current->Move(currentLetterPos);
		}
		//12. 선택영역이 다시 생겼기 때문에 복사하기, 잘라내기, 삭제 메뉴를 활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
		//13. 글자가 다시 추가되었기 때문에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();

		//14. 글자를 입력한 후에 command의 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
		//15. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			Long changedRowPos = this->rowIndex;
			Long changedLetterPos = this->letterIndex;
			Long originRowPos = 0;
			Long originLetterPos = 0;
			//15.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			//15.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
			this->rowIndex = originRowPos;
			this->letterIndex = originLetterPos;
		}
	}
}

//다시실행인지 여부 구하기
bool CutCommand::IsRedone()
{
	return this->isRedone;
}

//줄의 위치 구하기
Long CutCommand::GetRowIndex()
{
	return this->rowIndex;
}
//글자 위치 구하기
Long CutCommand::GetLetterIndex()
{
	return this->letterIndex;
}

//선택이 시작되는 글자의 위치구하기
Long CutCommand::GetSelectedStartXPos()
{
	return this->selectedStartXPos;
}

//선택이 시작되는 줄의 위치구하기
Long CutCommand::GetSelectedStartYPos()
{
	return this->selectedStartYPos;
}

//SetMacroEnd(실행취소 및 다시실행 매크로출력 종료지점 설정)
void CutCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void CutCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//실행취소 종료지점여부 구하기
bool CutCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//다시실행 종료지점여부 구하기 
bool CutCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}

//SetRedone(다시 실행이라고 설정함)
void CutCommand::SetRedone()
{
	this->isRedone = true;
}

//선택영역이 지워졌는지 확인 여부
bool CutCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}

//소멸자
CutCommand::~CutCommand()
{
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}