using System;
using System.Text;
using VisualHint.SmartPropertyGrid;
using System.Windows.Forms;
using System.Globalization;
using System.ComponentModel;
using System.Drawing;
using System.Resources;
using System.Reflection;
using System.Collections.Generic;
using System.Xml;

namespace RunfileEditor
{

    /// <summary>
    /// Need to do a LOT!!! with the Module Grid class!!
    /// </summary>

    //New class that inherits from tabpage -- constructor take in earlab module
    //adds controls to itself - from spg and any buttons and gifs
public partial class frmContainer : Form
    {

        //private vars
        //Module title 
        // other crap

        //basically a test method to test stuff out.
        public void ModuleTab()
        {

            //2.) Add Module Grid to Tab
            TabPage tabPage = new TabPage("none");

            ModuleGrid myModuleGrid = new ModuleGrid();
            myModuleGrid.Initialize();
            tabPage.Controls.Add(myModuleGrid);

            //3.) Tab location and size settings
            tabPage.Location = new System.Drawing.Point(4, 22);
            tabPage.Padding = new System.Windows.Forms.Padding(3);
            tabPage.Size = new System.Drawing.Size(831, 392);
            tabPage.TabIndex = 1;
            tabPage.UseVisualStyleBackColor = true;

            //(?)
            //Need to add this to the form for this to work right
            //TabControl tabControl1 = new System.Windows.Forms.TabControl();
            tabControl1.TabPages.Add(tabPage);
        }

        public void ModuleTab(EarLabModule theEarLabModule)
        {
            //This class creates tab on the form

            //1.) Create the Module Grid

            //Something like 
            ModuleGrid myModuleGrid = new ModuleGrid();

            //takes the module and creates the SPG
            myModuleGrid.Initialize(theEarLabModule);


            //A.) Need to get a TITLE to push into it.
            string ModuleTitle = theEarLabModule.theEarLabModuleInformation.InstanceName;


            //2.) Add Module Grid to Tab
            TabPage tabPage = new TabPage(ModuleTitle);
            tabPage.Controls.Add(myModuleGrid);


            //3.) Tab location and size settings
            tabPage.Location = new System.Drawing.Point(4, 22);
            tabPage.Padding = new System.Windows.Forms.Padding(3);
            tabPage.Size = new System.Drawing.Size(831, 392);
            tabPage.TabIndex = 1;
            tabPage.UseVisualStyleBackColor = true;

            //(?)
            //Need to add this to the form for this to work right
            //TabControl tabControl1 = new System.Windows.Forms.TabControl();
            tabControl1.TabPages.Add(tabPage);

        }

        //Just a wrapper to use the tabControl
        public void ModuleClearTabs()
        {
            //There is a bit of a problem -- you can't use an index, b/c you
            //are opporating on the collection.
            //TabPage TabHolder = tabPage1;
            //The above is not necessary, FormDesigner has created the
            //tabPage1 with the controls, and never clears that var.

            //clear all tabs
            tabControl1.TabPages.Clear();

            //Add back tabpage1
            tabControl1.TabPages.Add(tabPage1);

            //remove selected tab
            //tabControl1.TabPages.Remove(tabControl1.SelectedTab);
            // Removes all the tabs:
            //tabControl1.TabPages.Clear();
        }




    }//  You'll need to take this and throw this into the form.

//ModuleGrid takes the EarLabModule -> to 

public class ModuleGrid : VisualHint.SmartPropertyGrid.PropertyGrid
    {
        public ModuleGrid()
        {
        }

        // should this be in the constructor?
        // (?) change the location of the shit around?


        //test which works...
        public void Initialize()
        {
            PropertyEnumerator catEnum;
                catEnum = AppendRootCategory(1, "Paramter");

                PropertyEnumerator propEnum;
                
                propEnum = AppendManagedProperty(catEnum, 3, "Double Value",
                typeof(double), 9.09, "This is an input double");

            standardSettings();
        }


    //initialize a module which works for
    //test runfile modules
    public void Initialize(EarLabModule theEarLabModule)
        {
            //Information directly from the Module
            //This just mirrors everything else.
            int propertyID = 0;
            //int parameterNumber = 1;

            PropertyEnumerator catEnum;

            //There is a problem with the "PropertyEnumerator for the propEnum!"


            //if module has params
            if (theEarLabModule.EarlabParameters.Count > 0)
            {
                //start param, this works!
                catEnum = AppendRootCategory(propertyID++, "Parameters");

                //A method to make the ALL the parameters.
                MakeParameters(theEarLabModule.EarlabParameters, catEnum, ref propertyID);

            }
            else
            {
                catEnum = AppendRootCategory(propertyID++, "No Parameters in this Module");
            }


            //if module has inputs
            // same code
            if (theEarLabModule.EarlabInputs.Count > 0)
            {
                //start param
                catEnum = AppendRootCategory(propertyID++, "Inputs");

                //A method of all the inputs
                //MakeInputs(theEarLabModule.EarLabInputs, catEnum, ref propertyID);

            }
            else
            {
                catEnum = AppendRootCategory(propertyID++, "No Inputs in this Module");
            }


            //if module has outputs
            //same code, you can't use "null" as a conditional b/c there is something in there.
            if (theEarLabModule.EarlabOutputs.Count > 0)
            {
                //start param
                catEnum = AppendRootCategory(propertyID++, "Outputs");

                //A method of all the outputs
                //MakeOutputs(theEarLabModule.EarLabOutputs, catEnum, ref propertyID);
            }
            else
            {
                catEnum = AppendRootCategory(propertyID++, "No Outputs in this Module");
            }

            //Get some update stuff in another method 


            //set standards for the SPG
            standardSettings();
        }


    public void standardSettings()
        {
            CommentsVisibility = true;
            CommentsHeight = 100;
            //ExpandProperty(propEnum, true);
            this.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.Font = new System.Drawing.Font("Tahoma", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.World);
            this.Location = new System.Drawing.Point(116, 14);
            this.Name = "propertyGrid1";
            //this.PropertyComparer = propertyComparerDefaultSort1;
            this.Size = new System.Drawing.Size(695, 364);
        }

    protected override void OnPropertyChanged(VisualHint.SmartPropertyGrid.PropertyChangedEventArgs e)
    {
        // on property change
        // check against max, min and default for each property in the Module.


        // if out of range, give a new message
        // if out of range, turn it red

        //test

        if (e.PropertyEnum.Property.Id == 1)
            Application.UseWaitCursor = UseWaitCursor;


        //All code goes above this SPG standard fragment.
        //Section 4.12 of the spg manual, p 37
        base.OnPropertyChanged(e);
    }

    //Makes the parameters all at once
    public void MakeParameters(List<EarLabParameter> Parameters, PropertyEnumerator catEnum, ref int propertyID)
    {

        PropertyEnumerator propEnum;

        foreach (EarLabParameter EarLabParam in Parameters)

        {
            //???
            string paramType = EarLabParam.PType.ToLower();
            paramType = paramType.Replace(" ", "");

            //*Split into the broad categories of display type.
            //*Single Value
            //----> bools, ints, floats, doubles, strings etc.

            //*Array Value
            //----> ints, floats, doubles, 

            switch (paramType)
            {
                case "integer":
                case "int":

                    //Is there even a reason to have these separated??? 
                    propEnum = AppendProperty(
                        catEnum,
                        propertyID++,
                        EarLabParam.PName,  //Name of the Parameter
                        EarLabParam,        //Object reference
                        "PValue",           //String name of the Parameter Value, given via reflection
                        "it works"          //The comment, which will need to be updated.
                        );                  //end

                    //Standard Way of showing the thing.
                    propEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelEdit);
                    ExpandProperty(propEnum, true);

                    break;

                case "float":
                case "double":
                case "dbl":
                case "fl":
                    //Cast the Parameter
                    propEnum = AppendProperty(
                                    catEnum,
                                    propertyID++,
                                    EarLabParam.PName,  //Name of the Parameter
                                    EarLabParam,        //Object reference
                                    "PValue",           //String name of the Parameter Value, given via reflection
                                    "it works");        //The comment, which will need to be updated.

                    //Standard Way of showing the thing.
                    propEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelEdit);
                    ExpandProperty(propEnum, true);


                    break;

                case "string":
                    //Cast the Parameter
                    propEnum = AppendProperty(
                                    catEnum,
                                    propertyID++,
                                    EarLabParam.PName,  //Name of the Parameter
                                    EarLabParam,        //Object reference
                                    "PValue",           //String name of the Parameter Value, given via reflection
                                    "it works");        //The comment, which will need to be updated.

                    //Standard Way of showing the thing.
                    propEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelEdit);
                    ExpandProperty(propEnum, true);

                    break;

                case "boolean":
                    //Cast the Parameter
                    propEnum = AppendProperty(
                                    catEnum,
                                    propertyID++,
                                    EarLabParam.PName,  //Name of the Parameter
                                    EarLabParam,        //Object reference
                                    "PValue",           //String name of the Parameter Value, given via reflection
                                    "it works");        //The comment, which will need to be updated.

                    //Standard Way of showing the thing.
                    propEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelList);
                    ExpandProperty(propEnum, true);
                    break;


                //The Arrays will have to be done differently ---- 
                //How do you display arrays in SPG!
                // 

                case "integer[]":
                case "int[]":
                    //change, to the new system
                    propEnum = AppendProperty(
                                    catEnum,
                                    propertyID++,
                                    EarLabParam.PName,  //Name of the Parameter
                                    EarLabParam,        //Object reference
                                    "PValue",           //String name of the Parameter Value, given via reflection
                                    "it works");        //The comment, which will need to be updated.

                    //Standard Way of showing the thing.
                    propEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelEdit);
                    ExpandProperty(propEnum, true);


                    break;

                case "double[]":
                case "float[]":
                case "dbl[]":
                case "fl[]":
                    //Cast the Parameter
                    propEnum = AppendProperty(
                                    catEnum,
                                    propertyID++,
                                    EarLabParam.PName,  //Name of the Parameter
                                    EarLabParam,        //Object reference
                                    "PValue",           //String name of the Parameter Value, given via reflection
                                    "it works");        //The comment, which will need to be updated.

                    //Standard Way of showing the thing.
                    propEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelEdit);
                    ExpandProperty(propEnum, true);
                    break;

                default:
                    //Some error exception here (e)
                    //Shouldn't really be needed.

                    break;
            } 
        }



    }

    //Makes the inputs all at once
    public void MakeInputs(List<EarLabInput> Inputs, PropertyEnumerator catEnum, ref int propertyID)
    {

        //create inputs


    }
    
    //Makes the outputs all at once
    public void MakeOutputs(List<EarLabOutput> Outputs, PropertyEnumerator catEnum, ref int propertyID)
    {

        //make outputs

    }



    }//module grid


}//namespace


    



