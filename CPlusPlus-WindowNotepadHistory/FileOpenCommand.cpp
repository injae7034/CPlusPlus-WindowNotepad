#include "FileOpenCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "Glyph.h"
#include "File.h"
#include "afxdlgs.h"//CFileDialog�������

//����Ʈ������
FileOpenCommand::FileOpenCommand(NotepadForm* notepadForm) 
	:Command(notepadForm)
{
	
}

//Command ����
void FileOpenCommand::Execute()
{
	//1. file�� �����Ѵ�.
	File file;
	int messageBoxButton = IDCANCEL;
	string name;
	//2. �޸��忡 ��������� ������
	if (this->notepadForm->isDirty == true)
	{
		//2.1 �޼����ڽ��� �޼����� �����Ѵ�.
		string message = "���� ����";
		//2.2 ���ϰ�ΰ� ������ ������
		if (this->notepadForm->filePath != "")
		{
			//2.2.1 �޼����� �����Ѵ�.
			message = this->notepadForm->filePath;
		}
		//2.3 SaveBox �޼����� �����.
		message.insert(0, "���� ������ ");
		message += "�� �����Ͻðڽ��ϱ�?";
		//2.4 Save�޼����ڽ��� ����Ѵ�.
		messageBoxButton = SaveMessageBox(this->notepadForm->GetSafeHwnd(), message.c_str(), "�޸���", MB_YESNOCANCEL);
		//2.5 Save�޼����ڽ����� Yes�� ����������
		if (messageBoxButton == IDYES)
		{
			//2.5.1 ��Ʈ�� �̹� �ִ� �����̸�
			if (this->notepadForm->filePath != "")
			{
				//2.5.1.1 �޸����� �����Ѵ�.
				file.Save(this->notepadForm, this->notepadForm->filePath);
				//2.5.1.2 �޸��忡 ��������� ������ �����Ѵ�.
				this->notepadForm->isDirty = false;
				//2.5.1.3 �޸��� ������ �ٲ۴�.
				string name = this->notepadForm->fileName;
				name += " - �޸���";
				this->notepadForm->SetWindowText(CString(name.c_str()));
			}
			//2.5.2 ��Ʈ�� ó�� ������ �����̸�
			else
			{
				//2.5.2.1 ���ϰ��� ��ȭ���ڸ� �����Ѵ�.
				LPCTSTR  filesFilter = _T("�ؽ�Ʈ ����(*.txt) | *.txt; | ��� ���� | *.*;  ||");
				CFileDialog fileDialog(FALSE, _T("txt"), "*.txt", 0, filesFilter, this->notepadForm, 0, 1);
				//2.5.2.2 ���ϰ��� ��ȭ���ڸ� ����Ѵ�.
				if (fileDialog.DoModal() == IDOK)
				{
					//notepadForm�� fileName�� filePath�� private�̸� ������ ������ �� ����
					//�׷��ٰ� notepadForm�� �����ڸ� �̿��� ������ �����Ϸ����ϸ�
					//notepadForm�� ���� �������ؼ� �ȵ�
					//�ᱹ���� fileName�� filePath�� public���� �������.
					//2.5.2.2.1 ������ ������ �̸��� ���Ѵ�.
					this->notepadForm->fileName = string(fileDialog.GetFileTitle());
					//2.5.2.2.2 ������ ������ ��θ� ���Ѵ�.
					this->notepadForm->filePath = string(fileDialog.GetPathName());
				}
				//2.5.2.3 ������ �޸����� �����Ѵ�.
				file.Save(this->notepadForm, this->notepadForm->filePath);
				//2.5.2.4 �޸��忡 ��������� ������ �����Ѵ�.
				this->notepadForm->isDirty = false;
				//2.5.2.5 �޸��� ������ �ٲ۴�.
				string name = this->notepadForm->fileName;
				name += " - �޸���";
				this->notepadForm->SetWindowText(CString(name.c_str()));
			}
		}
	}
	//3. �޼����ڽ����� CANCEL�� �������� �ʾҰų� ����� ������ ������
	if (messageBoxButton != IDCANCEL || this->notepadForm->isDirty == false)
	{
		//3.1 ���ϰ��� ��ȭ���ڸ� �����Ѵ�.
		LPCTSTR  filesFilter = _T("�ؽ�Ʈ ����(*.txt) | *.txt; | ��� ���� | *.*;  ||");
		CFileDialog fileDialog(TRUE, _T("txt"), NULL, 0, filesFilter, this->notepadForm, 0, 1);
		//3.2 ���ϰ��� ��ȭ���ڸ� ����Ѵ�.
		if (fileDialog.DoModal() == IDOK)
		{
			//3.2.1 ������ ������ �̸��� ���Ѵ�.
			this->notepadForm->fileName = string(fileDialog.GetFileTitle());
			//3.2.2 ������ ������ ��θ� ���Ѵ�.
			this->notepadForm->filePath = string(fileDialog.GetPathName());
			//3.2.3 ���� �޸����� note�� �����.
			//�޸��忡 note�� �Ѱ��ۿ� ���� ���� �ٸ� �޸����� �ҷ����� ���� ����note�� ���ְ�
			//���� �ҷ����� �޸����� note�� Load�� ���� �ҷ����� ���ο� �޸��忡 ����������!!
			if (this->notepadForm->note != NULL)
			{
				delete this->notepadForm->note;
				//this->note�� �Ҹ��Ű�� note�� �ִ� Row�� letter�鵵 �� �Ҹ�ȴ�.
				//this->current�� Row�ε� �̹� this->note�� �Ҹ��Ű�鼭 Row���� �� �Ҹ�Ǿ��µ�
				//�� Row�� �Ҹ��϶�� �ϸ� �Ҹ��Ұ� ���µ� �Ҹ��϶�� �߱⶧���� ���̳���.!!!!!
				//delete this->current;
			}
			//�ҷ����� �޸����� ���� ���ο� note�� ����.
			//3.2.4 glyphCreator�� �����.
			GlyphCreator glyphCreator;
			this->notepadForm->note = glyphCreator.Create((char*)"\0");
			//3.2.6 ���� �����.
			Glyph* row = glyphCreator.Create((char*)"\n");
			//3.2.7 ���� �޸��忡 �߰��Ѵ�.
			Long rowIndex;
			rowIndex = this->notepadForm->note->Add(row);
			//3.2.8 ���� ���� ��ġ�� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
			//3.2.9 ������ �޸����� ��Ʈ(����)�� �ҷ��´�.
			file.Load(this->notepadForm, this->notepadForm->filePath);
			//3.2.10 �޸��� ������ �ٲ۴�.
			name = this->notepadForm->fileName;
			name += " - �޸���";
			this->notepadForm->SetWindowText(CString(name.c_str()));
			//3.2.11 flag���� �ʱ�ȭ��Ų��.
			this->notepadForm->isComposing = false;//false�� �ʱ�ȭ��Ŵ
			this->notepadForm->isDirty = false;//false�� �ʱ�ȭ��Ŵ
			//3.3.12 ĳ���� ���� ���� ��ġ�� ���� ó������ ������.
			rowIndex = this->notepadForm->note->First();
			//3.3.13 ���� ���� ��ġ�� �ٽ� �����Ѵ�.
			this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
			//3.3.14 ĳ���� ���� ���� ��ġ�� ���� ó������ ������.
			this->notepadForm->current->First();
			//3.2.15 ĳ���� ��ġ�� ũ�Ⱑ ����Ǿ����� �˸���.
			this->notepadForm->Notify();
			//3.2.16 �����Ѵ�.
			this->notepadForm->Invalidate(TRUE);
		}
	}
}

//�Ҹ���
FileOpenCommand::~FileOpenCommand()
{

}