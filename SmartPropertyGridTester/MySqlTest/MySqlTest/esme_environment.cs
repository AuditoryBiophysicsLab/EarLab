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
        public Database(string server, string username, string password)
        {
            sqlConnection = Connect(server, "esme_environment", username, password);
        }

        public Database(string server, string username, string password, int port)
        {
            sqlConnection = Connect(server, "esme_environment", username, password, port);
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
            Query = "SELECT idDataPoint FROM DataPoint WHERE idDataSubset=" + DataSubsetID + " ORDER BY idDataPoint DESC;";
            using (MySqlDataReader data = DoSelect(Query))
            {
                if (data.HasRows)
                {
                    foundit = true;
                    data.Read();
                    result = data.GetInt32("idDataPoint");
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
