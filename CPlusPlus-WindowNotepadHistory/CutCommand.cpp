#include "CutCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "DummyRow.h"

//디폴트생성자
CutCommand::CutCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void CutCommand::Execute()
{
	//복사하기 Command에 들어왔다는 말은 최소 한글자이상은 복사할게 있어서 들어왔다는 뜻임
	//복사할게 없었으면 애초에 여기에 들어올 수 없음 그러므로 복사하기의 Execute에서는
	//노트를 생성하고 줄을 한 개 생성해서 새로 생성한 노트에 추가까지 하는게 디폴트이다.
	//1. 복사할 텍스트를 담을 새노트를 생성한다.
	Glyph* copyNote = new Note();
	//2. 줄을 생성한다.(복사한 내용이 한 글자이든 수천줄이든 일단 무조건 줄은 하나 생성해야함.)
	//그리고 복사를 시작한 줄이 가짜줄이든 진짜줄이든 상관없이 처음 복사한 줄은 무조건 Row로 생성함. 
	Glyph* copyRow = new Row();
	//3. 새로 생성한 줄을 클립보드의 새로 생성한 노트에 추가한다.
	Long rowIndexOfCopyNote = copyNote->Add(copyRow);
	//4. 메모장에서 현재 줄과 글자의 위치를 구한다.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//5. 선택이 시작되는 줄과 글자, 선택이 끝나는 줄과 글자의 위치를 구한다.
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
		//7.1.4 시작하는 글자위치부터 끝나는 글자까지 복사한다.
		i = startLetterIndex;
		while (i < endLetterIndex)
		{
			//7.1.4.1 시작하는 줄에서 줄에서 글자를 구한다.
			letter = startRow->GetAt(i);
			//7.1.4.2 클립보드의 새로 만든 줄(복사하는 줄)에 글자를 추가한다.
			copyLetterIndex = copyRow->Add(letter->Clone());
			//7.1.4.3 메모장의 글자들은 선택이 되어 있는 상태로 남아 있어야 하지만
			//클립보드에 옮겨지는 글자들은 선택이 안된 상태로 바꿔줘여함(왜냐하면 메모장에서
			//선택이 된 상태로 복사가 되었기 때문에 일단 클립보드에 선택이 된 상태로 복사하고
			//나중에 따로 선택이 안된 상태로 바꿔줘야 붙여넣기를 할 때 선택이 안된 글자들이 붙여넣어짐)
			copyRow->GetAt(copyLetterIndex)->Select(false);
			//7.1.4.4 시작하는 글자의 다음 글자로 이동해서 글자를 복사한다.
			i++;
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
		//8.3 시작하는 줄을 구한다.
		startRow = this->notepadForm->note->GetAt(startRowIndex);
		//8.4 시작하는 글자위치부터 시작하는 줄의 마지막 글자까지 복사한다.
		i = startLetterIndex;
		while (i < startRow->GetLength())
		{
			//8.4.1 시작하는 줄에서 글자를 구한다.
			letter = startRow->GetAt(i);
			//8.4.2 클립보드의 새로 만든 줄(복사하는 줄)에 글자를 추가한다.
			copyLetterIndex = copyRow->Add(letter->Clone());
			//8.4.3 메모장의 글자들은 선택이 되어 있는 상태로 남아 있어야 하지만
			//클립보드에 옮겨지는 글자들은 선택이 안된 상태로 바꿔줘여함(왜냐하면 메모장에서
			//선택이 된 상태로 복사가 되었기 때문에 일단 클립보드에 선택이 된 상태로 복사하고
			//나중에 따로 선택이 안된 상태로 바꿔줘야 붙여넣기를 할 때 선택이 안된 글자들이 붙여넣어짐)
			copyRow->GetAt(copyLetterIndex)->Select(false);
			//8.4.4 시작하는 글자의 다음 글자로 이동해서 글자를 복사한다.
			i++;
		}
		//8.5 시작하는 줄의 다음줄로 이동한다.
		Long nextRowIndex = startRowIndex + 1;
		Glyph* realRow = copyNote->GetAt(rowIndexOfCopyNote);//진짜 줄의 주소를 담을 공간
		//8.6 다음줄부터 끝나는 줄전까지 반복한다.
		while (nextRowIndex < endRowIndex)
		{
			//8.6.1 메모장의 줄을 구한다.
			row = this->notepadForm->note->GetAt(nextRowIndex);
			//8.6.2 메모장의 줄이 진짜이면
			if (!dynamic_cast<DummyRow*>(row))
			{
				//8.6.2.1 진짜 줄을 생성한다.
				realRow = new Row();
				//8.6.2.2 진짜 줄을 새로 만든 노트에 추가한다.
				rowIndexOfCopyNote = copyNote->Add(realRow);
			}
			//8.6.3 메모장의 진짜 줄의 처음부터 마지막 글자까지 반복한다.
			letterIndex = 0;
			while (letterIndex < row->GetLength())
			{
				//8.6.3.1 메모장의 진짜 줄에서 글자를 구한다.
				letter = row->GetAt(letterIndex);
				//8.6.3.2 클립보드의 새로 만든 줄(복사하는 줄)에 글자를 추가한다.
				copyLetterIndex = realRow->Add(letter->Clone());
				//8.6.3.3 메모장의 글자들은 선택이 되어 있는 상태로 남아 있어야 하지만
				//클립보드에 옮겨지는 글자들은 선택이 안된 상태로 바꿔줘여함(왜냐하면 메모장에서
				//선택이 된 상태로 복사가 되었기 때문에 일단 클립보드에 선택이 된 상태로 복사하고
				//나중에 따로 선택이 안된 상태로 바꿔줘야 붙여넣기를 할 때 선택이 안된 글자들이 붙여넣어짐)
				realRow->GetAt(copyLetterIndex)->Select(false);
				//8.6.3.4 다음 글자로 이동해서 글자를 복사한다..
				letterIndex++;
			}
			//8.6.4 다음 줄로 이동한다.
			nextRowIndex++;
		}
		//8.7 메모장에서 끝나는 줄을 구한다.
		endRow = this->notepadForm->note->GetAt(nextRowIndex);
		//8.8 메모장에서 끝나는 줄이 진짜줄이면
		if (!dynamic_cast<DummyRow*>(endRow))
		{
			//8.8.1 진짜 줄을 생성한다.
			realRow = new Row();
			//8.8.2 진짜 줄을 새로 만든 노트에 추가한다.
			rowIndexOfCopyNote = copyNote->Add(realRow);
		}
		//새로운 노트의 진짜 줄에 메모장의 가짜줄의 글자를 복사한다.
		//8.9 글자 위치를 원위치시킨다.
		letterIndex = 0;
		//8.10 메모장의 가짜줄에서 마지막 글자까지 반복한다.
		while (letterIndex < endLetterIndex)
		{
			//8.10.1 메모장의 가짜줄에서 글자를 구한다.
			letter = endRow->GetAt(letterIndex);
			//8.10.2 클립보드의 새로 만든 줄(복사하는 줄)에 글자를 추가한다.
			copyLetterIndex = realRow->Add(letter->Clone());
			//8.10.3 메모장의 글자들은 선택이 되어 있는 상태로 남아 있어야 하지만
			//클립보드에 옮겨지는 글자들은 선택이 안된 상태로 바꿔줘여함(왜냐하면 메모장에서
			//선택이 된 상태로 복사가 되었기 때문에 일단 클립보드에 선택이 된 상태로 복사하고
			//나중에 따로 선택이 안된 상태로 바꿔줘야 붙여넣기를 할 때 선택이 안된 글자들이 붙여넣어짐)
			realRow->GetAt(copyLetterIndex)->Select(false);
			//8.10.4 다음 글자로 이동해서 글자를 복사한다.
			letterIndex++;
		}
	}
	//9. notepadForm의 clipboard에 새로 생성한 Note(복사한 내용)를 추가시켜준다.
	Long noteIndex = this->notepadForm->clipboard->Add(copyNote);
	//10. 복사한 texts가 생겼기 때문에 붙여넣기 메뉴를 활성화시켜준다.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_PASTE, MF_BYCOMMAND | MF_ENABLED);


	//내부클립보드에 복사한 내용을 외부클립보드로 옮기기
	//11. notepadForm의 clipboard에 새로 생성한 Note(복사한 내용)와 그 길이를 구한다.
	CString content = this->notepadForm->clipboard->
		GetAt(noteIndex)->GetContent().c_str();
	//저장할 문자열의 길이를 구한다. ('\0'까지 포함한 크기)
	int contentLength = content.GetLength() + 1;
	//12. 외부 클립보드가 열렸으면
	//클립보드는 여러 프로그램들이 공유할 수 있는 기능이기 때문에 어떤 프로그램(윈도우)이 클립보드를
	//사용하는지 지정하고 사용하는 것이 좋다
	if (this->notepadForm->OpenClipboard() != 0)
	{
		//12.1 notepadForm의 clipboard에 새로 생성한 Note(복사한 내용)을 클립보드에 옮길 공간을 생성
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

	currentRowPos = this->notepadForm->note->GetCurrent();
	currentLetterPos = this->notepadForm->current->GetCurrent();
	selectedStartRowPos = this->notepadForm->selectedStartYPos;//선택이 시작되는 줄
	selectedStartLetterPos = this->notepadForm->selectedStartXPos;//선택이 시작되는 글자
	selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
	selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
	//시작은 무조건 오른쪽방향임
	startRowIndex = 0;//시작하는 줄의 위치
	startLetterIndex = 0;//시작하는 글자 위치
	endRowIndex = 0;//끝나는 줄의 위치
	endLetterIndex = 0;//끝나는 글자 위치
	startRow = 0;//시작하는 줄의 위치
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

	//3.1 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	//15. 메모장 제목에 *를 추가한다.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//16. 메모장에 변경사항이 있음을 저장한다.
	this->notepadForm->isDirty = true;
}

//소멸자
CutCommand::~CutCommand()
{

}