using System;
using System.Collections.Generic;
using System.Text;
using MySql.Data.MySqlClient;

namespace MySqlTest
{
    public class esme_environment
    {
        private MySqlConnection sqlConnection;

        public esme_environment(string server, string username, string password)
        {
            sqlConnection = ConnectToDatabase(server, username, password);
        }
        
        private MySqlConnection ConnectToDatabase(string Server, string Database, string Username, string Password)
        {
            return ConnectToDatabase("Server=" + Server + ";Database=" + Database + ";User Id=" + Username + ";Password=" + Password);
        }

        private MySqlConnection ConnectToDatabase(string Server, string Database, string Username, string Password, int Port)
        {
            return ConnectToDatabase("Server=" + Server + ";Database=" + Database + ";Port=" + Port + ";User Id=" + Username + ";Password=" + Password);
        }

        private MySqlConnection ConnectToDatabase(string ConnectString)
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
    }

    public class esme_environment_data_type
    {
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
