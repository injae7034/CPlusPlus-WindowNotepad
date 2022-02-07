#include "FilePrintingCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog�������
#include "afxext.h"//CPrintInfo�������
#include "afxwin.h"//CView�������

//������
FilePrintingCommand::FilePrintingCommand(NotepadForm* notepadForm)
{

}

//Execute
void FilePrintingCommand::Execute()
{
    // Display the Windows Print dialog box with "All" radio button 
    // initially selected. All other radio buttons are disabled.
    CPrintDialog dlg(FALSE, PD_ALLPAGES | PD_COLLATE | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE);
    if (dlg.DoModal() == IDOK)
    {

       // CString str;
       // str.Format("Device Name : %s\r\nDriver Name : %s\r\nPortName : % s\r\nCopiesNum : % d",
       //     dlg.GetDeviceName(), dlg.GetDriverName(), dlg.GetPortName(), dlg.GetCopies());

        //MessageBox((HWND)str, "������ �������");

        // Create a printer device context (DC) based on the information
        // selected from the Print dialog.
        HDC hdc = dlg.CreatePrinterDC();
        ASSERT(hdc);
    }

}

//�Ҹ���
FilePrintingCommand::~FilePrintingCommand()
{

}