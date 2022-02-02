#ifndef _FILE_H
#define _FILE_H

#include<string>
using namespace std;
class NotepadForm;//주소를 사용하기때문에 전방선언해줌.

class File
{
public:
	File(NotepadForm* notepadForm = 0);//디폴트생성자
	File(NotepadForm *notepadForm, string name, string position);//매개변수를 3개 생성자
	File(const File& source);//복사생성자
	~File();//소멸자
	File& operator=(const File& source);//치환연산자
	void Load(string name);//불러오기
	void Save(string name);//저장하기
	//인라인함수
	string& GetName() const;
	string& GetPosition() const;
public://notepadForm의 주소는 public으로 설정함.
	NotepadForm* notepadForm;
private:
	string name;
	string position;
};

//인라인함수정의
inline string& File::GetName() const
{
	return const_cast<string&>(this->name);
}
inline string& File::GetPosition() const
{
	return const_cast<string&>(this->position);
}

#endif // !_FILE_H

