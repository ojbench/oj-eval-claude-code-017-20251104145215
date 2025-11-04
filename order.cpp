#include "order.h"
#include "train.h"
#include <cstring>
#include <cstdio>
#include <time.h>

OrderManager::OrderManager() : orderCount(0), nextOrderId(1) {}

int OrderManager::buyTicket(const char* username, const char* trainID, const char* dateStr,
                           int numTickets, const char* fromStation, const char* toStation,
                           bool queueIfUnavailable, int& totalPrice) {
    if (numTickets <= 0 || numTickets > 100000) return -1;

    // This is a simplified implementation
    // In a real system, we would need to coordinate with TrainManager

    totalPrice = numTickets * 100; // Placeholder price calculation

    if (orderCount >= MAX_ORDERS) return -1;

    Order& newOrder = orders[orderCount++];
    newOrder.id = nextOrderId++;
    strcpy(newOrder.username, username);
    strcpy(newOrder.trainID, trainID);
    strcpy(newOrder.fromStation, fromStation);
    strcpy(newOrder.toStation, toStation);
    newOrder.departureDate = parseDate(dateStr);
    newOrder.numTickets = numTickets;
    newOrder.price = totalPrice;
    newOrder.status = ORDER_SUCCESS;
    newOrder.timestamp = time(nullptr);

    return totalPrice;
}

int OrderManager::queryOrder(const char* username, char* result) {
    // Count user's orders
    int userOrderCount = 0;
    for (int i = 0; i < orderCount; i++) {
        if (strcmp(orders[i].username, username) == 0) {
            userOrderCount++;
        }
    }

    if (userOrderCount == 0) {
        strcpy(result, "0");
        return 0;
    }

    char* ptr = result;
    ptr += sprintf(ptr, "%d\n", userOrderCount);

    // Output orders in reverse order (newest first)
    for (int i = orderCount - 1; i >= 0; i--) {
        if (strcmp(orders[i].username, username) == 0) {
            const char* statusStr = "";
            switch (orders[i].status) {
                case ORDER_SUCCESS: statusStr = "success"; break;
                case ORDER_PENDING: statusStr = "pending"; break;
                case ORDER_REFUNDED: statusStr = "refunded"; break;
            }

            ptr += sprintf(ptr, "[%s] %s %s %02d-%02d %02d:%02d -> %s %02d-%02d %02d:%02d %d %d\n",
                          statusStr, orders[i].trainID, orders[i].fromStation,
                          orders[i].departureDate.month, orders[i].departureDate.day,
                          orders[i].departureTime.hour, orders[i].departureTime.minute,
                          orders[i].toStation, orders[i].departureDate.month,
                          orders[i].departureDate.day, orders[i].arrivalTime.hour,
                          orders[i].arrivalTime.minute, orders[i].price, orders[i].numTickets);
        }
    }

    return 0;
}

int OrderManager::refundTicket(const char* username, int orderIndex) {
    // Find user's orders
    int userOrderIndices[MAX_ORDERS];
    int userOrderCount = 0;

    for (int i = 0; i < orderCount; i++) {
        if (strcmp(orders[i].username, username) == 0) {
            userOrderIndices[userOrderCount++] = i;
        }
    }

    if (orderIndex < 1 || orderIndex > userOrderCount) return -1;

    int actualIndex = userOrderIndices[userOrderCount - orderIndex];
    Order& order = orders[actualIndex];

    if (order.status != ORDER_SUCCESS) return -1;

    order.status = ORDER_REFUNDED;
    return 0;
}

void OrderManager::clean() {
    orderCount = 0;
    nextOrderId = 1;
}