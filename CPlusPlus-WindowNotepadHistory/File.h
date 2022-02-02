#ifndef _FILE_H
#define _FILE_H

#include<string>
using namespace std;
class NotepadForm;//주소로 이용되면 전방선언을 해줌

class File
{
public:
	File();//디폴트생성자
	~File();//소멸자
	void Load(NotepadForm* notepadForm, string path);//가져오기
	void Save(NotepadForm* notepadForm, string path);//저장하기
};

#endif // !_FILE_H

