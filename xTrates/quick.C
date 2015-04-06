void quick(const char *schema, const char *table)
{
   //connect to server
   //TSQLServer *con = TSQLServer::Connect("mysql://seaquel.physics.illinois.edu:3283", "seaguest", "qqbar2mu+mu-");
   TSQLServer *con = TSQLServer::Connect("mysql://localhost:3306", "root", "");

   TTreeSQL * treeql = new TTreeSQL(con, schema, table);

}
