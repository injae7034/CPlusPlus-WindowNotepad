#include "ReplacingDialog.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include"resource.h"
#include "GlyphFinder.h"
#include "SelectingTexts.h"
#include "RowAutoChange.h"
#include "Row.h"

BEGIN_MESSAGE_MAP(ReplacingDialog, CFindReplaceDialog)
	ON_EN_CHANGE(IDC_EDIT_FINDINGCONTENT, OnFindingContentEditTyped)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE, OnReplacedButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_REPLACEALL, OnReplaceAllButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnCancelButtonClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//CFindReplaceDialog의 기본적인 것들을 상속받고, 나머지 상세사항을 더 구체화함!
ReplacingDialog::ReplacingDialog(CWnd* parent)
	:CFindReplaceDialog()
{
	this->notepadForm = (NotepadForm*)parent;
	//대화상자를 내가 만든 IDD_FINDINGDIALOG를 이용해서 커스터마이징할 때 추가해줘야 함
	this->m_fr.Flags |= FR_ENABLETEMPLATE;// |= 는 기존에꺼에 더해준다는 의미(누적)
	this->m_fr.hInstance = AfxGetResourceHandle();
	this->m_fr.lpTemplateName = MAKEINTRESOURCE(IDD);

}

//1. 바꾸기 프레임 윈도우가 생성될 때
BOOL ReplacingDialog::OnInitDialog()
{
	CFindReplaceDialog::OnInitDialog();
	//IDC_CHECKBOX_WRAPAROUND이 자꾸 바활성화되서 생성 되는게 디폴트라서
	//OnInitDialog에서 보이게 하고 활성화를 시켜준다.
	this->GetDlgItem(IDC_CHECKBOX_WRAPAROUND)->ShowWindow(SW_SHOW);
	this->GetDlgItem(IDC_CHECKBOX_WRAPAROUND)->EnableWindow(1);
	//찾기, 바꾸기, 모두 바꾸기 버튼을 글자가 없으면 수행이 안되기 때문에 디폴트값으로 비활성화 시킨다.
	this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(0);
	this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(0);
	this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(0);
	//1. '대/소문자 구분' 체크박스를 선택한다.
	((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->SetCheck(BST_CHECKED);
	//2. '주위에 배치' 체크박스를 선택하지 않는다.
	((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->SetCheck(BST_UNCHECKED);
	//3. 메모장에서 선택된 글자가 있으면
	if (this->notepadForm->isSelecting == true)
	{
		//메모장에 선택된 글자들을 저장한다.
		//3.1 선택된 글자들을 담을 공간인 줄을 생성한다.
		Glyph* copyRow = new Row();
		//3.2 메모장에서 현재 줄과 글자의 위치를 구한다.
		Long currentRowPos = this->notepadForm->note->GetCurrent();
		Long currentLetterPos = this->notepadForm->current->GetCurrent();
		//3.3 선택이 시작되는 줄과 글자, 선택이 끝나는 줄과 글자의 위치를 구한다.
		Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//선택이 시작되는 줄
		Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//선택이 시작되는 글자
		Long selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
		Long selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
		//3.4 선택의 방향과 상관없이 오른쪽 방향(다음)으로 시작하는 줄과 글자, 끝나는 줄과 글자위치를 정한다.
		Long startRowIndex = 0;//시작하는 줄의 위치
		Long startLetterIndex = 0;//시작하는 글자 위치
		Long endRowIndex = 0;//끝나는 줄의 위치
		Long endLetterIndex = 0;//끝나는 글자 위치
		Long i = 0;//반복제어변수
		Glyph* startRow = 0;//시작하는 줄의 위치
		Glyph* letter = 0;
		//3.5 선택이 시작되는 줄과 선택이 끝나는 줄이 같으면(복사가 한 줄 내에서 이뤄졌으면)
		if (selectedStartRowPos == selectedEndRowPos)
		{
			//3.5.1 선택이 오른쪽으로 진행되었으면
			if (selectedStartLetterPos < selectedEndLetterPos)
			{
				//3.5.1.1 시작하는 글자위치를 선택이 시작되는 글자위치로 정한다.
				startLetterIndex = selectedStartLetterPos;
				//3.5.1.2 끝나는 글자위치를 선택이 끝나는 글자위치로 정한다.
				endLetterIndex = selectedEndLetterPos;
				//3.5.1.3 시작하는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
				startRowIndex = selectedStartRowPos;

			}
			//3.5.2 선택이 왼쪽으로 진행되었으면
			else
			{
				//3.5.2.1 시작하는 글자위치를 선택이 끝나는 글자위치로 정한다.
				startLetterIndex = selectedEndLetterPos;
				//3.5.2.2 끝나는 글자위치를 선택이 시작하는 글자위치로 정한다.
				endLetterIndex = selectedStartLetterPos;
				//3.5.2.3 시작하는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
				startRowIndex = selectedEndRowPos;
			}
			//3.5.3 시작하는 줄을 구한다.
			startRow = this->notepadForm->note->GetAt(startRowIndex);
			//3.5.4 시작하는 글자위치부터 끝나는 글자까지 복사한다.
			i = startLetterIndex;
			while (i < endLetterIndex)
			{
				//3.5.4.1 메모장의 시작하는 줄에서 줄에서 글자를 구한다.
				letter = startRow->GetAt(i);
				//3.5.4.2 클립보드의 새로 만든 줄(복사하는 줄)에 글자를 추가한다.
				copyRow->Add(letter->Clone());
				//3.5.4.3 시작하는 글자의 다음 글자로 이동해서 글자를 복사한다.
				i++;
			}
		}
		//3.6 선택이 시작하는 줄과 선택이 끝나는 줄이 서로 다르면(선택이 여러줄에 걸쳐져서 되어 있으면)
		else
		{
			//3.6.1 선택이 오른쪽으로 진행되었으면 
			if (selectedStartRowPos < selectedEndRowPos)
			{
				//3.6.1.1 시작하는 글자위치를 선택이 시작되는 글자위치로 정한다.
				startLetterIndex = selectedStartLetterPos;
				//3.6.1.2 끝나는 글자위치를 선택이 끝나는 글자위치로 정한다.
				endLetterIndex = selectedEndLetterPos;
				//3.6.1.3 시작하는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
				startRowIndex = selectedStartRowPos;
				//3.6.1.4 끝나는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
				endRowIndex = selectedEndRowPos;
			}
			//3.6.2 선택이 왼쪽으로 진행되었으면
			else
			{
				//3.6.2.1 시작하는 글자위치를 선택이 끝나는 글자위치로 정한다.
				startLetterIndex = selectedEndLetterPos;
				//3.6.2.2 끝나는 글자위치를 선택이 시작하는 글자위치로 정한다.
				endLetterIndex = selectedStartLetterPos;
				//3.6.2.3 시작하는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
				startRowIndex = selectedEndRowPos;
				//3.6.2.4 끝나는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
				endRowIndex = selectedStartRowPos;
			}
			Glyph* endRow = 0;//끝나는 줄의 위치
			//3.6.3 끝나는 줄보다 작은동안 반복한다.
			while (startRowIndex < endRowIndex)
			{
				//3.6.3.1 시작하는 줄을 구한다.
				startRow = this->notepadForm->note->GetAt(startRowIndex);
				//3.6.3.2 시작하는 글자위치부터 시작하는 줄의 마지막 글자까지 복사한다.
				i = startLetterIndex;
				while (i < startRow->GetLength())
				{
					//3.6.3.2.1 시작하는 줄에서 글자를 구한다.
					letter = startRow->GetAt(i);
					//3.6.3.2.2 클립보드의 새로 만든 줄(복사하는 줄)에 글자를 추가한다.
					copyRow->Add(letter->Clone());
					//3.6.3.2.3 시작하는 글자의 다음 글자로 이동해서 글자를 복사한다.
					i++;
				}
				//3.6.3.3 시작하는 줄의 위치를 증가시킨다.
				startRowIndex++;
				//3.6.3.4 줄이 다음 줄로 이동했기 때문에 글자위치를 0으로 초기화해준다.
				startLetterIndex = 0;
			}
			//3.6.4 메모장에서 끝나는 줄을 구한다.
			endRow = this->notepadForm->note->GetAt(startRowIndex);
			//3.6.5 글자 위치를 원위치시킨다.
			i = 0;
			//3.6.6 메모장의 가짜줄에서 마지막 글자까지 반복한다.
			while (i < endLetterIndex)
			{
				//3.6.6.1 메모장의 가짜줄에서 글자를 구한다.
				letter = endRow->GetAt(i);
				//3.6.6.2 클립보드의 새로 만든 줄(복사하는 줄)에 글자를 추가한다.
				copyRow->Add(letter->Clone());
				//3.6.6.3 다음 글자로 이동해서 글자를 복사한다.
				i++;
			}
		}
		//3.7 줄에 담은 글자를 구한다.
		string keyword = copyRow->GetContent();
		//3.8 선택된 글자를 에디트컨트롤에 붙여넣는다.
		this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->SetWindowText(keyword.c_str());
		//3.9 힙에 할당된 줄을 할당해제한다.
		if (copyRow != 0)
		{
			delete copyRow;
		}
		//3.10 선택된 texts가 있기 때문에 찾기, 바꾸기, 모두 바꾸기를 활성화시켜준다.
		this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(1);
	}
	//4. 끝내다.
	return FALSE;
}

//2. 찾을 내용 에디트에 텍스트를 입력할 때
void ReplacingDialog::OnFindingContentEditTyped()
{
	//1. 에디트컨트롤에 적혀있는 글자를 읽는다.
	CString content;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(content);
	//2. content가 널문자가 아니면
	if (content != "")
	{
		//2.1 선택된 texts가 있기 때문에 찾기, 바꾸기, 모두 바꾸기를 활성화시켜준다.
		this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(1);
	}
	//3. content가 널문자이면
	else
	{
		//3.1 선택된 texts가 있기 때문에 찾기, 바꾸기, 모두 바꾸기를 비활성화시켜준다.
		this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(0);
		this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(0);
		this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(0);
	}
}

//3. 찾기 버튼을 클릭했을 때
void ReplacingDialog::OnFindButtonClicked()
{
	//1. GlyphFinder를 생성한다.
	GlyphFinder glyphFinder(this->notepadForm->note);
	//2. 에디트컨트롤에 적혀있는 글자를 읽는다.
	CString keyword;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(keyword);
	//3. 선택된 체크박스를 읽는다.
	int wrapAroundChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->GetCheck();
	int matchCaseChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->GetCheck();

	Long findingStartRowIndex = 0;
	Long findingStartLetterIndex = 0;
	Long findingEndRowIndex = 0;
	Long findingEndLetterIndex = 0;
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	Long currentLetterIndex = this->notepadForm->current->GetCurrent();
	Long endRowIndex = this->notepadForm->note->GetLength() - 1;

	//4. '대/소문자 구분' 체크박스가 선택되어 있고, '주위에 배치' 체크박스가 선택이 안되어있으면
	if (matchCaseChecked == BST_CHECKED && wrapAroundChecked == BST_UNCHECKED)
	{
		//4.1 아래로 찾기를 실행한다.
		glyphFinder.FindDown((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
			&findingEndRowIndex, &findingEndLetterIndex);
	}
	//5. '대/소문자 구분', '주위에 배치' 체크박스가 선택이 되어있으면
	else if (matchCaseChecked == BST_CHECKED && wrapAroundChecked == BST_CHECKED)
	{
		//5.1 아래로 찾기를 실행한다.
		glyphFinder.FindDown((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
			&findingEndRowIndex, &findingEndLetterIndex);
		//5.2 찾은게 없으면
		if (findingStartRowIndex == findingEndRowIndex &&
			findingStartLetterIndex == findingEndLetterIndex)
		{
			//5.2.1 현재 줄의 위치와 글자를 메모장의 제일 처음으로 보낸다.
			this->notepadForm->note->Move(0);
			this->notepadForm->current = this->notepadForm->note->GetAt(0);
			this->notepadForm->current->Move(0);
			//5.2.2 아래로 찾기를 실행한다.
			glyphFinder.FindDown((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
				&findingEndRowIndex, &findingEndLetterIndex);
		}
	}
	//6. '대/소문자 구분', '주위에 배치' 체크박스가 선택이 안되어있으면
	else if (matchCaseChecked == BST_UNCHECKED && wrapAroundChecked == BST_UNCHECKED)
	{
		//6.1 대/소문자 구분없이 아래로 찾기를 실행한다.
		glyphFinder.FindDownWithMatchCase((LPCTSTR)keyword, &findingStartRowIndex,
			&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
	}
	//7. '주위에 배치' 체크박스가 선택이 되어있고 '대/소문자 구분'이 선택 안되어있으면
	else if (matchCaseChecked == BST_UNCHECKED && wrapAroundChecked == BST_CHECKED)
	{
		//7.1 대/소문자 구분없이 아래로 찾기를 실행한다.
		glyphFinder.FindDownWithMatchCase((LPCTSTR)keyword, &findingStartRowIndex,
			&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
		//7.2 찾은게 없으면
		if (findingStartRowIndex == findingEndRowIndex &&
			findingStartLetterIndex == findingEndLetterIndex)
		{
			//7.2.1 현재 줄의 위치와 글자를 메모장의 제일 처음으로 보낸다.
			this->notepadForm->note->Move(0);
			this->notepadForm->current = this->notepadForm->note->GetAt(0);
			this->notepadForm->current->Move(0);
			//7.2.2 대/소문자 구분없이 아래로 찾기를 실행한다.
			glyphFinder.FindDownWithMatchCase((LPCTSTR)keyword, &findingStartRowIndex,
				&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
		}
	}
	
	//8. 찾은 게 있으면
	if (findingStartRowIndex != findingEndRowIndex ||
		findingStartLetterIndex != findingEndLetterIndex)
	{
		//8.1 선택이 처음 시작되면
		if (this->notepadForm->isSelecting == false)
		{
			//8.1.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
			this->notepadForm->isSelecting = true;
		}
		//8.2 이미 선택된 texts가 있으면
		else
		{
			//8.2.1 선택된 텍스트를 선택해제한다.(선택을 끝낸다)
			this->notepadForm->selectingTexts->Undo();
			//8.2.2 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
			this->notepadForm->selectedStartXPos = 0;
			//8.2.3 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
			this->notepadForm->selectedStartYPos = 0;
		}
		//8.3 선택이 시작되는 캐럿의 x좌표를 저장한다.
		this->notepadForm->selectedStartXPos = findingStartLetterIndex;
		//8.4 선택이 시작되는 캐럿의 y좌표를 저장한다.
		this->notepadForm->selectedStartYPos = findingStartRowIndex;
		//8.5 찾은 글자를 선택한다.
		this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
			findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
		//8.6 캐럿의 위치를 메모장의 찾은 문자열이 있는 줄의 찾은 문자열 마지막 글자위치로 이동한다.
		this->notepadForm->note->Move(findingEndRowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
		this->notepadForm->current->Move(findingEndLetterIndex);
		//8.7 선택이 되었기 때문에 복사하기, 잘라내기, 삭제 메뉴룰 활성화시켜준다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
	}
	//9. 찾은게 없으면
	else
	{
		//9.1 캐럿의 위치를 메모장의 찾은 문자열이 있는 줄의 찾은 문자열 마지막 글자위치로 이동한다.
		this->notepadForm->note->Move(currentRowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
		this->notepadForm->current->Move(currentLetterIndex);
		//9.2 "찾을 수 없습니다." 메세지박스를 출력한다.
		string message = (LPCTSTR)keyword;
		message.insert(0, "\"");
		message += "\"";
		message += "을(를) 찾을 수 없습니다.";
		int messageBoxButton = MessageBox(message.c_str(), "메모장", MB_OK);
	}
	//10. 캐럿의 위치가 변경되었음을 알린다.
	this->notepadForm->Notify();
	//11. 변경사항을 갱신한다.
	this->notepadForm->Invalidate(TRUE);
}

//4. 바꾸기 버튼을 클릭했을 떄
void ReplacingDialog::OnReplacedButtonClicked()
{
	//1. 선택된 texts가 없으면
	if (this->notepadForm->isSelecting == false)
	{
		//1.1 '찾기 버튼을 클릭했을 때'로 메세지를 보낸다.
		//윈도우에서 버튼을 클릭했을 때 메세지는 WM_COMMAND이다
		this->SendMessage(WM_COMMAND, IDC_BUTTON_FIND);
	}
	//2. 선택된 texts가 있으면
	else
	{
		//2.1 RemoveCommand로 메세지를 보내서 선택영역을 지운다.
		this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
		//2.2 바꾸기 에디트 컨트롤에 적혀있는 글자를 읽는다.
		CString word;
		this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->GetWindowText(word);
		//2.3 현재 줄의 위치를 구한다.
		Long currentRowIndex = this->notepadForm->note->GetCurrent();
		//2.4 현재 줄을 구한다.
		Glyph* currentRow = this->notepadForm->note->GetAt(currentRowIndex);
		//2.5 현재 줄에서 단어단위로 추가한다.
		currentRow->AddWord((LPCTSTR)word);
		//2.6 자동개행이 진행중이면 단어를 추가하고 자동개행시켜준다.
		if (this->notepadForm->isRowAutoChanging == true)
		{
			this->notepadForm->SendMessage(WM_SIZE);
		}
		//2.7 '찾기 버튼을 클릭했을 때'로 메세지를 보낸다.
		//윈도우에서 버튼을 클릭했을 때 메세지는 WM_COMMAND이다
		this->SendMessage(WM_COMMAND, IDC_BUTTON_FIND);
	}
	//10. 캐럿의 위치가 변경되었음을 알린다.
	this->notepadForm->Notify();
	//11. 변경사항을 갱신한다.
	this->notepadForm->Invalidate(TRUE);
}

//4. 모두 바꾸기 버튼을 클릭했을 떄
void ReplacingDialog::OnReplaceAllButtonClicked()
{
	Long findingStartRowIndex = 0;
	Long findingStartLetterIndex = 0;
	Long findingEndRowIndex = 0;
	Long findingEndLetterIndex = 0;
	Glyph* currentRow = 0;
	bool isFounded = true;//찾을 단어가 발견이 되었는지 아닌지를 판별할 flag
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
	//2. 선택한 texts들을 다 선택취소했으면
	if (this->notepadForm->isSelecting == false)
	{
		//2.1 복사하기, 잘라내기, 삭제 메뉴를 비활성화 시킨다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}

	//3. 현재 줄의 위치를 제일 처음으로 보낸다.
	Long currentRowIndex = this->notepadForm->note->First();
	//4. 메모장에서 현재 줄의 위치를 다시 저장한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//5. 현재 글자의 위치를 제일 처음으로 보낸다.
	Long currentLetterIndex = this->notepadForm->current->First();

	//6. GlyphFinder를 생성한다.
	GlyphFinder glyphFinder(this->notepadForm->note);
	//7. 찾기 에디트 컨트롤에 적혀있는 글자를 읽는다.
	CString keyword;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(keyword);
	//8. 바꾸기 에디트 컨트롤에 적혀있는 글자를 읽는다.
	CString word;
	this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->GetWindowText(word);
	//9. 대/소문자 구분 체크박스를 읽는다.
	int matchCaseChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->GetCheck();
	//10. 대/소문자 구분이 되어 있으면
	if (matchCaseChecked == BST_CHECKED)
	{
		//10.1 찾은게 있는 동안 반복한다.
		while (isFounded == true)
		{
			//10.1.1 아래로 찾기를 실행한다.
			glyphFinder.FindDown((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
				&findingEndRowIndex, &findingEndLetterIndex);
			//10.1.2 찾은 게 있으면
			if (findingStartRowIndex != findingEndRowIndex ||
				findingStartLetterIndex != findingEndLetterIndex)
			{
				//10.1.2.1 선택이 시작되는 캐럿의 x좌표를 저장한다.
				this->notepadForm->selectedStartXPos = findingStartLetterIndex;
				//10.1.2.2 선택이 시작되는 캐럿의 y좌표를 저장한다.
				this->notepadForm->selectedStartYPos = findingStartRowIndex;
				//10.1.2.3 찾은 글자를 선택한다.
				this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
					findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
				//10.1.2.4 캐럿의 위치를 메모장의 찾은 문자열이 있는 줄의 찾은 문자열 마지막 글자위치로 이동한다.
				this->notepadForm->note->Move(findingEndRowIndex);
				this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
				this->notepadForm->current->Move(findingEndLetterIndex);
				//10.1.2.5 RemoveCommand로 메세지를 보내서 선택영역을 지운다.
				this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
				//10.1.2.6 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
				this->notepadForm->selectingTexts->Undo();
				//10.1.2.7 선택이 끝난 상태로 바꾼다.
				this->notepadForm->isSelecting = false;
				//10.1.2.8 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
				this->notepadForm->selectedStartXPos = 0;
				//10.1.2.9 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
				this->notepadForm->selectedStartYPos = 0;
				//10.1.2.10 현재 줄의 위치를 구한다.
				currentRowIndex = this->notepadForm->note->GetCurrent();
				//10.1.2.11 현재 줄을 구한다.
				currentRow = this->notepadForm->note->GetAt(currentRowIndex);
				//10.1.2.12 현재 줄에서 단어단위로 추가한다.
				currentRow->AddWord((LPCTSTR)word);
				//10.1.2.13 자동개행이 진행중이면 단어를 추가하고 자동개행시켜준다.
				if (this->notepadForm->isRowAutoChanging == true)
				{
					this->notepadForm->SendMessage(WM_SIZE);
				}
			}
			//10.1.3 찾은 게 없으면
			else
			{
				isFounded = false;
			}
		}	
	}
	//11. 대/소문자 구분이 안되어었으면
	else
	{
		//11.1 찾은게 있는 동안 반복한다.
		while (isFounded == true)
		{
			//11.1.1 대/소문자 구분없이 아래로 찾기를 실행한다.
			glyphFinder.FindDownWithMatchCase((LPCTSTR)keyword, &findingStartRowIndex,
				&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
			//11.1.2 찾은 게 있으면
			if (findingStartRowIndex != findingEndRowIndex ||
				findingStartLetterIndex != findingEndLetterIndex)
			{
				//11.1.2.1 선택이 시작되는 캐럿의 x좌표를 저장한다.
				this->notepadForm->selectedStartXPos = findingStartLetterIndex;
				//11.1.2.2 선택이 시작되는 캐럿의 y좌표를 저장한다.
				this->notepadForm->selectedStartYPos = findingStartRowIndex;
				//11.1.2.3 찾은 글자를 선택한다.
				this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
					findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
				//11.1.2.4 캐럿의 위치를 메모장의 찾은 문자열이 있는 줄의 찾은 문자열 마지막 글자위치로 이동한다.
				this->notepadForm->note->Move(findingEndRowIndex);
				this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
				this->notepadForm->current->Move(findingEndLetterIndex);
				//11.1.2.5 RemoveCommand로 메세지를 보내서 선택영역을 지운다.
				this->notepadForm->SendMessage(WM_COMMAND, IDM_NOTE_REMOVE);
				//11.1.2.6 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
				this->notepadForm->selectingTexts->Undo();
				//11.1.2.7 선택이 끝난 상태로 바꾼다.
				this->notepadForm->isSelecting = false;
				//11.1.2.8 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
				this->notepadForm->selectedStartXPos = 0;
				//11.1.2.9 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
				this->notepadForm->selectedStartYPos = 0;
				//11.1.2.10 현재 줄의 위치를 구한다.
				currentRowIndex = this->notepadForm->note->GetCurrent();
				//11.1.2.11 현재 줄을 구한다.
				currentRow = this->notepadForm->note->GetAt(currentRowIndex);
				//11.1.2.12 현재 줄에서 단어단위로 추가한다.
				currentRow->AddWord((LPCTSTR)word);
				//11.1.2.13 자동개행이 진행중이면 단어를 추가하고 자동개행시켜준다.
				if (this->notepadForm->isRowAutoChanging == true)
				{
					this->notepadForm->SendMessage(WM_SIZE);
				}
			}
			//11.1.3 찾은 게 없으면
			else
			{
				isFounded = false;
			}
		}
	}
	//12. 현재 줄의 위치를 제일 처음으로 보낸다.
	currentRowIndex = this->notepadForm->note->First();
	//13. 메모장에서 현재 줄의 위치를 다시 저장한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(currentRowIndex);
	//14. 현재 글자의 위치를 제일 처음으로 보낸다.
	this->notepadForm->current->First();
	//15. 캐럿의 위치가 변경되었음을 알린다.
	this->notepadForm->Notify();
	//16. 변경사항을 갱신한다.
	this->notepadForm->Invalidate(TRUE);
}

//6. 취소 버튼을 클릭했을 때
void ReplacingDialog::OnCancelButtonClicked()
{
	//1. OnClose로 메세지를 보낸다.
	//SendMessage는 메세지를 보내고 거기서 행위가 끝나면 원래 메세지를 보낸 주체로 다시 돌아와서 이후에
	//돌아온 곳에서 나머지 남은 행위들을 수행한다. 근데 여기서 문제가 WM_CLOSE로 메세지를 보내면 
	//ReplacingDialog는 할당해제가 되서 사라지기 때문에 다시 돌아올 곳이 없다.
	//그래서 여기서는 메세지를 보내고 보낸 주체로 다시 돌아오지 않고 거기서 행위를 하고 그대로 끝나는
	//PostMessage를 사용해야한다.
	this->PostMessage(WM_CLOSE);
}

//7.닫기버튼을 클릭했을 때
void ReplacingDialog::OnClose()
{
#if 0
	// 찾기 프레임 윈도우를 띄우기 전에 찾기 프레암 윈도우가 있는지 확인하고 있으면 할당해제한다.
	if (this->notepadForm->findReplaceDialog != 0)
	{
		//CFindReplaceDialog를 할당해제할때는 delete대신에 DestroyWindow를 이용하자!
		this->notepadForm->findReplaceDialog->DestroyWindow();
		//delete this->notepadForm->findingDialog;
		//댕글링 포인터를 0으로 안바꿔주면 할당해제가 됬는데 다시 할당해제를 하러 들어와서 에러가난다.
		//this->notepadForm->findReplaceDialog = 0;
	}
#endif
	this->notepadForm->findReplaceDialog = 0;
	//1. 바꾸기 다이얼로그를 닫는다.
	CFindReplaceDialog::OnClose();
}
