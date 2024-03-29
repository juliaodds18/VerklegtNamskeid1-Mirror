#ifndef SERVICE_H
#define SERVICE_H
#include "data_types/pioneer.h"
#include "data_layer/pioneerconnection.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <ctype.h>
#include <algorithm>

using namespace std;

class PioneerService
{
public:
    PioneerService();
        // Default constructor
    void addPioneer(Pioneer pioneer);
        // Adds Pioneer object into tempList vector, then sends that vector to data layer to be added to SQL database
    string toLower(string name);
        // Puts all names to lowercase
    vector<Pioneer> getList();
        // Fetches list from connection
    vector<Pioneer> getTrash();
        // returns all pioneers in trash
    vector<Pioneer> search(string searchWord, string searchBy, string sex, string vitalStats, string orderBy, string direction);
        // Gets search word input from interface, sends word to service,
        // receives a vector with search results from service and sends vector to interface
    vector<Pioneer> printQuery(string sex, string dYear, string sortBy, string direction);
        // Sends query from user into Connection and returns the result of that query as a vector of Pioneer
    void removePioneer();
        // Sends a name of the person to delete into PioneerConnection
    void deleteAllPioneers();
        // Sends a message to PioneerConnection to delete all pioneers
    void editPioneer(Pioneer pio);
        // Sends information about a pioneer that needs to be edited
    int getHighestId();
        // Returns highest id in Pioneers table (most recently added entry)
    void pioneerToTrash(Pioneer pio);
        // Calls PioneerConnection to set delete flag on pio to "true"
    void removeSinglePioneer(Pioneer pio);
        // Calls PioneerConnection to remove pio from SQL database

private:
    PioneerConnection data;         // This object will hold onto all runtime information
    Pioneer temp;                   // Temporary object to be used in various situations
    vector<Pioneer> tempList;       // Vector used to send information to the data layer
};

#endif // SERVICE_H
