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
		//2.1 RemoveCommand로 메세지를 보내서 선택영역을 지운다.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
	}
	//외부 클립보드에 문자열이 저장되어 있으면 외부클립보드에서 문자열을 받아와서 contents에 저장한다.
	//contents는 한줄로 문자열을 저장하는데 줄을 구분하는 것은 '\n(개행문자)'여부이다. 이 개행문자를
	//통해서 줄단위로 나눠서 저장해야한다.
	//3. 외부 클립보드에 문자열이 저장되어 있으면
	//클립보드는 여러 프로그램들이 공유할 수 있는 기능이기 때문에 어떤 프로그램(윈도우)이 클립보드를
	//사용하는지 지정하고 사용하는 것이 좋다
	unsigned int priority_list = CF_TEXT;
	char* content = NULL;
	if (GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
	{
		//3.1 클립보드에 있는 문자열 정보를 얻기 위해서 클립보드를 연다.
		if (this->notepadForm->OpenClipboard())
		{
			//3.1.1 클립보드에서 문자열이 저장된 메모리의 핸들 값을 얻는다.
			HANDLE h_clipboard_data = GetClipboardData(CF_TEXT);
			if (h_clipboard_data != NULL)
			{
				//3.1.1.1 메모리 핸들 값을 이용하여 실제 사용가능한 주소를 얻는다.
				char* p_clipboard_data = (char*)::GlobalLock(h_clipboard_data);
				//3.1.1.2 클립보드에 저장된 문자열의 길이를 구한다. ('\0'포함 크기)
				int contentLength = strlen(p_clipboard_data) + 1;
				//3.1.1.3 클립보드에 저장된 문자열을 저장하기 위해 메모리를 할당한다.
				content = new char[contentLength];
				//3.1.1.4 할당된 메모리에 클리보드 문자열을 복사한다.
				memcpy(content, p_clipboard_data, contentLength);
				//3.1.1.5 문자열을 복사하기 위해서 Lock했던 메모리를 해제한다.
				GlobalUnlock(h_clipboard_data);
			}
			//3.1.2 클립보드를 닫는다.
			CloseClipboard();
		}
		//3.2 복사할 텍스트를 담을 새노트를 생성한다.
		Glyph* copyNote = new Note();
		//3.3 줄을 생성한다. 
		Glyph* copyRow = new Row();
		//3.4 새로 생성한 줄을 새로 생성한 노트에 추가한다.
		Long rowIndex = copyNote->Add(copyRow);
		//3.5 복사한 내용의 마지막까지 반복한다.(마지막에 널문자가 저장되어 있음)
		GlyphCreator glyphCreator;
		Glyph* glyph = 0;
		Long letterIndex = 0;
		char doubleByteLetter[3];
		char singleByteLetter[2];
		Long i = 0;//배열요소 초기화
		//3.6 문장의 마지막(NULL문자)이 아닌동안 반복한다
		while (content[i] != '\0')
		{
			//3.6.1 문장을 저장한 각 배열요소가 2byte 문자인지 1byte 문자인지 판단한다.
			if ((content[i] & 0x80))//한글이면(2byte문자)
			{
				//3.6.1.1 doubleByteLetter에 저장한다.
				doubleByteLetter[0] = content[i];
				i++;
				doubleByteLetter[1] = content[i];
				doubleByteLetter[2] = '\0';
				//3.6.1.2 doubleByteLetter를 생성한다.(팩토리메소드패턴)
				glyph = glyphCreator.Create((char*)doubleByteLetter);
				//3.6.1.3 줄에 doubleByteLetter를 추가한다.
				letterIndex = copyNote->GetAt(rowIndex)->Add(glyph);
			}
			//3.6.2 개행문자이면(줄을 바꾸면 줄을 추가한다)
			else if (content[i] == '\n' || content[i] == '\r')
			{
				//3.6.2.1 row를 생성한다.
				glyph = glyphCreator.Create(content + i);
				//3.6.2.2 새로운 줄을 추가한다.
				rowIndex = copyNote->Add(glyph);
				//3.6.2.3 다음 글자가 '\n'이면
				if (content[i + 1] == '\n')
				{
					i++;
				}
			}
			//3.6.3 입력받은 문자가 한글이 아니고 개행문자가 아니면(1byte문자)
			else
			{
				//3.6.3.1 singleByteLetter에 저장한다.
				singleByteLetter[0] = content[i];
				singleByteLetter[1] = '\0';
				//3.6.3.2 singleByteLetter를 생성한다.
				glyph = glyphCreator.Create(singleByteLetter);
				//3.6.3.3 줄에 singleByteLetter를 추가한다.
				letterIndex = copyNote->GetAt(rowIndex)->Add(glyph);
			}
			i++;
		}
		//3.7 앞에서 content를 할당했기 때문에 여기서 할당해제를 해줘야한다.
		if (content != 0)
		{
			delete[] content;
		}
		//3.8 notepadForm의 clipboard에 새로 생성한 Note(복사한 내용)를 추가시켜준다.
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
		//15.1 자동개행클래스를 생성한다.
		RowAutoChange rowAutoChange(this->notepadForm);
		//15.2 부분자동개행을 한다.
		Long endPastedRowPos = rowAutoChange.DoPartRows(currentRowPos, i);
		//15.3 붙여넣기가 끝나는 줄로 이동시킨다.
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