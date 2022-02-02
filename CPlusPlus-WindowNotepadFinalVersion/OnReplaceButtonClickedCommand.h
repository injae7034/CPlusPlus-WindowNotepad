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
	virtual Long GetStartXPos();//�����ϴ� ������ ��ġ���ϱ�
	virtual Long GetStartYPos();//�����ϴ� ���� ��ġ���ϱ�
	virtual Long GetSelectedEndXPos();//������ ������ ������ ��ġ���ϱ�
	virtual Long GetSelectedEndYPos();//������ ������ ���� ��ġ���ϱ�
	virtual Long GetPastingEndXPos();//�ٿ��ֱⰡ ������ ������ ��ġ���ϱ�
	virtual Long GetPastingEndYPos();//�ٿ��ֱⰡ ������ ���� ��ġ���ϱ�
	virtual bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	virtual bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	virtual bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	virtual bool IsSelectedTextsRemoved();//���ÿ����� �������� ���������� ����
	//�ζ����Լ� ����(�ζ����Լ��� �θ�Ŭ�������� �������� �� ����)
	Glyph* GetNote() const;//����� ����� ���ÿ��� ���ϱ�
	virtual ~OnReplaceButtonClickedCommand();//�Ҹ���
private:
	string replacingKeyword;//�ٲ� �ܾ�
	Glyph* note;//���ÿ����� ����� ���� ���ÿ����� ������ ����
	Long startXPos;//�����ϴ� ������ ��ġ
	Long startYPos;//�����ϴ� ���� ��ġ
	Long selectedEndXPos;//������ ������ ������ġ
	Long selectedEndYPos;//������ ������ ����ġ
	Long pastingEndXPos;//�ٿ��ֱⰡ ������ ������ġ
	Long pastingEndYPos;//�ٿ��ֱⰡ ������ ����ġ
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isRedone;//�ٽý������� �ƴ��� ����
	bool isSelectedTextsRemoved;//���ÿ����� �������� ���������� ����
};

//�ζ����Լ� ����
inline Glyph* OnReplaceButtonClickedCommand::GetNote() const
{
	return this->note;
}


#endif // !_ONREPLACEBUTTONCLICKEDCOMMAND_H