#include "FileSaveDifferentNameCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "Glyph.h"
#include "File.h"
#include "afxdlgs.h"//CFileDialog�������


//����Ʈ������
FileSaveDifferentNameCommand::FileSaveDifferentNameCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Command����
void FileSaveDifferentNameCommand::Execute()
{
	//1. File�� �����Ѵ�.
	File file;
	//2. ���ϰ��� ��ȭ���ڸ� �����Ѵ�.
	LPCTSTR  filesFilter = _T("�ؽ�Ʈ ����(*.txt) | *.txt; | ��� ���� | *.*;  ||");
	CFileDialog fileDialog(FALSE, _T("txt"), "*.txt", 0, filesFilter, this->notepadForm, 0, 1);
	//3. ���ϰ��� ��ȭ���ڸ� ����Ѵ�.
	if (fileDialog.DoModal() == IDOK)
	{
		//3.1 ������ ������ �̸��� ���Ѵ�.
		this->notepadForm->fileName = string(fileDialog.GetFileTitle());
		//3.2 ������ ������ ��θ� ���Ѵ�.
		this->notepadForm->filePath = string(fileDialog.GetPathName());
	}
	//4. ������ �޸����� �����Ѵ�.
	file.Save(this->notepadForm, this->notepadForm->filePath);
	//5. �޸��忡 ��������� ������ �����Ѵ�.
	this->notepadForm->isDirty = false;
	//6. �޸��� ������ �ٲ۴�.
	string name = this->notepadForm->fileName;
	name += " - �޸���";
	this->notepadForm->SetWindowText(CString(name.c_str()));
}

//�Ҹ���
FileSaveDifferentNameCommand::~FileSaveDifferentNameCommand()
{

}