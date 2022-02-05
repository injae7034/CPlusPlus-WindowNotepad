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

//소멸자
Command::~Command()
{

}