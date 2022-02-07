#ifndef _BACKSPACEKEYACTIONCOMMAND_H
#define _BACKSPACEKEYACTIONCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class Glyph;

class BackSpaceKeyActionCommand :public Command
{
public:
	BackSpaceKeyActionCommand(NotepadForm* notepadForm = 0);//����Ʈ������
	virtual void Execute();//����
	virtual void Unexecute();//�������
	virtual void SetUndoMacroEnd();//���������� �������� ����
	virtual void SetRedoMacroEnd();//�ٽý������ �������� ����
	virtual void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	virtual Long GetStartXPos();//�����ϴ� ������ ��ġ���ϱ�
	virtual Long GetStartYPos();//�����ϴ� ���� ��ġ���ϱ�
	virtual Long GetSelectedEndXPos();//������ ������ ������ ��ġ���ϱ�
	virtual Long GetSelectedEndYPos();//������ ������ ���� ��ġ���ϱ�
	virtual bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	virtual bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	virtual bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	virtual bool IsDirty();//��������� �ִ��� Ȯ�� ����
	virtual bool IsSelectedTextsRemoved();//���ÿ����� �������� ���������� ����
	virtual ~BackSpaceKeyActionCommand();//�Ҹ���
	virtual Glyph* GetGlyph();//����� ����� ���� ���ϱ�
private:
	Long startXPos;//�����ϴ� ������ ��ġ
	Long startYPos;//�����ϴ� ���� ��ġ
	Long selectedEndXPos;//������ ������ ������ġ
	Long selectedEndYPos;//������ ������ ����ġ
	Glyph* glyph;//����� ���� ���� ���ڸ� ���� ����
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isRedone;//�ٽý������� �ƴ��� ����
	bool isDirty;//��������� �ִ��� ����
	bool isSelectedTextsRemoved;//���ÿ����� �������� ���������� ����
};

#endif // !_BACKSPACEKEYACTIONCOMMAND_H

