#include "FileSaveDifferentNameCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "Glyph.h"
#include "File.h"
#include "afxdlgs.h"//CFileDialog헤더파일


//디폴트생성자
FileSaveDifferentNameCommand::FileSaveDifferentNameCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Command패턴
void FileSaveDifferentNameCommand::Execute()
{
	//1. File을 생성한다.
	File file;
	//2. 파일공통 대화상자를 생성한다.
	LPCTSTR  filesFilter = _T("텍스트 문서(*.txt) | *.txt; | 모든 파일 | *.*;  ||");
	CFileDialog fileDialog(FALSE, _T("txt"), "*.txt", 0, filesFilter, this->notepadForm, 0, 1);
	//3. 파일공통 대화상자를 출력한다.
	if (fileDialog.DoModal() == IDOK)
	{
		//3.1 저장할 파일의 이름을 구한다.
		this->notepadForm->fileName = string(fileDialog.GetFileTitle());
		//3.2 저장할 파일의 경로를 구한다.
		this->notepadForm->filePath = string(fileDialog.GetPathName());
	}
	//4. 선택한 메모장을 저장한다.
	file.Save(this->notepadForm, this->notepadForm->filePath);
	//5. 메모장에 변경사항이 없음을 저장한다.
	this->notepadForm->isDirty = false;
	//6. 메모장 제목을 바꾼다.
	string name = this->notepadForm->fileName;
	name += " - 메모장";
	this->notepadForm->SetWindowText(CString(name.c_str()));
}

//소멸자
FileSaveDifferentNameCommand::~FileSaveDifferentNameCommand()
{

}