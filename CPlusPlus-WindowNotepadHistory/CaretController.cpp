#include "CaretController.h"
#include "NotepadForm.h"
#include "TextExtent.h"
#include "Caret.h"
#include "Subject.h"
#include "Glyph.h"

//디폴트생성자
CaretController::CaretController(NotepadForm* notepadForm)
    : Observer(notepadForm)
{
    this->notepadForm = notepadForm;
    this->caret = new Caret(this);
    //CaretController를 ObserverList에 추가한다.
    this->subject->Attach(this);
}

//Update
void CaretController::Update()
{
    Long letterWidth = 0;
    //여기서 힙에 할당하면 캐럿이동을 할때마다 글자를 쓸때마다 매번 TextExtent를 할당하게 되므로
    //메모리 낭비가 엄청나게 심해진다!!! 그리고 그럼 매번 할당해제를 해줘야하므로 굉장히 번거롭다.
    //캐럿이동이나 글자를 새로 쓸때는 TextExtent를 새로 힙에 할당할 필요가 없다.
    //textExtent를 힙에 할달해야 하는 시기는 처음 notepadForm이 생성될 때 기본 디폴트 글꼴 정보를
    //이용해서 생성하면 되고 글꼴 정보가 바뀌는 경우 기존 TextExtent를 할당해제하고 새로 TextExtent를
    //힙에 할당하면 되므로, FontChangeCommand에서 새로 할당해제해주면 된다.
    //this->notepadForm->textExtent = new TextExtent(this->notepadForm);
    //1. 현재줄의 텍스트의 폭을 구한다.
    Long rowTextWidth = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
        current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
    //2. 한글이 조립중이면
    if (this->notepadForm->isComposing == true)
    {
        //2.1 한글 한 글자의 폭을 구한다.
        //현재 조립중인 한글 한글자를 구하려면 현재 캐럿의 가로 위치 앞의 글자를 구하면 되기 때문에
        //현재 캐럿의 가로 위치 -1 을 해준 값을 배열 요소에서 GetAt해오면 된다!
        letterWidth = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
            current->GetAt(this->notepadForm->current->GetCurrent() - 1)->GetContent());
    }
    //3. 스크롤의 현재 위치를 구한다.
    Long currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
    Long currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
    //4. 캐럿을 생성한다.
    this->caret->Create(letterWidth, this->notepadForm->textExtent->GetHeight());
    //5. 캐럿을 이동시킨다.
    //캐럿의 시작위치는 고정되어고 화면만 이동하므로 이동한만큼 빼줘야함!
    //그럼 원래 캐럿은 처음 시작점에 고정되어 있는데 -해줌으로써 캐럿이 움직이는 것처럼 보임.
    this->caret->Move(rowTextWidth - letterWidth - currentXPos,
        this->notepadForm->note->GetCurrent() *
        this->notepadForm->textExtent->GetHeight() - currentYPos);
    //6. 캐럿을 보이게 한다.
    this->caret->Show();  
}

//소멸자
CaretController::~CaretController()
{
    //notepadForm(subject)에서 detach를 호출하면 observer(caretManager)의 소멸자를 호출해서 
    //할당해주기 때문에 caretManager의 멤버인 힙에 할당된 caret만 할당해제해주면 됨!
    //1. CaretManager를 ObserverList에서 뺀다.
    //this->subject->Detach(this);
    //2. caret을 없앤다.
    if (this->caret != 0)
    {
        delete this->caret;
    }
}