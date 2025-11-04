#include "user.h"
#include <cstring>
#include <cstdio>
#include <cctype>

UserManager::UserManager() : userCount(0), firstUserAdded(false) {}

int UserManager::addUser(const char* curUsername, const char* username, const char* password,
                        const char* name, const char* mailAddr, int privilege) {
    // Check if first user
    if (!firstUserAdded) {
        // First user - special case
        if (userCount >= MAX_USERS) return -1;

        User& newUser = users[userCount++];
        strcpy(newUser.username, username);
        strcpy(newUser.password, password);
        strcpy(newUser.name, name);
        strcpy(newUser.mailAddr, mailAddr);
        newUser.privilege = 10;
        newUser.isLoggedIn = false;

        firstUserAdded = true;
        return 0;
    }

    // Check permissions
    User* curUser = findUser(curUsername);
    if (!curUser || !curUser->isLoggedIn) return -1;
    if (privilege >= curUser->privilege) return -1;

    // Check if user already exists
    if (findUser(username)) return -1;

    if (userCount >= MAX_USERS) return -1;

    // Validate inputs
    if (!isValidUsername(username) || !isValidPassword(password) ||
        !isValidName(name) || !isValidEmail(mailAddr)) return -1;
    if (privilege < 0 || privilege > 10) return -1;

    User& newUser = users[userCount++];
    strcpy(newUser.username, username);
    strcpy(newUser.password, password);
    strcpy(newUser.name, name);
    strcpy(newUser.mailAddr, mailAddr);
    newUser.privilege = privilege;
    newUser.isLoggedIn = false;

    return 0;
}

int UserManager::login(const char* username, const char* password) {
    User* user = findUser(username);
    if (!user) return -1;
    if (user->isLoggedIn) return -1;
    if (strcmp(user->password, password) != 0) return -1;

    user->isLoggedIn = true;
    return 0;
}

int UserManager::logout(const char* username) {
    User* user = findUser(username);
    if (!user) return -1;
    if (!user->isLoggedIn) return -1;

    user->isLoggedIn = false;
    return 0;
}

int UserManager::queryProfile(const char* curUsername, const char* username, char* result) {
    User* curUser = findUser(curUsername);
    if (!curUser || !curUser->isLoggedIn) return -1;

    User* targetUser = findUser(username);
    if (!targetUser) return -1;

    if (curUser->privilege <= targetUser->privilege && strcmp(curUsername, username) != 0) return -1;

    sprintf(result, "%s %s %s %d", targetUser->username, targetUser->name,
            targetUser->mailAddr, targetUser->privilege);
    return 0;
}

int UserManager::modifyProfile(const char* curUsername, const char* username, const char* password,
                              const char* name, const char* mailAddr, int privilege, char* result) {
    User* curUser = findUser(curUsername);
    if (!curUser || !curUser->isLoggedIn) return -1;

    User* targetUser = findUser(username);
    if (!targetUser) return -1;

    if (curUser->privilege <= targetUser->privilege && strcmp(curUsername, username) != 0) return -1;
    if (privilege != -1 && privilege >= curUser->privilege) return -1;

    if (password && strlen(password) > 0) {
        if (!isValidPassword(password)) return -1;
        strcpy(targetUser->password, password);
    }
    if (name && strlen(name) > 0) {
        if (!isValidName(name)) return -1;
        strcpy(targetUser->name, name);
    }
    if (mailAddr && strlen(mailAddr) > 0) {
        if (!isValidEmail(mailAddr)) return -1;
        strcpy(targetUser->mailAddr, mailAddr);
    }
    if (privilege != -1) {
        targetUser->privilege = privilege;
    }

    sprintf(result, "%s %s %s %d", targetUser->username, targetUser->name,
            targetUser->mailAddr, targetUser->privilege);
    return 0;
}

User* UserManager::findUser(const char* username) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return &users[i];
        }
    }
    return nullptr;
}

bool UserManager::isUserLoggedIn(const char* username) {
    User* user = findUser(username);
    return user && user->isLoggedIn;
}

int UserManager::getUserPrivilege(const char* username) {
    User* user = findUser(username);
    return user ? user->privilege : -1;
}

void UserManager::clean() {
    userCount = 0;
    firstUserAdded = false;
}