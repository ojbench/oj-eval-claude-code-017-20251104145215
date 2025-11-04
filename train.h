#ifndef TRAIN_H
#define TRAIN_H

#include "utils.h"

struct Train {
    char trainID[21];
    int stationNum;
    char stations[MAX_STATIONS][11];  // station names
    int seatNum;
    int prices[MAX_STATIONS - 1];     // prices between stations
    Time startTime;
    int travelTimes[MAX_STATIONS - 1]; // travel times in minutes
    int stopoverTimes[MAX_STATIONS - 2]; // stopover times in minutes
    Date saleDate[2];                 // start and end sale dates
    char type;
    bool isReleased;
    int totalSeats[MAX_STATIONS - 1]; // available seats between stations

    Train() : stationNum(0), seatNum(0), type(' '), isReleased(false) {
        trainID[0] = '\0';
        for (int i = 0; i < MAX_STATIONS - 1; i++) {
            prices[i] = 0;
            travelTimes[i] = 0;
            totalSeats[i] = 0;
        }
        for (int i = 0; i < MAX_STATIONS - 2; i++) {
            stopoverTimes[i] = 0;
        }
    }
};

struct TicketInfo {
    char trainID[21];
    char fromStation[11];
    char toStation[11];
    Date date;
    Time departureTime;
    Time arrivalTime;
    int price;
    int availableSeats;
};

class TrainManager {
private:
    Train trains[MAX_TRAINS];
    int trainCount;

public:
    TrainManager();

    int addTrain(const char* trainID, int stationNum, int seatNum, const char* stations,
                 const char* prices, const char* startTime, const char* travelTimes,
                 const char* stopoverTimes, const char* saleDate, char type);
    int releaseTrain(const char* trainID);
    int queryTrain(const char* trainID, const char* date, char* result);
    int deleteTrain(const char* trainID);
    int queryTicket(const char* fromStation, const char* toStation, const char* date,
                    const char* priority, char* result);
    int queryTransfer(const char* fromStation, const char* toStation, const char* date,
                      const char* priority, char* result);

    Train* findTrain(const char* trainID);
    bool isTrainReleased(const char* trainID);
    int getStationIndex(const Train* train, const char* station);
    int calculatePrice(const Train* train, int fromIndex, int toIndex);
    Time calculateArrivalTime(const Train* train, int stationIndex, const Date& departureDate);
    int getAvailableSeats(Train* train, int fromIndex, int toIndex, const Date& date);
    bool updateSeats(Train* train, int fromIndex, int toIndex, int numTickets, bool buy);

    void clean();
};

#endif // TRAIN_H