using System;
using System.Collections.Generic;
using System.Text;

namespace System.IO
{
    /// <summary>
    /// The Various levels of Status for the xDirectory 'StartCopy' method.
    /// </summary>
    public enum xDirectoryStatus
    {
        /// <summary>
        /// The xDirectory Copy Thread is Stopped.
        /// </summary>
        Stopped,

        /// <summary>
        /// The xDirectory Copy Thread is Starting.
        /// </summary>
        Started,

        /// <summary>
        /// The xDirectory Copy Thread is Indexing.
        /// </summary>
        Indexing,

        /// <summary>
        /// The xDirectory Copy Thread is Copying Folders.
        /// </summary>
        CopyingFolders,

        /// <summary>
        /// The xDirectory Copy Thread is Copying Files.
        /// </summary>
        CopyingFiles
    };
}
