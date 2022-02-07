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
	virtual Long GetRowIndex();//����� ����� ������ ���� ��ġ�� ���ϱ�
	virtual Long GetLetterIndex();//����� ���屭 ������ ĭ�� ��ġ�� ���ϱ�
	virtual Long GetSelectedStartXPos();//����� ����� ������ ���۵Ǵ� ������ ��ġ���ϱ�
	virtual Long GetSelectedStartYPos();//����� ����� ������ ���۵Ǵ� ���� ��ġ���ϱ�
	virtual bool IsUndoMacroEnd();//���������� �������� ���ϱ�
	virtual bool IsRedoMacroEnd();//�ٽý������ �������� ���ϱ�
	virtual bool IsRedone();//�ٽý������� �ƴ��� ���ϱ�
	virtual bool IsSelectedTextsRemoved();//���ÿ����� �������� ���������� ����
	//�ζ����Լ� ����(�ζ����Լ��� �θ�Ŭ�������� �������� �� ����)
	Glyph* GetGlyph() const;//����� ����� ���� ���ϱ�
	Glyph* GetNote() const;//����� ����� ��Ʈ ���ϱ�
private:
	Glyph* note;//OnCharCommand�� ������ �� ���õ� ������ ������ �� ���õ� ������ content�� ���� ����
	Glyph* glyph;//OnCharCommand�� ������ �� �Էµ� ����
	Long rowIndex;//�Էµ� ������ ����ġ
	Long letterIndex;//�Էµ� ������ ĭ��ġ
	bool isUndoMacroEnd;//���������� ��������
	bool isRedoMacroEnd;//�ٽý������ ��������
	bool isRedone;//�ٽý������� �ƴ��� ����
	Long selectedStartXPos;//������ ���۵Ǵ� ������ġ
	Long selectedStartYPos;//������ ����Ǵ� ������ġ
	bool isSelectedTextsRemoved;//���ÿ����� �������� ���������� ����
};

//�ζ����Լ� ����
inline Glyph* OnCharCommand::GetGlyph() const
{
	return this->glyph;
}
inline Glyph* OnCharCommand::GetNote() const
{
	return this->note;
}

#endif // !_ONCHARCOMMAND_H