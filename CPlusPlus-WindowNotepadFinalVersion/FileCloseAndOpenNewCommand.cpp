#include "FileCloseAndOpenNewCommand.h"
#include "NotepadForm.h"
#include "File.h"
#include "GlyphCreator.h"
#include "CommandHistory.h"
#include "PreviewForm.h"


//����Ʈ ������ ����
FileCloseAndOpenNewCommand::FileCloseAndOpenNewCommand(NotepadForm* notepadForm)
{
	this->notepadForm = notepadForm;
}

//Execute
void FileCloseAndOpenNewCommand::Execute()
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
		//3.2.1 ������ ������ �̸��� ���Ѵ�.
		this->notepadForm->fileName = "���� ����";
		//3.2.2 ������ ������ ��θ� ���Ѵ�.
		this->notepadForm->filePath = "";
		//3.2.3 ���� �޸����� note�� �����.
		//�޸��忡 note�� �Ѱ��ۿ� ���� ���� �ٸ� �޸����� �ҷ����� ���� ����note�� ���ְ�
		//���� �ҷ����� �޸����� note�� Load�� ���� �ҷ����� ���ο� �޸��忡 ����������!!
		if (this->notepadForm->note != NULL)
		{
			delete this->notepadForm->note;
			this->notepadForm->note = NULL;
		}
		//3.2.4 Ŭ�����带 �����.
		if (this->notepadForm->clipboard != NULL)
		{
			delete this->notepadForm->clipboard;
			this->notepadForm->clipboard = NULL;
		}
		//3.2.5 CFindReplaceDialog�� �����.
		if (this->notepadForm->findReplaceDialog != 0)
		{
			this->notepadForm->findReplaceDialog->DestroyWindow();
			this->notepadForm->findReplaceDialog = NULL;
		}
		//3.2.6 CommandHistory�� �Ҵ������Ѵ�.
		if (this->notepadForm->commandHistory != 0)
		{
			delete this->notepadForm->commandHistory;
			this->notepadForm->commandHistory = NULL;
		}
		//3.2.7 CommandHistory�� �����Ѵ�.
		this->notepadForm->commandHistory = new CommandHistory(this->notepadForm);
		//3.2.8 PreviewForm�� �����.
		if (this->notepadForm->previewForm != 0)
		{
			this->notepadForm->previewForm->SendMessage(WM_CLOSE);
			delete this->notepadForm->previewForm;
			this->notepadForm->previewForm = NULL;
		}
		//3.2.9 ����Ʈ������ �Ҵ������Ѵ�.
		if (this->notepadForm->printInformation != 0)
		{
			delete this->notepadForm->printInformation;
			this->notepadForm->printInformation = NULL;
		}
		//3.2.10 �������¾������� �����.
		if (this->notepadForm->pageSetUpInformation != 0)
		{
			delete this->notepadForm->pageSetUpInformation;
			this->notepadForm->pageSetUpInformation = NULL;
		}
		//�ҷ����� �޸����� ���� ���ο� note�� ����.
		//3.2.11 glyphCreator�� �����.
		GlyphCreator glyphCreator;
		//2. Ŭ�����带 �����.
		this->notepadForm->clipboard = glyphCreator.Create((char*)"clipboard");
		this->notepadForm->note = glyphCreator.Create((char*)"\0");
		//3.2.12 ���� �����.
		Glyph* row = glyphCreator.Create((char*)"\n");
		//3.2.13 ���� �޸��忡 �߰��Ѵ�.
		Long rowIndex;
		rowIndex = this->notepadForm->note->Add(row);
		//3.2.14 ���� ���� ��ġ�� �����Ѵ�.
		this->notepadForm->current = this->notepadForm->note->GetAt(rowIndex);
		//3.2.15 �޸��� ������ �ٲ۴�.
		name = this->notepadForm->fileName;
		name += " - �޸���";
		this->notepadForm->SetWindowText(CString(name.c_str()));
		//3.2.16 ���� ȭ���� ���� ���̸� �����Ѵ�.
		CRect rect;
		this->notepadForm->GetClientRect(&rect);
		this->notepadForm->previousPageWidth = rect.Width();
		//3.2.17 �޴��� �����ϱ�, �߶󳻱�, ����, �������, �ٽ� ���� �޴��� ��Ȱ��ȭ��Ų��.
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
		//3.2.18 flag���� �ʱ�ȭ��Ų��.
		this->notepadForm->isComposing = false;//false�� �ʱ�ȭ��Ŵ
		this->notepadForm->isDirty = false;//false�� �ʱ�ȭ��Ŵ
		this->notepadForm->isSelecting = false;//false�� �ʱ�ȭ��Ŵ
		this->notepadForm->isMouseLButtonClicked = false;//false�� �ʱ�ȭ ��Ŵ.
		this->notepadForm->selectedStartXPos = 0;//ó�������ɶ��� ���õ� texts�� ���� ������ 0���� �ʱ�ȭ����
		this->notepadForm->selectedStartYPos = 0;//ó�������ɶ��� ���õ� texts�� ���� ������ 0���� �ʱ�ȭ����
	}
}

//�Ҹ��� ����
FileCloseAndOpenNewCommand::~FileCloseAndOpenNewCommand()
{

}