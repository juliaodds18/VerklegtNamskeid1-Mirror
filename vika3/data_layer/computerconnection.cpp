#include "computerconnection.h"
#include "utilities/constants.h"
#include <QDebug>
#include <QMessageBox>

ComputerConnection::ComputerConnection(){
    QSqlDatabase db2;
    QString connectionName = "name2";
    QString dbName = "computer_science_db.sqlite";

    if(QSqlDatabase::contains(connectionName)){
            db2 = QSqlDatabase::database(connectionName);
        }
        else{
            db2 = QSqlDatabase::addDatabase("QSQLITE", connectionName);
            db2.setDatabaseName(dbName);

            db2.open();
        }

    query2 = QSqlQuery(db2);
}

vector<Computer> ComputerConnection::getComputerList(){
    // Read all information from the computers table
    query2.exec("SELECT * FROM Computers WHERE deleted = 'false'");

    while(query2.next()){
        Computer C = getCompValuesFromDB(query2);
        computers.push_back(C);
    }
    return computers;
}

vector<Computer> ComputerConnection::getComputerTrash(){
    // Read all information from the computers trash
    query2.exec("SELECT * FROM Computers WHERE deleted = 'true'");

    vector<Computer> computerTrash;

    while(query2.next()){
        Computer C = getCompValuesFromDB(query2);
        computerTrash.push_back(C);
    }
    return computerTrash;
}

Computer ComputerConnection::getCompValuesFromDB(QSqlQuery query2){

    int id = query2.value("id").toUInt();
    string name = query2.value("computer_name").toString().toStdString();
    int buildYear = query2.value("build_year").toUInt();
    string computerType = query2.value("computer_type").toString().toStdString();
    string wasBuilt = query2.value("was_built").toString().toStdString();
    string description = query2.value("description").toString().toStdString();
    QByteArray outByteArray = query2.value("image").toByteArray();
    Computer tempo(id, name, buildYear, computerType, wasBuilt, description,outByteArray);
    return tempo;
}

void ComputerConnection::addToCompTable(Computer computer){
        string name = computer.getComputerName();
        int buildYear = computer.getBuildYear();
        string computerType = computer.getComputerType();
        string wasBuilt = computer.getWasItBuilt();
        string desc = computer.getComputerDescription();
        string deleted = "false";
        QByteArray image = computer.getImageByteArray();


        transform(computerType.begin(), computerType.end(), computerType.begin(), ::tolower);
        transform(wasBuilt.begin(), wasBuilt.end(), wasBuilt.begin(), ::tolower);

        query2.prepare("INSERT INTO computers VALUES(NULL, :tempName, :tempYear, :tempType, :tempWasBuilt, :tempDesc, :tempImage, :tempDeleted)");
        query2.bindValue(":tempName", QString::fromStdString(name));
        if(buildYear == 0){
            query2.bindValue(":tempYear", QVariant(QVariant::String));
        }
        else{
            query2.bindValue(":tempYear", QString::number(buildYear));
        }
        query2.bindValue(":tempType", QString::fromStdString(computerType));
        query2.bindValue(":tempWasBuilt", QString::fromStdString(wasBuilt));
        query2.bindValue(":tempDesc", QString::fromStdString(desc));
        query2.bindValue(":tempDeleted", QString::fromStdString(deleted));  // Deleted
        if(image.isEmpty()){
            query2.bindValue(":tempImage", QVariant(QVariant::String));
        }
        else{
            query2.bindValue(":tempImage", image);       // Image BLOB value (set to NULL for now)
        }
        query2.exec();
}

vector<Computer> ComputerConnection::printQuery(string built, string type, string orderCol, string order){

    // built can be: "true" or "false"
    // type can be: "mechanical" or "electronic" or "transistor"
    // orderCol can be: "computer_name" or "build_year" or "computer_type";

    vector<Computer> list;

    if(built == "" && type == ""){			// If user doesn't choose any filters / wants entire list
        query2.prepare(QString("SELECT * FROM Computers WHERE deleted = 'false' ORDER BY %1 " + QString::fromStdString(order)).arg(QString::fromStdString(orderCol)));
    }
    else if(built != "" && type == ""){		// If user chooses to filter only by if the computer was built or not
        query2.prepare(QString("SELECT * FROM Computers WHERE was_built = :built AND deleted = 'false' ORDER BY %1 " + QString::fromStdString(order)).arg(QString::fromStdString(orderCol)));
    }
    else if(built == "" && type != ""){		// If user chooses to filter only by computer type
        query2.prepare(QString("SELECT * FROM Computers WHERE computer_type = :type AND deleted = 'false' ORDER BY %1 " + QString::fromStdString(order)).arg(QString::fromStdString(orderCol)));
    }
    else if (built != "" && type != ""){	// If user chooses to filter by both if computer was built and computer type
        query2.prepare(QString("SELECT * FROM Computers WHERE was_built = :built AND computer_type = :type AND deleted = 'false' ORDER BY %1 " + QString::fromStdString(order)).arg(QString::fromStdString(orderCol)));
    }
    query2.bindValue(":built",QString::fromStdString(built));
    query2.bindValue(":type",QString::fromStdString(type));

    query2.exec();

    while(query2.next()){
        int id = query2.value("id").toUInt();
        string name = query2.value("computer_name").toString().toStdString();
        int buildYear = query2.value("build_year").toUInt();
        string computerType = query2.value("computer_type").toString().toStdString();
        string wasBuilt = query2.value("was_built").toString().toStdString();
        string description = query2.value("description").toString().toStdString();
        QByteArray image = query2.value("image").toByteArray();

        list.push_back(Computer(id, name, buildYear, computerType, wasBuilt, description, image));
    }

    return list;
}

vector<Computer> ComputerConnection::searchComp(string searchWord, string searchBy, string built, string type, string orderBy, string direction){
    vector<Computer> searchTempComp;

    if(built == "" && type == ""){                              // If user doesn't choose any filters / wants entire list
            query2.prepare(QString("SELECT * FROM Computers WHERE deleted = 'false' AND " + QString::fromStdString(searchBy) + " LIKE '%'||:word||'%' ORDER BY " + QString::fromStdString(orderBy) + " " + QString::fromStdString(direction) + ";"));
    }
    else if(built != "" && type == ""){                         // If user chooses to filter only by if the computer was built or not
        query2.prepare(QString("SELECT * FROM Computers WHERE was_built = :built AND deleted = 'false' AND " + QString::fromStdString(searchBy) + " LIKE '%'||:word||'%' ORDER BY " + QString::fromStdString(orderBy) + " " + QString::fromStdString(direction) + ";"));
    }
    else if(built == "" && type != "" && type != "other"){		// If user chooses to filter only by computer type
        query2.prepare(QString("SELECT * FROM Computers WHERE computer_type = :type AND deleted = 'false' AND " + QString::fromStdString(searchBy) + " LIKE '%'||:word||'%' ORDER BY " + QString::fromStdString(orderBy) + " " + QString::fromStdString(direction) + ";"));
    }
    else if (built != "" && type != "" && type != "other"){     // If user chooses to filter by both if computer was built and computer type
        query2.prepare(QString("SELECT * FROM Computers WHERE was_built = :built AND computer_type = :type AND " + QString::fromStdString(searchBy) + " LIKE '%'||:word||'%' ORDER BY " + QString::fromStdString(orderBy) + " " + QString::fromStdString(direction) + ";"));
    }
    else if(built == "" && type == "other"){
        query2.prepare(QString("SELECT * FROM Computers WHERE deleted = 'false' AND computer_type not in (" + QString::fromStdString("'mechanical', ") + QString::fromStdString("'electronic', ") + QString::fromStdString("'transistor'") + ") AND " + QString::fromStdString(searchBy) + " LIKE '%'||:word||'%' ORDER BY " + QString::fromStdString(orderBy) + " " + QString::fromStdString(direction) + ";"));
    }
    else if(built != "" && type == "other"){
        query2.prepare(QString("SELECT * FROM Computers WHERE deleted = 'false' AND computer_type not in (" + QString::fromStdString("mechanical, ") + QString::fromStdString("electronic") + ") AND was_built = :built AND " + QString::fromStdString(searchBy) + " LIKE '%'||:word||'%' ORDER BY " + QString::fromStdString(orderBy) + " " + QString::fromStdString(direction) + ";"));
    }
    query2.bindValue(":word", QString::fromStdString(searchWord));
    query2.bindValue(":built",QString::fromStdString(built));
    query2.bindValue(":type",QString::fromStdString(type));
    query2.exec();

    while(query2.next()){
        int id = query2.value("id").toUInt();
        string name = query2.value("computer_name").toString().toStdString();
        int buildYear = query2.value("build_year").toUInt();
        string computerType = query2.value("computer_type").toString().toStdString();
        string wasBuilt = query2.value("was_built").toString().toStdString();
        string description = query2.value("description").toString().toStdString();
        QByteArray image = query2.value("image").toByteArray();

        searchTempComp.push_back(Computer(id, name, buildYear, computerType, wasBuilt, description, image));
    }
    return searchTempComp;
}
// ---------------------------------------------------------------------
//                           DELETE FUNCTIONS
// ---------------------------------------------------------------------

void ComputerConnection::removeSingleComputer(Computer comp){

    stringstream sqlQuery;
    sqlQuery << "DELETE FROM Computers WHERE deleted = 'true' AND id = " << comp.getId();
    query2.exec(QString::fromStdString(sqlQuery.str()));
}

void ComputerConnection::removeComputer(){

    query2.prepare("DELETE FROM Computers WHERE deleted = 'true'");
    query2.exec();
}
void ComputerConnection::computerToTrash(Computer comp){
    int id = comp.getId();
    string name = comp.getComputerName();
    string built = comp.getWasItBuilt();
    int buildYear = comp.getBuildYear();
    string type = comp.getComputerType();
    string compdesc = comp.getComputerDescription();
    string deleted = "true";

    string buildYearString;
    ostringstream convert;
    convert << buildYear;
    buildYearString = convert.str();

    string idString;
    ostringstream convert3;
    convert3 << id;
    idString = convert3.str();

    QString editId = QString::fromStdString(idString);
    QString editName = QString::fromStdString(name);
    QString editBuilt = QString::fromStdString(built);
    QString editYear = QString::fromStdString(buildYearString);
    QString editType = QString::fromStdString(type);
    QString editcompDesc = QString::fromStdString(compdesc);
    QString editDeleted = QString::fromStdString(deleted);

    query2.prepare("UPDATE Computers SET id = "+ editId +", computer_name = '"+ editName +"', was_built = '"+ editBuilt +"', build_year = '"+ editYear +"', computer_type = '"+ editType +"', description = '"+ editcompDesc+"', deleted = '"+ editDeleted +"' WHERE id = "+ editId +"");
    query2.exec();
}
void ComputerConnection::editComputer(Computer comp){
    int id = comp.getId();
    string name = comp.getComputerName();
    string built = comp.getWasItBuilt();
    int buildYear = comp.getBuildYear();
    string type = comp.getComputerType();
    string compdesc = comp.getComputerDescription();
    string deleted = "false";
    QByteArray image = comp.getImageByteArray();

    query2.prepare("UPDATE Computers SET id = :id, computer_name = :name, was_built = :built, build_year = :buildYear, computer_type = :type, description = :description, image = :image, deleted = :deleted WHERE id = :id");
    query2.bindValue(":id", QString::number(id));
    query2.bindValue(":name", QString::fromStdString(name));
    query2.bindValue(":built", QString::fromStdString(built));
    query2.bindValue(":buildYear", QString::number(buildYear));
    query2.bindValue(":type", QString::fromStdString(type));
    query2.bindValue(":description", QString::fromStdString(compdesc));
    if(image.isEmpty()){
        query2.bindValue(":image", QVariant(QVariant::String));
    }
    else{
        query2.bindValue(":image", image);       // Image BLOB value (set to NULL for now)
    }
    query2.bindValue(":deleted", QString::fromStdString(deleted));
    query2.exec();

    qDebug() << query2.lastError().text();

}

void ComputerConnection::deleteAllComputers(){
    query2.exec("DELETE FROM computers");
}

int ComputerConnection::getHighestId(){
    query2.exec("SELECT MAX(id) FROM Computers");
    int highestId;

    while(query2.next()){
        highestId = query2.value("MAX(id)").toUInt();
    }
    return highestId;
}
