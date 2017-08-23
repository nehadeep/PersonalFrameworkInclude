
// Library Name: Cdw.Framework.Web.js
// Dependencies: None
// -----------------------------------------------------------
// Modified:    By:             Description:
// 02/26/2008   Jason Kraft     Intial Release

// Namespace: ClientEvent
// Usage: Group functionality which works with client-side event model
var ClientEvent = {};

// Namespace: ErrorManagement
// Usage: Group functionality related to reporting errors to reporting errors.
var ErrorManagement = {};

// Namespace: GUI
// Usage: Group functionality related to manipulating the DOM here.
var GUI = {};
    
// Function Name: ClientEvent.addLoadEvent
// Date Added: 03/07/2008 by Jason Kraft
// Usage: ClientEvent.addLoadEvent(myFunctionToRunOnLoad);
// Description: Use this to add multiple function calls to the Window.Onload
// event.
ClientEvent.addLoadEvent = function (func){
    try
    {
        var oldonload = window.onload;
        if (typeof window.onload != 'function'){
	        window.onload = func;
	        }else{
	        window.onload = function(){
		        oldonload();
		        func();
	        }
        }
    }
    catch(e)
    {
        alert(e.ToString());
    }	
}

GUI.hide = function (element){
    try
    {
        document.getElementByName(element).style.visibility = "hidden";
    }
    catch(e)
    {
        alert(e.ToString());
    }
}

ErrorManagement.LogError = function (callername, error) {
        
}

// Object Name: 