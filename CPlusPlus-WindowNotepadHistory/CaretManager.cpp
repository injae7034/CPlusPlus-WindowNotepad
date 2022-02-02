#include "CaretManager.h"
#include "NotepadForm.h"
#include "Caret.h"
#include "Subject.h"
#include "Glyph.h"

//디폴트생성자
CaretManager::CaretManager(NotepadForm* notepadForm)
    : Observer(notepadForm)
{
    this->notepadForm = notepadForm;
    this->caret = new Caret(this);
    //CaretMaker를 ObserverList에 추가한다.
    this->subject->Attach(this);
}

//Update
void CaretManager::Update()
{
    CString letter;
    CSize letterSize;
    letterSize.cx = 0;
    //캐럿 위치랑 크기가 변경되었기 때문에 그에 맞게 다시 생성한다.
    //1. CClientDC를 생성한다.
    CClientDC dc(this->notepadForm);
    //2. CFont를 생성한다.
    CFont font;
    //3. 글씨크기와 글씨체를 정하다.
    font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
    //4 폰트를 dc에 지정한다.
    dc.SelectObject(font);
    //5. TEXTMETRIC을 생성한다.
    TEXTMETRIC textmetric;
    //6. 글꼴의 정보를 얻는다.
    dc.GetTextMetrics(&textmetric);
    //7. 현재줄의 텍스트들을 저장한다.
    //현재 줄의 글자 전체를 구하고 있기 때문에 캐럿의 현재 가로 위치는 항상 현재 줄의 전체길이에 위치함.
    //CString rowText = CString(this->notepadForm->current->GetContent().c_str());
    //위의 코드대로 하면 캐럿의 이동이 반영되지 못하고 글자가 추가될때 캐럿의 이동만 반영함.
    //NotepadForm의 OnKeyDown에서 키보드 동작을 인식해서 note의 캐럿의 현재 가로위치와
    //캐럿의 현재 새로 위치를 이동시키고 있다 이를 반영해야하는데 여기서 내가 알 수 있는 것은
    //current(캐럿의 현재 가로위치와 현재 새로위치)가 변경되었고 그 변경된 값을 알 수 있다.
    //이를 활용해서 새로운 함수를 만들어내면 된다.
    //우선 키보트 left와 right을 반영하려면 현재 줄의 current가 증가되고 감소되는것을 알수있는데
    //이를 활용해서 현재 줄의 글자를 string에 다 담지 말고(GetContent) 
    //current보다 작은동안만 string에 담아서 출력하면 된다.(GetPartOfContent)
    CString rowText = CString(this->notepadForm->current->
        GetPartOfContent(this->notepadForm->current->GetCurrent()).c_str());
    //8. 현재줄의 텍스트들의 size를 구한다.
    CSize rowTextSize = dc.GetTextExtent(rowText);
    //9. 한글이 조립중이면
    if (this->notepadForm->IsComposing == true)
    {
        //9.1 새로 추가한 한글 한 글자의 size를 구한다.
        letter = CString(this->notepadForm->current->GetAt(this->notepadForm->current->GetLength() - 1)->GetContent().c_str());
        letterSize = dc.GetTextExtent(letter);
    }
    //10. 캐럿을 생성한다.
    this->caret->Create(letterSize.cx, textmetric.tmHeight);
    //11. 캐럿을 이동시킨다.
    this->caret->Move(rowTextSize.cx - letterSize.cx, this->notepadForm->note->GetCurrent() * textmetric.tmHeight);
    //12. 캐럿을 보이게 한다.
    this->caret->Show();  
}

//소멸자
CaretManager::~CaretManager()
{
    //notepadForm(subject)에서 detach를 호출하면 observer(caretmaker)의 소멸자를 호출해서 
    //할당해주기 때문에 caretmaker의 멤버인 힙에 할당된 caret만 할당해제해주면 됨!
    //1. CaretMaker를 ObserverList에서 뺀다.
    //this->subject->Detach(this);
    //2. caret을 없앤다.
    if (this->caret != 0)
    {
        delete this->caret;
    }
}