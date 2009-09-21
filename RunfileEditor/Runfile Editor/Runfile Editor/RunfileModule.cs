using System;
using System.Collections.Generic;
using System.Text;

namespace RunfileEditor
{
    public class RunfileModule
    {
        #region Data Members
        public XmlNode theRunfileModuleNode;

        public RunfileModuleInformation ModuleInformation;

        public List<RunfileInput> theRunfileInputs = new List<RunfileInput>();

        public List<RunfileOutput> theRunfileOutputs = new List<RunfileOutput>();

        public List<RunfileParameter> RunfileParameters = new List<RunfileParameter>();
        #endregion

        #region Constructors

        public RunfileModule()
        {
        }

        public RunfileModule(XmlNode ModuleNode)
        {
            theRunfileModuleNode = ModuleNode;

            /*
            1 get rf module info -- 
             * use constructor (grab proper node) and store in in the public data member
             * 
             * 2 get I-O-Ps these are just foreach loops.
             * 
             * e.g. get P --- send parameters node.
                    add at end of each loop
             * 

            */
            //1.)constuctor to R F M I
            //theRunfileModuleInformation = new RunfileModuleInformation(ModuleNode["ModuleInformation"]);

            //Xpath info ...
            //(xml statement) 

            ModuleInformation = new RunfileModuleInformation(ModuleNode["ModuleInformation"]);


            //2.) I - O - P
            //Need to add error protection in front of each of these
            //(?)Possibly rewrite as a generic class? Is it worth it?

            //Inputs
            //(xml statement) 
            //must be fixed
            foreach (XmlNode eInput in ModuleNode["Inputs"].ChildNodes)
            {
                theRunfileInputs.Add(new RunfileInput(eInput));
            }

            //Outputs
            //(xml statement) 
            //must be fixed
            foreach (XmlNode eOutput in ModuleNode["Outputs"].ChildNodes)
            {
                theRunfileOutputs.Add(new RunfileOutput(eOutput));
            }

            // DOM to grab the parameter in the parameters node
            //(xml statement) 

            //
            //XmlNodeList ePList = ModuleNode["Parameters"].ChildNodes;
            foreach (XmlNode eParameter in ModuleNode["Parameters"].ChildNodes)
            {
                RunfileParameters.Add(new RunfileParameter(eParameter));
            }
        }

        #endregion
    }
}
