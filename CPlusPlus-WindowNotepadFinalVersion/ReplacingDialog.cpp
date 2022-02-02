#include "ReplacingDialog.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include"resource.h"
#include "GlyphFinder.h"
#include "SelectingTexts.h"
#include "RowAutoChange.h"
#include "Row.h"
#include "DummyRow.h"

BEGIN_MESSAGE_MAP(ReplacingDialog, CFindReplaceDialog)
	ON_EN_CHANGE(IDC_EDIT_FINDINGCONTENT, OnFindingContentEditTyped)
	ON_BN_CLICKED(IDC_BUTTON_SWAP, OnSwapButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE, OnReplacedButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_REPLACEALL, OnReplaceAllButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnCancelButtonClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//CFindReplaceDialog의 기본적인 것들을 상속받고, 나머지 상세사항을 더 구체화함!
ReplacingDialog::ReplacingDialog(CWnd* parent)
	:CFindReplaceDialog(), findingKeyword(), replacingKeyword()
{
	this->notepadForm = (NotepadForm*)parent;
	this->matchCaseChecked = BST_CHECKED;
	//대화상자를 내가 만든 IDD_FINDINGDIALOG를 이용해서 커스터마이징할 때 추가해줘야 함
	this->m_fr.Flags |= FR_ENABLETEMPLATE;// |= 는 기존에꺼에 더해준다는 의미(누적)
	this->m_fr.hInstance = AfxGetResourceHandle();
	this->m_fr.lpTemplateName = MAKEINTRESOURCE(IDD);

}

BOOL ReplacingDialog::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return this->PostMessage(WM_CLOSE);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


//1. 바꾸기 프레임 윈도우가 생성될 때
BOOL ReplacingDialog::OnInitDialog()
{
	CFindReplaceDialog::OnInitDialog();
	//IDC_CHECKBOX_WRAPAROUND이 자꾸 바활성화되서 생성 되는게 디폴트라서
	//OnInitDialog에서 보이게 하고 활성화를 시켜준다.
	this->GetDlgItem(IDC_CHECKBOX_WRAPAROUND)->ShowWindow(SW_SHOW);
	this->GetDlgItem(IDC_CHECKBOX_WRAPAROUND)->EnableWindow(1);
	//Swap, 찾기, 바꾸기, 모두 바꾸기 버튼을 글자가 없으면 수행이 안되기 때문에 디폴트값으로 비활성화 시킨다.
	this->GetDlgItem(IDC_BUTTON_SWAP)->EnableWindow(0);
	this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(0);
	this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(0);
	this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(0);
	//레지스트리에 저장된 라디오버튼과 체크박스 정보를 가져와서 정보를 표시한다.
	int matchCaseChecked = AfxGetApp()->GetProfileInt("NotepadSection",
		"ReplacingDialogMatchCaseChecked", BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->SetCheck(matchCaseChecked);
	int wrapAroundChecked = AfxGetApp()->GetProfileInt("NotepadSection",
		"ReplacingDialogWrapAroundChecked", BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->SetCheck(wrapAroundChecked);
	//레지스트리에 저장된 바꾸기 단어를 가져온다.
	CString replacingWord = AfxGetApp()->GetProfileString("NotepadSection", "ReplacingWord",
		"");
	//레지스트리에 저장된 글자를 에디트컨트롤에 붙여넣는다.
	this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->SetWindowText(replacingWord);
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
		//3.7 //2.7 메모장의 현재 줄에서 처음 글자부터 현재 글자까지의 content를 구한다.
		i = 0;
		letter = 0;
		string letterContent = "";
		string keyword = "";
		while (i < copyRow->GetLength())
		{
			//2.7.1 글자를 구한다.
			letter = copyRow->GetAt(i);
			letterContent = letter->GetContent();
			//2.7.2 줄의 content에 더해준다.
			keyword += letterContent;
			i++;
		}
		//3.8 선택된 글자를 에디트컨트롤에 붙여넣는다.
		this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->SetWindowText(keyword.c_str());
		//3.9 힙에 할당된 줄을 할당해제한다.
		if (copyRow != 0)
		{
			delete copyRow;
		}
		//3.10 선택된 texts가 있기 때문에 Swap, 찾기, 바꾸기, 모두 바꾸기를 활성화시켜준다.
		this->GetDlgItem(IDC_BUTTON_SWAP)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(1);
	}
	//4. 선택된 글자가 없으면
	else
	{
		//4.1 레지스트리에 저장된 글자를 읽는다.
		CString registerKeyword = AfxGetApp()->GetProfileString("NotepadSection",
			"ReplacingDialogFindingWord", "");
		//4.2 레지스트리에 저장된 글자가 있으면
		if (registerKeyword.Compare("") != 0)
		{
			//4.2.1 저장된 글자를 에디트컨트롤에 붙여넣는다.
			this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->SetWindowText(registerKeyword);
			//4.2.2 저장된 글자가 있기 때문에 찾기 버튼을 활성화 시킨다.
			this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(1);
		}
	}
	//5. 끝내다.
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
		//2.1 선택된 texts가 있기 때문에 swap, 찾기, 바꾸기, 모두 바꾸기를 활성화시켜준다.
		this->GetDlgItem(IDC_BUTTON_SWAP)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(1);
		this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(1);
	}
	//3. content가 널문자이면
	else
	{
		//3.1 선택된 texts가 있기 때문에 swap, 찾기, 바꾸기, 모두 바꾸기를 비활성화시켜준다.
		this->GetDlgItem(IDC_BUTTON_SWAP)->EnableWindow(0);
		this->GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(0);
		this->GetDlgItem(IDC_BUTTON_REPLACE)->EnableWindow(0);
		this->GetDlgItem(IDC_BUTTON_REPLACEALL)->EnableWindow(0);
	}
}

//3. 찾을 내용과 바꿀 내용을 바꾸기 버튼을 누를 때
void ReplacingDialog::OnSwapButtonClicked()
{
	//1. 찾기 에디트 컨트롤에 적혀있는 글자를 읽어서 저장한다.
	CString findingKeyword;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(findingKeyword);
	//2. 바꾸기 에디트 컨트롤에 적혀있는 글자를 읽어서 저장한다.
	CString replacingKeyword;
	this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->GetWindowText(replacingKeyword);
	//3. 찾기 에디트 컨트롤에서 읽은 글자를 바꾸기 에디트 컨트롤에 출력한다.
	this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->SetWindowText(findingKeyword);
	//4. 바꾸기 에디트 컨트롤에서 읽은 글자를 찾기 에디트 컨트롤에 출력한다.
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->SetWindowText(replacingKeyword);
}

//4. 찾기 버튼을 클릭했을 때
void ReplacingDialog::OnFindButtonClicked()
{
	//1. GlyphFinder를 생성한다.
	GlyphFinder glyphFinder(this->notepadForm->note);
	//2. 찾기 에디트컨트롤에 적혀있는 글자를 읽는다.
	CString keyword;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(keyword);
	//에디트컨트롤에 적혀 있는 글자를 레지스트리에 저장한다.
	AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingDialogFindingWord", keyword);
	//2. 바꾸기 에디트 컨트롤에 적혀있는 글자를 읽는다.
	CString replacingKeyword;
	this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->GetWindowText(replacingKeyword);
	this->replacingKeyword = replacingKeyword;
	//에디트컨트롤에 적혀 있는 글자를 레지스트리에 저장한다.
	AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingWord", replacingKeyword);
	//3. 선택된 체크박스를 읽는다.
	int wrapAroundChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->GetCheck();
	this->matchCaseChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->GetCheck();
	//선택된 체크박스와 라디오버튼의 정보를 레지스트리에 저장한다.
	AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogWrapAroundChecked",
		wrapAroundChecked);
	AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogMatchCaseChecked",
		this->matchCaseChecked);

	Long findingStartRowIndex = 0;
	Long findingStartLetterIndex = 0;
	Long findingEndRowIndex = 0;
	Long findingEndLetterIndex = 0;
	Long currentRowIndex = this->notepadForm->note->GetCurrent();
	Long currentLetterIndex = this->notepadForm->current->GetCurrent();
	Long endRowIndex = this->notepadForm->note->GetLength() - 1;

	//4. '대/소문자 구분' 체크박스가 선택되어 있고, '주위에 배치' 체크박스가 선택이 안되어있으면
	if (this->matchCaseChecked == BST_CHECKED && wrapAroundChecked == BST_UNCHECKED)
	{
		//4.1 아래로 찾기를 실행한다.
		glyphFinder.FindDown((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
			&findingEndRowIndex, &findingEndLetterIndex);
	}
	//5. '대/소문자 구분', '주위에 배치' 체크박스가 선택이 되어있으면
	else if (this->matchCaseChecked == BST_CHECKED && wrapAroundChecked == BST_CHECKED)
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
	else if (this->matchCaseChecked == BST_UNCHECKED && wrapAroundChecked == BST_UNCHECKED)
	{
		//6.1 대/소문자 구분없이 아래로 찾기를 실행한다.
		glyphFinder.FindDownWithMatchCase((LPCTSTR)keyword, &findingStartRowIndex,
			&findingStartLetterIndex, &findingEndRowIndex, &findingEndLetterIndex);
	}
	//7. '주위에 배치' 체크박스가 선택이 되어있고 '대/소문자 구분'이 선택 안되어있으면
	else if (this->matchCaseChecked == BST_UNCHECKED && wrapAroundChecked == BST_CHECKED)
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
		//8.1 선택이 진행되고 있는 중이었으면
		if (this->notepadForm->isSelecting == true)
		{
			//8.1.1 선택된 텍스트를 선택해제한다.(선택을 끝낸다.)
			this->notepadForm->selectingTexts->Undo();
			//8.1.2 선택이 끝난 상태로 바꾼다.
			this->notepadForm->isSelecting = false;
			//8.1.3 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
			this->notepadForm->selectedStartXPos = 0;
			//8.1.4 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
			this->notepadForm->selectedStartYPos = 0;
		}
		//8.2 선택이 시작되는 캐럿의 x좌표를 저장한다.
		this->notepadForm->selectedStartXPos = findingStartLetterIndex;
		//8.3 선택이 시작되는 캐럿의 y좌표를 저장한다.
		this->notepadForm->selectedStartYPos = findingStartRowIndex;
		//8.4 선택이 다시 시작되기 때문에 선택이 다시 시작됨을 표시한다.
		this->notepadForm->isSelecting = true;
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
	this->notepadForm->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

//5. 바꾸기 버튼을 클릭했을 떄
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
		// 선택된 texts를 읽는다.
		Long i = 0;
		string content = "";
		//현재 줄의 위치와 글자위치를 구한다.
		Long currentRowIndex = this->notepadForm->note->GetCurrent();
		Long currentLetterIndex = this->notepadForm->current->GetCurrent();
		Long nextRowIndex = 0;
		Glyph* row = 0;
		Glyph* letter = 0;
		//선택이 시작되는 줄과 선택이 끝나는 줄을 비교한다.
		if (this->notepadForm->selectedStartYPos < currentRowIndex)
		{
			//선택이 시작되는 줄을 구한다.
			row = this->notepadForm->note->GetAt(this->notepadForm->selectedStartYPos);
			//선택이 시작되는 줄의 선택이 시작되는 글자부터 끝까지 content를 읽어서 누적시킨다.
			i = this->notepadForm->selectedStartXPos;
			while (i < row->GetLength())
			{
				//글자를 구한다.
				letter = row->GetAt(i);
				//content를 누적시킨다.
				content += letter->GetContent();
				//다음 글자로 이동한다.
				i++;
			}
			//다음 줄로 이동한다.
			nextRowIndex = this->notepadForm->selectedStartYPos + 1;
			//다음 줄이 현재 줄의 위치보다 작은동안 반복한다.
			while (nextRowIndex < currentRowIndex)
			{
				//줄을 구한다.
				row = this->notepadForm->note->GetAt(nextRowIndex);
				//줄이 진짜 줄이면
				if (!dynamic_cast<DummyRow*>(row))
				{
					//다음 줄의 content를 추가하기 전에 content에 개행문자를 추가한다.
					content += '\n';
				}
				//다음 줄의 마지막 글자까지 반복한다.
				i = 0;
				while (i < row->GetLength())
				{
					//글자를 구한다.
					letter = row->GetAt(i);
					//content를 누적시킨다.
					content += letter->GetContent();
					//다음 글자로 이동한다.
					i++;
				}
				//다음 줄로 이동한다.
				nextRowIndex++;
			}
			//선택이 끝나는 줄을 구한다.
			row = this->notepadForm->note->GetAt(currentRowIndex);
			//선택이 끝나는 줄이 진짜 줄이면
			if (!dynamic_cast<DummyRow*>(row))
			{
				//다음 줄의 content를 추가하기 전에 content에 개행문자를 추가한다.
				content += '\n';
			}
			//선택이 끝나는 줄의 처음 글자부터 선택이 끝나는 글자까지 반복한다.
			i = 0;
			while (i < currentLetterIndex)
			{
				//글자를 구한다.
				letter = row->GetAt(i);
				//content를 누적시킨다.
				content += letter->GetContent();
				//다음 글자로 이동한다.
				i++;
			}
		}
		//선택이 시작되는 줄과 선택이 끝나는 줄의 위치가 같으면
		else
		{
			//선택이 시작되는 줄을 구한다.
			row = this->notepadForm->note->GetAt(this->notepadForm->selectedStartYPos);
			//선택이 시작되는 줄의 선택이 시작되는 글자부터 선택이 끝나는 글자까지 content를 읽어서 누적시킨다
			i = this->notepadForm->selectedStartXPos;
			while (i < currentLetterIndex)
			{
				//글자를 구한다.
				letter = row->GetAt(i);
				//content를 누적시킨다.
				content += letter->GetContent();
				//다음 글자로 이동한다.
				i++;
			}
		}
		// 찾기 에디트컨트롤에 적혀있는 글자를 읽는다.
		bool isMatched = false;
		CString keyword;
		this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(keyword);
		//에디트컨트롤에 적혀 있는 글자를 레지스트리에 저장한다.
		AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingDialogFindingWord",
			keyword);
		string word(keyword);
		//3. 선택된 체크박스를 읽는다.
		int wrapAroundChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->GetCheck();
		this->matchCaseChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->GetCheck();
		//선택된 체크박스와 라디오버튼의 정보를 레지스트리에 저장한다.
		AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogWrapAroundChecked",
			wrapAroundChecked);
		AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogMatchCaseChecked", 
			this->matchCaseChecked);
		// 대/소문자 구분이 체크되어 있으면
		if (this->matchCaseChecked == true)
		{
			if (content.compare(word) == 0)
			{
				isMatched = true;
			}
		}
		//대/소문자 구분이 체크 안되어 있으면
		else
		{
			//8.4.5.1 대/소문자 구분없이 비교해준다.
			if (_stricmp(content.c_str(), word.c_str()) == 0)
			{
				isMatched = true;
			}
		}
		//2.8.1 바꾸기 에디트 컨트롤에 적혀있는 글자를 읽는다.
		CString replacingWord;
		this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->GetWindowText(replacingWord);
		this->replacingKeyword = (LPCTSTR)replacingWord;
		//에디트컨트롤에 적혀 있는 글자를 레지스트리에 저장한다.
		AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingWord", replacingWord);
		//선택되어 있는 글자와 찾는 글자가 서로 일치하면
		if (isMatched == true)
		{
			//2.8.1 OnReplaceButtonClikedCommand로 메세지를 보낸다.
			this->notepadForm->SendMessage(WM_COMMAND, ID_ONREPLACEBUTTONCLICKEDCOMMAND);
			//2.8.2 바꿀 단어를 바꿨기 때문에 메모장의 바꿀 단어를 초기화해준다.
			this->replacingKeyword = "";
			//2.8.3 '찾기 버튼을 클릭했을 때'로 메세지를 보낸다.
			//윈도우에서 버튼을 클릭했을 때 메세지는 WM_COMMAND이다
			this->SendMessage(WM_COMMAND, IDC_BUTTON_FIND);
		}
		// 선택되어 있는 글자와 찾는 글자가 서로 일치하지 않으면
		else
		{
			//2.8.4 '찾기 버튼을 클릭했을 때'로 메세지를 보낸다.
			//윈도우에서 버튼을 클릭했을 때 메세지는 WM_COMMAND이다
			this->SendMessage(WM_COMMAND, IDC_BUTTON_FIND);
		}
	}
}

//6. 모두 바꾸기 버튼을 클릭했을 떄
void ReplacingDialog::OnReplaceAllButtonClicked()
{
	//1. 찾기 에디트 컨트롤에 적혀있는 글자를 읽는다.
	CString findingKeyword;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(findingKeyword);
	this->findingKeyword = findingKeyword;
	//에디트컨트롤에 적혀 있는 글자를 레지스트리에 저장한다.
	AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingDialogFindingWord",
		findingKeyword);
	//2. 바꾸기 에디트 컨트롤에 적혀있는 글자를 읽는다.
	CString replacingKeyword;
	this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->GetWindowText(replacingKeyword);
	this->replacingKeyword = replacingKeyword;
	//에디트컨트롤에 적혀 있는 글자를 레지스트리에 저장한다.
	AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingWord", replacingKeyword);
	//3. 선택된 체크박스를 읽는다.
	int wrapAroundChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->GetCheck();
	this->matchCaseChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->GetCheck();
	//선택된 체크박스와 라디오버튼의 정보를 레지스트리에 저장한다.
	AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogWrapAroundChecked",
		wrapAroundChecked);
	AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogMatchCaseChecked",
		this->matchCaseChecked);
	//4. OnReplaceAllButtonClikedCommand로 메세지를 보낸다.
	this->notepadForm->SendMessage(WM_COMMAND, ID_ONREPLACEALLBUTTONCLICKEDCOMMAND);
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
	//1. 찾기 에디트 컨트롤에 적혀있는 글자를 읽는다.
	CString findingKeyword;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(findingKeyword);
	this->findingKeyword = findingKeyword;
	//에디트컨트롤에 적혀 있는 글자를 레지스트리에 저장한다.
	AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingDialogFindingWord",
		findingKeyword);
	//2. 바꾸기 에디트 컨트롤에 적혀있는 글자를 읽는다.
	CString replacingKeyword;
	this->GetDlgItem(IDC_EDIT_REPLACINGCONTENT)->GetWindowText(replacingKeyword);
	this->replacingKeyword = replacingKeyword;
	//에디트컨트롤에 적혀 있는 글자를 레지스트리에 저장한다.
	AfxGetApp()->WriteProfileString("NotepadSection", "ReplacingWord", replacingKeyword);
	//3. 선택된 체크박스를 읽는다.
	int wrapAroundChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->GetCheck();
	this->matchCaseChecked = ((CButton*)GetDlgItem(IDC_CHECKBOX_MATCHCASE))->GetCheck();
	//선택된 체크박스와 라디오버튼의 정보를 레지스트리에 저장한다.
	AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogWrapAroundChecked",
		wrapAroundChecked);
	AfxGetApp()->WriteProfileInt("NotepadSection", "ReplacingDialogMatchCaseChecked",
		this->matchCaseChecked);

	this->notepadForm->findReplaceDialog = 0;
	//1. 바꾸기 다이얼로그를 닫는다.
	CFindReplaceDialog::OnClose();
}
