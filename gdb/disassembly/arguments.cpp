#include <stdio.h>
#include <string.h>
#include <time.h>

    struct Student
    {
        unsigned long   id;
        unsigned int    age;
        unsigned int    score;
        char            name[16];
    };

    struct StudentToken {
        unsigned long id;
        unsigned int timeStamp;
    };

    class StudentMgmt {
    public:
        StudentMgmt() : tokenSecret_(0) {}
        void setTokenSecret(unsigned long secret) {
            tokenSecret_ = secret;
        }
        StudentToken createToken(Student s) {
            StudentToken token;
            token.id = tokenSecret_ ^ s.id;
            token.timeStamp = time(NULL);
            return token;
        }
        unsigned long tokenSecret_;
    };

    int main(int argc, char **argv) {
        Student s;
        s.id = 9527;
        s.age = 18;
        s.score = 100;
        strcpy(s.name, "xiaoming");

        StudentMgmt mgmt;
        mgmt.setTokenSecret(0xfedcba9876543210UL);
        StudentToken token = mgmt.createToken(s);

        printf("id:%zx time:%u", token.id, token.timeStamp);
    }