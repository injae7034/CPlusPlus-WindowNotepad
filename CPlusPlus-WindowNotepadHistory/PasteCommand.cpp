#include "PasteCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"
#include "GlyphCreator.h"
#include "TextExtent.h"
#include "RowAutoChange.h"

//디폴트생성자
PasteCommand::PasteCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	this->startRowIndex = notepadForm->note->GetCurrent();
	this->startLetterIndex = notepadForm->current->GetCurrent();
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->glyph = 0;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
	this->isSelectedTextsRemoved = false;//처음에 생성될 때는 선택영역이 안지워졌으므로 false가 디폴트값
}

//Execute
void PasteCommand::Execute()
{
	//1. RowAutoChange를 생성한다.
	RowAutoChange rowAutoChange(this->notepadForm);
	Long changedRowPos = 0;
	Long changedLetterPos = 0;
	Long originRowPos = this->rowIndex;
	Long originLetterPos = this->letterIndex;
	//선택영역이 있으면 선택영역 삭제
	//2. 메모장에서 선택된 texts가 있으면
	if (this->notepadForm->isSelecting == true)
	{
		//2.1 RemoveCommand로 메세지를 보내서 선택영역을 지운다.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
		//2.2 Command에서 선택영역이 지워졌음을 표시한다.
		this->isSelectedTextsRemoved = true;
	}
	//3. 현재 줄의 위치와 글자 위치를 구한다.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//4. PasteCommand가 다시 실행되면
	if (this->isRedone == true)
	{
		//4.1 현재 줄의 위치와 글자 위치를 다시 조정한다.
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
		//4.2 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치가 저장되어 있음)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//4.2.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
			rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
				&changedRowPos);
			//4.2.2 현재 줄의 위치와 글자 위치를 다시 조정한다.
			currentRowPos = this->notepadForm->note->Move(changedRowPos);
			this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
			currentLetterPos = this->notepadForm->current->Move(changedLetterPos);
		}
	}
	Glyph* currentRow = 0;
	Glyph* splitedRow = 0;
	Long firstCopyRowPos = 0;
	Glyph* firstCopyRow = 0;
	Glyph* letter = 0;//글자를 담을 공간
	Long letterIndex = 0;
	Long nextCopyRowPos = 0;
	Glyph* copyRow = 0;
	Long i = 0;
	bool isSplited = false;
	//5. 처음 실행이면
	if (this->isRedone == false)
	{
		//외부 클립보드에 문자열이 저장되어 있으면 외부클립보드에서 문자열을 받아와서 contents에 저장한다.
		//contents는 한줄로 문자열을 저장하는데 줄을 구분하는 것은 '\n(개행문자)'여부이다. 이 개행문자를
		//통해서 줄단위로 나눠서 저장해야한다.
		//5.1 외부 클립보드에 문자열이 저장되어 있으면
		//클립보드는 여러 프로그램들이 공유할 수 있는 기능이기 때문에 어떤 프로그램(윈도우)이 클립보드를
		//사용하는지 지정하고 사용하는 것이 좋다
		Glyph* newRow = 0;
		unsigned int priority_list = CF_TEXT;
		char* content = NULL;
		if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
		{
			//5.1.1 클립보드에 있는 문자열 정보를 얻기 위해서 클립보드를 연다.
			if (this->notepadForm->OpenClipboard())
			{
				//5.1.1.1 클립보드에서 문자열이 저장된 메모리의 핸들 값을 얻는다.
				HANDLE h_clipboard_data = GetClipboardData(CF_TEXT);
				if (h_clipboard_data != NULL)
				{
					//5.1.1.1.1 메모리 핸들 값을 이용하여 실제 사용가능한 주소를 얻는다.
					char* p_clipboard_data = (char*)::GlobalLock(h_clipboard_data);
					//5.1.1.1.2 클립보드에 저장된 문자열의 길이를 구한다. ('\0'포함 크기)
					int contentLength = strlen(p_clipboard_data) + 1;
					//5.1.1.1.3 클립보드에 저장된 문자열을 저장하기 위해 메모리를 할당한다.
					content = new char[contentLength];
					//5.1.1.1.4 할당된 메모리에 클리보드 문자열을 복사한다.
					memcpy(content, p_clipboard_data, contentLength);
					//5.1.1.1.5 문자열을 복사하기 위해서 Lock했던 메모리를 해제한다.
					GlobalUnlock(h_clipboard_data);
				}
				//5.1.1.2 클립보드를 닫는다.
				CloseClipboard();
			}
			//5.1.2 복사할 텍스트를 담을 새노트를 생성한다.
			this->glyph = new Note();
			//5.1.3 줄을 생성한다. 
			newRow = new Row();
			//5.1.4 새로 생성한 줄을 새로 생성한 노트에 추가한다.
			Long rowIndex = this->glyph->Add(newRow);
			//5.1.5 복사한 내용의 마지막까지 반복한다.(마지막에 널문자가 저장되어 있음)
			GlyphCreator glyphCreator;
			Glyph* glyph = 0;
			Long letterIndex = 0;
			char doubleByteLetter[3];
			char singleByteLetter[2];
			Long i = 0;//배열요소 초기화
			//5.1.6 문장의 마지막(NULL문자)이 아닌동안 반복한다
			while (content[i] != '\0')
			{
				//5.1.6.1 문장을 저장한 각 배열요소가 2byte 문자인지 1byte 문자인지 판단한다.
				if ((content[i] & 0x80))//한글이면(2byte문자)
				{
					//5.1.6.1.1 doubleByteLetter에 저장한다.
					doubleByteLetter[0] = content[i];
					i++;
					doubleByteLetter[1] = content[i];
					doubleByteLetter[2] = '\0';
					//5.1.6.1.2 doubleByteLetter를 생성한다.(팩토리메소드패턴)
					glyph = glyphCreator.Create((char*)doubleByteLetter);
					//5.1.6.1.3 줄에 doubleByteLetter를 추가한다.
					letterIndex = this->glyph->GetAt(rowIndex)->Add(glyph);
				}
				//5.1.6.2 개행문자이면(줄을 바꾸면 줄을 추가한다)
				else if (content[i] == '\n' || content[i] == '\r')
				{
					//5.1.6.2.1 row를 생성한다.
					glyph = glyphCreator.Create(content + i);
					//5.1.6.2.2 새로운 줄을 추가한다.
					rowIndex = this->glyph->Add(glyph);
					//5.1.6.2.3 다음 글자가 '\n'이면
					if (content[i + 1] == '\n')
					{
						i++;
					}
				}
				//5.1.6.3 입력받은 문자가 한글이 아니고 개행문자가 아니면(1byte문자)
				else
				{
					//5.1.6.3.1 singleByteLetter에 저장한다.
					singleByteLetter[0] = content[i];
					singleByteLetter[1] = '\0';
					//5.1.6.3.2 singleByteLetter를 생성한다.
					glyph = glyphCreator.Create(singleByteLetter);
					//5.1.6.3.3 줄에 singleByteLetter를 추가한다.
					letterIndex = this->glyph->GetAt(rowIndex)->Add(glyph);
				}
				i++;
			}
			//5.1.7 앞에서 content를 할당했기 때문에 여기서 할당해제를 해줘야한다.
			if (content != 0)
			{
				delete[] content;
			}
			//5.1.8 notepadForm의 clipboard에 새로 생성한 Note(복사한 내용)를 추가시켜준다.
			Long noteIndex = this->notepadForm->clipboard->Add(this->glyph->Clone());
		}
	}
	//6. 복사한 content가 있으면
	if (this->glyph != 0)
	{
		//시작하는 줄의 위치와 글자위치를 저장한다.
		this->startRowIndex = currentRowPos;
		this->startLetterIndex = currentLetterPos;
		//6. 메모장의 현재 줄을 구한다.
		currentRow = this->notepadForm->note->GetAt(currentRowPos);
		//7. 메모장의 현재 글자위치가 줄의 글자개수보다 작으면
		//뒤에 split할 글자가 있으면 split하고 마지막 글자라서 뒤에 글자가 없으면 split하지 않는다.
		splitedRow = 0;
		if (currentLetterPos < currentRow->GetLength())
		{
			//7.1 메모장의 현재 줄에서 현재 글자위치 다음부터 split시킨다.(splitedRow가 힙에 할당됨)
			splitedRow = currentRow->Split(currentLetterPos);
			//7.2 split이 되었기 때문에 isSplited을 true로 바꿔준다.
			isSplited = true;
		}
		//8. 클립보드의 현재 노트의 첫번째 줄을 구한다.
		firstCopyRowPos = 0;
		firstCopyRow = this->glyph->GetAt(firstCopyRowPos);
		letter = 0;//글자를 담을 공간
		//9. 클립보드의 현재 노트의 첫번째 줄의 개수보다 작은동안 반복한다.
		letterIndex = 0;
		while (letterIndex < firstCopyRow->GetLength())
		{
			//9.1 클립보드의 현재 노트의 첫번째 줄의 글자를 구한다.
			letter = firstCopyRow->GetAt(letterIndex);
			//9.2 글자를 메모장의 현재 줄에 추가한다.(깊은 복사)
			currentRow->Add(letter->Clone());
			//9.3 다음 글자로 이동한다.
			letterIndex++;
		}
		//10. 클립보드의 현재 노트의 첫번째 줄에서 다음 줄로 이동한다.
		nextCopyRowPos = firstCopyRowPos + 1;
		copyRow = firstCopyRow;//첫번째 줄이 디폴트
		i = currentRowPos;
		//11. 클립보드의 현재 노트(복사한 내용)의 마지막줄까지 반복한다.
		while (nextCopyRowPos < this->glyph->GetLength())
		{
			//11.1 복사한 줄을 구한다.
			copyRow = this->glyph->GetAt(nextCopyRowPos);
			//11.2 메모장의 현재 줄의 다음 줄로 이동한다.
			i++;
			//11.3 구한 줄을 메모장의 현재 줄의 위치 다음부터 끼워넣는다.(깊은 복사)
			i = this->notepadForm->note->Add(i, copyRow->Clone());
			//11.4 복사한 노트에서 다음 줄로 이동한다.
			nextCopyRowPos++;
		}
		//12. 메모장에서 현재 줄을 구한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(i);
		//13. 메모장에서 현재 글자위치를 구한다.
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//14. 메모장에서 아까 split했던 글자들이 있으면
		if (isSplited == true)
		{
			//14.1 split한 줄을 메모장의 현재 줄에 Join시킨다.
			splitedRow->Join(this->notepadForm->current);
			//14.2 splitedRow를 할당해제 시켜준다.
			if (splitedRow != 0)
			{
				delete splitedRow;
			}
			//14.3 메모장에서 현재 글자위치를 다시 조정해준다.
			this->notepadForm->current->Move(currentLetterPos);
		}
		//15. 자동개행이 진행중이면 붙여넣은 줄들을 자동개행시켜준다.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//15.1 자동개행클래스를 생성한다.
			RowAutoChange rowAutoChange(this->notepadForm);
			//15.2 부분자동개행을 한다.
			Long endPastedRowPos = rowAutoChange.DoPartRows(currentRowPos, i);
			//15.3 붙여넣기가 끝나는 줄로 이동시킨다.
			//붙여넣기가 끝나는 줄은 OnSize에서 부분자동개행을 해서 처리되기 때문에 캐럿의 위치만 조정해주면 됨!
			this->notepadForm->note->Move(endPastedRowPos);
			this->notepadForm->current->Move(currentLetterPos);
		}
		//10. 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//11. 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->isDirty = true;
		//13. 글자를 지운 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
		//14. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
		if (this->notepadForm->isRowAutoChanging == true)
		{
			Long changedRowPos = this->rowIndex;
			Long changedLetterPos = this->letterIndex;
			Long originRowPos = 0;
			Long originLetterPos = 0;
			//14.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			//14.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
			this->rowIndex = originRowPos;
			this->letterIndex = originLetterPos;

			changedRowPos = this->startRowIndex;
			changedLetterPos = this->startLetterIndex;
			originRowPos = 0;
			originLetterPos = 0;
			//14.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
			rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
				&originRowPos);
			//14.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
			this->startRowIndex = originRowPos;
			this->startLetterIndex = originLetterPos;
		}
	}
}

//실행취소
void PasteCommand::Unexecute()
{
	//1. RowAutoChange를 생성한다.
	RowAutoChange rowAutoChange(this->notepadForm);
	Long changedRowPos = 0;
	Long changedLetterPos = 0;
	Long originRowPos = this->startRowIndex;
	Long originLetterPos = this->startLetterIndex;
	//2. 현재 줄과 글자 위치를 시작위치로 이동시킨다.(캐럿이 다른 곳에 있으면 그 곳에 글자가 지워지기 때문에)
	Long currentRowPos = this->notepadForm->note->Move(this->startRowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	Long currentLetterPos = this->notepadForm->current->Move(this->startLetterIndex);
	//3. content과 끝나는 줄과 글자 위치를 구한다.
	Long endRowPos = this->rowIndex;
	Long endLetterPos = this->letterIndex;
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
		//4.3 변경된 화면의 크기에 맞게 끝나는 줄과 글자위치를 다시 구한다.
		changedRowPos = 0;
		changedLetterPos = 0;
		originRowPos = this->rowIndex;
		originLetterPos = this->letterIndex;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		endRowPos = changedRowPos;
		endLetterPos = changedLetterPos;
	}
	//5. 시작하는 줄을 구한다.
	Glyph* startRow = this->notepadForm->current;
	//6. 시작하는 줄과 끝나는 줄이 같으면
	if (currentRowPos == endRowPos)
	{
		//6.1 시작하는 글자위치부터 끝나는 글자위치까지 글자를 지운다.
		while (currentLetterPos < endLetterPos)
		{
			startRow->Remove(currentLetterPos);
			endLetterPos--;
		}
	}
	//7. 시작하는 줄과 끝나는 줄이 서로 다르면
	else
	{
		Long i = startLetterIndex;
		//7.1 시작하는 줄의 글자를 지운다.
		while (i < startRow->GetLength())
		{
			//7.1.1 글자를 지운다.
			startRow->Remove(i);
		}
		//7.2 다음 줄을 구한다.
		Long nextRowPos = currentRowPos + 1;
		//7.3 다음 줄이 끝나는 줄보다 작은 동안 반복한다.
		Glyph* row = 0;
		while (nextRowPos < endRowPos)
		{
			//7.3.1 줄을 구한다.
			row = this->notepadForm->note->GetAt(nextRowPos);
			i = 0;
			//7.3.2 줄의 글자개수동안 반복한다.
			while (i < row->GetLength())
			{
				//7.3.2.1 글자를 지운다.
				row->Remove(i);
			}
			//7.3.3 글자가 다 지워졌기 때문에 줄을 지운다.
			this->notepadForm->note->Remove(nextRowPos);
			//7.3.4 앞의 줄이 지워졌기 때문에 끝다는 줄의 위치를 한 줄 감소시킨다.
			endRowPos--;
		}
		//7.4 끝나는 줄을 구한다.
		Glyph* endRow = this->notepadForm->note->GetAt(endRowPos);
		//7.5 끝나는 줄의 처음 글자부타 끝나는 글자위치까지 반복한다.
		i = 0;
		while (i < endLetterPos)
		{
			//7.5.1 글자를 지운다.
			endRow->Remove(i);
			endLetterPos--;
		}
		//7.6 끝나는 줄을 시작하는 줄에 Join시킨다.
		endRow->Join(startRow);
		//7.7 끝나는 줄을 노트에서 지운다.
		this->notepadForm->note->Remove(endRowPos);
		//7.8 글자 위치를 조정시킨다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->Move(startLetterIndex);
	}
	
	//12. 자동 줄 바꿈 메뉴가 체크되어 있으면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//12.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
		//OnSize에서 부분자동개행을 하도록 한다. 
		this->notepadForm->SendMessage(WM_SIZE);
	}
	//14. 글자를 입력한 후에 command의 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->letterIndex = this->notepadForm->current->GetCurrent();
	this->startRowIndex = this->rowIndex;
	this->startLetterIndex = this->letterIndex;
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
		this->startRowIndex = this->rowIndex;
		this->startLetterIndex = this->letterIndex;
	}
}

//다시실행인지 여부 구하기
bool PasteCommand::IsRedone()
{
	return this->isRedone;
}

//시작하는 줄의 위치구하기
Long PasteCommand::GetStartRowIndex()
{
	return this->startRowIndex;
}

//시작하는 글자의 위치구하기
Long PasteCommand::GetStartLetterIndex()
{
	return this->startLetterIndex;
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
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}