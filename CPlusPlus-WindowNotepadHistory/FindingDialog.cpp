#include "FindingDialog.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include"resource.h"
#include "GlyphFinder.h"
#include "SelectingTexts.h"
#include "RowAutoChange.h"
#include "Row.h"

BEGIN_MESSAGE_MAP(FindingDialog, CFindReplaceDialog)
	ON_BN_CLICKED(IDOK, OnFindButtonClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//CFindReplaceDialog의 기본적인 것들을 상속받고, 나머지 상세사항을 더 구체화함!
FindingDialog::FindingDialog(CWnd* parent)
	:CFindReplaceDialog()
{
	this->notepadForm = (NotepadForm*)parent;
	//대화상자를 내가 만든 IDD_FINDINGDIALOG를 이용해서 커스터마이징할 때 추가해줘야 함
	this->m_fr.Flags |= FR_ENABLETEMPLATE;// |= 는 기존에꺼에 더해준다는 의미(누적)
	this->m_fr.hInstance = AfxGetResourceHandle();
	this->m_fr.lpTemplateName = MAKEINTRESOURCE(IDD);
	
}

//1. 찾기 프레임 윈도우가 생성될 때
BOOL FindingDialog::OnInitDialog()
{
	CFindReplaceDialog::OnInitDialog();
	//IDC_CHECKBOX_WRAPAROUND이 자꾸 바활성화되서 생성 되는게 디폴트라서
	//OnInitDialog에서 보이게 하고 활성화를 시켜준다.
	this->GetDlgItem(IDC_CHECKBOX_WRAPAROUND)->ShowWindow(SW_SHOW);
	this->GetDlgItem(IDC_CHECKBOX_WRAPAROUND)->EnableWindow(1);
	//1. '아래로' 라디오버튼을 선택한다.
	((CButton*)GetDlgItem(IDC_RADIO_UP))->SetCheck(BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_RADIO_DOWN))->SetCheck(BST_CHECKED);
	//2. 메모장에서 선택된 글자가 있으면
	if (this->notepadForm->isSelecting == true)
	{
		//메모장에 선택된 글자들을 저장한다.
		//2.1 선택된 글자들을 담을 공간인 줄을 생성한다.
		Glyph* copyRow = new Row();
		//2.2 메모장에서 현재 줄과 글자의 위치를 구한다.
		Long currentRowPos = this->notepadForm->note->GetCurrent();
		Long currentLetterPos = this->notepadForm->current->GetCurrent();
		//2.3 선택이 시작되는 줄과 글자, 선택이 끝나는 줄과 글자의 위치를 구한다.
		Long selectedStartRowPos = this->notepadForm->selectedStartYPos;//선택이 시작되는 줄
		Long selectedStartLetterPos = this->notepadForm->selectedStartXPos;//선택이 시작되는 글자
		Long selectedEndRowPos = currentRowPos;//선택이 끝나는 줄
		Long selectedEndLetterPos = currentLetterPos;//선택이 끝나는 글자
		//2.4 선택의 방향과 상관없이 오른쪽 방향(다음)으로 시작하는 줄과 글자, 끝나는 줄과 글자위치를 정한다.
		Long startRowIndex = 0;//시작하는 줄의 위치
		Long startLetterIndex = 0;//시작하는 글자 위치
		Long endRowIndex = 0;//끝나는 줄의 위치
		Long endLetterIndex = 0;//끝나는 글자 위치
		Long i = 0;//반복제어변수
		Glyph* startRow = 0;//시작하는 줄의 위치
		Glyph* letter = 0;
		//2.5 선택이 시작되는 줄과 선택이 끝나는 줄이 같으면(복사가 한 줄 내에서 이뤄졌으면)
		if (selectedStartRowPos == selectedEndRowPos)
		{
			//2.5.1 선택이 오른쪽으로 진행되었으면
			if (selectedStartLetterPos < selectedEndLetterPos)
			{
				//2.5.1.1 시작하는 글자위치를 선택이 시작되는 글자위치로 정한다.
				startLetterIndex = selectedStartLetterPos;
				//2.5.1.2 끝나는 글자위치를 선택이 끝나는 글자위치로 정한다.
				endLetterIndex = selectedEndLetterPos;
				//2.5.1.3 시작하는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
				startRowIndex = selectedStartRowPos;

			}
			//2.5.2 선택이 왼쪽으로 진행되었으면
			else
			{
				//2.5.2.1 시작하는 글자위치를 선택이 끝나는 글자위치로 정한다.
				startLetterIndex = selectedEndLetterPos;
				//2.5.2.2 끝나는 글자위치를 선택이 시작하는 글자위치로 정한다.
				endLetterIndex = selectedStartLetterPos;
				//2.5.2.3 시작하는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
				startRowIndex = selectedEndRowPos;
			}
			//2.5.3 시작하는 줄을 구한다.
			startRow = this->notepadForm->note->GetAt(startRowIndex);
			//2.5.4 시작하는 글자위치부터 끝나는 글자까지 복사한다.
			i = startLetterIndex;
			while (i < endLetterIndex)
			{
				//2.5.4.1 메모장의 시작하는 줄에서 줄에서 글자를 구한다.
				letter = startRow->GetAt(i);
				//2.5.4.2 클립보드의 새로 만든 줄(복사하는 줄)에 글자를 추가한다.
				copyRow->Add(letter->Clone());
				//2.5.4.3 시작하는 글자의 다음 글자로 이동해서 글자를 복사한다.
				i++;
			}
		}
		//2.6 선택이 시작하는 줄과 선택이 끝나는 줄이 서로 다르면(선택이 여러줄에 걸쳐져서 되어 있으면)
		else
		{
			//2.6.1 선택이 오른쪽으로 진행되었으면 
			if (selectedStartRowPos < selectedEndRowPos)
			{
				//2.6.1.1 시작하는 글자위치를 선택이 시작되는 글자위치로 정한다.
				startLetterIndex = selectedStartLetterPos;
				//2.6.1.2 끝나는 글자위치를 선택이 끝나는 글자위치로 정한다.
				endLetterIndex = selectedEndLetterPos;
				//2.6.1.3 시작하는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
				startRowIndex = selectedStartRowPos;
				//2.6.1.4 끝나는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
				endRowIndex = selectedEndRowPos;
			}
			//2.6.2 선택이 왼쪽으로 진행되었으면
			else
			{
				//2.6.2.1 시작하는 글자위치를 선택이 끝나는 글자위치로 정한다.
				startLetterIndex = selectedEndLetterPos;
				//2.6.2.2 끝나는 글자위치를 선택이 시작하는 글자위치로 정한다.
				endLetterIndex = selectedStartLetterPos;
				//2.6.2.3 시작하는 줄의 위치를 선택이 끝나는 줄의 위치로 정한다.
				startRowIndex = selectedEndRowPos;
				//2.6.2.4 끝나는 줄의 위치를 선택이 시작하는 줄의 위치로 정한다.
				endRowIndex = selectedStartRowPos;
			}
			Glyph* endRow = 0;//끝나는 줄의 위치
			//2.6.3 끝나는 줄보다 작은동안 반복한다.
			while (startRowIndex < endRowIndex)
			{
				//2.6.3.1 시작하는 줄을 구한다.
				startRow = this->notepadForm->note->GetAt(startRowIndex);
				//2.6.3.2 시작하는 글자위치부터 시작하는 줄의 마지막 글자까지 복사한다.
				i = startLetterIndex;
				while (i < startRow->GetLength())
				{
					//2.6.3.2.1 시작하는 줄에서 글자를 구한다.
					letter = startRow->GetAt(i);
					//2.6.3.2.2 클립보드의 새로 만든 줄(복사하는 줄)에 글자를 추가한다.
					copyRow->Add(letter->Clone());
					//2.6.3.2.3 시작하는 글자의 다음 글자로 이동해서 글자를 복사한다.
					i++;
				}
				//2.6.3.3 시작하는 줄의 위치를 증가시킨다.
				startRowIndex++;
				//2.6.3.4 줄이 다음 줄로 이동했기 때문에 글자위치를 0으로 초기화해준다.
				startLetterIndex = 0;
			}
			//2.6.4 메모장에서 끝나는 줄을 구한다.
			endRow = this->notepadForm->note->GetAt(startRowIndex);
			//2.6.5 글자 위치를 원위치시킨다.
			i = 0;
			//2.6.6 메모장의 가짜줄에서 마지막 글자까지 반복한다.
			while (i < endLetterIndex)
			{
				//2.6.6.1 메모장의 가짜줄에서 글자를 구한다.
				letter = endRow->GetAt(i);
				//2.6.6.2 클립보드의 새로 만든 줄(복사하는 줄)에 글자를 추가한다.
				copyRow->Add(letter->Clone());
				//2.6.6.3 다음 글자로 이동해서 글자를 복사한다.
				i++;
			}
		}
		//2.7 줄에 담은 글자를 구한다.
		string keyword = copyRow->GetContent();
		//2.8 선택된 글자를 에디트컨트롤에 붙여넣는다.
		this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->SetWindowText(keyword.c_str());
		//2.9 힙에 할당된 줄을 할당해제한다.
		if (copyRow != 0)
		{
			delete copyRow;
		}
	}
	//3. 끝내다.
	return FALSE;
}

//_stricmp;//대소문자 구분 무시하는 함수(영어 대소문자를 같게 보게 해주는 함수)

//2. 찾기 버튼을 클릭했을 때
void FindingDialog::OnFindButtonClicked()
{
	//1. GlyphFinder를 생성한다.
	GlyphFinder glyphFinder(this->notepadForm->note);
	//2. 에디트컨트롤에 적혀있는 글자를 읽는다.
	CString keyword;
	this->GetDlgItem(IDC_EDIT_FINDINGCONTENT)->GetWindowText(keyword);
	//3. 아래로 찾기를 실행한다.
	Long findingStartRowIndex = 0;
	Long findingStartLetterIndex = 0;
	Long findingEndRowIndex = 0;
	Long findingEndLetterIndex = 0;
	glyphFinder.FindDown((LPCTSTR)keyword, &findingStartRowIndex, &findingStartLetterIndex,
		&findingEndRowIndex, &findingEndLetterIndex);
	//4. 찾은 게 있으면
	if (findingStartRowIndex != findingEndRowIndex ||
		findingStartLetterIndex != findingEndLetterIndex)
	{
		//4.1 선택이 처음 시작되면
		if (this->notepadForm->isSelecting == false)
		{
			//4.1.1 선택이 진행되고 있는 중으로 상태를 바꾼다.
			this->notepadForm->isSelecting = true;
		}
		//4.2 이미 선택된 texts가 있으면
		else
		{
			//4.2.1 선택된 텍스트를 선택해제한다.(선택을 끝낸다)
			this->notepadForm->selectingTexts->Undo();
			//4.2.2 선택이 끝났기 때문에 캐럿의 x좌표를 0으로 저장한다.
			this->notepadForm->selectedStartXPos = 0;
			//4.2.3 선택이 끝났기 때문에 캐럿의 y좌표를 0으로 저장한다.
			this->notepadForm->selectedStartYPos = 0;
		}
		//4.3 선택이 시작되는 캐럿의 x좌표를 저장한다.
		this->notepadForm->selectedStartXPos = findingStartLetterIndex;
		//4.4 선택이 시작되는 캐럿의 y좌표를 저장한다.
		this->notepadForm->selectedStartYPos = findingStartRowIndex;
		//4.5 찾은 글자를 선택한다.
		this->notepadForm->selectingTexts->DoNext(findingStartRowIndex,
			findingStartLetterIndex, findingEndRowIndex, findingEndLetterIndex);
		//4.6 캐럿의 위치를 메모장의 찾은 문자열이 있는 줄의 찾은 문자열 마지막 글자위치로 이동한다.
		this->notepadForm->note->Move(findingEndRowIndex);
		this->notepadForm->current = this->notepadForm->note->GetAt(findingEndRowIndex);
		this->notepadForm->current->Move(findingEndLetterIndex);
		//4.7 선택이 되었기 때문에 복사하기, 잘라내기, 삭제 메뉴룰 활성화시켜준다.
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_ENABLED);
		this->notepadForm->GetMenu()->EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_ENABLED);
		//4.8 캐럿의 위치가 변경되었음을 알린다.
		this->notepadForm->Notify();
		//4.9 변경사항을 갱신한다.
		this->notepadForm->Invalidate(TRUE);
	}
	//5. 찾은게 없으면
	else
	{

		//2.1 메세지박스의 메세지를 생성한다.
		
		//2.3 SaveBox 메세지 내용을 만든다.
		//message.insert(0, "변경 내용을 ");

		//5.1 "찾을 수 없습니다." 메세지박스를 출력한다.
		string message = (LPCTSTR)keyword;
		message.insert(0, "\"");
		message += "\"";
		message += "을(를) 찾을 수 없습니다.";
		int messageBoxButton = MessageBox(message.c_str(), "메모장", MB_OK);
	}
}


//3.닫기버튼을 클릭했을 때
void FindingDialog::OnClose()
{
	this->EndDialog(0);
}

