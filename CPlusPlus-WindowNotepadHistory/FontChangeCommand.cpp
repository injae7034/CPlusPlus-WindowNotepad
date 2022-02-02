#include "FontChangeCommand.h"
#include "NotepadForm.h"
#include "GlyphCreator.h"
#include "Glyph.h"
#include "File.h"
#include "afxdlgs.h"//CFileDialog헤더파일

//디폴트생성자
FontChangeCommand::FontChangeCommand(NotepadForm* notepadForm)
	:Command(notepadForm)
{

}

//Command패턴
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
        //2.1 폰트대화상자에서 선택한 글꼴의 정보를 얻는다.
        fontDialog.GetCurrentFont(&logFont);
        //2.2 폰트대화상자에서 선택한 글꼴의 색을 얻는다.
        selectedColor = fontDialog.GetColor();
        //2.3 notepadForm의 font에 선택한 글꼴의 정보를 저장한다.
        this->notepadForm->font = Font(logFont, selectedColor);
        //2.4 캐럿을 다시 생성한다.
        //2.4.1 CClientDC를 생성한다.
        CClientDC dc(this->notepadForm);
        //2.4.2 CFont를 생성한다.
        CFont font;
        //2.4.3 글씨크기와 글씨체를 정하다.
        //font.CreatePointFont(this->notepadForm->GetFont().GetSize(), this->notepadForm->GetFont().GetFaceName().c_str());
        font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
        //2.4.5 폰트를 dc에 지정한다.
        dc.SelectObject(font);
        //2.4.6 TEXTMETRIC을 생성한다.
        TEXTMETRIC text;
        //2.4.7 글꼴의 정보를 얻는다.
        dc.GetTextMetrics(&text);
        //2.4.8 캐럿을 생성한다.
        this->notepadForm->GetCaret().Create(0, text.tmHeight);
        //2.4.9 현재줄의 텍스트들을 저장한다.
        CString letter = CString(this->notepadForm->current->GetContent().c_str());
        //2.4.10 현재줄의 텍스트들의 size를 구한다.
        CSize letterSize = dc.GetTextExtent(letter);
        //2.4.11 캐럿을 이동시킨다.
        this->notepadForm->GetCaret().Move(letterSize.cx, (this->notepadForm->note->GetCurrent() - 1)* text.tmHeight);
        //2.4.12 캐럿을 보이게 한다.
        this->notepadForm->GetCaret().Show();
        //2.4.13 변경사항을 갱신한다.
        this->notepadForm->Invalidate(TRUE);
    }
}

//소멸자
FontChangeCommand::~FontChangeCommand()
{

}