#ifndef _CUTCOMMAND_H
#define _CUTCOMMAND_H

#include "Command.h"

class NotepadForm;
class Glyph;

class CutCommand :public Command
{
public:
	CutCommand(NotepadForm* notepadForm = 0);
	void Execute();
	//virtual void Unexecute();//�������
	//virtual void SetUndoMacroEnd();//���������� �������� ����
	//virtual void SetRedoMacroEnd();//�ٽý������ �������� ����
	//virtual void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	//virtual Long GetRowIndex();//����� ����� ������ ���� ��ġ�� ���ϱ�
	//virtual Long GetLetterIndex();//����� ���屭 ������ ĭ�� ��ġ�� ���ϱ�
	//virtual Long GetSelectedStartXPos();//����� ����� ������ ���۵Ǵ� ������ ��ġ���ϱ�
	//virtual Long GetSelectedStartYPos();//����� ����� ������ ���۵Ǵ� ���� ��ġ���ϱ�
	//virtual bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	//virtual bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	//virtual bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	//virtual bool IsDirty();//��������� �ִ��� Ȯ�� ����
	//�ζ����Լ� ����(�ζ����Լ��� �θ�Ŭ�������� �������� �� ����)
	Glyph* GetGlyph() const;//����� ����� ���� ���ϱ�
	~CutCommand();//�Ҹ���
private:
	Long rowIndex;//�Էµ� ������ ����ġ
	Long letterIndex;//�Էµ� ������ ĭ��ġ
	Glyph* glyph;//����� ���� ���� ���ڸ� ���� ����
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isRedone;//�ٽý������� �ƴ��� ����
	Long selectedStartXPos;//������ ���۵Ǵ� ������ġ
	Long selectedStartYPos;//������ ����Ǵ� ������ġ
};

//�ζ����Լ� ����
inline Glyph* CutCommand::GetGlyph() const
{
	return this->glyph;
}

#endif // !_CUTCOMMAND_H