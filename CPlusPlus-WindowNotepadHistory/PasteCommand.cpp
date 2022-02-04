#include "PasteCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"
#include "GlyphCreator.h"
#include "TextExtent.h"

//디폴트생성자
PasteCommand::PasteCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void PasteCommand::Execute()
{
	//1. 메모장의 현재 줄과 글자의 위치를 구한다.
	Long currentRowPos = 0;
	Long currentLetterPos = 0;
	//2. 메모장에서 선택된 texts가 있으면
	if (this->notepadForm->isSelecting == true)
	{
		currentRowPos = this->notepadForm->note->GetCurrent();
		currentLetterPos = this->notepadForm->current->GetCurrent();
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
		//2.1 선택이 시작되는 줄과 선택이 끝나는 줄이 같으면
		//(한 줄 내에서만 선택이 되어 있으므로 줄을 지워지지 않고 글자들만 지워짐)
		if (selectedStartRowPos == selectedEndRowPos)
		{
			//2.1.1 선택이 오른쪽으로 진행되었으면
			if (selectedStartLetterPos < selectedEndLetterPos)
			{
				//2.1.1.1 시작하는 글자위치를 선택이 시작되는 글자위치로 정한다.
				startLetterIndex = selectedStartLetterPos;
				//2.1.1.2 끝나는 글자위치를 선택이 끝나는 글자위치로 정한다.
				endLetterIndex = selectedEndLetterPos;
				//2.1.1.3 시작하는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
				startRowIndex = selectedStartRowPos;

			}
			//2.1.2 선택이 왼쪽으로 진행되었으면
			else
			{
				//2.1.2.1 시작하는 글자위치를 선택이 끝나는 글자위치로 정한다.
				startLetterIndex = selectedEndLetterPos;
				//2.1.2.2 끝나는 글자위치를 선택이 시작하는 글자위치로 정한다.
				endLetterIndex = selectedStartLetterPos;
				//2.1.2.3 시작하는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
				startRowIndex = selectedEndRowPos;
			}
			//2.1.3 시작하는 줄을 구한다.
			startRow = this->notepadForm->note->GetAt(startRowIndex);
			//2.1.4 시작하는 글자위치부터 끝나는 글자까지 지운다.
			while (startLetterIndex < endLetterIndex)
			{
				//2.1.4.1 줄에서 글자를 지운다.
				startRow->Remove(startLetterIndex);
				//2.1.4.2 줄에서 글자가 지워지면 줄의 개수가 줄고 시작하는 글자의 다음 글자가
				//선택이 시작하는 글자의 위치로 앞당겨져 오게 되므로 선택이 끝나는 줄의 값을 감소시킨다. 
				endLetterIndex--;
			}
		}
		//2.2 선택이 시작되는 줄과 선택이 끝나는 줄이 서로 다르면
		//(선택이 여러 줄에 걸쳐서 되어 있기 때문에 글자가 다 지워진 줄은 지워져야함)
		else
		{
			//2.2.1 선택이 오른쪽으로 진행되었으면 
			if (selectedStartRowPos < selectedEndRowPos)
			{
				//2.2.1.1 시작하는 글자위치를 선택이 시작되는 글자위치로 정한다.
				startLetterIndex = selectedStartLetterPos;
				//2.2.1.2 끝나는 글자위치를 선택이 끝나는 글자위치로 정한다.
				endLetterIndex = selectedEndLetterPos;
				//2.2.1.3 시작하는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
				startRowIndex = selectedStartRowPos;
				//2.2.1.4 끝나는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
				endRowIndex = selectedEndRowPos;
			}
			//2.2.2 선택이 왼쪽으로 진행되었으면
			else
			{
				//2.2.2.1 시작하는 글자위치를 선택이 끝나는 글자위치로 정한다.
				startLetterIndex = selectedEndLetterPos;
				//2.2.2.2 끝나는 글자위치를 선택이 시작하는 글자위치로 정한다.
				endLetterIndex = selectedStartLetterPos;
				//2.2.2.3 시작하는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
				startRowIndex = selectedEndRowPos;
				//2.2.2.4 끝나는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
				endRowIndex = selectedStartRowPos;
			}
			Glyph* endRow = 0;//끝나는 줄의 위치
			Glyph* row = 0;//줄의 주소를 담을 공간
			Long letterIndex = 0;//글자 위치
			//2.2.3 시작하는 줄을 구한다.
			startRow = this->notepadForm->note->GetAt(startRowIndex);
			//2.2.4 시작하는 글자위치부터 시작하는 줄의 마지막 글자까지 지운다.
			while (startLetterIndex < startRow->GetLength())
			{
				//2.2.4.1 줄에서 글자를 지운다.
				startRow->Remove(startLetterIndex);
			}
			//2.2.5 시작하는 줄의 다음줄부터 끝나는 줄전까지 글자와 줄을 지운다.
			Long nextRowIndex = startRowIndex + 1;
			while (nextRowIndex < endRowIndex)
			{
				//2.2.5.1 줄을 구한다.
				row = this->notepadForm->note->GetAt(nextRowIndex);
				//2.2.5.2 글자위치를 원위치시킨다.
				letterIndex = 0;
				//2.2.5.3 줄에서 마지막 글자까지 반복한다.
				while (letterIndex < row->GetLength())
				{
					//2.2.5.3.1 줄의 글자를 지운다.
					row->Remove(letterIndex);
				}
				//2.2.5.4 줄의 글자를 다지웠기때문에 메모장에서 줄을 지운다.
				this->notepadForm->note->Remove(nextRowIndex);
				//2.2.5.5 줄을 지웠기 때문에 선택이 끝나는 줄의 위치가 한칸 앞당겨진다.
				endRowIndex--;
			}
			//2.2.6 끝나는 줄을 구한다.
			endRow = this->notepadForm->note->GetAt(endRowIndex);
			//2.2.7 끝나는 줄의 처음부터 끝나는 글자까지 글자를 지운다.
			letterIndex = 0;
			while (letterIndex < endLetterIndex)
			{
				//2.2.7.1 끝나는 줄의 글자를 지운다.
				endRow->Remove(letterIndex);
				//2.2.7.2 끝나는 줄의 첫글자를 지우면 다음 글자부터 앞으로 한칸씩
				//당겨지기 때문에 끝나는 글자위치를 -1 감소시킨다.
				endLetterIndex--;
			}
			//2.2.8 끝나는 줄을 시작하는 줄로 Join시킨다.
			endRow->Join(startRow);
			//2.2.9 끝나는 줄이 시작하는 줄로 Join되었기 때문에
			//끝나는 줄을 메모장에서 지운다.
			this->notepadForm->note->Remove(endRowIndex);
			//2.2.10 현재 줄의 위치를 시작하는 줄의 위치로 변경한다.
			this->notepadForm->current = this->notepadForm->note->
				GetAt(startRowIndex);
			//2.2.11 현재 글자의 위치를 시작하는 글자의 위치로 변경한다.
			this->notepadForm->current->Move(startLetterIndex);
		}
		//2.3 메모장에서 선택된 texts를 다 지웠기 때문에 메모장에서 선택이 안된 상태로 바꾼다.
		this->notepadForm->isSelecting = false;
		//2.4 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
		this->notepadForm->selectedStartXPos = 0;
		//2.5 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
		this->notepadForm->selectedStartYPos = 0;
	}
	//3. 선택한 texts를 다 지웠으면(없으면)
	if (this->notepadForm->isSelecting == false)
	{
		//3.1 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}

	//외부 클립보드에 문자열이 저장되어 있으면 외부클립보드에서 문자열을 받아와서 contents에 저장한다.
	//contents는 한줄로 문자열을 저장하는데 줄을 구분하는 것은 '\n(개행문자)'여부이다. 이 개행문자를
	//통해서 줄단위로 나눠서 저장해야한다.
	//4. 외부 클립보드에 문자열이 저장되어 있으면
	//클립보드는 여러 프로그램들이 공유할 수 있는 기능이기 때문에 어떤 프로그램(윈도우)이 클립보드를
	//사용하는지 지정하고 사용하는 것이 좋다
	unsigned int priority_list = CF_TEXT;
	char* content = NULL;
	if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
	{
		//4.1 클립보드에 있는 문자열 정보를 얻기 위해서 클립보드를 연다.
		if (this->notepadForm->OpenClipboard())
		{
			//4.1.1 클립보드에서 문자열이 저장된 메모리의 핸들 값을 얻는다.
			HANDLE h_clipboard_data = GetClipboardData(CF_TEXT);
			if (h_clipboard_data != NULL)
			{
				//4.1.1.1 메모리 핸들 값을 이용하여 실제 사용가능한 주소를 얻는다.
				char* p_clipboard_data = (char*)::GlobalLock(h_clipboard_data);
				//4.1.1.2 클립보드에 저장된 문자열의 길이를 구한다. ('\0'포함 크기)
				int contentLength = strlen(p_clipboard_data) + 1;
				//4.1.1.3 클립보드에 저장된 문자열을 저장하기 위해 메모리를 할당한다.
				content = new char[contentLength];
				//4.1.1.4 할당된 메모리에 클리보드 문자열을 복사한다.
				memcpy(content, p_clipboard_data, contentLength);
				//4.1.1.5 문자열을 복사하기 위해서 Lock했던 메모리를 해제한다.
				GlobalUnlock(h_clipboard_data);
			}
			//4.1.2 클립보드를 닫는다.
			CloseClipboard();
		}
		//4.2 복사할 텍스트를 담을 새노트를 생성한다.
		Glyph* copyNote = new Note();
		//4.3 줄을 생성한다. 
		Glyph* copyRow = new Row();
		//4.4 새로 생성한 줄을 새로 생성한 노트에 추가한다.
		Long rowIndex = copyNote->Add(copyRow);
		//4.5 복사한 내용의 마지막까지 반복한다.(마지막에 널문자가 저장되어 있음)
		GlyphCreator glyphCreator;
		Glyph* glyph = 0;
		Long letterIndex = 0;
		char doubleByteLetter[3];
		char singleByteLetter[2];
		Long i = 0;//배열요소 초기화
		//4.6 문장의 마지막(NULL문자)이 아닌동안 반복한다
		while (content[i] != '\0')
		{
			//4.6.1 문장을 저장한 각 배열요소가 2byte 문자인지 1byte 문자인지 판단한다.
			if ((content[i] & 0x80))//한글이면(2byte문자)
			{
				//4.6.1.1 doubleByteLetter에 저장한다.
				doubleByteLetter[0] = content[i];
				i++;
				doubleByteLetter[1] = content[i];
				doubleByteLetter[2] = '\0';
				//4.6.1.2 doubleByteLetter를 생성한다.(팩토리메소드패턴)
				glyph = glyphCreator.Create((char*)doubleByteLetter);
				//4.6.1.3 줄에 doubleByteLetter를 추가한다.
				letterIndex = copyNote->GetAt(rowIndex)->Add(glyph);
			}
			//4.6.2 개행문자이면(줄을 바꾸면 줄을 추가한다)
			else if (content[i] == '\n' || content[i] == '\r')
			{
				//4.6.2.1 row를 생성한다.
				glyph = glyphCreator.Create(content + i);
				//4.6.2.2 새로운 줄을 추가한다.
				rowIndex = copyNote->Add(glyph);
				//4.6.2.3 다음 글자가 '\n'이면
				if (content[i + 1] == '\n')
				{
					i++;
				}
			}
			//4.6.3 입력받은 문자가 한글이 아니고 개행문자가 아니면(1byte문자)
			else
			{
				//4.6.3.1 singleByteLetter에 저장한다.
				singleByteLetter[0] = content[i];
				singleByteLetter[1] = '\0';
				//4.6.3.2 singleByteLetter를 생성한다.
				glyph = glyphCreator.Create(singleByteLetter);
				//4.6.3.3 줄에 singleByteLetter를 추가한다.
				letterIndex = copyNote->GetAt(rowIndex)->Add(glyph);
			}
			i++;
		}
		//4.7 앞에서 content를 할당했기 때문에 여기서 할당해제를 해줘야한다.
		if (content != 0)
		{
			delete[] content;
		}
		//4.8 notepadForm의 clipboard에 새로 생성한 Note(복사한 내용)를 추가시켜준다.
		Long noteIndex = this->notepadForm->clipboard->Add(copyNote);
	}
	//4. 메모장의 클립보드에서 현재 노트(복사한 내용)를 호출한다.
	Glyph* copyNote = this->notepadForm->clipboard->
		GetAt(this->notepadForm->clipboard->GetCurrent());
	//5. 선택된 texts들을 지웠으면 현재 줄과 글자위치가 변경되기 때문에 다시 구해준다.
	currentRowPos = this->notepadForm->note->GetCurrent();
	currentLetterPos = this->notepadForm->current->GetCurrent();
	//6. 메모장의 현재 줄을 구한다.
	Glyph* currentRow = this->notepadForm->note->GetAt(currentRowPos);
	//7. 메모장의 현재 글자위치가 줄의 글자개수보다 작으면
	//뒤에 split할 글자가 있으면 split하고 마지막 글자라서 뒤에 글자가 없으면 split하지 않는다.
	Glyph* splitedRow = 0;
	bool isSplited = false;
	if (currentLetterPos < currentRow->GetLength())
	{
		//7.1 메모장의 현재 줄에서 현재 글자위치 다음부터 split시킨다.(splitedRow가 힙에 할당됨)
		splitedRow = currentRow->Split(currentLetterPos);
		//7.2 split이 되었기 때문에 isSplited을 true로 바꿔준다.
		isSplited = true;
	}
	//8. 클립보드의 현재 노트의 첫번째 줄을 구한다.
	Long firstCopyRowPos = 0;
	Glyph* firstCopyRow = copyNote->GetAt(firstCopyRowPos);
	Glyph* letter = 0;//글자를 담을 공간
	//9. 클립보드의 현재 노트의 첫번째 줄의 개수보다 작은동안 반복한다.
	Long letterIndex = 0;
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
	Long nextCopyRowPos = firstCopyRowPos + 1;
	Glyph* copyRow = firstCopyRow;//첫번째 줄이 디폴트
	Long i = currentRowPos;
	//11. 클립보드의 현재 노트(복사한 내용)의 마지막줄까지 반복한다.
	while (nextCopyRowPos < copyNote->GetLength())
	{
		//11.1 복사한 줄을 구한다.
		copyRow = copyNote->GetAt(nextCopyRowPos);
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
		Long letterIndex = 0;
		Long rowTextWidth = 0;
		Glyph* glyph = 0;
		Long startPastedRowPos = currentRowPos;
		Long endPastedRowPos = i;
		Glyph* pastedRow = 0;
		//15.1 현재 화면의 크기를 구한다.
		CRect rect;
		this->notepadForm->GetClientRect(&rect);
		//15.2 현재 화면의 가로 길이를 구한다.
		Long pageWidth = rect.Width();
		//15.3 붙여넣은 줄 전까지 반복한다.
		while (startPastedRowPos < endPastedRowPos)
		{
			//15.3.1 메모장에서 붙여넣은 줄을 구한다.
			pastedRow = this->notepadForm->note->GetAt(startPastedRowPos);
			//15.3.2 letterIndex를 원위치시킨다.
			letterIndex = 0;
			//15.3.3 rowTextWidth를 원위치시킨다.
			rowTextWidth = 0;
			//15.3.4 letterIndex가 붙여넣은 줄의 총글자 개수보다 작은동안 
			//그리고 붙여넣은 줄의 가로길이가 현재화면의 가로길이보다 작은동안 반복한다.
			while (letterIndex < pastedRow->GetLength() && rowTextWidth < pageWidth)
			{
				//15.3.4.1 증가된 letterIndex까지의 가로 길이를 측정한다.
				rowTextWidth = this->notepadForm->textExtent->GetTextWidth
				(pastedRow->GetPartOfContent(letterIndex + 1));
				//15.3.4.2 letterIndex를 증가시킨다.
				letterIndex++;
			}
			//15.3.5 붙여넣은 줄의 가로 길이가 현재 화면의 가로 길이보다 크거나 같으면
			if (rowTextWidth >= pageWidth)
			{
				//15.3.5.1 letterIndex까지의 길이가 현재화면의 가로 길이(cx)보다 크기 때문에 
				//이 선택문에 들어왔다. 그래서 캐럿이 이전으로 한 칸 이동을 해서 길이를 재면
				//현재화면의 가로 길이(cx)보다 작다. 캐럿(letterIndex)은 다음 글자를 적을 위치를
				//반영하기 때문에 항상 현재 글자보다 한칸 앞서 있다
				//그래서 letterIndex-1에서 split을 해야 화면을 넘는 글자를 다음 줄로 보낼 수 있다.
				letterIndex--;
				//15.3.5.2 붙여넣은 줄의 가로 길이가 현재화면의 가로 길이보다 커진 시점의 글자부터 
				//붙여넣은 줄에서 letterIndex 다음 위치에 있는 글자들을 나눈다.(DummyRow생성)
				glyph = pastedRow->Split(letterIndex, true);
				//15.3.5.3 split해서 생성된 새로운 줄(DummyRow)을 
				//메모장의 노트에 splited된 줄 다음에 끼워넣는다. 
				startPastedRowPos = this->notepadForm->note->Add(startPastedRowPos + 1, glyph);
				//15.3.5.4 메모장의 노트에 줄이 추가되었기 때문에 줄의 개수가 증가했으므로 
				//붙여넣기가 끝나는 줄의 위치도 한 줄 증가시켜준다.
				endPastedRowPos++;
			}
			//15.3.6 letterIndex가 붙여넣기한 줄의 총글자 개수보다 크거나 같으면
			//붙여넣기 한 줄의 전체글자까지의 길이가 현재화면보다 작으면
			else if (letterIndex >= pastedRow->GetLength())
			{
				//15.3.6.1 다음 붙여넣기 한 줄로 이동한다.
				//줄이 메모장의 노트에 추가되지 않았기 때문에 노트의 줄개수가 늘어나지 않는다.
				//그래서 여기서는 붙여넣기가 끝나는 줄의 위치를 증가시켜줄 필요가 없다.
				startPastedRowPos++;
			}
		}
		//15.4 붙여넣기가 끝나는 줄로 이동시킨다.
		//붙여넣기가 끝나는 줄은 OnSize에서 부분자동개행을 해서 처리되기 때문에 캐럿의 위치만 조정해주면 됨!
		this->notepadForm->note->Move(endPastedRowPos);
		this->notepadForm->current->Move(currentLetterPos);
	}
	//16. 메모장 제목에 *를 추가한다.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//17. 메모장에 변경사항이 있음을 저장한다.
	this->notepadForm->isDirty = true;
}

//소멸자
PasteCommand::~PasteCommand()
{

}