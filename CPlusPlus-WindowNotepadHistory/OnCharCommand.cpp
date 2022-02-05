#include "OnCharCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "Row.h"
#include "RowAutoChange.h"
#include "DummyRow.h"
#include "TextExtent.h"

//디폴트생성자 정의
OnCharCommand::OnCharCommand(NotepadForm* notepadForm, Glyph* glyph)
	:Command(notepadForm)
{
	this->glyph = glyph;//NotepadForm의 OnChar에서 생성받은 Glyph를 저장함.
	this->rowIndex = notepadForm->note->GetCurrent();
	this->letterIndex = notepadForm->current->GetCurrent();
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
}

//Execute 정의
void OnCharCommand::Execute()
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
	}
	//3. 현재 줄의 위치와 글자 위치를 구한다.
	Long currentRowPos = this->notepadForm->note->GetCurrent();
	Long currentLetterPos = this->notepadForm->current->GetCurrent();
	//4. OnCharCommand가 다시 실행되면
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
	//NotepadForm의 OnChar에서 생성된 글자를 가져와서 노트와 줄에 추가함
	//5. 입력받은 문자가 개행문자가 아니면
	if (!dynamic_cast<Row*>(this->glyph))
	{
		//5.1 현재 줄의 글자 위치가 현재 줄의 글자개수와 같으면
		if (currentLetterPos == this->notepadForm->current->GetLength())
		{
			//5.1.1 현재 줄의 마지막 글자 뒤에 새로운 글자를 추가한다.
			//여기서 command가 가지고 있는 glyph를 깊은 복사해서 줄에 추가해준다.
			//Command의 glyph는 note와는 따로 가지고 있어야 나중에 에러가 안나고 할당해제하기 편하다.
			//그리고 glyph의 원래 주인은 command이기 때문에 note가 깊은 복사를 해서 가져가는게 논리상 맞다.
			//그럼 이제 note가 할당해제될 때 자기가 깊은 복사를 한 glyph를 할당해제하고,
			//command가 할당해제될 때 자기가 가지고 있는 glyph를 할당해제 하기 때문에 어디서 할당해제할지
			//말지 머리 아플 일이 없고, 여러모로 command의 glyph를 관리하기 편하기 때문에 
			//command는 note와는 별도로 자기만의 glyph를 가지고 있어야한다.
			//아니면 나중에 Backspace를 다지우고 다시 글자를 입력할 때 PushUndo에서 에러가 난다.!
			currentLetterPos = this->notepadForm->current->Add(this->glyph->Clone());
		}
		//5.2 현재 줄의 글자 위치가 현재 줄의 글자개수와 다르면
		else
		{
			//5.2.1 현재 줄의 글자 위치에 글자를 끼워서 추가한다.
			currentLetterPos = this->notepadForm->current->
				Add(currentLetterPos, this->glyph->Clone());
		}
	}
	//6. 입력받은 문자가 개행문자이면
	else
	{
		//6.1 기존에 저장된 줄을 할당해제한다.
		if (this->glyph != 0)
		{
			delete this->glyph;
		}
		//6.2 현재 줄에서 현재 글자 다음 위치에 있는 글자들을 떼어내 새로운 줄을 만들어 저장한다.
		this->glyph = this->notepadForm->current->Split(currentLetterPos);
		//6.3 현재 줄의 위치가 노트의 줄의 개수-1 과 같고(현재 줄의 위치가 마지막 줄이면)
		if (currentRowPos == this->notepadForm->note->GetLength() - 1)
		{
			//6.3.1 새로운 줄을 마지막 줄 다음에 추가한다.
			currentRowPos = this->notepadForm->note->Add(this->glyph->Clone());
		}
		//6.4 그게 아니면
		else
		{
			//6.4.1 새로운 줄을 현재 줄의 다음 위치에 끼워 넣는다.
			currentRowPos = this->notepadForm->note->
				Add(currentRowPos + 1, this->glyph->Clone());
		}
		//6.5 현재 줄을 새로 저장한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//6.6 현재 줄의 글자 위치를 처음으로 이동시킨다.
		this->notepadForm->current->First();
		//6.7 자동 줄 바꿈이 진행중이면
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//6.7.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
	//변경사항을 갱신함
	//7. isComposing을 false로 바꾼다.
	this->notepadForm->isComposing = false;
	//8. 메모장 제목에 *를 추가한다.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//9. 메모장에 변경사항이 있음을 저장한다.
	this->notepadForm->isDirty = true;
	//10. 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//11. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//11.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//11.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
	}
#if 0

	Long realRowIndex = 0;//진짜 줄의 위치
	Long realLetterIndex = 0;//진짜 줄의 글자 위치
	//현재 줄을 구한다.
	Glyph* currentRow = this->notepadForm->current;
	//줄이 가짜줄인 동안 반복한다. (진짜 줄의 위치 찾기)
	Long i = currentRowPos;
	while (i >= 0 && dynamic_cast<DummyRow*>(row))
	{
		i--;
		row = this->notepadForm->note->GetAt(i);
	}
	if (i >= 0)
	{
		//진짜 줄의 위치를 저장한다.
		realRowIndex = i;
	}
	//다음 진짜 줄까지 줄의 개수를 센다.
	i++;
	Glyph* nextRow = this->notepadForm->note->GetAt(i);
	while (i < this->notepadForm->note->GetLength() && dynamic_cast<DummyRow*>(row))
	{
		i++;
		nextRow = this->notepadForm->note->GetAt(i);
	}
	//진짜 줄이 가지고 있는 줄의 개수를 구한다.(자기 자신 + 가짜줄)
	Long rowCount = i - realRowIndex;
	//진짜 줄의 총 글자개수를 구한다.
	i = 0;
	Long j = realRowIndex;
	Long letterLength = this->notepadForm->note->GetAt(j)->GetLength() - 1;
	Long realRowLetterLength = letterLength + 1;
	while (i < rowCount)
	{
		j++;
		letterLength = this->notepadForm->note->GetAt(j)->GetLength() - 1;
		realRowLetterLength += letterLength;
		i++;
	}

	//10. 자동개행 중이 아니면
	if (this->notepadForm->isRowAutoChanging == false)
	{
		//10.1 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
	}
	//11. 자동개행 중이면
	else
	{
#if 0
		//11.1 현재 화면의 크기를 구한다.
		CRect rect;
		this->notepadForm->GetClientRect(&rect);
		//11.2 현재 화면의 가로 길이를 구한다.
		Long pageWidth = rect.Width();
		//11.3 현재 줄의 위치와 글자 위치를 구한다.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//11.4 현재 줄을 구한다.
		Glyph* row = this->notepadForm->current;
		//11.5 현재 줄에서 현재 글자를 추가하기 전 위치까지의 contents 가로길이를 구한다.
		Long rowTextWidth = this->notepadForm->textExtent->GetTextWidth
		(row->GetPartOfContent(currentLetterPos));
		//11.6 contents의 가로 길이가 화면의 가로 길이보다 크거나 같으면
		if (rowTextWidth >= pageWidth)
		{
			//11.6.1 command의 줄 위치에 현재 줄의 다음 줄을 저장한다.
			this->rowIndex = currentRowPos + 1;
			//11.6.2 command의 글자 위치에 1을 저장한다.
			this->letterIndex = 1;
		}
		//11.7 contents의 가로 길이가 화면의 가로 길이보다 작으면
		else
		{
			//10.1 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
			this->rowIndex = this->notepadForm->note->GetCurrent();
			this->letterIndex++;
		}
#endif

		//Long rowIndexBeforeRowAutoChange = 0;//자동개행 전의 원래 진짜 줄의 위치
		//Long letterIndexBeforeRowAutoChange = 0;//자동개행 전의 원래 진짜 글자의 위치
		//Long rowIndexAfterRowAutoChange = 0;//자동개행 후 줄의 위치
		//Long letterIndexAfterRowAutoChange = 0;//자동 개행 후 글자의 위치
		
		Long realRowIndex = 0;//진짜 줄의 위치
		Long realLetterIndex = 0;//진짜 줄의 글자 위치
		//현재 줄의 위치와 글자 위치를 구한다.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//줄을 구한다.
		Glyph* row = this->notepadForm->current;
		//줄이 가짜줄인 동안 반복한다. (진짜 줄의 위치 찾기)
		Long i = currentRowPos;
		while (i >= 0 && dynamic_cast<DummyRow*>(row))
		{
			i--;
			row = this->notepadForm->note->GetAt(i);
		}
		if (i >= 0)
		{
			//진짜 줄의 위치를 저장한다.
			realRowIndex = i;
		}
		i = realRowIndex;
		Long length = 0;
		while (i < currentRowPos)
		{
			row = this->notepadForm->note->GetAt(i);
			length += (row->GetLength() - 1);
			i++;
		}
		length += currentLetterPos;
		realLetterIndex = length;
		this->rowIndex = realRowIndex;
		this->letterIndex = realLetterIndex;
#if 0
		//다음 진짜 줄까지 줄의 개수를 센다.
		i++;
		row = this->notepadForm->note->GetAt(i);
		while (i < this->notepadForm->note->GetLength() && dynamic_cast<DummyRow*>(row))
		{
			i++;
			row = this->notepadForm->note->GetAt(i);
		}
		//진짜 줄이 가지고 있는 줄의 개수를 구한다.(자기 자신 + 가짜줄)
		Long rowCount = i - realRowIndex;
		//진짜 줄의 개수를 구한다.
		i = 0;
		Long j = realRowIndex;
		Long length = this->notepadForm->note->GetAt(j)->GetLength() - 1;
		Long realRowLength = length;
		while (i < rowCount)
		{
			j++;
			length = this->notepadForm->note->GetAt(j)->GetLength() - 1;
			realRowLength += length;
			i++;
		}
		realRowLength += 1;
#endif

	}
#endif
}

//Unexcute
void OnCharCommand::Unexecute()
{
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
	//5. 입력받은 문자가 개행문자가 아니면
	if (!dynamic_cast<Row*>(this->glyph))
	{
		//5.1 현재 글자를 지운다.
		this->notepadForm->current->Remove(currentLetterPos - 1);
		//5.2 자동 줄 바꿈이 진행중이면
		if (this->notepadForm->isRowAutoChanging == true)
		{
			//5.2.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
			//OnSize에서 부분자동개행을 하도록 한다. 
			this->notepadForm->SendMessage(WM_SIZE);
		}
	}
	//6. 입력받은 문자가 개행문자이면
	else
	{
		//6.1 현재 줄을 구한다.
		Glyph* currentRow = this->notepadForm->current;
		//6.2 현재 줄의 이전 줄을 구한다.
		Glyph* previousRow = this->notepadForm->note->GetAt(currentRowPos - 1);
		//6.3 현재 줄의 이전 줄의 마지막 글자 위치를 구한다.
		Long letterPos = previousRow->GetLength();
		//6.4 현재 줄을 이전 줄에 합친다.
		currentRow->Join(previousRow);
		//6.5 Note에서 현재 줄의 주소를 지운다.(내용은 지우면 안됨)
		this->notepadForm->note->Remove(currentRowPos);
		//6.6 줄이 지워졌기 때문에 줄의 위치를 다시 구한다.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//6.7 현재 줄의 글자 위치가 지금은 마지막이기 때문에 변경해준다.
		//이전 줄의 마지막 위치로 이동시킨다.
		Long index = this->notepadForm->current->Move(letterPos);
	}
	//7. 메모장 제목에 *를 추가한다.
	string name = this->notepadForm->fileName;
	name.insert(0, "*");
	name += " - 메모장";
	this->notepadForm->SetWindowText(CString(name.c_str()));
	//8. 메모장에 변경사항이 있음을 저장한다.
	this->notepadForm->isDirty = true;
	//9. 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
	this->rowIndex = this->notepadForm->note->GetCurrent();
	this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
	this->letterIndex = this->notepadForm->current->GetCurrent();
	//10. 자동개행이 진행중이면(command의 줄과 글자 위치는 항상 진짜 줄과 글자 위치를 저장해야함)
	if (this->notepadForm->isRowAutoChanging == true)
	{
		Long changedRowPos = this->rowIndex;
		Long changedLetterPos = this->letterIndex;
		Long originRowPos = 0;
		Long originLetterPos = 0;
		//10.1 변경된 화면 크기에 맞는 줄과 캐럿의 위치를 구한다.
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
		//10.2 command에 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = originRowPos;
		this->letterIndex = originLetterPos;
	}

#if 0
	//자동개행 진행중이면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		RowAutoChange rowAutoChange(this->notepadForm);
		Long changedRowPos = 0;
		Long changedLetterPos = 0;
		Long originRowPos = this->rowIndex;
		Long originLetterPos = this->letterIndex;
		this->rowIndex = changedRowPos;
		this->letterIndex = changedLetterPos;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		currentRowPos = this->notepadForm->note->Move(this->rowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		//5. 현재 글자의 위치를 이동시킨다.
		currentLetterPos = this->notepadForm->current->Move(this->letterIndex);
	}
#endif
#if 0
	//10. 자동개행 중이 아니면
	if (this->notepadForm->isRowAutoChanging == false)
	{
		//10.1 글자를 입력한 후에 현재 줄의 위치와 글자위치를 다시 저장한다.
		this->rowIndex = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(this->rowIndex);
		this->letterIndex = this->notepadForm->current->GetCurrent();
	}
	else
	{
		Long realRowIndex = 0;//진짜 줄의 위치
		Long realLetterIndex = 0;//진짜 줄의 글자 위치
		//현재 줄의 위치와 글자 위치를 구한다.
		currentRowPos = this->notepadForm->note->GetCurrent();
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
		currentLetterPos = this->notepadForm->current->GetCurrent();
		//줄을 구한다.
		Glyph* row = this->notepadForm->current;
		//줄이 가짜줄인 동안 반복한다. (진짜 줄의 위치 찾기)
		Long i = currentRowPos;
		while (i >= 0 && dynamic_cast<DummyRow*>(row))
		{
			i--;
			row = this->notepadForm->note->GetAt(i);
		}
		if (i >= 0)
		{
			//진짜 줄의 위치를 저장한다.
			realRowIndex = i;
		}
		i = realRowIndex;
		Long length = 0;
		while (i < currentRowPos)
		{
			row = this->notepadForm->note->GetAt(i);
			length += (row->GetLength() - 1);
			i++;
		}
		length += currentLetterPos;
		realLetterIndex = length;
		this->rowIndex = realRowIndex;
		this->letterIndex = realLetterIndex;
	}
#endif
}

//SetMacroEnd(실행취소 및 다시실행 매크로출력 종료지점 설정)
void OnCharCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void OnCharCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//SetRedone(다시 실행이라고 설정함)
void OnCharCommand::SetRedone()
{
	this->isRedone = true;
}

//줄의 위치 구하기
Long OnCharCommand::GetRowIndex()
{
	return this->rowIndex;
}
//글자 위치 구하기
Long OnCharCommand::GetLetterIndex()
{
	return this->letterIndex;
} 
//실행취소 종료지점여부 구하기
bool OnCharCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//다시실행 종료지점여부 구하기 
bool OnCharCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}
//다시실행인지 여부 구하기
bool OnCharCommand::IsRedone()
{
	return this->isRedone;
}

//소멸자 정의
OnCharCommand::~OnCharCommand()
{
	if (this->glyph != 0)
	{
		delete this->glyph;
	}
}