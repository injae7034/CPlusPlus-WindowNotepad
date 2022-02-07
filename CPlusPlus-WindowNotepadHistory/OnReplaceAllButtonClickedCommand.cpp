#include "OnReplaceAllButtonClickedCommand.h"
#include "CommandHistory.h"
#include "Row.h"
#include "RowAutoChange.h"
#include "DummyRow.h"
#include "TextExtent.h"
#include "SelectingTexts.h"
#include "ReplacingDialog.h"
#include "GlyphFinder.h"

//디폴트생성자
OnReplaceAllButtonClickedCommand::OnReplaceAllButtonClickedCommand(NotepadForm* notepadForm)
	:Command(notepadForm), 
	findingKeyword(((ReplacingDialog*)(this->notepadForm->findReplaceDialog))->findingKeyword),
	replacingKeyword(((ReplacingDialog*)(this->notepadForm->findReplaceDialog))->replacingKeyword)
{
	this->matchCaseChecked = ((ReplacingDialog*)(this->notepadForm->findReplaceDialog))
		->matchCaseChecked;
	this->isUndoMacroEnd = false;
	this->isRedoMacroEnd = false;
	this->isRedone = false;
}

//처음 및 다시 실행
void OnReplaceAllButtonClickedCommand::Execute()
{
	//1. 현재 줄의 위치와 글자 위치를 처음으로 이동시킨다.
	Long currentRowPos = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	Long currentLetterPos = this->notepadForm->current->First();
	//2. GlyphFinder를 생성한다.
	GlyphFinder glyphFinder(this->notepadForm->note);

	Long findingStartRowIndex = 0;//찾은 단어의 시작 줄위치
	Long findingStartLetterIndex = 0;//찾은 단어의 시작 글자위치
	Long findingEndRowIndex = 0;//찾은 단어의 끝 줄위치
	Long findingEndLetterIndex = 0;//찾은 단어의 끝 글자위치
	Long selectedStartRowPos = 0;//선택이 시작되는 줄의 위치
	Long selectedStartLetterPos = 0;//선택이 시작되는 글자 위치
	Long selectedEndRowPos = 0;//선택이 끝나는 줄 위치
	Long selectedEndLetterPos = 0;//선택이 끝나는 글자 위치
	bool isFounded = true;//찾을 단어가 발견이 되었는지 아닌지를 판별할 flag
	//3. 대/소문자 구분이 되어 있으면
	if (this->matchCaseChecked == BST_CHECKED)
	{
		//3.1 찾은게 있는 동안 반복한다.
		while (isFounded == true)
		{
			//3.1.1 아래로 찾기를 실행한다.
			glyphFinder.FindDown(this->findingKeyword, &findingStartRowIndex,
				&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
			//3.1.2 찾은 게 있으면
			if (findingStartRowIndex != findingEndRowIndex ||
				findingStartLetterIndex != findingEndLetterIndex)
			{
				//3.1.2.1 선택이 시작되는 캐럿의 x좌표를 저장한다.
				this->notepadForm->selectedStartXPos = findingStartLetterIndex;
				//3.1.2.2 선택이 시작되는 캐럿의 y좌표를 저장한다.
				this->notepadForm->selectedStartYPos = findingStartRowIndex;
				//3.1.2.3 찾은 글자를 선택한다.
				this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
					findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
				//3.1.2.4 캐럿의 위치를 메모장의 찾은 문자열이 있는 줄의 찾은 문자열 마지막 글자위치로 이동한다.
				currentRowPos = this->notepadForm->note->Move(findingEndRowIndex);
				this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
				currentLetterPos = this->notepadForm->current->Move(findingEndLetterIndex);
				//3.1.2.5 선택이 시작되는 줄과 글자 위치, 선택이 끝나는 줄과 글자 위치를 저장한다.
				selectedStartRowPos = this->notepadForm->selectedStartYPos;//선택이 시작되는 줄
				selectedStartLetterPos = this->notepadForm->selectedStartXPos;//선택이 시작되는 글자
				selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
				selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
				//3.1.2.6 단어를 지운다.
				this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
				//3.1.2.7 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.
				//(note의연산안에서 현재 줄의 위치와 글자 위치는 조정이 되지만 
				//notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//3.1.2.8 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
				this->notepadForm->selectingTexts->Undo();
				//3.1.2.9 선택이 끝난 상태로 바꾼다.
				this->notepadForm->isSelecting = false;
				//3.1.2.10 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
				this->notepadForm->selectedStartXPos = 0;
				//3.1.2.11 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
				this->notepadForm->selectedStartYPos = 0;
				//3.1.2.12 현재 줄에서 단어단위로 추가한다.
				this->notepadForm->current->AddWord(this->replacingKeyword);
				//3.1.2.13 자동개행이 진행중이면 단어를 추가하고 자동개행시켜준다.
				if (this->notepadForm->isRowAutoChanging == true)
				{
					this->notepadForm->SendMessage(WM_SIZE);
				}
			}
			//3.1.3 찾은 게 없으면
			else
			{
				isFounded = false;
			}
		}
	}
	//4. 대/소문자 구분이 안되어었으면
	else
	{
		//4.1 찾은게 있는 동안 반복한다.
		while (isFounded == true)
		{
			//4.1.1 대/소문자 구분없이 아래로 찾기를 실행한다.
			glyphFinder.FindDownWithMatchCase(this->findingKeyword, &findingStartRowIndex,
				&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
			//4.1.2 찾은 게 있으면
			if (findingStartRowIndex != findingEndRowIndex ||
				findingStartLetterIndex != findingEndLetterIndex)
			{
				//4.1.2.1 선택이 시작되는 캐럿의 x좌표를 저장한다.
				this->notepadForm->selectedStartXPos = findingStartLetterIndex;
				//4.1.2.2 선택이 시작되는 캐럿의 y좌표를 저장한다.
				this->notepadForm->selectedStartYPos = findingStartRowIndex;
				//4.1.2.3 찾은 글자를 선택한다.
				this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
					findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
				//4.1.2.4 캐럿의 위치를 메모장의 찾은 문자열이 있는 줄의 찾은 문자열 마지막 글자위치로 이동한다.
				currentRowPos = this->notepadForm->note->Move(findingEndRowIndex);
				this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
				currentLetterPos = this->notepadForm->current->Move(findingEndLetterIndex);
				//4.1.2.5 선택이 시작되는 줄과 글자 위치, 선택이 끝나는 줄과 글자 위치를 저장한다.
				selectedStartRowPos = this->notepadForm->selectedStartYPos;//선택이 시작되는 줄
				selectedStartLetterPos = this->notepadForm->selectedStartXPos;//선택이 시작되는 글자
				selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
				selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
				//4.1.2.6 content를 지운다.
				this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
				//4.1.2.7 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.
				//(note의연산안에서 현재 줄의 위치와 글자 위치는 조정이 되지만 
				//notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//4.1.2.8 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
				this->notepadForm->selectingTexts->Undo();
				//4.1.2.9 선택이 끝난 상태로 바꾼다.
				this->notepadForm->isSelecting = false;
				//4.1.2.10 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
				this->notepadForm->selectedStartXPos = 0;
				//4.1.2.11 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
				this->notepadForm->selectedStartYPos = 0;
				//4.1.2.12 현재 줄에서 단어단위로 추가한다.
				this->notepadForm->current->AddWord(this->replacingKeyword);
				//4.1.2.13 자동개행이 진행중이면 단어를 추가하고 자동개행시켜준다.
				if (this->notepadForm->isRowAutoChanging == true)
				{
					this->notepadForm->SendMessage(WM_SIZE);
				}
			}
			//4.1.3 찾은 게 없으면
			else
			{
				isFounded = false;
			}
		}
	}
	//5. 처음 실행이면
	if (this->isRedone == false)
	{
		//5.1 메모장 제목에 *를 추가한다.
		string name = this->notepadForm->fileName;
		name.insert(0, "*");
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//5.2 메모장에 변경사항이 있음을 저장한다.
		this->notepadForm->isDirty = true;
	}
	//6. 현재 줄의 위치와 글자 위치를 처음으로 이동시킨다.
	currentRowPos = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->First();
}

//실행취소
void OnReplaceAllButtonClickedCommand::Unexecute()
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
	//2. 현재 줄의 위치와 글자 위치를 처음으로 이동시킨다.
	Long currentRowPos = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	Long currentLetterPos = this->notepadForm->current->First();
	//3. GlyphFinder를 생성한다.
	GlyphFinder glyphFinder(this->notepadForm->note);

	Long findingStartRowIndex = 0;//찾은 단어의 시작 줄위치
	Long findingStartLetterIndex = 0;//찾은 단어의 시작 글자위치
	Long findingEndRowIndex = 0;//찾은 단어의 끝 줄위치
	Long findingEndLetterIndex = 0;//찾은 단어의 끝 글자위치
	Long selectedStartRowPos = 0;//선택이 시작되는 줄의 위치
	Long selectedStartLetterPos = 0;//선택이 시작되는 글자 위치
	Long selectedEndRowPos = 0;//선택이 끝나는 줄 위치
	Long selectedEndLetterPos = 0;//선택이 끝나는 글자 위치
	bool isFounded = true;//찾을 단어가 발견이 되었는지 아닌지를 판별할 flag
	//4. 대/소문자 구분이 되어 있으면
	if (this->matchCaseChecked == BST_CHECKED)
	{
		//4.1 찾은게 있는 동안 반복한다.
		while (isFounded == true)
		{
			//4.1.1 아래로 찾기를 실행한다.
			glyphFinder.FindDown(this->replacingKeyword, &findingStartRowIndex,
				&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
			//4.1.2 찾은 게 있으면
			if (findingStartRowIndex != findingEndRowIndex ||
				findingStartLetterIndex != findingEndLetterIndex)
			{
				//4.1.2.1 선택이 시작되는 캐럿의 x좌표를 저장한다.
				this->notepadForm->selectedStartXPos = findingStartLetterIndex;
				//4.1.2.2 선택이 시작되는 캐럿의 y좌표를 저장한다.
				this->notepadForm->selectedStartYPos = findingStartRowIndex;
				//4.1.2.3 찾은 글자를 선택한다.
				this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
					findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
				//4.1.2.4 캐럿의 위치를 메모장의 찾은 문자열이 있는 줄의 찾은 문자열 마지막 글자위치로 이동한다.
				currentRowPos = this->notepadForm->note->Move(findingEndRowIndex);
				this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
				currentLetterPos = this->notepadForm->current->Move(findingEndLetterIndex);
				//4.1.2.5 선택이 시작되는 줄과 글자 위치, 선택이 끝나는 줄과 글자 위치를 저장한다.
				selectedStartRowPos = this->notepadForm->selectedStartYPos;//선택이 시작되는 줄
				selectedStartLetterPos = this->notepadForm->selectedStartXPos;//선택이 시작되는 글자
				selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
				selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
				//4.1.2.6 단어를 지운다.
				this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
				//4.1.2.7 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.
				//(note의연산안에서 현재 줄의 위치와 글자 위치는 조정이 되지만 
				//notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//4.1.2.8 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
				this->notepadForm->selectingTexts->Undo();
				//4.1.2.9 선택이 끝난 상태로 바꾼다.
				this->notepadForm->isSelecting = false;
				//4.1.2.10 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
				this->notepadForm->selectedStartXPos = 0;
				//4.1.2.11 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
				this->notepadForm->selectedStartYPos = 0;
				//4.1.2.12 현재 줄에서 단어단위로 추가한다.
				this->notepadForm->current->AddWord(this->findingKeyword);
				//4.1.2.13 자동개행이 진행중이면 단어를 추가하고 자동개행시켜준다.
				if (this->notepadForm->isRowAutoChanging == true)
				{
					this->notepadForm->SendMessage(WM_SIZE);
				}
			}
			//4.1.3 찾은 게 없으면
			else
			{
				isFounded = false;
			}
		}
	}
	//5. 대/소문자 구분이 안되어었으면
	else
	{
		//5.1 찾은게 있는 동안 반복한다.
		while (isFounded == true)
		{
			//5.1.1 대/소문자 구분없이 아래로 찾기를 실행한다.
			glyphFinder.FindDownWithMatchCase(this->replacingKeyword, &findingStartRowIndex,
				&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
			//5.1.2 찾은 게 있으면
			if (findingStartRowIndex != findingEndRowIndex ||
				findingStartLetterIndex != findingEndLetterIndex)
			{
				//5.1.2.1 선택이 시작되는 캐럿의 x좌표를 저장한다.
				this->notepadForm->selectedStartXPos = findingStartLetterIndex;
				//5.1.2.2 선택이 시작되는 캐럿의 y좌표를 저장한다.
				this->notepadForm->selectedStartYPos = findingStartRowIndex;
				//5.1.2.3 찾은 글자를 선택한다.
				this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
					findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
				//5.1.2.4 캐럿의 위치를 메모장의 찾은 문자열이 있는 줄의 찾은 문자열 마지막 글자위치로 이동한다.
				currentRowPos = this->notepadForm->note->Move(findingEndRowIndex);
				this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
				currentLetterPos = this->notepadForm->current->Move(findingEndLetterIndex);
				//5.1.2.5 선택이 시작되는 줄과 글자 위치, 선택이 끝나는 줄과 글자 위치를 저장한다.
				selectedStartRowPos = this->notepadForm->selectedStartYPos;//선택이 시작되는 줄
				selectedStartLetterPos = this->notepadForm->selectedStartXPos;//선택이 시작되는 글자
				selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
				selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
				//5.1.2.6 content를 지운다.
				this->notepadForm->note->RemoveSelectedTexts(selectedStartRowPos,
					selectedStartLetterPos, selectedEndRowPos, selectedEndLetterPos);
				//5.1.2.7 연산이 끝났기 때문에 현재 줄의 위치를 다시 조정해준다.
				//(note의연산안에서 현재 줄의 위치와 글자 위치는 조정이 되지만 
				//notepadForm의 current(현재줄)는 조정할 수 없어서 notepadForm에서 해준다.)
				currentRowPos = this->notepadForm->note->GetCurrent();
				this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
				currentLetterPos = this->notepadForm->current->GetCurrent();
				//5.1.2.8 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
				this->notepadForm->selectingTexts->Undo();
				//5.1.2.9 선택이 끝난 상태로 바꾼다.
				this->notepadForm->isSelecting = false;
				//5.1.2.10 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
				this->notepadForm->selectedStartXPos = 0;
				//5.1.2.11 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
				this->notepadForm->selectedStartYPos = 0;
				//5.1.2.12 현재 줄에서 단어단위로 추가한다.
				this->notepadForm->current->AddWord(this->findingKeyword);
				//5.1.2.13 자동개행이 진행중이면 단어를 추가하고 자동개행시켜준다.
				if (this->notepadForm->isRowAutoChanging == true)
				{
					this->notepadForm->SendMessage(WM_SIZE);
				}
			}
			//5.1.3 찾은 게 없으면
			else
			{
				isFounded = false;
			}
		}
	}
	//6. 현재 줄의 위치와 글자 위치를 마지막으로 이동시킨다.
	currentRowPos = this->notepadForm->note->Last();
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowPos);
	currentLetterPos = this->notepadForm->current->Last();
}

//다시실행인지 여부 구하기
bool OnReplaceAllButtonClickedCommand::IsRedone()
{
	return this->isRedone;
}

//SetMacroEnd(실행취소 및 다시실행 매크로출력 종료지점 설정)
void OnReplaceAllButtonClickedCommand::SetUndoMacroEnd()
{
	this->isUndoMacroEnd = true;
}
void OnReplaceAllButtonClickedCommand::SetRedoMacroEnd()
{
	this->isRedoMacroEnd = true;
}

//실행취소 종료지점여부 구하기
bool OnReplaceAllButtonClickedCommand::IsUndoMacroEnd()
{
	return this->isUndoMacroEnd;
}
//다시실행 종료지점여부 구하기 
bool OnReplaceAllButtonClickedCommand::IsRedoMacroEnd()
{
	return this->isRedoMacroEnd;
}

//SetRedone(다시 실행이라고 설정함)
void OnReplaceAllButtonClickedCommand::SetRedone()
{
	this->isRedone = true;
}

//소멸자
OnReplaceAllButtonClickedCommand::~OnReplaceAllButtonClickedCommand()
{
	
}