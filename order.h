#ifndef ORDER_H
#define ORDER_H

#include "utils.h"
#include "user.h"

class TrainManager; // Forward declaration

enum OrderStatus {
    ORDER_SUCCESS,
    ORDER_PENDING,
    ORDER_REFUNDED
};

struct Order {
    int id;
    char username[21];
    char trainID[21];
    char fromStation[11];
    char toStation[11];
    Date departureDate;
    Time departureTime;
    Time arrivalTime;
    int price;
    int numTickets;
    OrderStatus status;
    long long timestamp;

    Order() : id(0), price(0), numTickets(0), status(ORDER_SUCCESS), timestamp(0) {
        username[0] = '\0';
        trainID[0] = '\0';
        fromStation[0] = '\0';
        toStation[0] = '\0';
    }
};

class OrderManager {
private:
    Order orders[MAX_ORDERS];
    int orderCount;
    int nextOrderId;

public:
    OrderManager();

    int buyTicket(const char* username, const char* trainID, const char* date,
                  int numTickets, const char* fromStation, const char* toStation,
                  bool queueIfUnavailable, int& totalPrice, TrainManager* trainManager);
    int queryOrder(const char* username, char* result);
    int refundTicket(const char* username, int orderIndex);

    void processPendingOrders(TrainManager* trainManager);
    Order* getUserOrders(const char* username, int& count);
    bool canRefundOrder(const char* username, int orderIndex);

    void clean();
};

#endif // ORDER_H