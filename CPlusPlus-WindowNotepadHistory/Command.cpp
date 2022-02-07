#include "Command.h"

//디폴트생성자
Command::Command(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//Command패턴 실행
void Command::Execute()
{

}

//Command패턴 실행취소
void Command::Unexecute()
{

}

//실행취소 매크로 종료지점인지 아닌지 확인
bool Command::IsUndoMacroEnd()
{
	return -1;
}

//다시실행 매크로 종료지점인지 아닌지 확인
bool Command::IsRedoMacroEnd()
{
	return -1;
}

//실행취소 매크로 종료지점 설정
void Command::SetUndoMacroEnd()
{

}

//다시실랭 매크로 종료지점 설정
void Command::SetRedoMacroEnd()
{

}

//다시실행이라고 표시
void Command::SetRedone()
{
	
}

//다시 실행인지 아닌지 여부 확인
bool Command::IsRedone()
{
	return -1;
}

//변경사항이 있는지 여부 확인
bool Command::IsDirty()
{
	return -1;
}

//선택영역을 지웠는지 여부 확인
bool Command::IsSelectedTextsRemoved()
{
	return -1;
}

//시작하는 줄의 위치구하기
Long Command::GetStartPasteRowIndex()
{
	return - 1;
}

//시작하는 글자의 위치구하기
Long Command::GetStartPasteLetterIndex()
{
	return -1;
}

//줄의 위치구하기
Long Command::GetRowIndex()
{
	return -1;
}

//글자 위치구하기
Long Command::GetLetterIndex()
{
	return -1;
}

//시작하는 글자의 위치구하기
Long Command::GetStartXPos()
{
	return -1;
}
//시작하는 줄의 위치구하기
Long Command::GetStartYPos()
{
	return -1;
}
//선택이 끝나는 줄의 위치 구하기
Long Command::GetSelectedEndYPos()
{
	return -1;
}
//선택이 끝나는 글자 위치 구하기
Long Command::GetSelectedEndXPos()
{
	return -1;
}
//붙여넣기가 끝나는 줄의 위치구하기
Long Command::GetPastingEndYPos()
{
	return -1;
}
//붙여넣기가 끝나는 글자의 위치구하기
Long Command::GetPastingEndXPos()
{
	return -1;
}

//소멸자
Command::~Command()
{

}