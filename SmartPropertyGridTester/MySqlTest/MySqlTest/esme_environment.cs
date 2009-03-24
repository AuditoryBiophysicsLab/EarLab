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
        private List<DataType> mDataTypes = new List<DataType>();

        public Database(string server, string username, string password)
        {
            sqlConnection = Connect(server, "esme_environment", username, password);
        }

        public Database(string server, string username, string password, int port)
        {
            sqlConnection = Connect(server, "esme_environment", username, password, port);
        }

        #region Database connection routines
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

        public void GetDatatypes()
        {
            MySqlCommand command;
            if (sqlConnection.State != ConnectionState.Open)
                sqlConnection.Open();
            mDataTypes.Clear();
            command = new MySqlCommand("SELECT * from datatype;", sqlConnection);
            using (MySqlDataReader data = command.ExecuteReader())
            {
                if (data.HasRows)
                {
                    while (data.Read())
                        mDataTypes.Add(new DataType(data.GetInt32("idDataType"), data.GetString("Name")));
                }
            }
            sqlConnection.Close();
        }

    }

    public class DataTypeTable
    {
        private List<DataType> mDataTypes = new List<DataType>();
        MySqlConnection sqlConnection;

        public DataTypeTable(MySqlConnection Connection)
        {
            MySqlCommand command;

            sqlConnection = Connection;
            if (sqlConnection.State != ConnectionState.Open)
                sqlConnection.Open();
            mDataTypes.Clear();
            command = new MySqlCommand("SELECT * from datatype;", sqlConnection);
            using (MySqlDataReader data = command.ExecuteReader())
            {
                if (data.HasRows)
                {
                    while (data.Read())
                        mDataTypes.Add(new DataType(data.GetInt32("idDataType"), data.GetString("Name")));
                }
            }
            sqlConnection.Close();
        }

        public Nullable<int> this[int id]
        {

        }
    }

    public class DataType
    {
        private int mDatatypeID;
        private string mDatatypeName;

        public DataType(int DatatypeID, string DatatypeName)
        {
            mDatatypeID = DatatypeID;
            mDatatypeName = DatatypeName;
        }

        public int idDataType { get { return mDatatypeID; } }
        public string Name { get { return mDatatypeName; } }
    }

    public class esme_environment_data_set
    {
    }

    public class esme_environment_data_subset
    {
    }

    public class esme_environment_data_point
    {
    }

    public class esme_environment_datum
    {
    }
}
