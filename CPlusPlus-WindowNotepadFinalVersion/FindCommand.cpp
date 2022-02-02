#include "FindCommand.h"
#include "FindingDialog.h"
#include "ReplacingDialog.h"
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
	// 찾기 프레임 윈도우를 띄우기 전에 찾기 프레암 윈도우가 있는지 확인하고 있으면 할당해제한다.
	if (this->notepadForm->findReplaceDialog != 0)
	{
		//CFindReplaceDialog를 할당해제할때는 delete대신에 DestroyWindow를 이용하자!
		this->notepadForm->findReplaceDialog->DestroyWindow();
		//delete this->notepadForm->findingDialog;
		//댕글링 포인터를 0으로 안바꿔주면 할당해제가 됬는데 다시 할당해제를 하러 들어와서 에러가난다.
		this->notepadForm->findReplaceDialog = 0;
	}

	// 찾기 프레임 윈도우를 띄운다.
	// 찾기 프레임 윈도우는 무조건 힙에 할당해서 생성해줘야 한다.
	// NotepadForm의 멤버인 findDialog에 FindDialog를 할당한다.
	this->notepadForm->findReplaceDialog = new FindingDialog(this->notepadForm);
	// 생성된 찾기 프레임 윈도우를 좀 더 구체적으로 설정해준다.
	this->notepadForm->findReplaceDialog->Create
	(TRUE, _T(""), _T(""), FR_DOWN, this->notepadForm);
	//Modal 대화상자는 DoModal로 화면을 띄우고, Modaless 대화상자는 ShowWindow로 화면을 띄운다.
	this->notepadForm->findReplaceDialog->ShowWindow(SW_SHOW);
}

//소멸자
FindCommand::~FindCommand()
{

}