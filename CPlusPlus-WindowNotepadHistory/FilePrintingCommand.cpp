#include "FilePrintingCommand.h"
#include "NotepadForm.h"
#include "afxdlgs.h"//CCommonDialog헤더파일
#include "afxext.h"//CPrintInfo헤더파일
#include "afxwin.h"//CView헤더파일

//생성자
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

        //MessageBox((HWND)str, "프린터 출력정보");

        // Create a printer device context (DC) based on the information
        // selected from the Print dialog.
        HDC hdc = dlg.CreatePrinterDC();
        ASSERT(hdc);
    }

}

//소멸자
FilePrintingCommand::~FilePrintingCommand()
{

}