#ifndef _CARET_H
#define _CARET_H

typedef signed long int Long;
class NotepadForm;//���漱��(�ּҷ� �̿�Ǳ⶧���� �ʿ�)

class Caret
{
public:
	//Caret()�⺻�����ڰ� �ʿ���
	//Caret(NotepadForm* notepadForm)�� �Ű�����1���������̰�
	//�� Ŭ������ ������ Caret()�� ���� ������ �⺻�����ڰ� �ʿ���
	//�⺻�����ھ��� ������ Caret(NotepadForm* notepadForm = 0);�� ���� ���·�
	//NotepadForm* notepadForm = 0 ����Ʈ�Ű������� �ָ� �ȴ�.
	//Array���� Long capacity=256���� ����Ʈ�Ű������� �༭ �⺻�����ڷ� �̿�����!!!
	Caret(NotepadForm* notepadForm = 0);//����Ʈ�����ڿ��� notepadForm�� �޾Ƽ� ĳ���̶� ��Ʈ�е�� ����ξ��ֱ�
	//Caret(NotepadForm* notepadForm, Long height, Long width);//�Ű�����3��������
	Caret(const Caret& source);//���������
	~Caret();//�Ҹ���
	Caret& operator=(const Caret& source);//ġȯ������
	void Create(Long height, Long width);//ĳ�� �����ϱ� ����,�ʺ� �Է¹޾� ĳ���� �����ϱ� createsolidCaret
	void Move(Long x, Long y);//ĳ�� �̵��ϱ� SetCaretPos
	void Show();//ĳ�� �����ֱ� ShowCaret
	//�ζ����Լ�
	//CDC& GetDc() const; CDC�� ĳ���̶� ���踦 �δ°� �ƴ϶� notepadForm�̶� ĳ���̶� ����α�
	//NotepadForm* GetNotepadForm() const;//notepadForm�� public�̱� ������ �ζ����Լ��� �ʿ����
	//Long GetHeight() const;
	//Long GetWidth() const;
//private: AccoubtBookForm�� AccountBook�� �ּҸ� public���� ������ �ִ°Ͱ� ����.
	//CDC dc;
public:
	NotepadForm* notepadForm;// ĳ���̶� notepadForm�̶� ���踦 �α�
	//ĳ���� ���̿� �ʺ�� �׻� �ٲ�� ������ ĳ���� ����� �ʺ� ���̰� ���� ���� �ʿ䰡 ����!
	//Long height;
	//Long width;
};

#if 0
//�ζ����Լ����� �� �� �ٺ����� CDC�� ĳ���̶� ���踦 �������� ������?
//���ݸ� �� �����غ��� �翬�� ĳ���̶� notepadForm�̶� ���踦 �δ°� �翬�ߴµ�
//���������� �����ϰ� �����Ϸ��� �������� ������ ���� �����ϰ� �ڵ�������
inline CDC& Caret::GetDc() const
{
	return const_cast<CDC&>(this->dc);
	
}
#endif
#if 0
inline NotepadForm* Caret::GetNotepadForm() const
{
	return this->notepadForm;
}
#endif
#if 0
inline Long Caret::GetHeight() const
{
	return this->height;
}
inline Long Caret::GetWidth() const
{
	return this->width;
}
#endif
#endif // !_CARET_H
