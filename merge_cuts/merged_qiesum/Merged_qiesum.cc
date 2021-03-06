#include <mysql.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

char stmt[5000];

MYSQL_RES* res;
MYSQL_ROW row;
MYSQL* con;

void PrintHelp();
int MysqlErrorCheck();

clock_t time_delta;
clock_t time_start;
double currenttime;
void TimeStamp(clock_t starttime);

int main(int argc, char **argv)
{
  time_start = clock();

  bool listFromMysql = 1;
  bool setFile = false;

  bool kDim = 0;
  bool jDim = 0;

  char inputFile[1000];
  char schemaOutput[100];
  char login[30], password[30], server[100];

  sprintf(schemaOutput, "BeamAngle");
  sprintf(login, "root");
  sprintf(password, "");
  sprintf(server, "localhost");
  sprintf(inputFile, "newMerge");

  vector<string> schemaVector;

  int port = 3283;  // 3306 for most servers, 3283 for seaquel

  // command line parsing
  if (argc == 1)
  {
    PrintHelp();
    return 0;
  }

  for (int j = 1; j < argc; j=j+2)
  {
    if (!strcmp(argv[j], "-f"))
      strcpy(inputFile, argv[j + 1]);
    if (!strcmp(argv[j], "-k"))
      kDim = atoi(argv[j + 1]);
    if (!strcmp(argv[j], "-j"))
      jDim = atoi(argv[j + 1]);
    if (!strcmp(argv[j], "-s"))
      strcpy(schemaOutput, argv[j + 1]);
    if (!strcmp(argv[j], "-l"))
      strcpy(login, argv[j + 1]);
    if (!strcmp(argv[j], "-p"))
      strcpy(password, argv[j + 1]);
    if (!strcmp(argv[j], "-m") || !strcmp(argv[j], "-server"))
      strcpy(server, argv[j + 1]);
    if (!strcmp(argv[j], "-port"))
      port = atoi(argv[j+1]);
    if (!strcmp(argv[j], "-h") || !strcmp(argv[j], "-help"))
      {
	PrintHelp();
	return 0;
      }
  }

  con = mysql_init(NULL);
  mysql_real_connect(con, server, login, password, NULL, port, NULL, 0);

  sprintf(stmt, "CREATE SCHEMA IF NOT EXISTS %s;", schemaOutput);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  sprintf(stmt, "USE %s;", schemaOutput);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  sprintf(stmt, "DROP TABLE IF EXISTS Beam, BeamDAQ, Spill, Luminosity;");
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);
  
  sprintf(stmt, "CREATE TABLE Beam LIKE %s.Beam;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  sprintf(stmt, "CREATE TABLE BeamDAQ LIKE %s.BeamDAQ;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  sprintf(stmt, "CREATE TABLE Spill LIKE %s.Spill;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  sprintf(stmt, "INSERT INTO Beam SELECT Beam.* FROM %s.Beam", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  sprintf(stmt, "INSERT INTO BeamDAQ SELECT BeamDAQ.* FROM %s.BeamDAQ", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  
  sprintf(stmt, "INSERT INTO Spill SELECT Spill.* FROM %s.Spill", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);
  
  //add live proton information to Spill
  sprintf(stmt, "ALTER TABLE Spill ADD COLUMN liveProton DOUBLE, "
	  "ADD COLUMN G2SEM DOUBLE, "
	  "ADD COLUMN liveFraction DOUBLE;");
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  sprintf(stmt, "UPDATE Spill, Beam, BeamDAQ SET Spill.liveProton = Beam.value * (BeamDAQ.QIEsum - BeamDAQ.inhibit_block_sum - BeamDAQ.trigger_sum_no_inhibit) / BeamDAQ.QIEsum "
	  "WHERE Spill.spillID = Beam.spillID AND Spill.spillID = BeamDAQ.spillID AND Beam.name = 'S:G2SEM'");
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  sprintf(stmt, "UPDATE Spill, Beam SET Spill.G2SEM = Beam.value "
	  "WHERE Spill.spillID = Beam.spillID AND Beam.name = 'S:G2SEM'");
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  sprintf(stmt, "UPDATE Spill, BeamDAQ SET Spill.liveFraction = (BeamDAQ.QIEsum - BeamDAQ.inhibit_block_sum - BeamDAQ.trigger_sum_no_inhibit) / BeamDAQ.QIEsum "
	  "WHERE Spill.spillID = BeamDAQ.spillID");
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  sprintf(stmt, "DROP TABLE IF EXISTS Beam, BeamDAQ;");
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);


  sprintf(stmt, "ALTER TABLE Spill ORDER BY spillID;");
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);
  
  sprintf(stmt, "OPTIMIZE TABLES Luminosity;");
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);
}

void PrintHelp()
{
  cout << "\n\nCommand Line Options:\n" <<
    "-f arg                         :Uses arg as the schema name to cut on\n" <<
    "-s arg                         :Writes the data to MySQL schema arg\n" <<
    "-j arg                         :Also merge kDimuon tables if 1\n" <<
    "-k arg                         :Also merge jDimuon tables if 1\n" <<
    "-l arg                         :Changes the MySQL login to arg\n" <<
    "-p arg                         :Changes the MySQL password to arg\n" <<
    "-m arg, -server arg            :Changes the MySQL server to arg\n" <<
    "-port arg                      :Changes the MySQL port number to arg\n" <<
    "-h, -help                      :Writes this message to screen and exits\n\n" << endl;
}

int MysqlErrorCheck()
{
  cout << stmt << "\n" << endl;

  if (mysql_errno(con) != 0)
    {
    cout << mysql_errno(con) << endl;
    cout << mysql_error(con) << endl;
    return 1;
  }
  else
    return 0;
}

void TimeStamp(clock_t starttime)
{
  time_delta = clock() - starttime;
  currenttime = double(time_delta) / CLOCKS_PER_SEC;
  //cout << "\t The start time was " << starttime <<  " seconds." << endl;
  cout << "\t Current Running time is " << currenttime <<  " seconds." << endl;
  //cout << "\t Current Running time is " << time_delta <<  " seconds." << endl;

}
