#include "FileSetupCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog�������
#include "PageSetUpDialog.h"
#include "PrintInformation.h"

//������
FileSetupCommand::FileSetupCommand(NotepadForm* notepadForm)
{

}

//Execute
void FileSetupCommand::Execute()
{
    PageSetupDialog pageSetupDialog(this->notepadForm);

    LONG id;
    id = pageSetupDialog.DoModal();
    if (id == IDOK)
    {
        //2.2 ����Ʈ ���̾�α׿��� hdc�� ���Ѵ�.
        HDC hdc = this->notepadForm->printInformation->GetHDC();
        ASSERT(hdc);
        //2.3 ����Ʈ ���̾�α��� hdc���� cdc�� ���Ѵ�.
        CDC* cdc = CDC::FromHandle(hdc);
           // ->SetDC(pageSetupDialog.CreatePrinterDC());
    }

    GlobalFree(pageSetupDialog.m_psd.hDevMode);
    GlobalFree(pageSetupDialog.m_psd.hDevNames);

#if 0
    CPageSetupDialog dlg(PSD_MARGINS | PSD_INHUNDREDTHSOFMILLIMETERS);

    // Initialize margins
    dlg.m_psd.rtMargin.top = 1000;
    dlg.m_psd.rtMargin.left = 1250;
    dlg.m_psd.rtMargin.right = 1250;
    dlg.m_psd.rtMargin.bottom = 1000;
    //dlg.m_psd.lpfnPagePaintHook = (LPPAGEPAINTHOOK)PaintHook;

    if (IDOK == dlg.DoModal())
    {
        // Propagate changes to the app
        AfxGetApp()->SelectPrinter(dlg.m_psd.hDevNames, dlg.m_psd.hDevMode);
    }
    else
    {
        TRACE(_T("CommDlgExtendedError returned error %d from ")
            _T("CPageSetupDialog::DoModal().\n"),
            (int)CommDlgExtendedError());
    }
#endif
}

//�Ҹ���
FileSetupCommand::~FileSetupCommand()
{

}