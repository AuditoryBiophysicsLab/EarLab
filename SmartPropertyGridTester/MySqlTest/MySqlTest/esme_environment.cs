using System;
using System.Collections.Generic;
using System.Text;
using MySql.Data.MySqlClient;
using System.Data;

namespace ESME.Environment
{
    public class Database
    {
        private MySqlConnection sqlConnection;

        #region Constructors
        public Database(string server, string database, string username, string password)
        {
            sqlConnection = Connect(server, database, username, password);
        }

        public Database(string server, string database, string username, string password, int port)
        {
            sqlConnection = Connect(server, database, username, password, port);
        }
        
        #endregion

        #region Database Connection Routines
        private MySqlConnection Connect(string Server, string Database, string Username, string Password)
        {
            return Connect("Server=" + Server + ";Database=" + Database + ";User Id=" + Username + ";Password=" + Password);
        }

        private MySqlConnection Connect(string Server, string Database, string Username, string Password, int Port)
        {
            return Connect("Server=" + Server + ";Database=" + Database + ";Port=" + Port + ";User Id=" + Username + ";Password=" + Password);
        }

        private MySqlConnection Connect(string ConnectString)
        {
            MySqlConnection connection;
            connection = new MySqlConnection(ConnectString);
            connection.Open();
            if (connection.State != ConnectionState.Open)
            {
                if (connection.State != ConnectionState.Open)
                    throw new Exception("Cannot connect to requested database with supplied credentials");
            }
            connection.Close();
            return connection;
        }
        #endregion

        #region Database and table creation

        public void Create(string DatabaseName)
        {
            if (sqlConnection.State != ConnectionState.Open)
                sqlConnection.Open();

            // (Re)create the database if it does not exist
            ExecuteSQL("DROP DATABASE IF EXISTS " + DatabaseName + ";");
            ExecuteSQL("CREATE DATABASE IF NOT EXISTS " + DatabaseName + " CHARACTER SET latin1 COLLATE latin1_bin;");
            ExecuteSQL("USE " + DatabaseName + ";");

            ExecuteSQL("CREATE TABLE IF NOT EXISTS DataType " +
                       //"COMMENT 'ESME Environmental data types are stored here.  Only modify this table if you are SURE you know what you are doing!' " +
                       "(" +
                       "    idDataType INT NOT NULL AUTO_INCREMENT PRIMARY KEY, " +
                       "    Name VARCHAR(45) NOT NULL UNIQUE KEY" +
                       ")" +
                       ";");

            ExecuteSQL("CREATE TABLE IF NOT EXISTS DataSet " +
                       //"COMMENT 'Environmental data sets are stored here.  If you have a new data set for any of the predefined types in the DataType table, feel free to add it here' " +
                       "(" +
                       "    idDataSet INT NOT NULL AUTO_INCREMENT PRIMARY KEY, " +
                       "    idDataType INT NOT NULL, " +
                       "    Name VARCHAR(45) NOT NULL, " +
                       "    FOREIGN KEY (idDataType) REFERENCES DataType(idDataType) ON DELETE CASCADE " +
                       ")" +
                       ";");

            ExecuteSQL("CREATE TABLE IF NOT EXISTS DataSubSet " +
                       //"COMMENT 'Environmental data subsets are stored here.  If you have a new subset for a previously defined data set, feel free to add it here' " +
                       "(" +
                       "    idDataSubSet INT NOT NULL AUTO_INCREMENT PRIMARY KEY, " +
                       "    idDataSet INT NOT NULL, " +
                       "    Name VARCHAR(45) NOT NULL, " +
                       "    FOREIGN KEY (idDataSet) REFERENCES DataSet(idDataSet) ON DELETE CASCADE " +
                       ")" +
                       ";");

            ExecuteSQL("CREATE TABLE IF NOT EXISTS DataPoint " +
                       //"COMMENT 'Data points for environmental data are stored here.  If you have a new data point for a previously defined data subset, feel free to add it here' " +
                       "(" +
                       "    idDataPoint INT NOT NULL AUTO_INCREMENT PRIMARY KEY, " +
                       "    idDataSubSet INT NOT NULL, " +
                       "    Latitude FLOAT NOT NULL, " +
                       "    Longitude FLOAT NOT NULL, " +
                       "    FOREIGN KEY (idDataSubSet) REFERENCES DataSubSet(idDataSubSet) ON DELETE CASCADE " +
                       ")" +
                       ";");

            ExecuteSQL("CREATE TABLE IF NOT EXISTS Datum " +
                       //"COMMENT 'Environmental data are stored here.  If you have data for a previously defined data point, feel free to add it here.  Depths (below sea level) are POSITIVE, elevations (above sea level, i.e. land) are NEGATIVE' " +
                       "(" +
                       "    idDatum INT NOT NULL AUTO_INCREMENT PRIMARY KEY, " +
                       "    idDataPoint INT NOT NULL, " +
                       "    Depth_Meters FLOAT NOT NULL, " +
                       "    Datum FLOAT NOT NULL, " +
                       "    FOREIGN KEY (idDataPoint) REFERENCES DataPoint(idDataPoint) ON DELETE CASCADE " +
                       ")" +
                       ";");

            string[] types = {"Bathymetry", "Salinity",     "Temperature",  "Sound Speed",              "Salinity Standard Deviation",  "Temperature Standard Deviation"};
            string[] sets =  {"DBDB-V",     "GDEM-V 3.0",   "GDEM-V 3.0",   "Computed from GDEM-V 3.0", "GDEM-V 3.0",                   "GDEM-V 3.0"};
            string[] months = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
            for (int i = 0; i < types.Length; i++)
            {
                AddDataType(types[i]);
                AddDataSet(types[i], sets[i]);
                if (types[i] != "Bathymetry")
                {
                    foreach (string month in months)
                        AddDataSubset(types[i], sets[i], month);
                }
            }
#if false
            ExecuteSQL("DROP TRIGGER IF EXISTS DataPoint_deletion");
            ExecuteSQL("CREATE TRIGGER DataPoint_deletion BEFORE DELETE " +
                       "ON DataPoint FOR EACH ROW " +
                       "BEGIN " +
                       "    DELETE FROM Datum WHERE idDataPoint=OLD.idDataPoint; " +
                       "END");
            
#endif
        }
        private void ExecuteSQL(string SQL)
        {
            MySqlCommand command = new MySqlCommand(SQL, sqlConnection);
            command.ExecuteNonQuery();
        }
        #endregion

        #region List getter functions
        public string[] DataTypeList()
        {
            return GetNameList("Name", "DataType");
        }

        public string[] DataSetList(int DataTypeID)
        {
            return GetNameList("Name", "DataSet", "idDataType", DataTypeID);
        }

        public string[] DataSubsetList(int DataSubsetID)
        {
            return GetNameList("Name", "DataSubset", "idDataSet", DataSubsetID);
        }

        private string[] GetNameList(string FieldName, string TableName)
        {
            return GetNameList(FieldName, TableName, null, 0);
        }

        private string[] GetNameList(string FieldName, string TableName, string ForeignKeyName, int ForeignKeyValue)
        {
            List<string> theResults = new List<string>();
            string Query;

            if (sqlConnection.State != ConnectionState.Open)
                sqlConnection.Open();

            if (ForeignKeyName != null)
                Query = "SELECT " + FieldName + " FROM " + TableName + " WHERE `" + ForeignKeyName + "`=" + ForeignKeyValue + ";";
            else
                Query = "SELECT " + FieldName + " FROM " + TableName + ";";
            using (MySqlDataReader data = DoSelect(Query))
            {
                if (data.HasRows)
                {
                    while (data.Read())
                        theResults.Add(data.GetString(FieldName));
                }
            }
            sqlConnection.Close();
            if (theResults.Count == 0)
                return null;
            return theResults.ToArray();
        }
        #endregion

        #region ID Getter Functions
        public int GetDataTypeID(string DataType)
        {
            return GetIDField("idDataType", "DataType", "Name", DataType);
        }

        public int GetDataSetID(string DataType, string DataSet)
        {
            int TypeID = GetDataTypeID(DataType);
            return GetDataSetID(DataSet, TypeID);
        }

        public int GetDataSetID(string DataSet, int DataTypeID)
        {
            return GetIDField("idDataSet", "DataSet", "Name", DataSet, "idDataType", DataTypeID);
        }

        public int GetDataSubsetID(string DataType, string DataSet, string DataSubset)
        {
            int TypeID = GetDataTypeID(DataType);
            int SetID = GetDataSetID(DataSet, TypeID);
            return GetDataSubsetID(DataSubset, SetID);
        }

        public int GetDataSubsetID(string DataSubset, int DataSetID)
        {
            return GetIDField("idDataSubset", "DataSubset", "Name", DataSubset, "idDataSet", DataSetID);
        }

        private int GetIDField(string IDField, string TableName, string NameField, string Name)
        {
            return GetIDField(IDField, TableName, NameField, Name, null, 0);
        }

        private int GetIDField(string IDField, string TableName, string NameField, string Name, string ForeignKeyName, int ForeignKeyValue)
        {
            int result = 0;
            bool foundit = false;
            string Query;

            if (sqlConnection.State != ConnectionState.Open)
                sqlConnection.Open();

            if (ForeignKeyName != null)
                Query = "SELECT " + IDField + " FROM " + TableName + " WHERE " + NameField + "='" + Name + "' AND `" + ForeignKeyName + "`=" + ForeignKeyValue + ";";
            else
                Query = "SELECT " + IDField + " FROM " + TableName + " WHERE " + NameField + "='" + Name + "';";
            using (MySqlDataReader data = DoSelect(Query))
            {
                if (data.HasRows)
                {
                    foundit = true;
                    data.Read();
                    result = data.GetInt32(IDField);
                }
            }
            sqlConnection.Close();
            if (foundit)
                return result;
            throw new ArgumentException("Requested " + TableName + " \"" + Name + "\" not found in database");
        }
        #endregion

        #region Row adders
        public int AddDataType(string NewDataType)
        {
            AddItem("DataType", "Name", NewDataType, null, 0);
            return GetDataTypeID(NewDataType);
        }

        public int AddDataSet(string DataType, string NewDataSet)
        {
            AddItem("DataSet", "Name", NewDataSet, "idDataType", GetDataTypeID(DataType));
            return GetDataSetID(DataType, NewDataSet);
        }

        public int AddDataSet(int DataTypeID, string NewDataSet)
        {
            AddItem("DataSet", "Name", NewDataSet, "idDataType", DataTypeID);
            return GetDataSetID(NewDataSet, DataTypeID);
        }

        public int AddDataSubset(string DataType, string DataSet, string NewDataSubset)
        {
            AddItem("DataSubset", "Name", NewDataSubset, "idDataSet", GetDataSetID(DataType, DataSet));
            return GetDataSubsetID(DataType, DataSet, NewDataSubset);
        }

        public int AddDataSubset(int DataSetID, string NewDataSubset)
        {
            AddItem("DataSubset", "Name", NewDataSubset, "idDataSet", DataSetID);
            return GetDataSubsetID(NewDataSubset, DataSetID);
        }

        public int AddDataPoint(int DataSubsetID, float Latitude, float Longitude)
        {
            string Insert, Query;
            bool foundit = false;
            int result = 0;

            if (sqlConnection.State != ConnectionState.Open)
                sqlConnection.Open();

            Insert = "INSERT INTO DataPoint (Latitude, Longitude, idDataSubset) VALUES (" + Latitude + ", " + Longitude + ", " + DataSubsetID + ");";
            MySqlCommand command = new MySqlCommand(Insert, sqlConnection);
            command.ExecuteNonQuery();
            Query = "SELECT LAST_INSERT_ID();";
            using (MySqlDataReader data = DoSelect(Query))
            {
                if (data.HasRows)
                {
                    foundit = true;
                    data.Read();
                    result = data.GetInt32(0);
                }
            }
            sqlConnection.Close();
            if (foundit)
                return result;
            throw new ArgumentException("Requested DataPoint not found in database");
        }

        public void AddData(int DataPointID, float[] Depths, float[] Data)
        {
            if (Depths.Length != Data.Length)
                throw new ArgumentException("AddData: Depth and Data arrays MUST be the same size");
            for (int i = 0; i < Depths.Length; i++)
                AddDatum(DataPointID, Depths[i], Data[i]);
        }

        public void AddDatum(int DataPointID, float Depth, float Datum)
        {
            string Insert;

            if (sqlConnection.State != ConnectionState.Open)
                sqlConnection.Open();

            Insert = "INSERT INTO Datum (idDataPoint, Depth_Meters, Datum) VALUES (" + DataPointID + ", " + Depth + ", " + Datum + ");";
            MySqlCommand command = new MySqlCommand(Insert, sqlConnection);
            command.ExecuteNonQuery();
            sqlConnection.Close();
        }

        private void AddItem(string TableName, string FieldName, string NewValue, string ForeignKeyName, int ForeignKeyValue)
        {
            string Insert;

            if (sqlConnection.State != ConnectionState.Open)
                sqlConnection.Open();

            if (ForeignKeyName != null)
                Insert = "INSERT INTO " + TableName + " (" + FieldName + ", " + ForeignKeyName + ") VALUES ('" + NewValue + "', " + ForeignKeyValue + ");";
            else
                Insert = "INSERT INTO " + TableName + " (" + FieldName + ") VALUES ('" + NewValue + "');";
            MySqlCommand command = new MySqlCommand(Insert, sqlConnection);
            command.ExecuteNonQuery();
            sqlConnection.Close();
        }
        #endregion

        #region Deleters
        public void ClearSubset(int DataSubsetID)
        {
            string Query;
            MySqlCommand deleteCommand;
            List<int> PointList = new List<int>();

            if (sqlConnection.State != ConnectionState.Open)
                sqlConnection.Open();

            // Get all data point IDs corresponding to this subset
            Query = "SELECT idDataPoint FROM DataPoint WHERE idDataSubset=" + DataSubsetID + ";";
            using (MySqlDataReader data = DoSelect(Query))
            {
                if (data.HasRows)
                    while (data.Read())
                        PointList.Add(data.GetInt32("idDataPoint"));
            }
            deleteCommand = new MySqlCommand("DELETE FROM Datum WHERE idDataPoint=@pointID;", sqlConnection);
            deleteCommand.Prepare();
            deleteCommand.Parameters.Add("@pointID", 0);
            foreach (int curPointID in PointList)
            {
                // Delete all Datum rows corresponding to each data point ID
                deleteCommand.Parameters[0].Value = curPointID;
                deleteCommand.ExecuteNonQuery();
            }
            // Delete all data point IDs corresponding to the specified subset
            deleteCommand = new MySqlCommand("DELETE FROM DataPoint WHERE idDataSubset=@subsetID;", sqlConnection);
            deleteCommand.Parameters.Add("@subsetID", 0);
            deleteCommand.Parameters[0].Value = DataSubsetID;
            deleteCommand.ExecuteNonQuery();
            sqlConnection.Close();
        }
        #endregion

        #region Utility functions
        private MySqlDataReader DoSelect(string SelectStatement)
        {
            MySqlCommand command = new MySqlCommand(SelectStatement, sqlConnection);
            return command.ExecuteReader();
        }
        #endregion
    }
}
