#include "CaretController.h"
#include "NotepadForm.h"
#include "TextExtent.h"
#include "Caret.h"
#include "Subject.h"
#include "Glyph.h"

//����Ʈ������
CaretController::CaretController(NotepadForm* notepadForm)
    : Observer(notepadForm)
{
    this->notepadForm = notepadForm;
    this->caret = new Caret(this);
    //CaretController�� ObserverList�� �߰��Ѵ�.
    this->subject->Attach(this);
}

//Update
void CaretController::Update()
{
    Long letterWidth = 0;
    //���⼭ ���� �Ҵ��ϸ� ĳ���̵��� �Ҷ����� ���ڸ� �������� �Ź� TextExtent�� �Ҵ��ϰ� �ǹǷ�
    //�޸� ���� ��û���� ��������!!! �׸��� �׷� �Ź� �Ҵ������� ������ϹǷ� ������ ���ŷӴ�.
    //ĳ���̵��̳� ���ڸ� ���� ������ TextExtent�� ���� ���� �Ҵ��� �ʿ䰡 ����.
    //textExtent�� ���� �Ҵ��ؾ� �ϴ� �ñ�� ó�� notepadForm�� ������ �� �⺻ ����Ʈ �۲� ������
    //�̿��ؼ� �����ϸ� �ǰ� �۲� ������ �ٲ�� ��� ���� TextExtent�� �Ҵ������ϰ� ���� TextExtent��
    //���� �Ҵ��ϸ� �ǹǷ�, FontChangeCommand���� ���� �Ҵ��������ָ� �ȴ�.
    //this->notepadForm->textExtent = new TextExtent(this->notepadForm);
    //1. �������� �ؽ�Ʈ�� ���� ���Ѵ�.
    Long rowTextWidth = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
        current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
    //2. �ѱ��� �������̸�
    if (this->notepadForm->isComposing == true)
    {
        //2.1 �ѱ� �� ������ ���� ���Ѵ�.
        //���� �������� �ѱ� �ѱ��ڸ� ���Ϸ��� ���� ĳ���� ���� ��ġ ���� ���ڸ� ���ϸ� �Ǳ� ������
        //���� ĳ���� ���� ��ġ -1 �� ���� ���� �迭 ��ҿ��� GetAt�ؿ��� �ȴ�!
        letterWidth = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
            current->GetAt(this->notepadForm->current->GetCurrent() - 1)->GetContent());
    }
    //3. ��ũ���� ���� ��ġ�� ���Ѵ�.
    Long currentXPos = this->notepadForm->GetScrollPos(SB_HORZ);
    Long currentYPos = this->notepadForm->GetScrollPos(SB_VERT);
    //4. ĳ���� �����Ѵ�.
    this->caret->Create(letterWidth, this->notepadForm->textExtent->GetHeight());
    //5. ĳ���� �̵���Ų��.
    //ĳ���� ������ġ�� �����Ǿ�� ȭ�鸸 �̵��ϹǷ� �̵��Ѹ�ŭ �������!
    //�׷� ���� ĳ���� ó�� �������� �����Ǿ� �ִµ� -�������ν� ĳ���� �����̴� ��ó�� ����.
    this->caret->Move(rowTextWidth - letterWidth - currentXPos,
        this->notepadForm->note->GetCurrent() *
        this->notepadForm->textExtent->GetHeight() - currentYPos);
    //6. ĳ���� ���̰� �Ѵ�.
    this->caret->Show();  
}

//�Ҹ���
CaretController::~CaretController()
{
    //notepadForm(subject)���� detach�� ȣ���ϸ� observer(caretManager)�� �Ҹ��ڸ� ȣ���ؼ� 
    //�Ҵ����ֱ� ������ caretManager�� ����� ���� �Ҵ�� caret�� �Ҵ��������ָ� ��!
    //1. CaretManager�� ObserverList���� ����.
    //this->subject->Detach(this);
    //2. caret�� ���ش�.
    if (this->caret != 0)
    {
        delete this->caret;
    }
}