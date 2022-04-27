using namespace std;
#include <iostream>
#include <string>

class Person{
public:
 std::string name;
 int height;
 bool Male;

 void Introduce(){
 cout<<"Hello my name is "<<name<<std::endl<<"My height is "<<height<<std::endl;}
};

int main(void){

Person Herman;
Herman.height = 168;
Herman.Male = true;
Herman.name = "Lee Kong Yau";


Herman.Introduce();
}
