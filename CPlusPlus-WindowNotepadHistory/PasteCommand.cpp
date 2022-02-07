#include "PasteCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"
#include "GlyphCreator.h"
#include "TextExtent.h"
#include "RowAutoChange.h"
#include "SelectingTexts.h"

//디폴트생성자
PasteCommand::PasteCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->glyph = 0;//붙여넣을 content를 저장할 공간
	this->note = 0;//선택된 texts가 있으면 지우기 전에 저장할 공간
	this->startYPos = notepadForm->note->GetCurrent();
	this->startXPos = notepadForm->current->GetCurrent();
	this->selectedEndYPos = 0;
	this->selectedEndXPos = 0;
	this->pastingEndYPos = 0;
	this->pastingEndXPos = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isDirty = false;//처음에 생성될 때는 변경사항이 없으므로 false가 디폴트값임.
	this->isSelectedTextsRemoved = false;//처음에 생성될 때는 선택영역이 안지워졌으므로 false가 디폴트값
}

//Execute
void PasteCommand::Execute()
{
	//1. RowAutoChange를 생성한다.
	RowAutoChange rowAutoChange(this->notepadForm);

	Long changedRowPos = 0;//자동개행 후 줄 위치
	Long changedLetterPos = 0;//자동개행 후 글자 위치
	Long originRowPos = 0;//자동개행 전 줄 위치
	Long originLetterPos = 0;//자동개행 전 글자 위치
	Long currentRowPos = 0;//현재 줄의 위치
	Long currentLetterPos = 0;//현재 글자의 위치
	Long selectedStartRowPos = 0;//선택이 시작되는 줄 위치
	Long selectedStartLetterPos = 0;//선택이 시작되는 글자 위치
	Long selectedEndRowPos = 0;//선택이 끝나는 줄 위치
	Long selectedEndLetterPos = 0;//선택이 끝나는 글자 위치

	//2. 처음 실행이면
	if (this->isRedone == false)
	{
		//외부 클립보드에 문자열이 저장되어 있으면 외부클립보드에서 문자열을 받아와서 contents에 저장한다.
		//contents는 한줄로 문자열을 저장하는데 줄을 구분하는 것은 '\n(개행문자)'여부이다. 이 개행문자를
		//통해서 줄단위로 나눠서 저장해야한다.
		//2.1 외부 클립보드에 문자열이 저장되어 있으면
		//클립보드는 여러 프로그램들이 공유할 수 있는 기능이기 때문에 어떤 프로그램(윈도우)이 클립보드를
		//사용하는지 지정하고 사용하는 것이 좋다
		Glyph* newRow = 0;
		unsigned int priority_list = CF_TEXT;
		char* content = NULL;
		if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
		{
			//2.1.1 클립보드에 있는 문자열 정보를 얻기 위해서 클립보드를 연다.
			if (this->notepadForm->OpenClipboard())
			{
				//2.1.1.1 클립보드에서 문자열이 저장된 메모리의 핸들 값을 얻는다.
				HANDLE h_clipboard_data = GetClipboardData(CF_TEXT);
				if (h_clipboard_data != NULL)
				{
					//2.1.1.1.1 메모리 핸들 값을 이용하여 실제 사용가능한 주소를 얻는다.
					char* p_clipboard_data = (char*)::GlobalLock(h_clipboard_data);
					//2.1.1.1.2 클립보드에 저장된 문자열의 길이를 구한다. ('\0'포함 크기)
					int contentLength = strlen(p_clipboard_data) + 1;
					//2.1.1.1.3 클립보드에 저장된 문자열을 저장하기 위해 메모리를 할당한다.
					content = new char[contentLength];
					//2.1.1.1.4 할당된 메모리에 클리보드 문자열을 복사한다.
					memcpy(content, p_clipboard_data, contentLength);
					//2.1.1.1.5 문자열을 복사하기 위해서 Lock했던 메모리를 해제한다.
					GlobalUnlock(h_clipboard_data);
				}
				//2.1.1.2 클립보드를 닫는다.
				CloseClipboard();
			}
			//2.1.2 복사할 텍스트를 담을 새노트를 생성한다.
			this->glyph = new Note();
			//2.1.3 줄을 생성한다. 
			newRow = new Row();
			//2.1.4 새로 생성한 줄을 새로 생성한 노트에 추가한다.
			Long rowIndex = this->glyph->Add(newRow);
			//2.1.5 복사한 내용의 마지막까지 반복한다.(마지막에 널문자가 저장되어 있음)
			GlyphCreator glyphCreator;
			Glyph* glyph = 0;
			Long letterIndex = 0;
			char doubleByteLetter[3];
			char singleByteLetter[2];
			Long i = 0;//배열요소 초기화
			//2.1.6 문장의 마지막(NULL문자)이 아닌동안 반복한다
			while (content[i] != '\0')
			{
				//2.1.6.1 문장을 저장한 각 배열요소가 2byte 문자인지 1byte 문자인지 판단한다.
				if ((content[i] & 0x80))//한글이면(2byte문자)
				{
					//2.1.6.1.1 doubleByteLetter에 저장한다.
					doubleByteLetter[0] = content[i];
					i++;
					doubleByteLetter[1] = content[i];
					doubleByteLetter[2] = '\0';
					//2.1.6.1.2 doubleByteLetter를 생성한다.(팩토리메소드패턴)
					glyph = glyphCreator.Create((char*)doubleByteLetter);
					//2.1.6.1.3 줄에 doubleByteLetter를 추가한다.
					letterIndex = this->glyph->GetAt(rowIndex)->Add(glyph);
				}
				//2.1.6.2 개행문자이면(줄을 바꾸면 줄을 추가한다)
				else if (content[i] == '\n' || content[i] == '\r')
				{
					//2.1.6.2.1 row를 생성한다.
					glyph = glyphCreator.Create(content + i);
					//2.1.6.2.2 새로운 줄을 추가한다.
					rowIndex = this->glyph->Add(glyph);
					//2.1.6.2.3 다음 글자가 '\n'이면
					if (content[i + 1] == '\n')
					{
						i++;
					}
				}
				//2.1.6.3 입력받은 문자가 한글이 아니고 개행문자가 아니면(1byte문자)
				else
				{
					//2.1.6.3.1 singleByteLetter에 저장한다.
					singleByteLetter[0] = content[i];
					singleByteLetter[1] = '\0';
					//2.1.6.3.2 singleByteLetter를 생성한다.
					glyph = glyphCreator.Create(singleByteLetter);
					//2.1.6.3.3 줄에 singleByteLetter를 추가한다.
					letterIndex = this->glyph->GetAt(rowIndex)->Add(glyph);
				}
				i++;
			}
			//2.1.7 앞에서 content를 할당했기 때문에 여기서 할당해제를 해줘야한다.
			if (content != 0)
			{
				delete[] content;
			}
			//2.1.8 notepadForm의 clipboard에 새로 생성한 Note(복사한 내용)를 추가시켜준다.
			Long noteIndex = this->notepadForm->clipboard->Add(this->glyph->Clone());
		}
	}
	//3. 복사한 content가 있으면
	if (this->glyph != 0)
	{
		//선택영역이 있으면 선택영역 삭제
		//3.1 메모장에서 선택된 texts가 있거나 또는 선택된 영역을 지웠으면
		if (this->notepadForm->isSelecting == true || this->isSelectedTextsRemoved == true)
		{
			//3.1.1 OnReplaceButtonClickedCommand가 처음 실행되면
			if (this->isRedone == false)
			{
				//3.1.1.1 현재 줄의 위치와 글자 위치를 구한다.
				currentRowPos = this->notepadForm->note->GetCurrent();
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//3.1.1.2 선택이 시작되는 줄의 위치와 글자 위치를 구한다.
				selectedStartRowPos = this->notepadForm->selectedStartYPos;
				this->startYPos = selectedStartRowPos;
				selectedStartLetterPos = this->notepadForm->selectedStartXPos;
				this->startXPos = selectedStartLetterPos;
				//3.1.1.3 선택이 끝나는 줄의 위치와 글자 위치를 구한다.
				selectedEndRowPos = currentRowPos;
				this->selectedEndYPos = selectedEndRowPos;
				selectedEndLetterPos = currentLetterPos;
				this->selectedEndXPos = selectedEndLetterPos;
				//3.1.1.4 선택된 단어(찾은 단어)를 command에 복사해서 저장하고 지운다.
				this->note = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
				//3.1.1.8 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의 연산 안에서 
				//현재 줄의 위치와 글자 위치는 조정이 되지만 notepadForm의 current(현재줄)는 
				//조정할 수 없어서 notepadForm에서 해준다.)
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//3.1.1.9 선택된 단어를 지웠기 때문에 다시 시작하는 줄의 위치와 글자위치를 갱신한다.
				this->startYPos = currentRowPos;
				this->startXPos = currentLetterPos;
				//3.1.1.10 자동 줄 바꿈 메뉴가 체크되어 있으면(선택영역이 삭제 됬기 때문에 자동개행을 다시해줘야함)
				if (this->notepadForm->isRowAutoChanging == true)
				{
					//3.1.1.10.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
					//OnSize에서 부분자동개행을 하도록 한다. 
					this->notepadForm->SendMessage(WM_SIZE);
					//3.1.1.10.2 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가
					//바뀔 수있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
					this->startYPos = this->notepadForm->note->GetCurrent();
					this->startXPos = this->notepadForm->current->GetCurrent();
					//3.1.1.10.3 지금 현재 startYPos와 startXPos의 위치는 자동개행이 적용된 상태의 위치이기
					//때문에 자동개행을 안한 상태에서 선택하기가 시작되는 진짜 줄의 위치와 글자위치를 구한다.
					changedRowPos = this->startYPos;
					changedLetterPos = this->startXPos;
					originRowPos = 0;
					originLetterPos = 0;
					rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
						&originRowPos);
					this->startYPos = originRowPos;
					this->startXPos = originLetterPos;
				}
			}
			//3.1.2 OnReplaceButtonClickedCommand가 다시 실행되면
			else
			{
				//3.1.2.1 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치가
				//저장되어 있음)
				if (this->notepadForm->isRowAutoChanging == true)
				{
					//3.1.2.1.1 변경된 화면 크기에 맞는 현재 줄과 글자 위치를 구한다.
					changedRowPos = 0;
					changedLetterPos = 0;
					originRowPos = this->selectedEndYPos;
					originLetterPos = this->selectedEndXPos;
					rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
						&changedRowPos);
					//3.1.2.1.2 자동개행을 반영해서 현재 줄의 위치와 글자 위치를 다시 조정한다.
					currentRowPos = this->notepadForm->note->Move(changedRowPos);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
					currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
					//3.1.2.1.3 자동개행을 반영해서 선택이 끝나는 줄의 위치와 글자 위치를 다시 조정한다.
					selectedEndRowPos = currentRowPos;
					selectedEndLetterPos = currentLetterPos;
					//3.1.2.1.4 변경된 화면 크기에 맞는 선택이 시작되는 줄과 글자 위치를 구한다.
					changedRowPos = 0;
					changedLetterPos = 0;
					originRowPos = this->startYPos;
					originLetterPos = this->startXPos;
					rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
						&changedRowPos);
					//3.1.2.1.5 자동개행을 반영해서 선택이 시작되는 줄과 글자 위치를 다시 조정한다.
					selectedStartRowPos = changedRowPos;//선택이 시작되는 줄
					selectedStartLetterPos = changedLetterPos;//선택이 시작되는 글자
				}
				//3.1.2.2 자동개행 진행 중이 아니면
				else
				{
					//3.1.2.2.1 현재 줄의 위치와 글자 위치를 다시 조정한다.
					currentRowPos = this->notepadForm->note->Move(this->selectedEndYPos);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
					currentLetterPos = this->notepadForm->current->Move(this->selectedEndXPos);
					//3.1.2.2.2 선택이 시작되는 줄의 위치와 글자 위치를 다시 조정한다.
					selectedStartRowPos = this->startYPos;
					selectedStartLetterPos = this->startXPos;
					//3.1.2.2.3 선택이 끝나는 줄의 위치와 글자 위치를 다시 조정한다.
					selectedEndRowPos = currentRowPos;
					selectedEndLetterPos = currentLetterPos;
				}
				//3.1.2.3 선택된 단어(찾은 단어)를 지운다.
				this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
				//3.1.2.4 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의 연산 안에서 현재 줄의 위치와 글자 
				//위치는 조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//3.1.2.5 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가 바뀔 수
				//있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
				this->startYPos = this->notepadForm->note->GetCurrent();
				this->startXPos = this->notepadForm->current->GetCurrent();
				//3.1.2.6 자동 줄 바꿈 메뉴가 체크되어 있으면(선택영역이 삭제 됬기 때문에 자동개행을 다시해줘야함)
				if (this->notepadForm->isRowAutoChanging == true)
				{
					//3.1.2.6.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
					//OnSize에서 부분자동개행을 하도록 한다. 
					this->notepadForm->SendMessage(WM_SIZE);
					//3.1.2.6.2 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가
					//바뀔 수 있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
					this->startYPos = this->notepadForm->note->GetCurrent();
					this->startXPos = this->notepadForm->current->GetCurrent();
					//3.1.2.6.3 지금 현재 startYPos와 startXPos의 위치는 자동개행이 적용된 상태의 위치이기
					//때문에 자동개행을 안한 상태에서 선택하기가 시작되는 진짜 줄의 위치와 글자위치를 구한다.
					changedRowPos = this->startYPos;
					changedLetterPos = this->startXPos;
					originRowPos = 0;
					originLetterPos = 0;
					rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
						&originRowPos);
					this->startYPos = originRowPos;
					this->startXPos = originLetterPos;
				}
			}
			//3.1.3 메모장에서 선택된 단어를 지웠기 때문에 메모장에서 선택이 안된 상태로 바꾼다.
			this->notepadForm->isSelecting = false;
			//3.1.4 선택이 끝났기 때문에 선택이 시작되는 x좌표를 0으로 저장한다.
			this->notepadForm->selectedStartXPos = 0;
			//3.1.5 선택이 끝났기 때문에 선택이 시작되는 y좌표를 0으로 저장한다.
			this->notepadForm->selectedStartYPos = 0;
			//3.1.6 선택된 영역이 없어졌기 때문에 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			//3.1.7 command가 선택영역을 지웠다고 표시한다.
			this->isSelectedTextsRemoved = true;
		}
		//3.2 선택영역이 없거나 선택영역을 안지웠으면
		else
		{
			//3.2.1 처음 실행이면
			if (this->isRedone == false)
			{
				//3.2.1.1 현재 줄의 위치와 글자 위치를 구한다.
				currentRowPos = this->notepadForm->note->GetCurrent();
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//3.2.1.2 현재 줄의 위치와 글자위치를 시작하는 줄과 글자 위치로 저장한다.
				this->startYPos = currentRowPos;
				this->startXPos = currentLetterPos;
				//3.2.1.3 자동 줄 바꿈 메뉴가 체크되어 있으면(진짜 줄의 위치와 글자위치로 저장한다.)
				if (this->notepadForm->isRowAutoChanging == true)
				{
					//3.2.1.3.1 지금 현재 startYPos와 startXPos의 위치는 자동개행이 적용된 상태의 위치이기
					//때문에 자동개행을 안한 상태에서 선택하기가 시작되는 진짜 줄의 위치와 글자위치를 구한다.
					changedRowPos = this->startYPos;
					changedLetterPos = this->startXPos;
					originRowPos = 0;
					originLetterPos = 0;
					rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
						&originRowPos);
					this->startYPos = originRowPos;
					this->startXPos = originLetterPos;
				}
			}
			//3.2.2 다시 실행이면
			else
			{
				//3.2.2.1 자동개행이 진행중이면(command의 시작하는 줄과 글자 위치는 
				//항상 진짜 줄과 글자 위치가 저장되어 있기 때문에 자동개행에 맞게 바꿔줘야함)
				if (this->notepadForm->isRowAutoChanging == true)
				{
					//3.2.2.1.1 자동개행을 반영하여 시작되는 줄과 글자 위치를 구한다.
					changedRowPos = 0;
					changedLetterPos = 0;
					originRowPos = this->startYPos;
					originLetterPos = this->startXPos;
					rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
						&changedRowPos);
					//3.2.2.1.2 자동개행을 반영해서 현재 줄의 위치와 글자 위치를 다시 조정한다.
					currentRowPos = this->notepadForm->note->Move(changedRowPos);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
					currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
				}
				//3.2.2.2 자동개행 진행중이 아니면
				else
				{
					//3.2.2.2.1 현재 줄의 위치와 글자 위치를 다시 조정한다.
					currentRowPos = this->notepadForm->note->Move(this->startYPos);
					this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
					currentLetterPos = this->notepadForm->current->Move(this->startXPos);
				}
			}
		}
		//3.3 붙일 내용을 삽입하기 전에 줄의 위치를 저장한다.
		Long previousRowIndex = currentRowPos;
		//3.4 붙일 내용을 삽입하고 현재 줄의 위치를 반환받는다.
		currentRowPos = this->notepadForm->note->
			InsertTexts(currentRowPos, currentLetterPos, this->glyph);
		//3.5 메모장의 현재 줄을 저장한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//3.6 메모장에 붙여넣었기 때문에 붙여넣기가 끝나는 줄과 글자위치를 갱신한다.
		this->pastingEndYPos = currentRowPos;
		this->pastingEndXPos = currentLetterPos;
		//3.7 자동개행이 진행중이면 붙여넣은 줄들을 자동개행시켜준다.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//3.7.1 부분자동개행을 한다.(마지막 줄은 제외하고 자동개행함)
			Long endPastedRowPos = rowAutoChange.DoPartRows(previousRowIndex, currentRowPos);
			//3.7.2 붙여넣기가 끝나는 줄로 이동시킨다.
			//붙여넣기가 끝나는 줄은 OnSize에서 부분자동개행을 해서 처리되기 때문에 캐럿의 위치만 조정해주면 됨!
			currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			this->notepadForm->current->Move(currentLetterPos);
			//3.7.3 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. (마지막 줄을 자동개행시킴)
			this->notepadForm->SendMessage(WM_SIZE);
			//3.7.4 메모장의 현재 줄을 저장한다.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//3.7.5 메모장에 선택영역이 복원되었고, 자동개행인 반영된 선택이 끝나는 줄과 글자위치를 저장한다. 
			this->pastingEndYPos = currentRowPos;
			this->pastingEndXPos = currentLetterPos;
			//3.7.6 지금 현재 선택이 끝나는 줄과 글자 위치는 자동개행이 적용된 상태의 위치이기 때문에
			//자동개행을 안한 상태에서 선택하기가 끝나는 진짜 줄의 위치와 글자위치를 구한다.
			changedRowPos = this->pastingEndYPos;
			changedLetterPos = this->pastingEndXPos;
			originRowPos = 0;
			originLetterPos = 0;
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			this->pastingEndYPos = originRowPos;
			this->pastingEndXPos = originLetterPos;
		}
		//3.8 처음 실행이면
		if (this->isRedone == false)
		{
			//3.8.1 메모장 제목에 *를 추가한다.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - 메모장";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.8.2 메모장에 변경사항이 있음을 저장한다.
			this->notepadForm->isDirty = true;
			//3.8.3 command에 변경사항이 있음을 표시한다.
			this->isDirty = true;
		}
	}
}

//실행취소
void PasteCommand::Unexecute()
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
	//2. RowAutoChange를 생성한다.
	RowAutoChange rowAutoChange(this->notepadForm);

	Long changedRowPos = 0;//자동개행 후 줄 위치
	Long changedLetterPos = 0;//자동개행 후 글자 위치
	Long originRowPos = 0;//자동개행 전 줄 위치
	Long originLetterPos = 0;//자동개행 전 글자 위치
	Long currentRowPos = 0;//현재 줄의 위치
	Long currentLetterPos = 0;//현재 글자의 위치
	Long selectedStartRowPos = 0;//선택이 시작되는 줄 위치
	Long selectedStartLetterPos = 0;//선택이 시작되는 글자 위치
	Long selectedEndRowPos = 0;//선택이 끝나는 줄 위치
	Long selectedEndLetterPos = 0;//선택이 끝나는 글자 위치

	//3. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치가 저장되어 있음)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//3.1 자동개행을 반영해서 붙여넣기가 끝나는(현재) 줄과 글자 위치를 구한다.
		changedRowPos = 0;
		changedLetterPos = 0;
		originRowPos = this->pastingEndYPos;
		originLetterPos = this->pastingEndXPos;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//3.2 자동개행을 반영해서 현재 줄의 위치와 글자 위치를 다시 조정한다.
		currentRowPos = this->notepadForm->note->Move(changedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
		//3.3 자동개행을 반영해서 붙여넣기가 끝나는 줄의 위치와 글자 위치를 다시 조정한다.
		selectedEndRowPos = currentRowPos;
		selectedEndLetterPos = currentLetterPos;
		//3.4 자동개행을 반영해서 붙여넣기가 시작되는 줄과 글자 위치를 구한다.
		changedRowPos = 0;
		changedLetterPos = 0;
		originRowPos = this->startYPos;
		originLetterPos = this->startXPos;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//3.5 자동개행을 반영해서 붙여넣기가 시작되는 줄과 글자 위치를 다시 조정한다.
		selectedStartRowPos = changedRowPos;//선택이 시작되는 줄
		selectedStartLetterPos = changedLetterPos;//선택이 시작되는 글자
	}
	//4. 자동개행 진행 중이 아니면
	else
	{
		//4.1 현재 줄의 위치와 글자 위치를 다시 조정한다.
		currentRowPos = this->notepadForm->note->Move(this->pastingEndYPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->pastingEndXPos);
		//4.2 붙여넣기가 시작되는 줄의 위치와 글자 위치를 다시 조정한다.
		selectedStartRowPos = this->startYPos;
		selectedStartLetterPos = this->startXPos;
		//4.3 붙여넣기가 끝나는 줄의 위치와 글자 위치를 다시 조정한다.
		selectedEndRowPos = currentRowPos;
		selectedEndLetterPos = currentLetterPos;
	}
	//5. Execute에서 붙여넣은 바꾼 단어를 지운다.
	this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
		selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
	//6. 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의연산안에서 현재 줄의 위치와 글자 위치는
	//조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
	currentRowPos = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//7. 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가 바뀔 수
	//있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
	this->startYPos = this->notepadForm->note->GetCurrent();
	this->startXPos = this->notepadForm->current->GetCurrent();
	//8. 자동 줄 바꿈 메뉴가 체크되어 있으면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//8.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
		//OnSize에서 부분자동개행을 하도록 한다. 
		this->notepadForm->SendMessage(WM_SIZE);
		//8.2 선택영역이 삭제되었기 때문에 자동개행이 끝나고 나면 줄의 위치와 글자 위치가 바뀔 수
		//있기 때문에 시작하는 줄의 위치를 현재 줄의 위치와 글자위치로 재갱신해줘야한다.
		this->startYPos = this->notepadForm->note->GetCurrent();
		this->startXPos = this->notepadForm->current->GetCurrent();
		//8.3 지금 현재 startYPos와 startXPos의 위치는 자동개행이 적용된 상태의 위치이기 때문에
		//자동개행을 안한 상태에서 선택하기가 시작되는 진짜 줄의 위치와 글자위치를 구한다.
		changedRowPos = this->startYPos;
		changedLetterPos = this->startXPos;
		originRowPos = 0;
		originLetterPos = 0;
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		this->startYPos = originRowPos;
		this->startXPos = originLetterPos;
	}
	//9. 선택된 영역을 지웠으면
	if (this->isSelectedTextsRemoved == true)
	{
		//9.1 바꾼단어를 지웠고 이제 선택영역을 다시 복원하기 때문에 현재 줄과 글자의 위치를
		//선택영역을 시작 줄과 글자 위치로 설정한다.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		this->notepadForm->selectedStartYPos = currentRowPos;
		this->notepadForm->selectedStartXPos = currentLetterPos;
		//9.2 메모장에서 선택된 texts를 다시 복구하기 때문에 메모장에서 선택이 된 상태로 바꾼다.
		this->notepadForm->isSelecting = true;
		//9.3 선택영역이 다시 생겼기 때문에 복사하기, 잘라내기, 삭제 메뉴를 활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
		//9.4 선택영역을 다시 복원하기 전에 줄의 위치를 저장한다.
		Long previousRowIndex = currentRowPos;
		//9.5 선택영역을 다시 복원하고 현재 줄의 위치를 반환받는다.
		currentRowPos = this->notepadForm->note->
			InsertTexts(currentRowPos, currentLetterPos, this->note);
		//9.6 메모장의 현재 줄을 저장한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//9.7 메모장에 선택영역이 복원되었기 때문에 선택이 끝나는 줄과 글자위치를 갱신한다.
		this->selectedEndYPos = currentRowPos;
		this->selectedEndXPos = currentLetterPos;
		//9.8 자동개행이 진행중이면 붙여넣은 줄들을 자동개행시켜준다.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//9.8.1 부분자동개행을 한다.(마지막 줄은 제외하고 자동개행함)
			Long endPastedRowPos = rowAutoChange.DoPartRows(previousRowIndex, currentRowPos);
			//9.8.2 선택영역이 끝나는 줄로 이동시킨다.
			//선택영역이 끝나는 줄은 OnSize에서 부분자동개행을 해서 처리되기 때문에 캐럿의 위치만 조정해주면 됨!
			currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			this->notepadForm->current->Move(currentLetterPos);
			//9.8.3 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. (마지막 줄을 자동개행시킴)
			this->notepadForm->SendMessage(WM_SIZE);
			//9.8.4 메모장의 현재 줄을 저장한다.
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//9.8.5 메모장에 선택영역이 복원되었고, 자동개행인 반영된 선택이 끝나는 줄과 글자위치를 저장한다. 
			this->selectedEndYPos = currentRowPos;
			this->selectedEndXPos = currentLetterPos;
			//9.8.6 지금 현재 선택이 끝나는 줄과 글자 위치는 자동개행이 적용된 상태의 위치이기 때문에
			//자동개행을 안한 상태에서 선택하기가 끝나는 진짜 줄의 위치와 글자위치를 구한다.
			changedRowPos = this->selectedEndYPos;
			changedLetterPos = this->selectedEndXPos;
			originRowPos = 0;
			originLetterPos = 0;
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			this->selectedEndYPos = originRowPos;
			this->selectedEndXPos = originLetterPos;
		}
	}
}

//다시실행인지 여부 구하기
bool PasteCommand::IsRedone()
{
	return this->isRedone;
}

//시작하는 줄의 위치구하기
Long PasteCommand::GetStartYPos()
{
	return this->startYPos;
}
//시작하는 글자의 위치구하기
Long PasteCommand::GetStartXPos()
{
	return this->startXPos;
}
//선택이 끝나는 줄의 위치 구하기
Long PasteCommand::GetSelectedEndYPos()
{
	return this->selectedEndYPos;
}
//선택이 끝나는 글자 위치 구하기
Long PasteCommand::GetSelectedEndXPos()
{
	return this->selectedEndXPos;
}
//붙여넣기가 끝나는 줄의 위치구하기
Long PasteCommand::GetPastingEndYPos()
{
	return this->pastingEndYPos;
}
//붙여넣기가 끝나는 글자의 위치구하기
Long PasteCommand::GetPastingEndXPos()
{
	return this->pastingEndXPos;
}

//SetMacroEnd(실행취소 및 다시실행 매크로출력 종료지점 설정)
void PasteCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void PasteCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//실행취소 종료지점여부 구하기
bool PasteCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//다시실행 종료지점여부 구하기 
bool PasteCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//변경사항이 있는지 확인 여부
bool PasteCommand::IsDirty()
{
	return this->isDirty;
}

//SetRedone(다시 실행이라고 설정함)
void PasteCommand::SetRedone()
{
	this->isRedone = true;
}

//선택영역이 지워졌는지 확인 여부
bool PasteCommand::IsSelectedTextsRemoved()
{
	return this->isSelectedTextsRemoved;
}

//소멸자
PasteCommand::~PasteCommand()
{
	if (this->note != 0)
	{
		delete this->note;
	}
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}