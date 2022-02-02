#ifndef _ONCHARCOMMAND_H
#define _ONCHARCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class Glyph;

class OnCharCommand :public Command
{
public:
	OnCharCommand(NotepadForm* notepadForm, Glyph* glyph);//����Ʈ������
	virtual void Execute();//����
	virtual void Unexecute();//�������
	virtual void SetUndoMacroEnd();//���������� �������� ����
	virtual void SetRedoMacroEnd();//�ٽý������ �������� ����
	virtual void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	virtual ~OnCharCommand();//�Ҹ���
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
	virtual Glyph* GetGlyph();//����� ����� ���� ���ϱ�
	virtual Glyph* GetNote();//����� ����� ��Ʈ ���ϱ�
private:
	Glyph* note;//OnCharCommand�� ������ �� ���õ� ������ ������ �� ���õ� ������ content�� ���� ����
	Glyph* glyph;//OnCharCommand�� ������ �� �Էµ� ����
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

#endif // !_ONCHARCOMMAND_H