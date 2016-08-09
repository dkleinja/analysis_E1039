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

//stupid clock stuff
time_t time_delta;
time_t time_start;
time_t time_end;
double currenttime;
void TimeStamp(time_t starttime);

int main(int argc, char **argv)
{
  time (&time_start);

  bool listFromMysql = 1;
  bool setFile = false;

  bool kDim = 0;
  bool jDim = 0;
  int roadset = 61;

  char inputFile[1000];
  char schemaOutput[100];
  char login[30], password[30], server[100];

  sprintf(schemaOutput, "test_dkleinja_merged_roadset%d_R005_V001", roadset);
  sprintf(login, "seaguest");
  sprintf(password, "qqbar2mu+mu-");
  //sprintf(server, "seaquel.physics.illinois.edu");
  sprintf(server, "e906-db1.fnal.gov");
  //sprintf(login, "root");
  //sprintf(password, "");
  //sprintf(server, "localhost");
  sprintf(inputFile, "merged_roadset%d_R005_V001", roadset);
  vector<string> schemaVector;

  int port = 3306;  // 3306 for most servers, 3283 for seaquel

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
    if (!strcmp(argv[j], "-r"))
      roadset = atoi(argv[j + 1]);
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


  sprintf(schemaOutput, "test_dkleinja_merged_roadset%d_R005_V001", roadset);
  sprintf(inputFile, "merged_roadset%d_R005_V001", roadset);
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

  sprintf(stmt, "DROP TABLE IF EXISTS Run, TriggerRoads, Spill, Event, tempRunList, "
                "QIE, BeamDAQ, Beam, Target, Scaler, tempSpillList, tempEventList, dimuonsAfterTrackCuts;");
  mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

  if (kDim)
  {
    sprintf(stmt, "DROP TABLE IF EXISTS kDimuon, kTrack;");
    mysql_query(con, stmt);
      if (MysqlErrorCheck() == 1)
        return 1;
      TimeStamp(time_start);
  }

  if (jDim)
  {
    sprintf(stmt, "DROP TABLE IF EXISTS jDimuon, jTrack;");
    mysql_query(con, stmt);
      if (MysqlErrorCheck() == 1)
        return 1;
      TimeStamp(time_start);
  }

  sprintf(stmt, "CREATE TABLE IF NOT EXISTS Run LIKE %s.Run;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  sprintf(stmt, "CREATE TABLE tempSpillList (spillID INT);");
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);

  sprintf(stmt, "CREATE TABLE tempRunList (runID INT);");
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);
  if (kDim)
  {
    sprintf(stmt, "CREATE TABLE kDimuon SELECT * FROM %s.kDimuon LIMIT 1;", inputFile);  //I hate the default indices
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "DELETE FROM kDimuon;", inputFile);
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "CREATE INDEX spill_event ON kDimuon (spillID, eventID);");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "CREATE INDEX run_dimuon ON kDimuon (runID, dimuonID);");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "CREATE INDEX spill_pos ON kDimuon (spillID, posTrackID);");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "CREATE INDEX spill_neg ON kDimuon (spillID, negTrackID);");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "CREATE TABLE kTrack SELECT * FROM %s.kTrack LIMIT 1;", inputFile);  //I hate the default indices
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "DELETE FROM kTrack;", inputFile);
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);
    sprintf(stmt, "CREATE INDEX spill_event ON kTrack (spillID, eventID);");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "CREATE INDEX spill_track ON kTrack (spillID, trackID);");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);
  }

  if (jDim)
  {
    sprintf(stmt, "CREATE TABLE IF NOT EXISTS jDimuon LIKE %s.jDimuon;", inputFile);
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "CREATE TABLE IF NOT EXISTS jTrack LIKE %s.jTrack;", inputFile);
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);
  }

  sprintf(stmt, "CREATE TABLE Spill LIKE %s.Spill;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "CREATE TABLE BeamDAQ LIKE %s.BeamDAQ;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "CREATE TABLE Beam LIKE %s.Beam;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "CREATE TABLE IF NOT EXISTS Event LIKE %s.Event;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "CREATE TABLE IF NOT EXISTS Target LIKE %s.Target;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "CREATE TABLE IF NOT EXISTS Scaler LIKE %s.Scaler;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);
/*
  sprintf(stmt, "INSERT INTO tempRunList SELECT DISTINCT runID FROM %s.Spill;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;

  sprintf(stmt, "INSERT INTO tempRunList SELECT DISTINCT runID FROM %s.kTrack;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
*/


  sprintf(stmt, "INSERT INTO tempRunList SELECT DISTINCT run FROM %s.production WHERE ktracked = 1;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);


    sprintf(stmt, "SELECT COUNT(1) FROM %s.Spill;", inputFile);
    mysql_query(con, stmt);
      if (MysqlErrorCheck() == 1)
        return 1;
    TimeStamp(time_start);
    res = mysql_store_result(con);
    row = mysql_fetch_row(res);
    cout << row[0] << " spills before spill cuts" <<endl;

  sprintf(stmt, "INSERT INTO tempSpillList SELECT Spill.spillID FROM %s.Spill "
                "JOIN %s.Beam AS b1 ON b1.spillID = Spill.spillID AND b1.name = 'S:G2SEM' "
                "JOIN %s.Beam AS b2 ON b2.spillID = Spill.spillID AND b2.name = 'F:NM3ION' "
                "JOIN %s.BeamDAQ ON Spill.spillID = BeamDAQ.spillID "
                "JOIN %s.Target ON Target.spillID = Spill.spillID AND Target.name = 'TARGPOS_CONTROL' "
                "JOIN %s.Scaler AS s1 ON s1.spillID = Spill.spillID AND s1.scalerName = 'TSgo' AND s1.spillType = 'EOS' "
                "JOIN %s.Scaler AS s2 ON s2.spillID = Spill.spillID AND s2.scalerName = 'AcceptedMatrix1' AND s2.spillType = 'EOS' "
                "JOIN %s.Scaler AS s3 ON s3.spillID = Spill.spillID AND s3.scalerName = 'AfterInhMatrix1' AND s3.spillType = 'EOS' "
                "JOIN tempRunList ON tempRunList.runID = Spill.runID "
                "WHERE Spill.dataQuality = 0 "
                //"AND Spill.status = 0 "
                "AND targetPos BETWEEN 1 AND 7 "
                "AND targetPos = Target.value "
                "AND Spill.spillID != 0;",
                inputFile, inputFile, inputFile, inputFile, inputFile, inputFile, inputFile, inputFile);
/*
  sprintf(stmt, "INSERT INTO tempSpillList SELECT Spill.spillID FROM %s.Spill "
                "JOIN %s.Beam AS b1 ON b1.spillID = Spill.spillID AND b1.name = 'S:G2SEM' "
                "JOIN %s.Beam AS b2 ON b2.spillID = Spill.spillID AND b2.name = 'F:NM3ION' "
                "JOIN %s.BeamDAQ ON Spill.spillID = BeamDAQ.spillID "
                "JOIN %s.Target ON Target.spillID = Spill.spillID AND Target.name = 'TARGPOS_CONTROL' "
                "JOIN %s.Scaler AS s1 ON s1.spillID = Spill.spillID AND s1.scalerName = 'TSgo' AND s1.spillType = 'EOS' "
                "JOIN %s.Scaler AS s2 ON s2.spillID = Spill.spillID AND s2.scalerName = 'AcceptedMatrix1' AND s2.spillType = 'EOS' "
                "JOIN %s.Scaler AS s3 ON s3.spillID = Spill.spillID AND s3.scalerName = 'AfterInhMatrix1' AND s3.spillType = 'EOS' "
                "JOIN tempRunList ON tempRunList.runID = Spill.runID "
                "WHERE b1.value BETWEEN 2e12 AND 1e13 "
                "AND b2.value BETWEEN 2e12 AND 1e13 "
                "AND s1.value BETWEEN 1e3 AND 8e3 "
                "AND s2.value BETWEEN 1e3 AND 8e3 "
                "AND s3.value BETWEEN 1e3 AND 3e4 "
                "AND s2.value/s3.value BETWEEN 0.2 AND 0.9 "
                "AND targetPos BETWEEN 1 AND 7 "
                "AND targetPos = Target.value "
                "AND QIEsum BETWEEN 4e10 AND 1e12 "
                "AND inhibit_block_sum BETWEEN 4e9 AND 1e11 "
                "AND trigger_sum_no_inhibit BETWEEN 4e9 AND 1e11 "
                "AND dutyfactor53MHz BETWEEN 15 AND 60 "
                "AND Spill.spillID != 0;",
                inputFile, inputFile, inputFile, inputFile, inputFile, inputFile, inputFile, inputFile);
*/
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);
  
  sprintf(stmt, "SELECT COUNT(1) FROM tempSpillList;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
  TimeStamp(time_start);
  res = mysql_store_result(con);
  row = mysql_fetch_row(res);
  cout << row[0] << " spills after Spill.dataQuality cuts" <<endl;
  
  sprintf(stmt, "DELETE FROM tempSpillList USING tempSpillList JOIN "
                "(SELECT spillID FROM %s.Spill GROUP BY spillID HAVING COUNT(1) != 1) AS t1 "
                "ON t1.spillID = tempSpillList.spillID;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "SELECT COUNT(1) FROM tempSpillList;", inputFile);
    mysql_query(con, stmt);
      if (MysqlErrorCheck() == 1)
        return 1;
    TimeStamp(time_start);
    res = mysql_store_result(con);
    row = mysql_fetch_row(res);
    cout << row[0] << " spills after Spill cuts" <<endl;

  sprintf(stmt, "DELETE FROM tempSpillList USING tempSpillList JOIN "
                "(SELECT spillID FROM %s.BeamDAQ GROUP BY spillID HAVING COUNT(1) != 1) AS t1 "
                "ON t1.spillID = tempSpillList.spillID;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "SELECT COUNT(1) FROM tempSpillList;", inputFile);
    mysql_query(con, stmt);
      if (MysqlErrorCheck() == 1)
        return 1;
    TimeStamp(time_start);
    res = mysql_store_result(con);
    row = mysql_fetch_row(res);
    cout << row[0] << " spills after BeamDAQ cuts" <<endl;

  sprintf(stmt, "DELETE FROM tempSpillList USING tempSpillList JOIN "
                "(SELECT spillID FROM %s.Beam WHERE name = 'S:G2SEM' GROUP BY spillID HAVING COUNT(1) != 1) AS t1 "
                "ON t1.spillID = tempSpillList.spillID;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "DELETE FROM tempSpillList USING tempSpillList JOIN "
                "(SELECT spillID FROM %s.Beam WHERE name = 'F:NM3ION' GROUP BY spillID HAVING COUNT(1) != 1) AS t1 "
                "ON t1.spillID = tempSpillList.spillID;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "SELECT COUNT(1) FROM tempSpillList;", inputFile);
    mysql_query(con, stmt);
      if (MysqlErrorCheck() == 1)
        return 1;
    TimeStamp(time_start);
    res = mysql_store_result(con);
    row = mysql_fetch_row(res);
    cout << row[0] << " spills after Beam cuts" <<endl;

  sprintf(stmt, "DELETE FROM tempSpillList USING tempSpillList JOIN "
                "(SELECT spillID FROM %s.Target WHERE name = 'TARGPOS_CONTROL' GROUP BY spillID HAVING COUNT(1) != 1) AS t1 "
                "ON t1.spillID = tempSpillList.spillID;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "SELECT COUNT(1) FROM tempSpillList;", inputFile);
    mysql_query(con, stmt);
      if (MysqlErrorCheck() == 1)
        return 1;
    TimeStamp(time_start);
    res = mysql_store_result(con);
    row = mysql_fetch_row(res);
    cout << row[0] << " spills after Target cuts" <<endl;

  sprintf(stmt, "DELETE FROM tempSpillList USING tempSpillList JOIN "
                "(SELECT spillID FROM %s.Scaler WHERE scalerName = 'AcceptedMATRIX1' AND spillType = 'EOS' "
                "GROUP BY spillID HAVING COUNT(1) != 1) AS t1 "
                "ON t1.spillID = tempSpillList.spillID;", inputFile); 
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "DELETE FROM tempSpillList USING tempSpillList JOIN "
                "(SELECT spillID FROM %s.Scaler WHERE scalerName = 'AfterInhMATRIX1' AND spillType = 'EOS' "
                "GROUP BY spillID HAVING COUNT(1) != 1) AS t1 "
                "ON t1.spillID = tempSpillList.spillID;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "DELETE FROM tempSpillList USING tempSpillList JOIN "
                "(SELECT spillID FROM %s.Scaler WHERE scalerName = 'TSgo' AND spillType = 'EOS' "
                "GROUP BY spillID HAVING COUNT(1) != 1) AS t1 "
                "ON t1.spillID = tempSpillList.spillID;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "SELECT COUNT(1) FROM tempSpillList;", inputFile);
    mysql_query(con, stmt);
      if (MysqlErrorCheck() == 1)
        return 1;
    TimeStamp(time_start);
    res = mysql_store_result(con);
    row = mysql_fetch_row(res);
    cout << row[0] << " spills after Scaler cuts" <<endl;

  sprintf(stmt, "CREATE INDEX spillID ON tempSpillList(spillID);");
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "OPTIMIZE TABLE tempSpillList;");
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  mysql_store_result(con);
  sprintf(stmt, "INSERT INTO Run SELECT Run.* FROM %s.Run JOIN tempRunList "
                "ON Run.runID = tempRunList.runID;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "INSERT INTO Spill SELECT t1.* FROM %s.Spill AS t1 JOIN tempSpillList AS t2 "
                "ON t1.spillID = t2.spillID;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "INSERT INTO BeamDAQ SELECT BeamDAQ.* FROM %s.BeamDAQ JOIN "
                "tempSpillList ON tempSpillList.spillID = BeamDAQ.spillID;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "INSERT INTO Beam SELECT Beam.* FROM %s.Beam JOIN "
                "tempSpillList ON tempSpillList.spillID = Beam.spillID;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "INSERT INTO Event SELECT Event.* FROM %s.Event JOIN "
                "tempSpillList ON tempSpillList.spillID = Event.spillID;",
                inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "INSERT INTO Target SELECT Target.* FROM %s.Target JOIN "
                "tempSpillList ON tempSpillList.spillID = Target.spillID;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  sprintf(stmt, "INSERT INTO Scaler SELECT Scaler.* FROM %s.Scaler JOIN "
                "tempSpillList ON tempSpillList.spillID = Scaler.spillID;", inputFile);
  mysql_query(con, stmt);
  if (MysqlErrorCheck() == 1)
    return 1;
    TimeStamp(time_start);

  if (kDim)
  {
    sprintf(stmt, "INSERT INTO kDimuon SELECT kDimuon.* FROM %s.kDimuon "
                  "JOIN tempSpillList ON kDimuon.spillID = tempSpillList.spillID "
                  "JOIN Event ON Event.spillID = kDimuon.spillID AND Event.eventID = kDimuon.eventID "
                  "WHERE MATRIX1 > 0;", inputFile);
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "INSERT INTO kTrack SELECT kTrack.* FROM %s.kTrack "
                  "JOIN tempSpillList ON kTrack.spillID = tempSpillList.spillID "
                  "JOIN Event ON Event.spillID = kTrack.spillID AND Event.eventID = kTrack.eventID "
                  "WHERE MATRIX1 > 0;", inputFile);
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);
  }

  if (jDim)
  {
    sprintf(stmt, "INSERT INTO jDimuon SELECT jDimuon.* FROM %s.jDimuon WHERE "
                  "spillID IN (SELECT spillID FROM tempSpillList);",
                  inputFile);
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "INSERT INTO jTrack SELECT jTrack.* FROM %s.jTrack WHERE "
                  "spillID IN (SELECT spillID FROM tempSpillList);",
                  inputFile);
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);
  }

/*
  sprintf(stmt, "DROP TABLE IF EXISTS tempSpillList, tempRunList;");
  mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
*/

  sprintf(stmt, "OPTIMIZE TABLES Spill, Event, Beam, BeamDAQ, Target, Scaler;");
  mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

  mysql_store_result(con);  // This should be done after optimize statements or the next query won't work

  if (kDim)
  {
    sprintf(stmt, "SELECT COUNT(1) FROM kDimuon;");
    mysql_query(con, stmt);
      if (MysqlErrorCheck() == 1)
        return 1;
    TimeStamp(time_start);
    res = mysql_store_result(con);
    row = mysql_fetch_row(res);
    cout << row[0] << " dimuons after spill cuts" <<endl;

    sprintf(stmt, "DELETE FROM kTrack WHERE "
                  "numHits < 15 OR "
                  "z0 < -400 OR "
                  "z0 > 200 OR "
                  "RoadID = 0 OR "
                  "(numHits != 18 AND pz1 < 18) OR "
                  "chisq/(numHits - 5) > 5;");
    mysql_query(con, stmt);
      if (MysqlErrorCheck() == 1)
        return 1;
    TimeStamp(time_start);

    sprintf(stmt, "DELETE FROM kDimuon USING kDimuon "
                  "LEFT JOIN kTrack AS t1 ON t1.spillID = kDimuon.spillID AND t1.trackID = kDimuon.posTrackID "
                  "LEFT JOIN kTrack AS t2 ON t2.spillID = kDimuon.spillID AND t2.trackID = kDimuon.negTrackID "
                  "WHERE (t1.roadID * t2.roadID > 0) OR "
                  //"WHERE t1.trackID IS NULL OR "
                  "t1.trackID IS NULL OR "
                  "t2.trackID IS NULL;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "CREATE TABLE dimuonsAfterTrackCuts "
                  "SELECT kDimuon.spillID, kDimuon.dimuonID, kDimuon.posTrackID, kDimuon.negTrackID "
                  "FROM kDimuon;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "SELECT COUNT(1) FROM kDimuon;");
    mysql_query(con, stmt);
      if (MysqlErrorCheck() == 1)
        return 1;
    TimeStamp(time_start);
    res = mysql_store_result(con);
    row = mysql_fetch_row(res);
    cout << row[0] << " dimuons after kTrack cuts" <<endl;

    sprintf(stmt, "ALTER TABLE kTrack ADD COLUMN target BOOL, ADD COLUMN dump BOOL;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "ALTER TABLE kDimuon ADD COlUMN target BOOL, ADD COLUMN dump BOOL;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "UPDATE kDimuon SET target = 0, dump = 0;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "UPDATE kTrack SET target = 0, dump = 0;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "OPTIMIZE TABLE kTrack, kDimuon;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);
    res = mysql_store_result(con);

    sprintf(stmt, "UPDATE kTrack SET target = 1 "
                  "WHERE chisq_dump - chisq_target > 10;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "UPDATE kTrack SET dump = 1 "
                  "WHERE chisq_target - chisq_dump > 10;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "UPDATE kDimuon, kTrack AS t1, kTrack AS t2 SET kDimuon.target = 1 "
                  "WHERE kDimuon.dz > -300 AND kDimuon.dz < -60 "
                  "AND kDimuon.spillID = t1.spillID AND kDimuon.posTrackID = t1.trackID "
                  "AND kDimuon.spillID = t2.spillID AND kDimuon.negTrackID = t2.trackID "
                  "AND t1.target = 1 AND t2.target = 1;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "UPDATE kDimuon, kTrack AS t1, kTrack AS t2 SET kDimuon.dump = 1 "
                  "WHERE kDimuon.dz > 0 AND kDimuon.dz < 150 "
                  "AND kDimuon.spillID = t1.spillID AND kDimuon.posTrackID = t1.trackID "
                  "AND kDimuon.spillID = t2.spillID AND kDimuon.negTrackID = t2.trackID "
                  "AND t1.dump = 1 AND t2.dump = 1;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "DELETE FROM kDimuon WHERE "
                  "(ABS(dx) > 2 AND ABS(dy) > 2) OR "
                  "dz NOT BETWEEN -300 AND 200 OR "
                  "(ABS(dpx) > 3 AND ABS(dpy) > 3) OR "
                  "dpz NOT BETWEEN 30 AND 120 OR "
                  "xF NOT BETWEEN -1 AND 1 OR "
                  "xB NOT BETWEEN 0 AND 1 OR "
                  "xT NOT BETWEEN 0 AND 1 OR "
                  "chisq_dimuon > 15 OR "
                  "mass NOT BETWEEN 0 AND 10 OR "
                  "(px1 < 0 AND px2 > 0) OR "
                  //"(px1 > 0 AND px2 < 0) OR "
                  "ABS(trackSeparation) > 250;");
    mysql_query(con, stmt);
      if (MysqlErrorCheck() == 1)
        return 1;
    TimeStamp(time_start);

    sprintf(stmt, "SELECT COUNT(1) FROM kDimuon;");
    mysql_query(con, stmt);
      if (MysqlErrorCheck() == 1)
        return 1;
    TimeStamp(time_start);
    res = mysql_store_result(con);
    row = mysql_fetch_row(res);
    cout << row[0] << " dimuons after kDimuon cuts" <<endl;

    //need to add kTrack momentums here
    sprintf(stmt, "ALTER TABLE kDimuon ADD COLUMN posx1 DOUBLE, "
            "ADD COLUMN posy1 DOUBLE, "
            "ADD COLUMN posx3 DOUBLE, "
            "ADD COLUMN posy3 DOUBLE, "
            "ADD COLUMN negx1 DOUBLE, "
            "ADD COLUMN negy1 DOUBLE, "
            "ADD COLUMN negx3 DOUBLE, "
            "ADD COLUMN negy3 DOUBLE, "
            "ADD COLUMN negHits INT, "
            "ADD COLUMN posHits INT");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

    sprintf(stmt, "UPDATE kDimuon SET posx1 = 0, posy1 = 0, posx3 = 0, posy3 = 0, negx1 = 0, negy1 = 0, negx3 = 0, negy3 = 0, negHits = 0, posHits = 0;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

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

    sprintf(stmt, "UPDATE kDimuon, kTrack SET kDimuon.negHits=kTrack.numHits "
            "WHERE kDimuon.spillID = kTrack.spillID AND kDimuon.negTrackID = kTrack.TrackID");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;

    sprintf(stmt, "UPDATE kDimuon, kTrack SET kDimuon.posHits=kTrack.numHits "
            "WHERE kDimuon.spillID = kTrack.spillID AND kDimuon.posTrackID = kTrack.TrackID");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;

    sprintf(stmt, "ALTER TABLE kDimuon ADD COLUMN m3hm DOUBLE, "
            "ADD COLUMN m3hs DOUBLE, "
            "ADD COLUMN m3vm DOUBLE, "
            "ADD COLUMN m3vs DOUBLE");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);

        sprintf(stmt, "UPDATE kDimuon SET m3hm = 0, m3hs = 0, m3vm = 0, m3vs = 0");
    //sprintf(stmt, "UPDATE kDimuon SET m3hm = 0");
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

    sprintf(stmt, "ALTER TABLE Spill ADD COLUMN liveProton2 BIGINT;");
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

    sprintf(stmt, "OPTIMIZE TABLES kDimuon, kTrack;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);
    mysql_store_result(con);

    //ok, cleanup
    TimeStamp(time_start);
    sprintf(stmt, "DROP TABLE IF EXISTS Event, Target, Scaler;");
    mysql_query(con, stmt);
    if (MysqlErrorCheck() == 1)
      return 1;
    TimeStamp(time_start);


  }


  if (jDim) // Should add cuts here at some point
  {
    sprintf(stmt, "OPTIMIZE TABLES jDimuon, jTrack;");
    mysql_query(con, stmt);
      if (MysqlErrorCheck() == 1)
        return 1;
    TimeStamp(time_start);
    mysql_store_result(con);
  }

  mysql_close(con);

  return 0;
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

void TimeStamp(time_t starttime)
{
  time (&time_end);

  double currenttime = difftime(time_end, starttime);
  //cout << "\t The start time was " << starttime <<  " seconds." << endl;
  cout << "\t Current Running time is " << currenttime <<  " seconds." << endl;
  //cout << "\t Current Running time is " << time_delta <<  " seconds." << endl;
  
}
