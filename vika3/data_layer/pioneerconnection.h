#ifndef PIONEERCONNECTION_H
#define PIONEERCONNECTION_H
#include "data_types/pioneer.h"
#include "data_types/computer.h"
#include <QCoreApplication>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

class PioneerConnection
{
public:
    PioneerConnection();
        // Default constructor, creates connection with SQLite database
    vector<Pioneer> getPioneerList();
        // Returns pioneers vector
    Pioneer getPioValuesFromDB(QSqlQuery query);
        //gets the pioneer values and returns them
    void addToPioTable(vector<Pioneer>& list);
        // Adds list vector to Pioneers table in SQL database
    vector<Pioneer> getPioneerListAsc();
        // Returns pioneers vector but with the name in ascending order
    vector<Pioneer> getPioneerListDesc();
        // Returns pioneers vector but with the name in descending order
    string convertToString(int year);
        // Converts integers to strings
    void deleteSinglePio(string name);
        // Deletes a single pioneer from table
    void deleteAllPioneers();
        // Deletes the entire table

    // ---------------------------------------------------------------------
    //                        PRINT QUERY FUNCTIONS:
    // ---------------------------------------------------------------------

    vector<Pioneer> printQueryPioneers(string sex, string dYear, string orderCol, string order);
        // Gets input from Print menu in interface and returns what the user wants to see printed
    vector<Pioneer> searchPio(string searchWord, int input);
        // Searches through database for a name of pioneer that matches a search word

private:
    vector<Pioneer> pioneers;       // Vector that holds onto all information on runtime
    Pioneer temp;
    QSqlQuery query;                // Object used to send query to SQLite database
};


#endif // PIONEERCONNECTION_H