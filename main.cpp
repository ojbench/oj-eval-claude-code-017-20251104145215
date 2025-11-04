#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "user.h"
#include "train.h"
#include "order.h"

class TicketSystem {
private:
    UserManager userManager;
    TrainManager trainManager;
    OrderManager orderManager;

public:
    void processCommand(const char* command) {
        char cmd[32];
        char args[1024] = "";

        // Parse command
        int parsed = sscanf(command, "%s %[^\n]", cmd, args);
        if (parsed == 1) {
            args[0] = '\0'; // No arguments
        }

        // Debug output
        // printf("DEBUG: Command='%s', parsed=%d, cmd='%s', args='%s'\n", command, parsed, cmd, args);

        if (strcmp(cmd, "clean") == 0) {
            handleClean();
        } else if (strcmp(cmd, "exit") == 0) {
            handleExit();
        } else if (strcmp(cmd, "add_user") == 0) {
            handleAddUser(args);
        } else if (strcmp(cmd, "login") == 0) {
            handleLogin(args);
        } else if (strcmp(cmd, "logout") == 0) {
            handleLogout(args);
        } else if (strcmp(cmd, "query_profile") == 0) {
            handleQueryProfile(args);
        } else if (strcmp(cmd, "modify_profile") == 0) {
            handleModifyProfile(args);
        } else if (strcmp(cmd, "add_train") == 0) {
            handleAddTrain(args);
        } else if (strcmp(cmd, "release_train") == 0) {
            handleReleaseTrain(args);
        } else if (strcmp(cmd, "query_train") == 0) {
            handleQueryTrain(args);
        } else if (strcmp(cmd, "delete_train") == 0) {
            handleDeleteTrain(args);
        } else if (strcmp(cmd, "query_ticket") == 0) {
            handleQueryTicket(args);
        } else if (strcmp(cmd, "query_transfer") == 0) {
            handleQueryTransfer(args);
        } else if (strcmp(cmd, "buy_ticket") == 0) {
            handleBuyTicket(args);
        } else if (strcmp(cmd, "query_order") == 0) {
            handleQueryOrder(args);
        } else if (strcmp(cmd, "refund_ticket") == 0) {
            handleRefundTicket(args);
        } else {
            printf("-1\n");
        }
    }

private:
    void parseArgs(const char* args, char* keys[], char* values[], int& count) {
        count = 0;
        if (!args || strlen(args) == 0) return;

        char* argsCopy = new char[strlen(args) + 1];
        strcpy(argsCopy, args);

        char* token = strtok(argsCopy, " ");
        while (token != nullptr && count < 20) {
            if (token[0] == '-' && strlen(token) == 2) {
                keys[count] = new char[3];
                strcpy(keys[count], token);

                token = strtok(nullptr, " ");
                if (token != nullptr) {
                    values[count] = new char[strlen(token) + 1];
                    strcpy(values[count], token);
                    count++;
                }
            }
            token = strtok(nullptr, " ");
        }

        delete[] argsCopy;
    }

    void freeArgs(char* keys[], char* values[], int count) {
        for (int i = 0; i < count; i++) {
            delete[] keys[i];
            delete[] values[i];
        }
    }

    const char* getArgValue(char* keys[], char* values[], int count, const char* key) {
        for (int i = 0; i < count; i++) {
            if (strcmp(keys[i], key) == 0) {
                return values[i];
            }
        }
        return nullptr;
    }

    void handleAddUser(const char* args) {
        char* keys[20], *values[20];
        int count;
        parseArgs(args, keys, values, count);

        const char* curUsername = getArgValue(keys, values, count, "-c");
        const char* username = getArgValue(keys, values, count, "-u");
        const char* password = getArgValue(keys, values, count, "-p");
        const char* name = getArgValue(keys, values, count, "-n");
        const char* mailAddr = getArgValue(keys, values, count, "-m");
        const char* privilegeStr = getArgValue(keys, values, count, "-g");

        if (!username || !password || !name || !mailAddr) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        int privilege = privilegeStr ? parseInt(privilegeStr) : 0;
        int result = userManager.addUser(curUsername, username, password, name, mailAddr, privilege);
        printf("%d\n", result);

        freeArgs(keys, values, count);
    }

    void handleLogin(const char* args) {
        char* keys[20], *values[20];
        int count;
        parseArgs(args, keys, values, count);

        const char* username = getArgValue(keys, values, count, "-u");
        const char* password = getArgValue(keys, values, count, "-p");

        if (!username || !password) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        int result = userManager.login(username, password);
        printf("%d\n", result);

        freeArgs(keys, values, count);
    }

    void handleLogout(const char* args) {
        char* keys[20], *values[20];
        int count;
        parseArgs(args, keys, values, count);

        const char* username = getArgValue(keys, values, count, "-u");

        if (!username) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        int result = userManager.logout(username);
        printf("%d\n", result);

        freeArgs(keys, values, count);
    }

    void handleQueryProfile(const char* args) {
        char* keys[20], *values[20];
        int count;
        parseArgs(args, keys, values, count);

        const char* curUsername = getArgValue(keys, values, count, "-c");
        const char* username = getArgValue(keys, values, count, "-u");

        if (!curUsername || !username) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        char result[256];
        int ret = userManager.queryProfile(curUsername, username, result);
        if (ret == 0) {
            printf("%s\n", result);
        } else {
            printf("-1\n");
        }

        freeArgs(keys, values, count);
    }

    void handleModifyProfile(const char* args) {
        char* keys[20], *values[20];
        int count;
        parseArgs(args, keys, values, count);

        const char* curUsername = getArgValue(keys, values, count, "-c");
        const char* username = getArgValue(keys, values, count, "-u");
        const char* password = getArgValue(keys, values, count, "-p");
        const char* name = getArgValue(keys, values, count, "-n");
        const char* mailAddr = getArgValue(keys, values, count, "-m");
        const char* privilegeStr = getArgValue(keys, values, count, "-g");

        if (!curUsername || !username) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        int privilege = privilegeStr ? parseInt(privilegeStr) : -1;
        char result[256];
        int ret = userManager.modifyProfile(curUsername, username, password, name, mailAddr, privilege, result);
        if (ret == 0) {
            printf("%s\n", result);
        } else {
            printf("-1\n");
        }

        freeArgs(keys, values, count);
    }

    void handleAddTrain(const char* args) {
        char* keys[20], *values[20];
        int count;
        parseArgs(args, keys, values, count);

        const char* trainID = getArgValue(keys, values, count, "-i");
        const char* stationNumStr = getArgValue(keys, values, count, "-n");
        const char* seatNumStr = getArgValue(keys, values, count, "-m");
        const char* stations = getArgValue(keys, values, count, "-s");
        const char* prices = getArgValue(keys, values, count, "-p");
        const char* startTime = getArgValue(keys, values, count, "-x");
        const char* travelTimes = getArgValue(keys, values, count, "-t");
        const char* stopoverTimes = getArgValue(keys, values, count, "-o");
        const char* saleDate = getArgValue(keys, values, count, "-d");
        const char* type = getArgValue(keys, values, count, "-y");

        if (!trainID || !stationNumStr || !seatNumStr || !stations || !prices ||
            !startTime || !travelTimes || !stopoverTimes || !saleDate || !type) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        int stationNum = parseInt(stationNumStr);
        int seatNum = parseInt(seatNumStr);
        int result = trainManager.addTrain(trainID, stationNum, seatNum, stations, prices,
                                          startTime, travelTimes, stopoverTimes, saleDate, type[0]);
        printf("%d\n", result);

        freeArgs(keys, values, count);
    }

    void handleReleaseTrain(const char* args) {
        char* keys[20], *values[20];
        int count;
        parseArgs(args, keys, values, count);

        const char* trainID = getArgValue(keys, values, count, "-i");

        if (!trainID) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        int result = trainManager.releaseTrain(trainID);
        printf("%d\n", result);

        freeArgs(keys, values, count);
    }

    void handleQueryTrain(const char* args) {
        char* keys[20], *values[20];
        int count;
        parseArgs(args, keys, values, count);

        const char* trainID = getArgValue(keys, values, count, "-i");
        const char* date = getArgValue(keys, values, count, "-d");

        if (!trainID || !date) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        char result[4096];
        int ret = trainManager.queryTrain(trainID, date, result);
        if (ret == 0) {
            printf("%s", result);
        } else {
            printf("-1\n");
        }

        freeArgs(keys, values, count);
    }

    void handleDeleteTrain(const char* args) {
        char* keys[20], *values[20];
        int count;
        parseArgs(args, keys, values, count);

        const char* trainID = getArgValue(keys, values, count, "-i");

        if (!trainID) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        int result = trainManager.deleteTrain(trainID);
        printf("%d\n", result);

        freeArgs(keys, values, count);
    }

    void handleQueryTicket(const char* args) {
        char* keys[20], *values[20];
        int count;
        parseArgs(args, keys, values, count);

        const char* fromStation = getArgValue(keys, values, count, "-s");
        const char* toStation = getArgValue(keys, values, count, "-t");
        const char* date = getArgValue(keys, values, count, "-d");
        const char* priority = getArgValue(keys, values, count, "-p");

        if (!fromStation || !toStation || !date) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        char result[4096];
        const char* priorityStr = priority ? priority : "time";
        int ret = trainManager.queryTicket(fromStation, toStation, date, priorityStr, result);
        if (ret == 0) {
            printf("%s", result);
        } else {
            printf("-1\n");
        }

        freeArgs(keys, values, count);
    }

    void handleQueryTransfer(const char* args) {
        char* keys[20], *values[20];
        int count;
        parseArgs(args, keys, values, count);

        const char* fromStation = getArgValue(keys, values, count, "-s");
        const char* toStation = getArgValue(keys, values, count, "-t");
        const char* date = getArgValue(keys, values, count, "-d");
        const char* priority = getArgValue(keys, values, count, "-p");

        if (!fromStation || !toStation || !date) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        char result[4096];
        const char* priorityStr = priority ? priority : "time";
        int ret = trainManager.queryTransfer(fromStation, toStation, date, priorityStr, result);
        if (ret == 0) {
            printf("%s", result);
        } else {
            printf("0\n");
        }

        freeArgs(keys, values, count);
    }

    void handleBuyTicket(const char* args) {
        char* keys[20], *values[20];
        int count;
        parseArgs(args, keys, values, count);

        const char* username = getArgValue(keys, values, count, "-u");
        const char* trainID = getArgValue(keys, values, count, "-i");
        const char* date = getArgValue(keys, values, count, "-d");
        const char* numTicketsStr = getArgValue(keys, values, count, "-n");
        const char* fromStation = getArgValue(keys, values, count, "-f");
        const char* toStation = getArgValue(keys, values, count, "-t");
        const char* queueStr = getArgValue(keys, values, count, "-q");

        if (!username || !trainID || !date || !numTicketsStr || !fromStation || !toStation) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        if (!userManager.isUserLoggedIn(username)) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        int numTickets = parseInt(numTicketsStr);
        bool queueIfUnavailable = queueStr ? (strcmp(queueStr, "true") == 0) : false;
        int totalPrice;

        int result = orderManager.buyTicket(username, trainID, date, numTickets,
                                           fromStation, toStation, queueIfUnavailable, totalPrice);

        if (result == -1) {
            printf("-1\n");
        } else if (result == -2) { // queue
            printf("queue\n");
        } else {
            printf("%d\n", result);
        }

        freeArgs(keys, values, count);
    }

    void handleQueryOrder(const char* args) {
        char* keys[20], *values[20];
        int count;
        parseArgs(args, keys, values, count);

        const char* username = getArgValue(keys, values, count, "-u");

        if (!username) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        if (!userManager.isUserLoggedIn(username)) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        char result[8192];
        int ret = orderManager.queryOrder(username, result);
        if (ret == 0) {
            printf("%s", result);
        } else {
            printf("-1\n");
        }

        freeArgs(keys, values, count);
    }

    void handleRefundTicket(const char* args) {
        char* keys[20], *values[20];
        int count;
        parseArgs(args, keys, values, count);

        const char* username = getArgValue(keys, values, count, "-u");
        const char* orderIndexStr = getArgValue(keys, values, count, "-n");

        if (!username) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        if (!userManager.isUserLoggedIn(username)) {
            printf("-1\n");
            freeArgs(keys, values, count);
            return;
        }

        int orderIndex = orderIndexStr ? parseInt(orderIndexStr) : 1;
        int result = orderManager.refundTicket(username, orderIndex);
        printf("%d\n", result);

        freeArgs(keys, values, count);
    }

    void handleClean() {
        userManager.clean();
        trainManager.clean();
        orderManager.clean();
        printf("0\n");
    }

    void handleExit() {
        printf("bye\n");
    }
};

int main() {
    TicketSystem system;
    char command[4096];

    while (fgets(command, sizeof(command), stdin)) {
        // Remove newline
        command[strcspn(command, "\n")] = 0;

        if (strlen(command) == 0) continue;

        system.processCommand(command);
    }

    return 0;
}