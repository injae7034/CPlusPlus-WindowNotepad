#include "FileSaveCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "Glyph.h"
#include "File.h"
#include "afxdlgs.h"//CFileDialog헤더파일


//디폴트생성자
FileSaveCommand::FileSaveCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	
}

//Command 패턴
void FileSaveCommand::Execute()
{
	//1. 파일을 생성한다.
	File file;
	//2. 노트가 이미 있던 파일이면
	if (this->notepadForm->filePath != "")
	{
		//2.1 메모장을 저장한다.
		file.Save(this->notepadForm, this->notepadForm->filePath);
		//2.2 메모장에 변경사항이 없음을 저장한다.
		this->notepadForm->IsDirty = false;
		//2.3 메모장 제목을 바꾼다.
		string name = this->notepadForm->fileName;
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
	}
	//3. 노트가 처음 생성된 파일이면
	else
	{
		//3.1 파일공통 대화상자를 생성한다.
		LPCTSTR  filesFilter = _T("텍스트 문서(*.txt) | *.txt; | 모든 파일 | *.*;  ||");
		CFileDialog fileDialog(FALSE, _T("txt"), "*.txt", 0, filesFilter, this->notepadForm, 0, 1);
		//3.2 파일공통 대화상자를 출력한다.
		if (fileDialog.DoModal() == IDOK)
		{
			//3.2.1 저장할 파일의 이름을 구한다.
			this->notepadForm->fileName = string(fileDialog.GetFileTitle());
			//3.2.2 저장할 파일의 경로를 구한다.
			this->notepadForm->filePath = string(fileDialog.GetPathName());
		}
		//3.3 선택한 메모장을 저장한다.
		file.Save(this->notepadForm, this->notepadForm->filePath);
		//3.4 메모장에 변경사항이 없음을 저장한다.
		this->notepadForm->IsDirty = false;
		//3.5 메모장 제목을 바꾼다.
		string name = this->notepadForm->fileName;
		name += " - 메모장";
		this->notepadForm->SetWindowText(CString(name.c_str()));
	}
}

//소멸자
FileSaveCommand::~FileSaveCommand()
{

}