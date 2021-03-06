#include "FontChangeCommand.h"
#include "NotepadForm.h"
#include "TextExtent.h"
#include "afxdlgs.h"//CCommonDialog헤더파일

//디폴트생성자
FontChangeCommand::FontChangeCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

void FontChangeCommand::Execute()
{
    //1. this->notepadForm->font의 글꼴을 바탕으로 fontDialog를 생성한다.
    CFontDialog fontDialog(&this->notepadForm->font.GetLogFont(),
        CF_EFFECTS | CF_SCREENFONTS, NULL, NULL);
    COLORREF selectedColor;
    LOGFONT logFont;
    //logFont = LOGFONT(10, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, 0, this->notepadForm->GetFont().GetFaceName());
    //2. this->notepadForm->font의 색깔이 반영이 안되어있기 때문에
    //fontDialog 대화상자를 출력하기전에 this->notepadForm->font의 색깔을 반영한다.
    //m_cf가 Dialog생성자에서 미처 설정하지 못한 것들을 세세하게 설정할 수 있도록 도와줌!
    fontDialog.m_cf.rgbColors = this->notepadForm->font.GetColor();
    //3. this->notepadForm->font의 글꼴과 색을 바탕으로 fontDialog를 출력한다.
    if (fontDialog.DoModal() == IDOK)
    {
        //현재 클래스에서 다른 클래스 멤버의 내용을 얻어올때는 인라인함수 Get을 이용하면됨
        //현재 클래스에서 다른 클래스 멤버의 내용을 변경할때는 그 다른 클래스의 생성자를 호출해서
        //원하는 내용으로 변경시키면 됨!!!
        //NotepadForm의 Font의 멤버에 내용을 변경하고 싶으면 현재 여기서
        //NotepadForm의 Font의 생성자를 호출해서 내용을 변경하면 된다.
        //주소록의 Correct에서 사용한 원리와 같음!!!
        //3.1 폰트대화상자에서 선택한 글꼴의 정보를 얻는다.
        //fontDialog.GetSize();
        fontDialog.GetCurrentFont(&logFont);
        //3.2 폰트대화상자에서 선택한 글꼴의 색을 얻는다.
        selectedColor = fontDialog.GetColor();
        //3.3 notepadForm의 font에 선택한 글꼴의 정보를 저장한다.
        //private멤버의 내용에 접근하고 싶으면 인라인함수(Get)를 이용하고
        //private멤버의 내용을 변경하고 싶으면 생성자를 이용한다!
        this->notepadForm->font = Font(logFont, fontDialog.GetSize(), selectedColor);
        //3.4 기존 글꼴 정보를 가지고 있는 TextExtent를 할당해제(소멸)해준다.
        if (this->notepadForm->textExtent != NULL)
        {
            delete this->notepadForm->textExtent;
        }
        //3.5 새로 선택한 글꼴 정보를 반영한 TextExtent를 힙에 할당(생성)한다.
        this->notepadForm->textExtent = new TextExtent(this->notepadForm);
        //3.6 캐럿의 크기와 위치가 변경되었음을 알린다.
        //글자크기가 바꼈기 때문에 여기서 스크롤을 조절해줘야한다!!!
        this->notepadForm->Notify();
        //4.5 자동 줄 바꿈 메뉴가 체크되어 있으면
        if (this->notepadForm->isRowAutoChanging == true)
        {
            //4.5.1 OnSize로 메세지가 가지 않기 때문에 OnSize로 가는 메세지를 보내서
            //OnSize에서 부분자동개행을 하도록 한다. 
            //전체 글꼴과 크기가 바뀌기 때문에 사이즈를 의도적으로 변경해서 전체자동개행이 이뤄지도록 유도함
            this->notepadForm->previousPageWidth = -1;
            this->notepadForm->SendMessage(WM_SIZE);
        }
    }
}

//소멸자
FontChangeCommand::~FontChangeCommand()
{

}