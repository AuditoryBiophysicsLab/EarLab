using System;
using System.Collections.Generic;
using System.Diagnostics; 
using System.IO;

/// <summary>
/// This is a generic class that handles writing errors
/// I've made some additions specific to RunfileEditor
/// So as to make it a more specific diagnostic tool
/// 
/// Three types of errors:
/// 1.) Syntax errors -- 
/// 2.) Semantic 
/// 3.) logic
/// 
/// Worried about faulty input into the program --
/// 1.) SPG has validation code
/// 2.) There are some custom written Tryparse codes as well for array values
/// 3.) However, on XML we can't be sure ... each Tryparse could be an error from the raw xml
/// 
/// if (flag == flase)
/// {
/// MessageBox.Show("Expected digit characters only. Re-enter.", "Input Error");
/// txtQuantity.Focus();
/// return;
/// }
/// 
/// Types of Data Validation
/// 1. Type Checking        -- tryparse, existing paradigm 
/// 2. Consistency check    -- comparese several answers against each other.
/// 3. Range Checking       -- SPG does not handle
/// 4. Length Checking      -- SPG does not handle
/// 
/// 
/// When there are issues make smaller code statements.
/// 
/// So, any error that gets thrown...
/// we show the user the error
/// and
/// log the error.
/// 
/// We will have to write code for custom exceptions later.
/// 
/// 
/// Have an option to call up the person's e-mail client and send them to us.
/// 
/// p. 286 
/// 
/// catch (DivideByZeroException)
/// {
/// 
///     MessageBox.Show("Expression in XYZ is zero. Please Reenter", "Exception Thrown");
///     txtExpression1.Focus();
///     return;
/// }
/// catch (Exception ex)
/// { 
/// 
///     MessageBox.Show("Error: " + ex.Messge, "Exception Thrown");
/// 
/// }
/// 
/// every option should be logged
/// 
/// if & try-catch on all potentially error filled sections, streams, etc.
/// 
///     1.) Log error
///     2.) Pop error msg.
///     3.) At any time give option to send errors.
/// 
/// 
/// Maybe Introduce an XML format?
/// 
/// </summary>
/// 
namespace RunfileEditor
{
    class errorLog
    {
        //=== Private data members ============================================///
        private string fileName;
        private string pathName;
        private string errorMessage;
        private int errorFlag;

        StreamWriter sw = null;
        StreamReader sr = null;

        //=== Constructor =====================================================///
        public errorLog(string msg)
        {
            errorMessage = msg;
            errorFlag = 0;
            fileName = "ErrorLog.txt";
        }


        //=== Property Methods ================================================///
        public string FileName
        {
            get
            {
                return fileName;
            }
            set
            {
                if (value.Length > 0)
                    fileName = value;
            }
        }
        public string Message
        {
            get
            {
                return errorMessage;
            }
            set
            {
                if (value.Length > 0)
                    errorMessage = value;
            }
        }
        public string PathName
        {
            get 
            { 
                return pathName; 
            }
            set
            {
                if (value.Length > 0)
                    pathName = value;
            }
        }


        //=== General Methods ===============================================///

        /// <summary>
        ///This reads the error log file.
        ///
        ///Parameter list:  
        ///Return value
        ///string          the contents of the error log message file
        ///
        ///</summary>

        public string ReadErrorLog()
        {
            //An internal buffer .. limit characters??? 
            string buff;

            try
            {

                //     A string containing the combined paths. If one of the specified paths is
                //     a zero-length string, this method returns the other path. If path2 contains
                //     an absolute path, this method returns path2.
                string pfn = Path.Combine(pathName, fileName);

                if (File.Exists(pfn) == true)
                {
                    //get ready to read some data
                    sr = new StreamReader(pfn);

                    //read to buffer
                    buff = sr.ReadToEnd();

                    //close stream
                    sr.Close();

                    //return it.
                    return buff;
                }
            }
            catch
            {
                return "";
            }
            return "";
        }




        /// 
        ///<summary>
        ///*****
        ///* Purpose: 
        ///*          This writes an error log message to the error log file. The
        ///*          message has the date and time, the type of error, and the
        ///*          stack trace for the error.
        ///* 
        ///* Parameter list:
        ///*          n/a
        ///*  
        ///* Return value
        ///*          int          0 = no errors, 1 otherwise
        ///*****/
        /// </summary>
        public int WriteErrorLog()
        {

            //set error flag as 0
            errorFlag = 0;

            // Save the date and time
            DateTime currentDT = DateTime.Now;

            try
            {
                // Do we have all the stings need?
                // if there are an error msg, path and file then try to write the item
                if (errorMessage.Length != 0 && pathName.Length != 0 && fileName.Length != 0)
                {

                    //create a new stream writer 
                    sw = new StreamWriter(Path.Combine(pathName, fileName), false);

                    //write the line | Date | Time | error message
                    sw.WriteLine(currentDT.ToShortDateString() + ", " +
                                 currentDT.ToShortTimeString() + ": " +
                                 errorMessage);
                    sw.WriteLine("----------------");
                    sw.Close();
                }
                //we had some bad problem.
                else
                {
                    errorFlag = 1;	// Something bad happened
                }
            }
            catch (Exception ex)
            {
                errorMessage = ex.Message;
                errorFlag = 1;		// Something bad happened
            }
            return errorFlag;
        }

        ///<summary>
        ///*****
        ///* Purpose: 
        ///             This writes an error log message to the error log file.
        ///* 
        ///* Parameter list:
        ///*            string msg       the error message to write
        ///*  
        ///* Return value
        ///*            int              0 = no errors, 1 otherwise
        ///*****/
        ///</summary>
        public int WriteErrorLog(string msg)
        {
            // Copy the message
            errorMessage = msg;

            // Now call original one     
            errorFlag = WriteErrorLog();

            // Return the error flag
            return errorFlag;
        }
    }

}

