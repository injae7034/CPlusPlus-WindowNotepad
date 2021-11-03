#include "FileCloseAndOpenNewCommand.h"
#include "NotepadForm.h"
#include "File.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "PreviewForm.h"


//디폴트 생성자 정의
FileCloseAndOpenNewCommand::FileCloseAndOpenNewCommand(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//Execute
void FileCloseAndOpenNewCommand::Execute()
{
	//1. file을 생성한다.
	File file;
	int messageBoxButton = IDCANCEL;
	string name;
	//2. 메모장에 변경사항이 있으면
	if (this->notepadForm->isDirty == true)
	{
		//2.1 메세지박스의 메세지를 생성한다.
		string message = "제목 없음";
		//2.2 파일경로가 정해져 있으면
		if (this->notepadForm->filePath != "")
		{
			//2.2.1 메세지를 변경한다.
			message = this->notepadForm->filePath;
		}
		//2.3 SaveBox 메세지를 만든다.
		message.insert(0, "변경 내용을 ");
		message += "에 저장하시겠습니까?";
		//2.4 Save메세지박스를 출력한다.
		messageBoxButton = SaveMessageBox(this->notepadForm->GetSafeHwnd(), message.c_str(), "메모장", MB_YESNOCANCEL);
		//2.5 Save메세지박스에서 Yes를 선택했으면
		if (messageBoxButton == IDYES)
		{
			//2.5.1 노트가 이미 있던 파일이면
			if (this->notepadForm->filePath != "")
			{
				//2.5.1.1 메모장을 저장한다.
				file.Save(this->notepadForm, this->notepadForm->filePath);
				//2.5.1.2 메모장에 변경사항이 없음을 저장한다.
				this->notepadForm->isDirty = false;
				//2.5.1.3 메모장 제목을 바꾼다.
				string name = this->notepadForm->fileName;
				name += " - 메모장";
				this->notepadForm->SetWindowText(CString(name.c_str()));
			}
			//2.5.2 노트가 처음 생성된 파일이면
			else
			{
				//2.5.2.1 파일공통 대화상자를 생성한다.
				LPCTSTR  filesFilter = _T("텍스트 문서(*.txt) | *.txt; | 모든 파일 | *.*;  ||");
				CFileDialog fileDialog(FALSE, _T("txt"), "*.txt", 0, filesFilter, this->notepadForm, 0, 1);
				//2.5.2.2 파일공통 대화상자를 출력한다.
				if (fileDialog.DoModal() == IDOK)
				{
					//notepadForm의 fileName과 filePath가 private이면 내용을 변경할 수 없음
					//그렇다고 notepadForm의 생성자를 이용해 내용을 변경하려고하면
					//notepadForm을 새로 만들어야해서 안됨
					//결국에는 fileName과 filePath를 public으로 해줘야함.
					//2.5.2.2.1 저장할 파일의 이름을 구한다.
					this->notepadForm->fileName = string(fileDialog.GetFileTitle());
					//2.5.2.2.2 저장할 파일의 경로를 구한다.
					this->notepadForm->filePath = string(fileDialog.GetPathName());
				}
				//2.5.2.3 선택한 메모장을 저장한다.
				file.Save(this->notepadForm, this->notepadForm->filePath);
				//2.5.2.4 메모장에 변경사항이 없음을 저장한다.
				this->notepadForm->isDirty = false;
				//2.5.2.5 메모장 제목을 바꾼다.
				string name = this->notepadForm->fileName;
				name += " - 메모장";
				this->notepadForm->SetWindowText(CString(name.c_str()));
			}
		}
	}
	//3. 메세지박스에서 CANCEL을 선택하지 않았거나 변경된 사항이 없으면
	if (messageBoxButton != IDCANCEL || this->notepadForm->isDirty == false)
	{
		//3.2.1 저장할 파일의 이름을 구한다.
		this->notepadForm->fileName = "제목 없음";
		//3.2.2 저장할 파일의 경로를 구한다.
		this->notepadForm->filePath = "";
		//3.2.3 기존 메모장의 note를 지운다.
		//메모장에 note는 한개밖에 없다 따라서 다른 메모장을 불러오는 순간 기존note는 없애고
		//새로 불러오는 메모장의 note를 Load를 통해 불러오는 새로운 메모장에 출력해줘야함!!
		if (this->notepadForm->note != NULL)
		{
			delete this->notepadForm->note;
			this->notepadForm->note = NULL;
		}
		//3.2.4 클립보드를 지운다.
		if (this->notepadForm->clipboard != NULL)
		{
			delete this->notepadForm->clipboard;
			this->notepadForm->clipboard = NULL;
		}
		//3.2.5 CFindReplaceDialog를 지운다.
		if (this->notepadForm->findReplaceDialog != 0)
		{
			this->notepadForm->findReplaceDialog->DestroyWindow();
			this->notepadForm->findReplaceDialog = NULL;
		}
		//3.2.6 CommandHistory를 할당해제한다.
		if (this->notepadForm->commandHistory != 0)
		{
			delete this->notepadForm->commandHistory;
			this->notepadForm->commandHistory = NULL;
		}
		//3.2.7 CommandHistory를 생성한다.
		this->notepadForm->commandHistory = new CommandHistory(this->notepadForm);
		//3.2.8 PreviewForm을 지운다.
		if (this->notepadForm->previewForm != 0)
		{
			this->notepadForm->previewForm->SendMessage(WM_CLOSE);
			delete this->notepadForm->previewForm;
			this->notepadForm->previewForm = NULL;
		}
		//3.2.9 프린트정보를 할당해제한다.
		if (this->notepadForm->printInformation != 0)
		{
			delete this->notepadForm->printInformation;
			this->notepadForm->printInformation = NULL;
		}
		//3.2.10 페이지셋업정보를 지운다.
		if (this->notepadForm->pageSetUpInformation != 0)
		{
			delete this->notepadForm->pageSetUpInformation;
			this->notepadForm->pageSetUpInformation = NULL;
		}
		//불러오는 메모장을 위해 새로운 note를 만듬.
		//3.2.11 glyphCreator를 만든다.
		GlyphCreator glyphCreator;
		//2. 클립보드를 만든다.
		this->notepadForm->clipboard = glyphCreator.Create((char*)"clipboard");
		this->notepadForm->note = glyphCreator.Create((char*)"\0");
		//3.2.12 줄을 만든다.
		Glyph* row = glyphCreator.Create((char*)"\n");
		//3.2.13 줄을 메모장에 추가한다.
		Long rowIndex;
		rowIndex = this->notepadForm->note->Add(row);
		//3.2.14 현재 줄의 위치를 저장한다.
		this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
		//3.2.15 메모장 제목을 바꾼다.
		name = this->notepadForm->fileName;
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//3.2.16 현재 화면의 가로 길이를 저장한다.
		CRect rect;
		this->notepadForm->GetClientRect(&rect);
		this->notepadForm->previousPageWidth = rect.Width();
		//3.2.17 메뉴의 복사하기, 잘라내기, 삭제, 실행취소, 다시 실행 메뉴를 비활성화시킨다.
		this->notepadForm->GetMenu()->
			EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->
			EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->
			EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->
			EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->GetMenu()->
			EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->mouseRButtonMenu.
			EnableMenuItem(IDM_NOTE_COPY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->mouseRButtonMenu.
			EnableMenuItem(IDM_NOTE_CUT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->mouseRButtonMenu.
			EnableMenuItem(IDM_NOTE_REMOVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->mouseRButtonMenu.
			EnableMenuItem(IDM_NOTE_UNDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		this->notepadForm->mouseRButtonMenu.
			EnableMenuItem(IDM_NOTE_REDO, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		//3.2.18 flag들을 초기화시킨다.
		this->notepadForm->isComposing = false;//false로 초기화시킴
		this->notepadForm->isDirty = false;//false로 초기화시킴
		this->notepadForm->isSelecting = false;//false로 초기화시킴
		this->notepadForm->isMouseLButtonClicked = false;//false로 초기화 시킴.
		this->notepadForm->selectedStartXPos = 0;//처음생성될때는 선택된 texts가 없기 때문에 0으로 초기화해줌
		this->notepadForm->selectedStartYPos = 0;//처음생성될때는 선택된 texts가 없기 때문에 0으로 초기화해줌
	}
}

//소멸자 정의
FileCloseAndOpenNewCommand::~FileCloseAndOpenNewCommand()
{

}