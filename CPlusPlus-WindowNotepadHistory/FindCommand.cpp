#include "FindCommand.h"
#include "FindingDialog.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog헤더파일

//디폴트생성자
FindCommand::FindCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Execute
void FindCommand::Execute()
{
	// 찾기 프레임 윈도우를 띄운다.
	// 찾기 프레임 윈도우는 무조건 힙에 할당해서 생성해줘야 한다.
	// NotepadForm의 멤버인 findDialog에 FindDialog를 할당한다.
	FindingDialog *findingDialog= new FindingDialog(this->notepadForm);
	// 생성된 찾기 프레임 윈도우를 좀 더 구체적으로 설정해준다.
	findingDialog->Create(TRUE, _T(""), _T(""), FR_DOWN, this->notepadForm);
	//Modal 대화상자는 DoModal로 화면을 띄우고, Modaless 대화상자는 ShowWindow로 화면을 띄운다.
	findingDialog->ShowWindow(SW_SHOW);

}

//소멸자
FindCommand::~FindCommand()
{

}