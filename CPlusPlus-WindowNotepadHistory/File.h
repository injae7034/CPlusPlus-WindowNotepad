#ifndef _FILE_H
#define _FILE_H

#include<string>
using namespace std;
class NotepadForm;//�ּҸ� ����ϱ⶧���� ���漱������.

class File
{
public:
	File(NotepadForm* notepadForm = 0);//����Ʈ������
	File(NotepadForm *notepadForm, string name, string position);//�Ű������� 3�� ������
	File(const File& source);//���������
	~File();//�Ҹ���
	File& operator=(const File& source);//ġȯ������
	void Load(string name);//�ҷ�����
	void Save(string name);//�����ϱ�
	//�ζ����Լ�
	string& GetName() const;
	string& GetPosition() const;
public://notepadForm�� �ּҴ� public���� ������.
	NotepadForm* notepadForm;
private:
	string name;
	string position;
};

//�ζ����Լ�����
inline string& File::GetName() const
{
	return const_cast<string&>(this->name);
}
inline string& File::GetPosition() const
{
	return const_cast<string&>(this->position);
}

#endif // !_FILE_H

