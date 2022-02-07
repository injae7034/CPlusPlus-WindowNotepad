#include "FileSetupCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog�������

//������
FileSetupCommand::FileSetupCommand(NotepadForm* notepadForm)
{

}

//Execute
void FileSetupCommand::Execute()
{
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
}

//�Ҹ���
FileSetupCommand::~FileSetupCommand()
{

}