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
	this->startPasteRowIndex = 0;//붙여넣기가 시작되는 줄위치
	this->startPasteLetterIndex = 0;//붙여넣기가 시작되는 글자위치
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isDirty = false;//처음에 생성될 때는 변경사항이 없으므로 false가 디폴트값임.
	this->isSelectedTextsRemoved = false;//처음에 생성될 때는 선택영역이 안지워졌으므로 false가 디폴트값
}

//※ this->rowIndex와 this->letterIndex는 항상 원래 진짜 줄과 글자 위치를 저장한다. 그리고 Execute할 때
//자동개행이 되어 있으면 자동개행이 된 상태에 맞게 위치를 수정해주고, 자동개행이 안되어있으면 그대로 쓰면 됨.
//this->startPasteRowIndex와 this->startPasteLetterIndex는 자동개행이 진행 중이면 진행 중인 상태의 값을
//저장하고, 자동개행이 진행 중이 아니면 자동개행이 진행 중이 아닌 상태의 값을 저장한다. 그래도 괜찮은 게 매번
//Execute될 때 마다 자동개행이 진행 중인 상태든 진행 중인 상태가 아니든 그 값을 저장하고, rowIndex와 letter
//Index는 매번 자동개행 여부에 따라 값을 바꾸기 때문에 이상이 없음! 그래서 rowIndex와 letterIndex는 진짜 줄과
//글자 위치를 저장하고, this->startPasteRowIndex와 this->startPasteLetterIndex는 자동개행 진행 여부와 
//상관없이 그냥 매번 그 값을 저장하면 된다.

//Execute
void PasteCommand::Execute()
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
	//3. PasteCommand가 다시 실행되면
	if (this->isRedone == true)
	{
		//3.1 현재 줄의 위치와 글자 위치를 다시 조정한다.
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
	//4. 처음 실행이면
	if (this->isRedone == false)
	{
		//외부 클립보드에 문자열이 저장되어 있으면 외부클립보드에서 문자열을 받아와서 contents에 저장한다.
		//contents는 한줄로 문자열을 저장하는데 줄을 구분하는 것은 '\n(개행문자)'여부이다. 이 개행문자를
		//통해서 줄단위로 나눠서 저장해야한다.
		//4.1 외부 클립보드에 문자열이 저장되어 있으면
		//클립보드는 여러 프로그램들이 공유할 수 있는 기능이기 때문에 어떤 프로그램(윈도우)이 클립보드를
		//사용하는지 지정하고 사용하는 것이 좋다
		Glyph* newRow = 0;
		unsigned int priority_list = CF_TEXT;
		char* content = NULL;
		if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
		{
			//4.1.1 클립보드에 있는 문자열 정보를 얻기 위해서 클립보드를 연다.
			if (this->notepadForm->OpenClipboard())
			{
				//4.1.1.1 클립보드에서 문자열이 저장된 메모리의 핸들 값을 얻는다.
				HANDLE h_clipboard_data = GetClipboardData(CF_TEXT);
				if (h_clipboard_data != NULL)
				{
					//4.1.1.1.1 메모리 핸들 값을 이용하여 실제 사용가능한 주소를 얻는다.
					char* p_clipboard_data = (char*)::GlobalLock(h_clipboard_data);
					//4.1.1.1.2 클립보드에 저장된 문자열의 길이를 구한다. ('\0'포함 크기)
					int contentLength = strlen(p_clipboard_data) + 1;
					//4.1.1.1.3 클립보드에 저장된 문자열을 저장하기 위해 메모리를 할당한다.
					content = new char[contentLength];
					//4.1.1.1.4 할당된 메모리에 클리보드 문자열을 복사한다.
					memcpy(content, p_clipboard_data, contentLength);
					//4.1.1.1.5 문자열을 복사하기 위해서 Lock했던 메모리를 해제한다.
					GlobalUnlock(h_clipboard_data);
				}
				//4.1.1.2 클립보드를 닫는다.
				CloseClipboard();
			}
			//4.1.2 복사할 텍스트를 담을 새노트를 생성한다.
			this->glyph = new Note();
			//4.1.3 줄을 생성한다. 
			newRow = new Row();
			//4.1.4 새로 생성한 줄을 새로 생성한 노트에 추가한다.
			Long rowIndex = this->glyph->Add(newRow);
			//4.1.5 복사한 내용의 마지막까지 반복한다.(마지막에 널문자가 저장되어 있음)
			GlyphCreator glyphCreator;
			Glyph* glyph = 0;
			Long letterIndex = 0;
			char doubleByteLetter[3];
			char singleByteLetter[2];
			Long i = 0;//배열요소 초기화
			//4.1.6 문장의 마지막(NULL문자)이 아닌동안 반복한다
			while (content[i] != '\0')
			{
				//4.1.6.1 문장을 저장한 각 배열요소가 2byte 문자인지 1byte 문자인지 판단한다.
				if ((content[i] & 0x80))//한글이면(2byte문자)
				{
					//4.1.6.1.1 doubleByteLetter에 저장한다.
					doubleByteLetter[0] = content[i];
					i++;
					doubleByteLetter[1] = content[i];
					doubleByteLetter[2] = '\0';
					//4.1.6.1.2 doubleByteLetter를 생성한다.(팩토리메소드패턴)
					glyph = glyphCreator.Create((char*)doubleByteLetter);
					//4.1.6.1.3 줄에 doubleByteLetter를 추가한다.
					letterIndex = this->glyph->GetAt(rowIndex)->Add(glyph);
				}
				//4.1.6.2 개행문자이면(줄을 바꾸면 줄을 추가한다)
				else if (content[i] == '\n' || content[i] == '\r')
				{
					//4.1.6.2.1 row를 생성한다.
					glyph = glyphCreator.Create(content + i);
					//4.1.6.2.2 새로운 줄을 추가한다.
					rowIndex = this->glyph->Add(glyph);
					//4.1.6.2.3 다음 글자가 '\n'이면
					if (content[i + 1] == '\n')
					{
						i++;
					}
				}
				//4.1.6.3 입력받은 문자가 한글이 아니고 개행문자가 아니면(1byte문자)
				else
				{
					//4.1.6.3.1 singleByteLetter에 저장한다.
					singleByteLetter[0] = content[i];
					singleByteLetter[1] = '\0';
					//4.1.6.3.2 singleByteLetter를 생성한다.
					glyph = glyphCreator.Create(singleByteLetter);
					//4.1.6.3.3 줄에 singleByteLetter를 추가한다.
					letterIndex = this->glyph->GetAt(rowIndex)->Add(glyph);
				}
				i++;
			}
			//4.1.7 앞에서 content를 할당했기 때문에 여기서 할당해제를 해줘야한다.
			if (content != 0)
			{
				delete[] content;
			}
			//4.1.8 notepadForm의 clipboard에 새로 생성한 Note(복사한 내용)를 추가시켜준다.
			Long noteIndex = this->notepadForm->clipboard->Add(this->glyph->Clone());
		}
	}
	//5. 복사한 content가 있으면
	if (this->glyph != 0)
	{
		//선택영역이 있으면 선택영역 삭제
		//5.1 메모장에서 선택된 texts가 있거나 또는 선택된 영역을 지웠으면
		if (this->notepadForm->isSelecting == true || this->isSelectedTextsRemoved == true)
		{
			//5.1.1 다시 실행이면
			if (this->isRedone == true)
			{
				//5.1.1.1 notepadForm의 선택이 시작되는 줄의 위치와 글자 위치를 재설정해준다. 
				this->notepadForm->selectedStartYPos = this->startPasteRowIndex;
				this->notepadForm->selectedStartXPos = this->startPasteLetterIndex;
			}
			//5.1.2 선택이 시작되는 줄과 글자 위치, 선택이 끝나는 줄과 글자 위치를 저장한다.
			Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//선택이 시작되는 줄
			Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//선택이 시작되는 글자
			Long selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
			Long selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
			//5.1.3 처음 실행이면
			if (this->isRedone == false)
			{
				//5.1.3.1 content를 복사하고 지운다.
				this->note = this->notepadForm->note->CopySelectedTextsAndRemove(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
			}
			//5.1.4 다시 실행이면
			else
			{
				//5.1.4.1 content를 지운다.
				this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
			}
			//5.1.5 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의연산안에서 현재 줄의 위치와 글자 
			//위치는 조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
			currentRowPos = this->notepadForm->note->GetCurrent();
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			//5.1.6 자동 줄 바꿈 메뉴가 체크되어 있으면
			if (this->notepadForm->isRowAutoChanging == true)
			{
				//5.1.6.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
				//OnSize에서 부분자동개행을 하도록 한다. 
				this->notepadForm->SendMessage(WM_SIZE);
			}
			//5.1.7 메모장에서 선택된 texts를 다 지웠기 때문에 메모장에서 선택이 안된 상태로 바꾼다.
			this->notepadForm->isSelecting = false;
			//5.1.8 선택된 texts를 지웠기 때문에 command가 선택된 영역을 지웠다고 표시한다.
			this->isSelectedTextsRemoved = true;
			//5.1.9 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
			this->notepadForm->selectedStartXPos = 0;
			//5.1.10 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
			this->notepadForm->selectedStartYPos = 0;
			//5.1.11 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
		//5.2 붙여넣기 전에 붙여넣기가 시작되는 줄의 위치를 저장한다.
		this->startPasteRowIndex = currentRowPos;
		//5.3 붙여넣기 전에 붙여넣기가 시작되는 글자의 위치를 저장한다.
		this->startPasteLetterIndex = currentLetterPos;
		//5.4 현재 위치에 command가 가지고 있는 content를 삽입한다.
		Long rowIndex = this->notepadForm->note->
			InsertTexts(currentRowPos, currentLetterPos, this->glyph);
		//5.5 메모장의 현재 줄을 저장한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
		//5.6 자동개행이 진행중이면 붙여넣은 줄들을 자동개행시켜준다.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//5.6.1 자동개행클래스를 생성한다.
			RowAutoChange rowAutoChange(this->notepadForm);
			//5.6.2 부분자동개행을 한다.
			Long endPastedRowPos = rowAutoChange.DoPartRows(currentRowPos, rowIndex);
			//5.6.3 붙여넣기가 끝나는 줄로 이동시킨다.
			//붙여넣기가 끝나는 줄은 OnSize에서 부분자동개행을 해서 처리되기 때문에 캐럿의 위치만 조정해주면 됨!
			currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			this->notepadForm->current->Move(currentLetterPos);
		}
		//5.7 처음 실행이면
		if (this->isRedone == false)
		{
			//5.7.1 메모장 제목에 *를 추가한다.
			string name = this->notepadForm->fileName;
			name.insert(0, "*");
			name += " - 메모장";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//5.7.2 메모장에 변경사항이 있음을 저장한다.
			this->notepadForm->isDirty = true;
		}
		//5.8 글자를 지운 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
		//5.9 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			Long changedRowPos = this->rowIndex;
			Long changedLetterPos = this->letterIndex;
			Long originRowPos = 0;
			Long originLetterPos = 0;
			//5.9.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			//5.9.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
			this->rowIndex = originRowPos;
			this->letterIndex = originLetterPos;
		}
		//5.10 command에 변경사항(붙여넣은 내용)이 있음을 표시한다.
		this->isDirty = true;
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
	Long changedRowPos = 0;
	Long changedLetterPos = 0;
	Long originRowPos = this->rowIndex;
	Long originLetterPos = this->letterIndex;
	//3. 현재 줄의 위치를 이동시킨다.(캐럿이 다른 곳에 있으면 그 곳에 글자가 지워지기 때문에)
	Long currentRowPos = this->notepadForm->note->Move(this->rowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	//4. 현재 글자의 위치를 이동시킨다.
	Long currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	//5. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치가 저장되어 있음)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//5.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		//5.2 현재 줄의 위치와 글자 위치를 다시 조정한다.
		currentRowPos = this->notepadForm->note->Move(changedRowPos);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
	}
	//6. 선택이 시작되는 줄과 글자 위치, 선택이 끝나는 줄과 글자 위치를 저장한다.
	Long selectedStartRowPos = this->startPasteRowIndex;//선택이 시작되는 줄
	Long selectedStartLetterPos = this->startPasteLetterIndex;//선택이 시작되는 글자
	Long selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
	Long selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
	//7. content를 지운다.
	this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
		selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
	//8. 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.(note의연산안에서 현재 줄의 위치와 글자 위치는
	//조정이 되지만 notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
	currentRowPos = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//9. 자동 줄 바꿈 메뉴가 체크되어 있으면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//9.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
		//OnSize에서 부분자동개행을 하도록 한다. 
		this->notepadForm->SendMessage(WM_SIZE);
	}
	//10. 선택된 영역을 지웠으면
	if (this->isSelectedTextsRemoved == true)
	{
		//10.1 현재 줄의 위치를 이동시킨다.(캐럿이 다른 곳에 있으면 그 곳에 글자가 지워지기 때문에)
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//10.2 메모장에서 선택된 texts를 다시 복구하기 때문에 메모장에서 선택이 된 상태로 바꾼다.
		this->notepadForm->isSelecting = true;
		//10.3 선택이 다시 시작했기 때문에 캐럿의 x좌표를 command에 저장된 현재 글자 위치로 저장한다.
		this->notepadForm->selectedStartXPos = currentLetterPos;
		this->startPasteLetterIndex = this->notepadForm->selectedStartXPos;
		//10.4 선택이 다시 시작했기 때문에 캐럿의 y좌표를 command에 저장된 현재 줄의 위치로 저장한다.
		this->notepadForm->selectedStartYPos = currentRowPos;
		this->startPasteRowIndex = this->notepadForm->selectedStartYPos;
		//10.5 현재 위치에 command가 가지고 있는 content를 삽입한다.
		Long rowIndex = this->notepadForm->note->
			InsertTexts(currentRowPos, currentLetterPos, this->note);
		//10.6 메모장의 현재 줄을 저장한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
		//10.7 자동개행이 진행중이면 붙여넣은 줄들을 자동개행시켜준다.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//10.7.1 부분자동개행을 한다.
			Long endPastedRowPos = rowAutoChange.DoPartRows(currentRowPos, rowIndex);
			//10.7.2 붙여넣기가 끝나는 줄로 이동시킨다.
			//붙여넣기가 끝나는 줄은 OnSize에서 부분자동개행을 해서 처리되기 때문에 캐럿의 위치만 조정해주면 됨!
			currentRowPos = this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->GetCurrent();
			this->notepadForm->current->Move(currentLetterPos);
		}
		//10.8 선택영역이 다시 생겼기 때문에 복사하기, 잘라내기, 삭제 메뉴를 활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	}
	//11. 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//12. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//12.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//12.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
	}
}

//다시실행인지 여부 구하기
bool PasteCommand::IsRedone()
{
	return this->isRedone;
}

//줄의 위치 구하기
Long PasteCommand::GetRowIndex()
{
	return this->rowIndex;
}
//글자 위치 구하기
Long PasteCommand::GetLetterIndex()
{
	return this->letterIndex;
}
//선택이 시작되는 글자의 위치구하기
Long PasteCommand::GetStartPasteRowIndex()
{
	return this->startPasteRowIndex;
}

//선택이 시작되는 줄의 위치구하기
Long PasteCommand::GetStartPasteLetterIndex()
{
	return this->startPasteLetterIndex;
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