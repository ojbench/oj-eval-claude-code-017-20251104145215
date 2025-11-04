#ifndef USER_H
#define USER_H

#include "utils.h"

struct User {
    char username[21];
    char password[31];
    char name[16];  // 2-5 Chinese characters
    char mailAddr[31];
    int privilege;
    bool isLoggedIn;

    User() : privilege(0), isLoggedIn(false) {
        username[0] = '\0';
        password[0] = '\0';
        name[0] = '\0';
        mailAddr[0] = '\0';
    }
};

class UserManager {
private:
    User users[MAX_USERS];
    int userCount;
    bool firstUserAdded;

public:
    UserManager();

    int addUser(const char* curUsername, const char* username, const char* password,
                const char* name, const char* mailAddr, int privilege);
    int login(const char* username, const char* password);
    int logout(const char* username);
    int queryProfile(const char* curUsername, const char* username, char* result);
    int modifyProfile(const char* curUsername, const char* username, const char* password,
                      const char* name, const char* mailAddr, int privilege, char* result);

    User* findUser(const char* username);
    bool isUserLoggedIn(const char* username);
    int getUserPrivilege(const char* username);
    bool isFirstUserAdded() { return firstUserAdded; }

    void clean();
};

#endif // USER_H