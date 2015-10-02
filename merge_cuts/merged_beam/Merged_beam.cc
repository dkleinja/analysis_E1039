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

  int roadset = 61;

  sprintf(schemaOutput, "Analysis_roadset%d_R005_V001", roadset);
  sprintf(login, "root");
  sprintf(password, "");
  sprintf(server, "localhost");
  sprintf(inputFile, "cuts1489v1_merged_roadset%d_R005_V001", roadset);

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

  sprintf(stmt, "DROP TABLE IF EXISTS kDimuon, kTrack, Beam, BeamDAQ, Spill;");
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
  
  sprintf(stmt, "CREATE TABLE kDimuon SELECT * FROM %s.kDimuon", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  sprintf(stmt, "CREATE TABLE kTrack SELECT * FROM %s.kTrack", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  sprintf(stmt, "CREATE INDEX spill_pos ON kDimuon (spillID, posTrackID);");
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  sprintf(stmt, "CREATE INDEX spill_track ON kTrack (spillID, trackID);");
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);
 
    //need to add kTrack momentums here
    sprintf(stmt, "ALTER TABLE kDimuon ADD COLUMN posx1 DOUBLE, "
	    "ADD COLUMN posy1 DOUBLE, "
	    "ADD COLUMN posx3 DOUBLE, "
	    "ADD COLUMN posy3 DOUBLE, "
	    "ADD COLUMN negx1 DOUBLE, "
	    "ADD COLUMN negy1 DOUBLE, "
	    "ADD COLUMN negx3 DOUBLE, "
	    "ADD COLUMN negy3 DOUBLE");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);   

    sprintf(stmt, "UPDATE kDimuon SET posx1 = 0, posy1 = 0, posx3 = 0, posy3 = 0, negx1 = 0, negy1 = 0, negx3 = 0, negy3 = 0;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "OPTIMIZE TABLES kTrack, kDimuon;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);
    res = mysql_store_result(con);

    sprintf(stmt, "UPDATE kDimuon, kTrack SET kDimuon.posx1=kTrack.x1 "
            "WHERE kDimuon.spillID = kTrack.spillID AND kDimuon.posTrackID = kTrack.TrackID");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    
    sprintf(stmt, "UPDATE kDimuon, kTrack SET kDimuon.posy1=kTrack.y1 "
            "WHERE kDimuon.spillID = kTrack.spillID AND kDimuon.posTrackID = kTrack.TrackID");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;

    sprintf(stmt, "UPDATE kDimuon, kTrack SET kDimuon.posx3=kTrack.x3 "
            "WHERE kDimuon.spillID = kTrack.spillID AND kDimuon.posTrackID = kTrack.TrackID");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;

    sprintf(stmt, "UPDATE kDimuon, kTrack SET kDimuon.posy3=kTrack.y3 "
            "WHERE kDimuon.spillID = kTrack.spillID AND kDimuon.posTrackID = kTrack.TrackID");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;

    sprintf(stmt, "UPDATE kDimuon, kTrack SET kDimuon.negx1=kTrack.x1 "
            "WHERE kDimuon.spillID = kTrack.spillID AND kDimuon.negTrackID = kTrack.TrackID");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    
    sprintf(stmt, "UPDATE kDimuon, kTrack SET kDimuon.negy1=kTrack.y1 "
            "WHERE kDimuon.spillID = kTrack.spillID AND kDimuon.negTrackID = kTrack.TrackID");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;

    sprintf(stmt, "UPDATE kDimuon, kTrack SET kDimuon.negx3=kTrack.x3 "
            "WHERE kDimuon.spillID = kTrack.spillID AND kDimuon.negTrackID = kTrack.TrackID");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;

    sprintf(stmt, "UPDATE kDimuon, kTrack SET kDimuon.negy3=kTrack.y3 "
            "WHERE kDimuon.spillID = kTrack.spillID AND kDimuon.negTrackID = kTrack.TrackID");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
  
    //add beam information to kDimuon
/*
    sprintf(stmt, "ALTER TABLE kDimuon ADD COLUMN targetPos TINYINT UNSIGNED;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);
*/
    sprintf(stmt, "ALTER TABLE kDimuon ADD COLUMN m3hm DOUBLE, "
	    "ADD COLUMN m3hs DOUBLE, "
	    "ADD COLUMN m3vm DOUBLE, "
	    "ADD COLUMN m3vs DOUBLE");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "ALTER TABLE kDimuon ADD COLUMN m2hm DOUBLE, "
	    "ADD COLUMN m2hs DOUBLE, "
	    "ADD COLUMN m2vm DOUBLE, "
	    "ADD COLUMN m2vs DOUBLE");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);
/*
    sprintf(stmt, "UPDATE kDimuon, Spill SET kDimuon.targetPos = Spill.targetPos "
	    "WHERE kDimuon.spillID = Spill.spillID");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);
*/
    sprintf(stmt, "UPDATE kDimuon SET m3hm = 0, m3hs = 0, m3vm = 0, m3vs = 0");
    //sprintf(stmt, "UPDATE kDimuon SET m3hm = 0");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);
    
    sprintf(stmt, "UPDATE kDimuon SET m2hm = 0, m2hs = 0, m2vm = 0, m2vs = 0");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "UPDATE kDimuon, Beam SET kDimuon.m3hm = Beam.value "
	    "WHERE kDimuon.spillID = Beam.spillID AND Beam.name = 'E:M3TGHM'");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "UPDATE kDimuon, Beam SET kDimuon.m3hs = Beam.value "
	    "WHERE kDimuon.spillID = Beam.spillID AND Beam.name = 'E:M3TGHS'");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "UPDATE kDimuon, Beam SET kDimuon.m3vm = Beam.value "
	    "WHERE kDimuon.spillID = Beam.spillID AND Beam.name = 'E:M3TGVM'");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "UPDATE kDimuon, Beam SET kDimuon.m3vs = Beam.value "
	    "WHERE kDimuon.spillID = Beam.spillID AND Beam.name = 'E:M3TGVS'");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "UPDATE kDimuon, Beam SET kDimuon.m2hm = Beam.value "
	    "WHERE kDimuon.spillID = Beam.spillID AND Beam.name = 'E:M2C2HM'");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "UPDATE kDimuon, Beam SET kDimuon.m2hs = Beam.value "
	    "WHERE kDimuon.spillID = Beam.spillID AND Beam.name = 'E:M2C2HS'");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "UPDATE kDimuon, Beam SET kDimuon.m2vm = Beam.value "
	    "WHERE kDimuon.spillID = Beam.spillID AND Beam.name = 'E:M2C2VM'");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "UPDATE kDimuon, Beam SET kDimuon.m2vs = Beam.value "
	    "WHERE kDimuon.spillID = Beam.spillID AND Beam.name = 'E:M2C2VS'");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    //add live proton information to Spill
  sprintf(stmt, "ALTER TABLE Spill ADD COLUMN liveProton2 INT;");
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

    sprintf(stmt, "UPDATE Spill, Beam, BeamDAQ SET Spill.liveProton2 = Beam.value * (BeamDAQ.QIEsum - BeamDAQ.inhibit_block_sum - BeamDAQ.trigger_sum_no_inhibit) / BeamDAQ.QIEsum "
            "WHERE Spill.spillID = Beam.spillID AND Spill.spillID = BeamDAQ.spillID AND Beam.name = 'S:G2SEM'");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    //ok, cleanup
    TimeStamp(time_start); 
    sprintf(stmt, "DROP TABLE IF EXISTS Beam, BeamDAQ, kTrack;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);
    
    sprintf(stmt, "OPTIMIZE TABLES kDimuon, Spill;");
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
  cout << "\t Current Running time is " << currenttime <<  " seconds.  CLOCKS_PER_SEC = " << CLOCKS_PER_SEC << endl;
  //cout << "\t Current Running time is " << time_delta <<  " seconds." << endl;

}

