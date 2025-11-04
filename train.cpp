#include "train.h"
#include "utils.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>

TrainManager::TrainManager() : trainCount(0) {}

int TrainManager::addTrain(const char* trainID, int stationNum, int seatNum, const char* stations,
                          const char* prices, const char* startTime, const char* travelTimes,
                          const char* stopoverTimes, const char* saleDate, char type) {
    // Check if train already exists
    if (findTrain(trainID)) return -1;

    if (trainCount >= MAX_TRAINS) return -1;
    if (stationNum < 2 || stationNum > MAX_STATIONS) return -1;
    if (seatNum <= 0 || seatNum > 100000) return -1;

    Train& newTrain = trains[trainCount++];
    strcpy(newTrain.trainID, trainID);
    newTrain.stationNum = stationNum;
    newTrain.seatNum = seatNum;
    newTrain.type = type;
    newTrain.isReleased = false;

    // Parse stations
    char* stationsCopy = new char[strlen(stations) + 1];
    strcpy(stationsCopy, stations);
    char* token = strtok(stationsCopy, "|");
    int stationIndex = 0;
    while (token != nullptr && stationIndex < stationNum) {
        strcpy(newTrain.stations[stationIndex], token);
        token = strtok(nullptr, "|");
        stationIndex++;
    }
    delete[] stationsCopy;

    // Parse prices
    char* pricesCopy = new char[strlen(prices) + 1];
    strcpy(pricesCopy, prices);
    token = strtok(pricesCopy, "|");
    int priceIndex = 0;
    while (token != nullptr && priceIndex < stationNum - 1) {
        newTrain.prices[priceIndex] = parseInt(token);
        token = strtok(nullptr, "|");
        priceIndex++;
    }
    delete[] pricesCopy;

    // Parse start time
    newTrain.startTime = parseTime(startTime);

    // Parse travel times
    char* travelCopy = new char[strlen(travelTimes) + 1];
    strcpy(travelCopy, travelTimes);
    token = strtok(travelCopy, "|");
    int travelIndex = 0;
    while (token != nullptr && travelIndex < stationNum - 1) {
        newTrain.travelTimes[travelIndex] = parseInt(token);
        token = strtok(nullptr, "|");
        travelIndex++;
    }
    delete[] travelCopy;

    // Parse stopover times
    if (stationNum > 2) {
        char* stopoverCopy = new char[strlen(stopoverTimes) + 1];
        strcpy(stopoverCopy, stopoverTimes);
        token = strtok(stopoverCopy, "|");
        int stopoverIndex = 0;
        while (token != nullptr && stopoverIndex < stationNum - 2) {
            newTrain.stopoverTimes[stopoverIndex] = parseInt(token);
            token = strtok(nullptr, "|");
            stopoverIndex++;
        }
        delete[] stopoverCopy;
    }

    // Parse sale dates
    char* saleCopy = new char[strlen(saleDate) + 1];
    strcpy(saleCopy, saleDate);
    token = strtok(saleCopy, "|");
    if (token) {
        newTrain.saleDate[0] = parseDate(token);
        token = strtok(nullptr, "|");
        if (token) {
            newTrain.saleDate[1] = parseDate(token);
        }
    }
    delete[] saleCopy;

    // Initialize seat availability
    for (int i = 0; i < stationNum - 1; i++) {
        newTrain.totalSeats[i] = seatNum;
    }

    return 0;
}

int TrainManager::releaseTrain(const char* trainID) {
    Train* train = findTrain(trainID);
    if (!train) return -1;
    if (train->isReleased) return -1;

    train->isReleased = true;
    return 0;
}

int TrainManager::queryTrain(const char* trainID, const char* dateStr, char* result) {
    Train* train = findTrain(trainID);
    if (!train) return -1;

    Date queryDate = parseDate(dateStr);
    if (queryDate < train->saleDate[0] || queryDate > train->saleDate[1]) return -1;

    char* ptr = result;
    ptr += sprintf(ptr, "%s %c\n", train->trainID, train->type);

    for (int i = 0; i < train->stationNum; i++) {
        Time arrivalTime, departureTime;
        Date arrivalDate = queryDate;
        Date departureDate = queryDate;

        if (i == 0) {
            // Starting station
            departureTime = train->startTime;
            arrivalTime = Time(-1, -1); // Invalid time
        } else {
            // Calculate arrival time
            int totalMinutes = 0;
            for (int j = 0; j < i; j++) {
                totalMinutes += train->travelTimes[j];
                if (j < i - 1) {
                    totalMinutes += train->stopoverTimes[j];
                }
            }

            Time tempTime = train->startTime + totalMinutes;
            arrivalTime = tempTime;

            // Handle date changes
            int dayOffset = (train->startTime.hour * 60 + train->startTime.minute + totalMinutes) / (24 * 60);
            arrivalDate.day += dayOffset;
            if (arrivalDate.day > 31) {
                arrivalDate.month++;
                arrivalDate.day -= 31;
            }
        }

        if (i == train->stationNum - 1) {
            // Terminal station
            departureTime = Time(-1, -1); // Invalid time
        } else {
            if (i > 0) {
                departureTime = arrivalTime + train->stopoverTimes[i - 1];
                departureDate = arrivalDate;

                // Handle date changes for departure
                if (departureTime.hour < arrivalTime.hour ||
                    (departureTime.hour == arrivalTime.hour && departureTime.minute < arrivalTime.minute)) {
                    departureDate.day++;
                    if (departureDate.day > 31) {
                        departureDate.month++;
                        departureDate.day -= 31;
                    }
                }
            }
        }

        int price = 0;
        for (int j = 0; j < i; j++) {
            price += train->prices[j];
        }

        int seats = (i < train->stationNum - 1) ? train->totalSeats[i] : 0;

        if (i == 0) {
            ptr += sprintf(ptr, "%s xx-xx xx:xx -> %02d-%02d %02d:%02d %d %d\n",
                          train->stations[i], departureDate.month, departureDate.day,
                          departureTime.hour, departureTime.minute, price, seats);
        } else if (i == train->stationNum - 1) {
            ptr += sprintf(ptr, "%s %02d-%02d %02d:%02d -> xx-xx xx:xx %d x\n",
                          train->stations[i], arrivalDate.month, arrivalDate.day,
                          arrivalTime.hour, arrivalTime.minute, price);
        } else {
            ptr += sprintf(ptr, "%s %02d-%02d %02d:%02d -> %02d-%02d %02d:%02d %d %d\n",
                          train->stations[i], arrivalDate.month, arrivalDate.day,
                          arrivalTime.hour, arrivalTime.minute, departureDate.month,
                          departureDate.day, departureTime.hour, departureTime.minute,
                          price, seats);
        }
    }

    return 0;
}

int TrainManager::deleteTrain(const char* trainID) {
    Train* train = findTrain(trainID);
    if (!train) return -1;
    if (train->isReleased) return -1;

    // Remove train by shifting array
    int index = train - trains;
    for (int i = index; i < trainCount - 1; i++) {
        trains[i] = trains[i + 1];
    }
    trainCount--;

    return 0;
}

Train* TrainManager::findTrain(const char* trainID) {
    for (int i = 0; i < trainCount; i++) {
        if (strcmp(trains[i].trainID, trainID) == 0) {
            return &trains[i];
        }
    }
    return nullptr;
}

bool TrainManager::isTrainReleased(const char* trainID) {
    Train* train = findTrain(trainID);
    return train && train->isReleased;
}

int TrainManager::getStationIndex(const Train* train, const char* station) {
    for (int i = 0; i < train->stationNum; i++) {
        if (strcmp(train->stations[i], station) == 0) {
            return i;
        }
    }
    return -1;
}

int TrainManager::calculatePrice(const Train* train, int fromIndex, int toIndex) {
    int price = 0;
    for (int i = fromIndex; i < toIndex; i++) {
        price += train->prices[i];
    }
    return price;
}

int TrainManager::queryTicket(const char* fromStation, const char* toStation, const char* dateStr,
                               const char* priority, char* result) {
    Date queryDate = parseDate(dateStr);
    char* ptr = result;
    int count = 0;

    // Simple implementation - just return 0 for now
    ptr += sprintf(ptr, "0\n");

    return 0;
}

int TrainManager::queryTransfer(const char* fromStation, const char* toStation, const char* dateStr,
                               const char* priority, char* result) {
    // Simple implementation - return 0 (no transfer found)
    sprintf(result, "0");
    return 0;
}

void TrainManager::clean() {
    trainCount = 0;
}