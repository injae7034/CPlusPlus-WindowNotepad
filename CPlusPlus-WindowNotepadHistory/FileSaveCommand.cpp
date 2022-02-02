#include "FileSaveCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "Glyph.h"
#include "File.h"
#include "afxdlgs.h"//CFileDialog�������


//����Ʈ������
FileSaveCommand::FileSaveCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{
	
}

//Command ����
void FileSaveCommand::Execute()
{
	//1. ������ �����Ѵ�.
	File file;
	//2. ��Ʈ�� �̹� �ִ� �����̸�
	if (this->notepadForm->filePath != "")
	{
		//2.1 �޸����� �����Ѵ�.
		file.Save(this->notepadForm, this->notepadForm->filePath);
		//2.2 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->IsDirty = false;
		//2.3 �޸��� ������ �ٲ۴�.
		string name = this->notepadForm->fileName;
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
	}
	//3. ��Ʈ�� ó�� ������ �����̸�
	else
	{
		//3.1 ���ϰ��� ��ȭ���ڸ� �����Ѵ�.
		LPCTSTR  filesFilter = _T("�ؽ�Ʈ ����(*.txt) | *.txt; | ��� ���� | *.*;  ||");
		CFileDialog fileDialog(FALSE, _T("txt"), "*.txt", 0, filesFilter, this->notepadForm, 0, 1);
		//3.2 ���ϰ��� ��ȭ���ڸ� ����Ѵ�.
		if (fileDialog.DoModal() == IDOK)
		{
			//3.2.1 ������ ������ �̸��� ���Ѵ�.
			this->notepadForm->fileName = string(fileDialog.GetFileTitle());
			//3.2.2 ������ ������ ��θ� ���Ѵ�.
			this->notepadForm->filePath = string(fileDialog.GetPathName());
		}
		//3.3 ������ �޸����� �����Ѵ�.
		file.Save(this->notepadForm, this->notepadForm->filePath);
		//3.4 �޸��忡 ��������� ������ �����Ѵ�.
		this->notepadForm->IsDirty = false;
		//3.5 �޸��� ������ �ٲ۴�.
		string name = this->notepadForm->fileName;
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
	}
}

//�Ҹ���
FileSaveCommand::~FileSaveCommand()
{

}