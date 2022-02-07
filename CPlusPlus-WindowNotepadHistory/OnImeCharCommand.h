#ifndef _ONIMECHARCOMMAND_H
#define _ONIMECHARCOMMAND_H

#include "Command.h"
#include "NotepadForm.h"

class Glyph;

class OnImeCharCommand :public Command
{
public:
	OnImeCharCommand(NotepadForm* notepadForm, Glyph* glyph);//����Ʈ������
	virtual void Execute();//����
	virtual void Unexecute();//�������
	void SetUndoMacroEnd();//���������� �������� ����
	void SetRedoMacroEnd();//�ٽý������ �������� ����
	void SetRedone();//�ٽý����̶�� ǥ�ø� ����
	Long GetRowIndex();//����� ����� ������ ���� ��ġ�� ���ϱ�
	Long GetLetterIndex();//����� ���屭 ������ ĭ�� ��ġ�� ���ϱ�
	virtual Long GetSelectedStartXPos();//����� ����� ������ ���۵Ǵ� ������ ��ġ���ϱ�
	virtual Long GetSelectedStartYPos();//����� ����� ������ ���۵Ǵ� ���� ��ġ���ϱ�
	bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	virtual bool IsSelectedTextsRemoved();//���ÿ����� �������� ���������� ����
	virtual ~OnImeCharCommand();//�Ҹ���
	//�ζ����Լ� ����(�ζ����Լ��� �θ�Ŭ�������� �������� �� ����)
	inline Glyph* GetGlyph() const;//����� ����� ���� ���ϱ�
	Glyph* GetNote() const;//����� ����� ��Ʈ ���ϱ�
private:
	Glyph* note;//OnCharCommand�� ������ �� ���õ� ������ ������ �� ���õ� ������ content�� ���� ����
	Glyph* glyph;//OnImeCharCommand�� ������ �� �Էµ� ����
	Long rowIndex;//�Էµ� ������ ����ġ
	Long letterIndex;//�Էµ� ������ ĭ��ġ
	Long selectedStartYPos;//������ ���۵Ǵ� ����ġ
	Long selectedStartXPos;//������ ���۵Ǵ� ������ġ
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isRedone;//�ٽý������� �ƴ��� ����
	bool isSelectedTextsRemoved;//���ÿ����� �������� ���������� ����
};

//�ζ����Լ� ����
inline Glyph* OnImeCharCommand::GetGlyph() const
{
	return this->glyph;
}
inline Glyph* OnImeCharCommand::GetNote() const
{
	return this->note;
}

#endif // !_ONIMECHARCOMMAND_H