#ifndef _REMOVECOMMAND_H
#define _REMOVECOMMAND_H

#include "Command.h"

class Glyph;
class NotepadForm;

class RemoveCommand :public Command
{
public:
	RemoveCommand(NotepadForm* notepadForm = 0);
	virtual void Execute();
	virtual void Unexecute();
	virtual Long GetStartXPos();//�����ϴ� ������ ��ġ���ϱ�
	virtual Long GetStartYPos();//�����ϴ� ���� ��ġ���ϱ�
	virtual Long GetSelectedEndXPos();//������ ������ ������ ��ġ���ϱ�
	virtual Long GetSelectedEndYPos();//������ ������ ���� ��ġ���ϱ�
	virtual void SetUndoMacroEnd();//���������� �������� ����
	virtual void SetRedoMacroEnd();//�ٽý������ �������� ����
	virtual void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	virtual bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	virtual bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	virtual bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	virtual bool IsSelectedTextsRemoved();//���ÿ����� �������� ���������� ����
	virtual Glyph* GetGlyph();//����� ����� ���� ���ϱ�
	virtual ~RemoveCommand();//�Ҹ���
private:
	Glyph* glyph;//����� ���� ���� ���ڸ� ���� ����
	bool isRedone;//�ٽý������� �ƴ��� ����
	Long startXPos;//�����ϴ� ������ ��ġ
	Long startYPos;//�����ϴ� ���� ��ġ
	Long selectedEndXPos;//������ ������ ������ġ
	Long selectedEndYPos;//������ ������ ����ġ
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isSelectedTextsRemoved;//���ÿ����� �������� ���������� ����
};

#endif // !_REMOVECOMMAND_H