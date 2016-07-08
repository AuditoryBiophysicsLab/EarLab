// JavaScript Document:
// Search Central - general menu include for DHTML menus
// All URLs are absolute
//
function mmLoadMenus() {
if (window.softwaremenu) return;
window.softwaremenu = new Menu("root",190,16,"Verdana, Arial, Helvetica, sans-serif",10,"#555555","#6585A3","#ffffff","#F2F3F4","left","middle",3,3,300,-5,7,true,true,true,3,true,true);
softwaremenu.addMenuItem("Desktop Modeling Environment","location='/software/desktop/'");
softwaremenu.addMenuItem("DataViewer Application","location='/software/dataviewer/'");
softwaremenu.hideOnMouseOut=true;
softwaremenu.menuBorder=1;
softwaremenu.menuLiteBgColor='#ffffff';
softwaremenu.menuBorderBgColor='#999999';
softwaremenu.bgColor='#ffffff';
window.databasesmenu = new Menu("root",155,16,"Verdana, Arial, Helvetica, sans-serif",10,"#555555","#6585A3","#ffffff","#F2F3F4","left","middle",3,3,300,-5,7,true,true,true,3,false,true);
databasesmenu.addMenuItem("Revcor Database","location='/databases/revcor/'");
databasesmenu.addMenuItem("CIPIC HRTF Database","location='/databases/cipic/'");
databasesmenu.addMenuItem("Published Data Database","location='/databases/published/'");
databasesmenu.addMenuItem("EFI Modules Database","location='/databases/modules/'");
databasesmenu.hideOnMouseOut=true;
databasesmenu.menuBorder=1;
databasesmenu.menuLiteBgColor='#ffffff';
databasesmenu.menuBorderBgColor='#999999';
databasesmenu.bgColor='#ffffff';
window.modelingmenu = new Menu("root",145,16,"Verdana, Arial, Helvetica, sans-serif",10,"#555555","#6585A3","#ffffff","#F2F3F4","left","middle",3,3,300,-5,7,true,true,true,3,true,true);
modelingmenu.addMenuItem("Tutorial & Instructions","location='/modeling/tutorial/'");
modelingmenu.addMenuItem("My Modeling Home","window.open('/modeling/authenticated/','modeling_console','height=480,width=790,resizable=no,scrollbars=no,toolbar=no,menubar=no,location=no,status=no,directories=no,hotkeys=no');");
modelingmenu.hideOnMouseOut=true;
modelingmenu.menuBorder=1;
modelingmenu.menuLiteBgColor='#ffffff';
modelingmenu.menuBorderBgColor='#999999';
modelingmenu.bgColor='#ffffff';
window.supportmenu = new Menu("root",145,16,"Verdana, Arial, Helvetica, sans-serif",10,"#555555","#6585A3","#ffffff","#F2F3F4","left","middle",3,3,300,-5,7,true,true,true,3,true,true);
supportmenu.addMenuItem("File Specs & Schemas","location='/support/specs/'");
supportmenu.addMenuItem("Contact EarLab Team","location='/support/Contact.aspx'");
supportmenu.hideOnMouseOut=true;
supportmenu.menuBorder=1;
supportmenu.menuLiteBgColor='#ffffff';
supportmenu.menuBorderBgColor='#999999';
supportmenu.bgColor='#ffffff';
window.tutorialsmenu = new Menu("root",90,16,"Verdana, Arial, Helvetica, sans-serif",10,"#555555","#6585A3","#ffffff","#F2F3F4","left","middle",3,3,300,-5,7,true,true,true,3,true,true);
tutorialsmenu.addMenuItem("Introduction","location='/tutorials/introduction/Default.aspx'");
tutorialsmenu.addMenuItem("Anatomy","location='/tutorials/anatomy/Default.aspx'");
tutorialsmenu.addMenuItem("Physiology","location='/tutorials/physiology/Default.aspx'");
tutorialsmenu.hideOnMouseOut=true;
tutorialsmenu.menuBorder=1;
tutorialsmenu.menuLiteBgColor='#ffffff';
tutorialsmenu.menuBorderBgColor='#999999';
tutorialsmenu.bgColor='#ffffff';
   
tutorialsmenu.writeMenus();
}
