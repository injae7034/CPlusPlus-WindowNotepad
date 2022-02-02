#ifndef _CARET_H
#define _CARET_H

typedef signed long int Long;
class CaretManager;//���漱��(�ּҷ� �̿�Ǳ⶧���� �ʿ�)

class Caret
{
public:
	CaretManager* caretManager;
public:
	//Caret()�⺻�����ڰ� �ʿ���
	//Caret(NotepadForm* notepadForm)�� �Ű�����1���������̰�
	//�� Ŭ������ ������ Caret()�� ���� ������ �⺻�����ڰ� �ʿ���
	//�⺻�����ھ��� ������ Caret(NotepadForm* notepadForm = 0);�� ���� ���·�
	//NotepadForm* notepadForm = 0 ����Ʈ�Ű������� �ָ� �ȴ�.
	//Array���� Long capacity=256���� ����Ʈ�Ű������� �༭ �⺻�����ڷ� �̿�����!!!
	Caret(CaretManager* caretManager = 0);//����Ʈ�����ڿ��� notepadForm�� �޾Ƽ� ĳ���̶� ��Ʈ�е�� ����ξ��ֱ�
	Caret(const Caret& source);//���������
	~Caret();//�Ҹ���
	Caret& operator=(const Caret& source);//ġȯ������
	void Create(Long height, Long width);//ĳ�� �����ϱ� ����,�ʺ� �Է¹޾� ĳ���� �����ϱ� createsolidCaret
	void Move(Long x, Long y);//ĳ�� �̵��ϱ� SetCaretPos
	void Show();//ĳ�� �����ֱ� ShowCaret
};

#endif // !_CARET_H
