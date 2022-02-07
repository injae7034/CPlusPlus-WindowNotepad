#ifndef _DELETEKEYACTIONCOMMAND_H
#define _DELETEKEYACTIONCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class Glyph;

class DeleteKeyActionCommand :public Command
{
public:
	DeleteKeyActionCommand(NotepadForm* notepadForm = 0);//����Ʈ������
	virtual void Execute();//����
	virtual void Unexecute();//�������
	virtual void SetUndoMacroEnd();//���������� �������� ����
	virtual void SetRedoMacroEnd();//�ٽý������ �������� ����
	virtual void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	virtual Long GetRowIndex();//����� ����� ������ ���� ��ġ�� ���ϱ�
	virtual Long GetLetterIndex();//����� ���屭 ������ ĭ�� ��ġ�� ���ϱ�
	virtual Long GetSelectedStartXPos();//����� ����� ������ ���۵Ǵ� ������ ��ġ���ϱ�
	virtual Long GetSelectedStartYPos();//����� ����� ������ ���۵Ǵ� ���� ��ġ���ϱ�
	virtual bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	virtual bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	virtual bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	virtual bool IsDirty();//��������� �ִ��� Ȯ�� ����
	virtual bool IsSelectedTextsRemoved();//���ÿ����� �������� ���������� ����
	virtual ~DeleteKeyActionCommand();//�Ҹ���
	//�ζ����Լ� ����(�ζ����Լ��� �θ�Ŭ�������� �������� �� ����)
	inline Glyph* GetGlyph() const;//����� ����� ���� ���ϱ�
private:
	Long rowIndex;//�Էµ� ������ ����ġ
	Long letterIndex;//�Էµ� ������ ĭ��ġ
	Glyph* glyph;//����� ���� ���� ���ڸ� ���� ����
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isRedone;//�ٽý������� �ƴ��� ����
	bool isDirty;//��������� �ִ��� ����
	Long selectedStartXPos;//������ ���۵Ǵ� ������ġ
	Long selectedStartYPos;//������ ����Ǵ� ������ġ
	bool isSelectedTextsRemoved;//���ÿ����� �������� ���������� ����
};

//�ζ����Լ� ����
inline Glyph* DeleteKeyActionCommand::GetGlyph() const
{
	return this->glyph;
}

#endif // !_DELETEKEYACTIONCOMMAND_H

