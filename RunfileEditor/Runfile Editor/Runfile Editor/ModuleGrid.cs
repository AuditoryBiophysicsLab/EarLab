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
    /// 
    ///New class that inherits from tabpage -- constructor take in Earlab module
    ///adds controls to itself - from spg and any buttons and gifs
    /// 
    /// </summary>
public partial class frmContainer : Form
    {

        //Handle the tabs ============================================================================================//
        public void ModuleTab(EarlabModule theEarlabModule)
        {
            //This class creates tab on the form

            //1.) Create the Module Grid
            ModuleGrid myModuleGrid = new ModuleGrid();

            //takes the module and creates the SPG
            myModuleGrid.Initialize(theEarlabModule);

            //
            //A.) Need to get a TITLE to push into it.
            //B.) You want to update the item with a "*" to say it has changed since last saved?
            // use "HasChanged" at the module level. HasChanged doesn't work :/
            string ModuleTitle;

            //[change]
            //if (theEarlabModule.HasChanged == true)
            //{
            //    ModuleTitle = theEarlabModule.theEarlabModuleInformation.InstanceName + "*";
            //}
            //else
            //{
            ModuleTitle = theEarlabModule.theEarlabModuleInformation.InstanceName;
            //}

            //2.) Add Module Grid to Tab
            TabPage tabPage = new TabPage(ModuleTitle);
            tabPage.Controls.Add(myModuleGrid);


            //3.) Tab location and size settings
            tabPage.Location = new System.Drawing.Point(4, 22);
            tabPage.Padding = new System.Windows.Forms.Padding(3);
            tabPage.Size = new System.Drawing.Size(831, 392);
            tabPage.TabIndex = 1;
            tabPage.UseVisualStyleBackColor = true;

            //4.) Add tab to control
            tabControl1.TabPages.Add(tabPage);
        }

        //Just a wrapper to use the tabControl
        public void ModuleClearTabs()
        {
            //clear all tabs
            tabControl1.TabPages.Clear();

            //Add back tabpage1
            tabControl1.TabPages.Add(tabPage1);

            //remove selected tab, might be another option
            //tabControl1.TabPages.Remove(tabControl1.SelectedTab);
        }

        public void AbstractModuleTab(EarlabRunfile RunfileObject)
        {


        }


    }



public class ModuleGrid : VisualHint.SmartPropertyGrid.PropertyGrid
    {
        public ModuleGrid()
        {
        }

    //initialize a module 
    public void Initialize(EarlabModule theEarlabModule)
        {
        
        
            //[Display Event]
            //this.Clear();
            //Information directly from the Module
            //This just mirrors everything else.
            int propertyID = 0;
            //int parameterNumber = 1;

            PropertyEnumerator catEnum;

            //There is a problem with the "PropertyEnumerator for the propEnum!"

            //testing something -- earlab class knows what prop grid it is displayed on.
            //theEarlabModule.DisplayGrid = this; //[Display Event]

            //if module has params
            if (theEarlabModule.EarlabParameters.Count > 0)
            {
                //start param, this works!
                catEnum = AppendRootCategory(propertyID++, "Parameters");

                //A method to make the ALL the parameters.
                MakeParameters(theEarlabModule.EarlabParameters, catEnum, ref propertyID);

            }
            else
            {
                catEnum = AppendRootCategory(propertyID++, "No Parameters in this Module");
            }


            //if module has inputs
            // same code
            if (theEarlabModule.EarlabInputs.Count > 0)
            {
                //start param
                catEnum = AppendRootCategory(propertyID++, "Inputs");

                //A method of all the inputs
                //MakeInputs(theEarlabModule.EarlabInputs, catEnum, ref propertyID);

            }
            else
            {
                catEnum = AppendRootCategory(propertyID++, "No Inputs in this Module");
            }


            //if module has outputs
            //same code, you can't use "null" as a conditional b/c there is something in there.
            if (theEarlabModule.EarlabOutputs.Count > 0)
            {
                //start param
                catEnum = AppendRootCategory(propertyID++, "Outputs");

                //A method of all the outputs
                //MakeOutputs(theEarlabModule.EarlabOutputs, catEnum, ref propertyID);
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



    //[Property Changes] property changed This can call all the changes!
    //Make a settings function that lists all the changes
    //1.) EFI
    //2.) If bad update
    //3.) SPG
    //4.) List View
    //5.) Tab
    //6.) Else 
    //7.) Show button
    protected override void OnPropertyChanged(VisualHint.SmartPropertyGrid.PropertyChangedEventArgs e)
    {

        //[Change] == SPG can allow for the module XML to update the acceptable Dt
        // on property change
        // check against max, min and default for each property in the Module.
        // if out of range, give a new message
        // if out of range, turn it red

        if (e.PropertyEnum.Property.Id > 0)
        {
            ;
        }

        //All code goes above this SPG standard fragment.
        //Section 4.12 of the spg manual, p 37
        base.OnPropertyChanged(e);
    }


    //Makes the parameters all at once
    public void MakeParameters(List<EarlabParameter> Parameters, PropertyEnumerator catEnum, ref int propertyID)
    {

        PropertyEnumerator propEnum;

        foreach (EarlabParameter EarlabParam in Parameters)

        {
            string paramType = EarlabParam.PType.ToLower();
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

                    propEnum = AppendProperty(
                        catEnum,
                        propertyID++,
                        EarlabParam.PName,  //Name of the Parameter
                        EarlabParam,        //Object reference
                        "PValue",           //String name of the Parameter Value, given via reflection
                        EarlabParam.Message //The comment, which will need to be updated.
                        );                  //end

                    //Standard Way of showing the thing.
                    propEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelEdit);
         

                    if (EarlabParam.Severity == "Warning")
                    {
                        //If there is a problem::::
                        propEnum.Property.BackColor = Color.Firebrick;   //<-- back ground color change
                        propEnum.Property.ForeColor = Color.FloralWhite; //<-- text color change
                    }


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
                                    EarlabParam.PName,  //Name of the Parameter
                                    EarlabParam,        //Object reference
                                    "PValue",           //String name of the Parameter Value, given via reflection
                                    EarlabParam.Message//The comment, which will need to be updated.
                                    
                                    ); 

                    //Standard Way of showing the thing.
                    propEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelEdit);

                    if (EarlabParam.Severity == "Warning")
                    {
                        propEnum.Property.BackColor = Color.Firebrick;   //<-- back ground color change
                        propEnum.Property.ForeColor = Color.FloralWhite; //<-- text color change
                    }



                    ExpandProperty(propEnum, true);


                    break;

                case "string":
                    //Cast the Parameter
                    propEnum = AppendProperty(
                                    catEnum,
                                    propertyID++,
                                    EarlabParam.PName,  //Name of the Parameter
                                    EarlabParam,        //Object reference
                                    "PValue",           //String name of the Parameter Value, given via reflection
                                    EarlabParam.Message);        //The comment, which will need to be updated.

                    //Standard Way of showing the thing.
                    propEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelEdit);


                    if (EarlabParam.Severity == "Warning")
                    {
                        propEnum.Property.BackColor = Color.Firebrick;   //<-- back ground color change
                        propEnum.Property.ForeColor = Color.FloralWhite; //<-- text color change
                    }
                    ExpandProperty(propEnum, true);

                    break;

                case "boolean":
                    //Cast the Parameter
                    propEnum = AppendProperty(
                                    catEnum,
                                    propertyID++,
                                    EarlabParam.PName,  //Name of the Parameter
                                    EarlabParam,        //Object reference
                                    "PValue",           //String name of the Parameter Value, given via reflection
                                    EarlabParam.Message);        //The comment, which will need to be updated.

                    //Standard Way of showing the thing.
                    propEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelList);


                    if (EarlabParam.Severity == "Warning")
                    {
                        propEnum.Property.BackColor = Color.Firebrick;   //<-- back ground color change
                        propEnum.Property.ForeColor = Color.FloralWhite; //<-- text color change
                    }

                    ExpandProperty(propEnum, true);
                    break;





                //The Arrays will have to be done differently =======================================================///
                case "integer[]":
                case "int[]":

                    //change, to the new system
                    propEnum = AppendProperty(
                                    catEnum,
                                    propertyID++,
                                    EarlabParam.PName,  //Name of the Parameter
                                    EarlabParam,        //Object reference
                                    "PValue",           //String name of the Parameter Value, given via reflection
                                    EarlabParam.Message); //The comment, which will need to be updated.

                    //Standard Way of showing the thing.
                    propEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelEdit);


                    if (EarlabParam.Severity == "Warning")
                    {
                        propEnum.Property.BackColor = Color.Firebrick;   //<-- back ground color change
                        propEnum.Property.ForeColor = Color.FloralWhite; //<-- text color change
                    }

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
                                    EarlabParam.PName,  //Name of the Parameter
                                    EarlabParam,        //Object reference
                                    "PValue",           //String name of the Parameter Value, given via reflection
                                    EarlabParam.Message);        //The comment, which will need to be updated.

                    //Standard Way of showing the thing.
                    propEnum.Property.Feel = GetRegisteredFeel(VisualHint.SmartPropertyGrid.PropertyGrid.FeelEdit);

                    if (EarlabParam.Severity == "Warning")
                    {
                        propEnum.Property.BackColor = Color.Firebrick;   //<-- back ground color change
                        propEnum.Property.ForeColor = Color.FloralWhite; //<-- text color change
                    }

                    ExpandProperty(propEnum, true);
                    break;

                default:
                    //Some error exception here (e)
                    //Shouldn't really be needed.

                    break;
            } 
        }



    }


}
}


    



