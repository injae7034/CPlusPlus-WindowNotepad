#include "CaretManager.h"
#include "NotepadForm.h"
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
    CString letter;
    CSize letterSize;
    letterSize.cx = 0;
    //ĳ�� ��ġ�� ũ�Ⱑ ����Ǿ��� ������ �׿� �°� �ٽ� �����Ѵ�.
    //1. CClientDC�� �����Ѵ�.
    CClientDC dc(this->notepadForm);
    //2. CFont�� �����Ѵ�.
    CFont font;
    //3. �۾�ũ��� �۾�ü�� ���ϴ�.
    font.CreateFontIndirect(&this->notepadForm->font.GetLogFont());
    //4 ��Ʈ�� dc�� �����Ѵ�.
    dc.SelectObject(font);
    //5. TEXTMETRIC�� �����Ѵ�.
    TEXTMETRIC textmetric;
    //6. �۲��� ������ ��´�.
    dc.GetTextMetrics(&textmetric);
    //7. �������� �ؽ�Ʈ���� �����Ѵ�.
    //���� ���� ���� ��ü�� ���ϰ� �ֱ� ������ ĳ���� ���� ���� ��ġ�� �׻� ���� ���� ��ü���̿� ��ġ��.
    //CString rowText = CString(this->notepadForm->current->GetContent().c_str());
    //���� �ڵ��� �ϸ� ĳ���� �̵��� �ݿ����� ���ϰ� ���ڰ� �߰��ɶ� ĳ���� �̵��� �ݿ���.
    //NotepadForm�� OnKeyDown���� Ű���� ������ �ν��ؼ� note�� ĳ���� ���� ������ġ��
    //ĳ���� ���� ���� ��ġ�� �̵���Ű�� �ִ� �̸� �ݿ��ؾ��ϴµ� ���⼭ ���� �� �� �ִ� ����
    //current(ĳ���� ���� ������ġ�� ���� ������ġ)�� ����Ǿ��� �� ����� ���� �� �� �ִ�.
    //�̸� Ȱ���ؼ� ���ο� �Լ��� ������ �ȴ�.
    //�켱 Ű��Ʈ left�� right�� �ݿ��Ϸ��� ���� ���� current�� �����ǰ� ���ҵǴ°��� �˼��ִµ�
    //�̸� Ȱ���ؼ� ���� ���� ���ڸ� string�� �� ���� ����(GetContent) 
    //current���� �������ȸ� string�� ��Ƽ� ����ϸ� �ȴ�.(GetPartOfContent)
    CString rowText = CString(this->notepadForm->current->
        GetPartOfContent(this->notepadForm->current->GetCurrent()).c_str());
    //8. �������� �ؽ�Ʈ���� size�� ���Ѵ�.
    CSize rowTextSize = dc.GetTextExtent(rowText);
    //9. �ѱ��� �������̸�
    if (this->notepadForm->IsComposing == true)
    {
        //9.1 ���� �߰��� �ѱ� �� ������ size�� ���Ѵ�.
        letter = CString(this->notepadForm->current->GetAt(this->notepadForm->current->GetLength() - 1)->GetContent().c_str());
        letterSize = dc.GetTextExtent(letter);
    }
    //10. ĳ���� �����Ѵ�.
    this->caret->Create(letterSize.cx, textmetric.tmHeight);
    //11. ĳ���� �̵���Ų��.
    this->caret->Move(rowTextSize.cx - letterSize.cx, this->notepadForm->note->GetCurrent() * textmetric.tmHeight);
    //12. ĳ���� ���̰� �Ѵ�.
    this->caret->Show();  
}

//�Ҹ���
CaretManager::~CaretManager()
{
    //notepadForm(subject)���� detach�� ȣ���ϸ� observer(caretmaker)�� �Ҹ��ڸ� ȣ���ؼ� 
    //�Ҵ����ֱ� ������ caretmaker�� ����� ���� �Ҵ�� caret�� �Ҵ��������ָ� ��!
    //1. CaretMaker�� ObserverList���� ����.
    //this->subject->Detach(this);
    //2. caret�� ���ش�.
    if (this->caret != 0)
    {
        delete this->caret;
    }
}