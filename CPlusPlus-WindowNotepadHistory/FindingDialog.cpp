#include "FindingDialog.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include"resource.h"
#include "GlyphFinder.h"
#include "SelectingTexts.h"
#include "RowAutoChange.h"

BEGIN_MESSAGE_MAP(FindingDialog, CFindReplaceDialog)
	ON_BN_CLICKED(IDOK, OnFindButtonClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//CFindReplaceDialog의 기본적인 것들을 상속받고, 나머지 상세사항을 더 구체화함!
FindingDialog::FindingDialog(CWnd* parent)
	:CFindReplaceDialog()
{
	this->notepadForm = (NotepadForm*)parent;
}

//1. 찾기 프레임 윈도우가 생성될 때
BOOL FindingDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	//1. '아래로' 라디오버튼을 선택한다.
	((CButton*)GetDlgItem(IDC_RADIO_UP))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_RADIO_DOWN))->SetCheck(BST_CHECKED);
	//2. 메모장에서 선택된 글자가 있으면
	if (this->notepadForm->isSelecting == true)
	{
		//2.1 선택된 글자를 구한다.

		//2.1 선택된 글자를 에디트컨트롤에 붙여넣는다.
		//CString keyword = this->notepadForm->GetDlgItem(IDC_EDIT_FINDINGCONTENT)
		//	->SetWindowText();
	}
	//2. 끝내다.
	return FALSE;
}

//2. 찾기 버튼을 클릭했을 때
void FindingDialog::OnFindButtonClicked()
{
	//1. GlyphFinder를 생성한다.
	GlyphFinder glyphFinder(this->notepadForm->note);
	//2. 에디트컨트롤에 적혀있는 글자를 읽는다.
	CString keyword;
	GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(keyword);
#if 0
	//3. 메모장에서 자동개행이 되어 있으면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//3.1 자동개행 전의 원래 위치를 구한다.
		RowAutoChange rowAutoChange(this->notepadForm);
		Long changedRowPos = this->notepadForm->note->GetCurrent();
		Long changedLetterPos = this->notepadForm->current->GetCurrent();
		Long originRowPos = 0;
		Long originLetterPos = 0;
		rowAutoChange.GetOriginPos(changedLetterPos, changedRowPos, &originLetterPos,
			&originRowPos);
	}
#endif
	//3. 아래로 찾기를 실행한다.
	Long findingRowIndex = 0;
	Long findingLetterIndex = 0;
	Long keywordLetterCount = 0;
	glyphFinder.FindDown((LPCTSTR)keyword, &findingRowIndex, &findingLetterIndex,
		&keywordLetterCount);

	//4. 자동개행이 되어 었으면
	if (this->notepadForm->isRowAutoChanging == true)
	{
		//4.1 자동개행이 되지 전의 원래 위치가 FindDown에서 출력되기 때문에 
		//자동개행이 되었을 때의 위치를 구해준다.
		RowAutoChange rowAutoChange(this->notepadForm);
		Long changedRowPos = 0;
		Long changedLetterPos = 0;
		Long originRowPos = findingRowIndex;
		Long originLetterPos = findingLetterIndex;
		rowAutoChange.GetChangedPos(originLetterPos, originRowPos, &changedLetterPos,
			&changedRowPos);
		findingRowIndex = changedRowPos;
		findingLetterIndex = changedLetterPos;
	}

	//_stricmp;//대소문자 구분 무시하는 함수(영어 대소문자를 같게 보게 해주는 함수)

	//5. 찾은 글자를 선택한다.
	this->notepadForm->selectingTexts->DoNext(findingRowIndex,
		findingLetterIndex - keywordLetterCount, findingRowIndex, findingLetterIndex);
	//6. 캐럿의 위치를 조정한다.
	this->notepadForm->note->Move(findingRowIndex);
	this->notepadForm->current = this->notepadForm->note->GetAt(findingRowIndex);
	this->notepadForm->current->Move(findingLetterIndex);
	//7. 캐럿의 위치가 변경되었음을 알린다.
	this->notepadForm->Notify();
	//8. 변경사항을 갱신한다.
	this->notepadForm->Invalidate(TRUE);
}


//3.닫기버튼을 클릭했을 때
void FindingDialog::OnClose()
{
	this->EndDialog(0);
}

