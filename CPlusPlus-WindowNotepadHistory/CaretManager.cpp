#include "CaretManager.h"
#include "NotepadForm.h"
#include "TextExtent.h"
#include "Caret.h"
#include "Subject.h"
#include "Glyph.h"

//����Ʈ������
CaretManager::CaretManager(NotepadForm* notepadForm)
    : Observer(notepadForm)
{
    this->notepadForm = notepadForm;
    this->caret = new Caret(this);
    //CaretMaker�� ObserverList�� �߰��Ѵ�.
    this->subject->Attach(this);
}

//Update
void CaretManager::Update()
{
    Long letterWidth = 0;
    //���⼭ ���� �Ҵ��ϸ� ĳ���̵��� �Ҷ����� ���ڸ� �������� �Ź� TextExtent�� �Ҵ��ϰ� �ǹǷ�
    //�޸� ���� ��û���� ��������!!! �׸��� �׷� �Ź� �Ҵ������� ������ϹǷ� ������ ���ŷӴ�.
    //ĳ���̵��̳� ���ڸ� ���� ������ TextExtent�� ���� ���� �Ҵ��� �ʿ䰡 ����.
    //textExtent�� ���� �Ҵ��ؾ� �ϴ� �ñ�� ó�� notepadForm�� ������ �� �⺻ ����Ʈ �۲� ������
    //�̿��ؼ� �����ϸ� �ǰ� �۲� ������ �ٲ�� ��� ���� TextExtent�� �Ҵ������ϰ� ���� TextExtent��
    //���� �Ҵ��ϸ� �ǹǷ�, FontChangeCommand���� ���� �Ҵ��������ָ� �ȴ�.
    //7. textExtent�� ���� �Ҵ��Ѵ�.
    //this->notepadForm->textExtent = new TextExtent(this->notepadForm);
    //1. �������� �ؽ�Ʈ�� ���� ���Ѵ�.
    Long rowTextWidth = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
        current->GetPartOfContent(this->notepadForm->current->GetCurrent()));
    //2. �ѱ��� �������̸�
    if (this->notepadForm->IsComposing == true)
    {
        //2.1 �ѱ� �� ������ ���� ���Ѵ�.
        letterWidth = this->notepadForm->textExtent->GetTextWidth(this->notepadForm->
            current->GetAt(this->notepadForm->current->GetLength() - 1)->GetContent());
    }
    //3. ĳ���� �����Ѵ�.
    this->caret->Create(letterWidth, this->notepadForm->textExtent->GetHeight());
    //4. ĳ���� �̵���Ų��.
    this->caret->Move(rowTextWidth - letterWidth, this->notepadForm->note->GetCurrent() *
        this->notepadForm->textExtent->GetHeight());
    //5. ĳ���� ���̰� �Ѵ�.
    this->caret->Show();  
}

//�Ҹ���
CaretManager::~CaretManager()
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