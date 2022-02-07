#ifndef _ONREPLACEBUTTONCLICKEDCOMMAND_H
#define _ONREPLACEBUTTONCLICKEDCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class Glyph;

class OnReplaceButtonClickedCommand :public Command
{
public:
	OnReplaceButtonClickedCommand(NotepadForm* notepadForm = 0);//����Ʈ������
	void Execute();
	virtual void Unexecute();//�������
	virtual void SetUndoMacroEnd();//���������� �������� ����
	virtual void SetRedoMacroEnd();//�ٽý������ �������� ����
	virtual void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	virtual Long GetRowIndex();//����� ����� ������ ���� ��ġ�� ���ϱ�
	virtual Long GetLetterIndex();//����� ���屭 ������ ĭ�� ��ġ�� ���ϱ�
	virtual Long GetStartPasteRowIndex();//����� ����� �ٿ��ֱⰡ ���۵Ǵ� ���� ��ġ���ϱ�
	virtual Long GetStartPasteLetterIndex();//����� ����� �ٿ��ֱⰡ ���۵Ǵ� ������ ��ġ���ϱ�
	virtual bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	virtual bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	virtual bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	//�ζ����Լ� ����(�ζ����Լ��� �θ�Ŭ�������� �������� �� ����)
	Glyph* GetNote() const;//����� ����� ���ÿ��� ���ϱ�
	virtual ~OnReplaceButtonClickedCommand();//�Ҹ���
private:
	string replacingKeyword;//�ٲ� �ܾ�
	Glyph* note;//���ÿ����� ����� ���� ���ÿ����� ������ ����
	Long startPasteRowIndex;//�ٿ��ֱⰡ ���۵Ǵ� ������ġ
	Long startPasteLetterIndex;//�ٿ��ֱⰡ ����Ǵ� ������ġ
	Long rowIndex;//�Էµ� ������ ����ġ(�Է��� ������ ���� ��ġ)
	Long letterIndex;//�Էµ� ������ ĭ��ġ(�Է��� ������ ������ ��ġ)
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isRedone;//�ٽý������� �ƴ��� ����
};

//�ζ����Լ� ����
inline Glyph* OnReplaceButtonClickedCommand::GetNote() const
{
	return this->note;
}


#endif // !_ONREPLACEBUTTONCLICKEDCOMMAND_H