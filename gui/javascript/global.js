function MM_goToURL() {	//v3.0
    var i, args = MM_goToURL.arguments; document.MM_returnValue = false;
    for (i = 0; i < (args.length - 1) ; i += 2) eval(args[i] + ".location='" + args[i + 1] + "'");
};

function MM_preloadImages() {	//v3.0
    var d = document; if (d.images) {
        if (!d.MM_p) d.MM_p = new Array();
        var i, j = d.MM_p.length, a = MM_preloadImages.arguments; for (i = 0; i < a.length; i++)

            if (a[i].indexOf("#") != 0)
            { d.MM_p[j] = new Image; d.MM_p[j++].src = a[i]; }
}
};

function MM_swapImgRestore() {	//v3.0
    var i, x, a = document.MM_sr;
    for (i = 0; a && i < a.length && (x = a[i]) && x.oSrc; i++) x.src = x.oSrc;
};

function MM_openBrWindow(theURL, winName, features) {	//v2.0
    window.open(theURL, winName, features);
};

function MM_findObj(n, d) {	//v4.01
    var p, i, x;
    if (!d) d = document; if ((p = n.indexOf("?")) > 0 && parent.frames.length)
    { d = parent.frames[n.substring(p + 1)].document; n = n.substring(0, p); }

    if (!(x = d[n]) && d.all) x = d.all[n];
	
    for (i = 0; !x && i < d.forms.length; i++) x = d.forms[i][n];
	
    for (i = 0; !x && d.layers && i < d.layers.length; i++) x = MM_findObj(n, d.layers[i].document);
	
    if (!x && d.getElementById) x = d.getElementById(n); return x;
};


function MM_swapImage() {	//v3.0
    var i, j = 0, x, a = MM_swapImage.arguments; document.MM_sr = new Array;
    for (i = 0; i < (a.length - 2) ; i += 3)

        if ((x = MM_findObj(a[i])) != null)
        { document.MM_sr[j++] = x; if (!x.oSrc) x.oSrc = x.src; x.src = a[i + 2]; }
};

//Change the state on an element based on the action...
function change(elem, action) {
	if (action == 'hide')
		document.getElementById(elem).style.display = 'none';
	else if (action == 'show')
    { document.getElementById(elem).style.display = 'block'; }

};

//switch the current state of a specific element.
function toggle(elem) {
    document.getElementById(elem).style.display = document.getElementById(elem).style.display == 'none' ? document.all ? "block" : "table-row" : "none";
};
/*************************************************************************************************\
*					Functions added for the homepage:  Features and Headlines					  *
\*************************************************************************************************/

//Change the state of a group of elements based on the action...
/*function HideFeaturesAndHeadlines() {
	var CurrentFeature;
	var CurrentHeadline;
	var NewFeatureStyle;
	var NewHeadlineStyle;
	var Features;
	var Headlines;
	
    Features = document.getElementById('MainFeatures');
    Headlines = document.getElementById('FeatureHeadlines');
	
	var IndivFeatures = Features.getElementsByTagName('div');

	//loop through the child "div" tags of the MainFeatures div.
	//	This will hide all of the Features.
    for (var i = 0; i < IndivFeatures.length; i++) {
		CurrentFeature = IndivFeatures[i].id; 
		//if the element isn't "" or Undefined, update the div by calling the change function...
        try {
            if (CurrentFeature != "" && CurrentFeature.toLowerCase() != 'undefined') {
				NewFeatureStyle = change(CurrentFeature, 'hide')
			}
		}
        catch (ex)
        {; }
	}

	var IndivHeadlines = Headlines.getElementsByTagName('div');
		
	//loop through the child "div" tags of the the FeatureHeadlines Div.
	//	This will set all of the headlines to inactive.
    for (var j = 0; j < IndivHeadlines.length; j++) {
		CurrentHeadline = IndivHeadlines[j].id; 
		//if the element isn't "" or Undefined, update the div by calling the change function...
        try {
            if (CurrentHeadline != "" && CurrentHeadline.toLowerCase() != 'undefined') {
				NewHeadlineStyle = DeselectHeadline(CurrentHeadline);
			}
		}
        catch (ex)
        {; }
	}
};

function SelectHeadline(elem)
{
	if (document.location.protocol == "http:")
	{
		document.getElementById(elem).style.backgroundImage = 'url(http://img.cdw.com/cdw/homepage/gui/arrow_active.gif)';
	}
	else
	{
		document.getElementById(elem).style.backgroundImage = 'url(https://img.cdw.com/cdw/homepage/gui/arrow_active.gif)';
	}
	document.getElementById(elem).style.color = '#CC0000';
}

function DeselectHeadline(elem) {
    if (document.location.protocol == "http:") {
		document.getElementById(elem).style.backgroundImage = 'url(http://img.cdw.com/cdw/homepage/gui/arrow_inactive.gif)';
	}
    else {
		document.getElementById(elem).style.backgroundImage = 'url(https://img.cdw.com/cdw/homepage/gui/arrow_inactive.gif)';
	}
	document.getElementById(elem).style.color = '#000000';
};

function SetCursorLocation(formName, fieldName) {
	//var focusString = "document." + formName + "." + fieldName + ".focus();";
	//var selectString = "document." + formName + "." + fieldName + ".select();";

	//alert(focusString + "...<br />" + selectString);
//	initializeHacks();
//	document.getElementById(fieldName).focus();
//	document.getElementById(fieldName).select();
	

	document.globalSearch.key.focus();
	document.globalSearch.key.select();
};*/









/**************************************************************
************From global.js in content/assets/pages/framework
*************This belongs in TFS as one file.
************Keep white space - minification will handle.
**************************************************************/

/* remove textbox default value */
function refreshValues() {
	//var passwordId = document.getElementById("password");
	//var usernameId = document.getElementById("username");
	
    if (document.getElementById("username")) {
        document.getElementById("username").value = "Enter Username";
	}
    if (document.getElementById("password")) {
        document.getElementById("password").value = "";
	}
    if (document.getElementById("gn_checkbox1")) {
   		document.getElementById("gn_checkbox1").checked = false;
	}
    if (document.getElementById("gn_checkbox2")) {
   		document.getElementById("gn_checkbox2").checked = false;
	}
};

function setupRemoveDefault() {
		
var els;
if (document.getElementsByTagName("input")) {
	els = document.getElementsByTagName("input");
}
if (els.length) {
        for (var i = 0, j = els.length; i < j; i++) {
            els[i].onclick = function () { removeDefault(this); }
            els[i].onfocus = function () { removeDefault(this); }//Fixed for tab functionality on Logon popup
	 }    
	}
};

function removeDefault(el) {
    if ((!el.rel || el.value == el.rel) && (el.type == "text" || el.type == "password")) {
	 el.rel = el.value;
	 el.value = '';
	 var elName = el.id;
        if ((elName == "password") || (elName == "password-pop")) {
		var newObj = changeInputType(el, "password");
		giveFocus(newObj); 
		if (document.getElementById(el.id)) {
			document.getElementById(el.id).focus();
		}
	 	//newObj.onfocus = function () {removeDefault(newObj);}

		newObj.onblur = function () { 
                if (!newObj.value) { changeInputType(newObj, "text"); addDefault(elName); }
		}
	 }
        el.onblur = function () { if (!el.value) { addDefault(elName); } }
	 
	 return true;
	}
};

function addDefault(elName) {
	if (document.getElementById(elName)) {
		var el = document.getElementById(elName);
		el.value = el.rel;
	}
};

function changeInputType(oldObject, oType) {
  var newObject = document.createElement('input');
  newObject.type = oType;
    if (oldObject.size) newObject.size = oldObject.size;
    if (oldObject.value) newObject.value = oldObject.value;
    if (oldObject.name) newObject.name = oldObject.name;
    if (oldObject.id) newObject.id = oldObject.id;
    if (oldObject.rel) newObject.rel = oldObject.rel;
    if (oldObject.onfocus) newObject.onfocus = oldObject.onfocus;
    if (oldObject.tabIndex) newObject.tabIndex = oldObject.tabIndex;//Fixed for tab functionality on Logon popup (specific)
  
    if (oldObject.className) newObject.className = oldObject.className;
  if (window.attachEvent) {
        newObject.attachEvent("onfocus", setupRemoveDefault);
  }
  else if (window.addEventListener) {
        newObject.addEventListener("focus", setupRemoveDefault, true);
  }
    oldObject.parentNode.replaceChild(newObject, oldObject);
  return newObject;
};

function giveFocus(newObject) {
    if (document.getElementById(newObject.id)) {
  		document.getElementById(newObject.id).focus();
	}
};
/* drop down menu script */

function showMenu(menuName) {
  	hideAllMenus(menuName);
  	//show iframe
    if (document.getElementById(menuName)) {
        document.getElementById(menuName).style.visibility = 'visible';
	}
	//adjust corresponding iframe height
    if (document.getElementById(menuName + "-iframe")) {
	  document.getElementById(menuName + "-iframe").style.height = document.getElementById(menuName).offsetHeight - 13; 
	}
	//document.getElementById(menuName + "-iframe").style.height = document.getElementById(menuName).offsetHeight - 13; 
    if (document.getElementById(menuName + "-iframe")) {
        document.getElementById(menuName + "-iframe").style.visibility = 'visible';
	}
  	//document.getElementById(menuName + "-iframe").style.visibility='visible';
};
  
function hideAllMenus(menuName) {
    if (document.getElementById(menuName)) {
        document.getElementById(menuName).style.visibility = 'hidden';
	 }
    if (document.getElementById(menuName + "-iframe")) {
        document.getElementById(menuName + "-iframe").style.visibility = 'hidden';
	 }	 
};

/* display checkbox as image */

window.setTimeout("init()", 500);
var d=document;

function init() {	
	 so_createCustomCheckBoxes()
};

/*function so_checkCanCreate() {
	testImage = d.body.appendChild(d.createElement("img"));
	testImage.src = "$$$image_root$$$/assets/pages/framework/blank.gif" + new Date().valueOf();
	testImage.id = "so_testImage";
	testImage.onload = so_createCustomCheckBoxes;
};*/

function so_createCustomCheckBoxes() {

	// bail out is this is an older browser
    if (!d.getElementById) {
		return;
	}
	
	// remove our test image from the DOM
	//d.body.removeChild(d.getElementById("so_testImage"));
	// an array of applicable events that we'll need to carry over to our custom checkbox
	var events = new Array("onfocus", "onblur", "onselect", "onchange", "onclick", "ondblclick", "onmousedown", "onmouseup", "onmouseover", "onmousemove", "onmouseout", "onkeypress", "onkeydown", "onkeyup");
	// a reference var to all the forms in the document
	if (d.getElementsByTagName("form")) {
		frm = d.getElementsByTagName("form");
	}	
	// loop over the length of the forms in the document
	if (frm.length) {
        for (i = 0; i < frm.length; i++) {
			// reference to the elements of the form
			c = frm[i].elements;
			
            if (c != null) {
				// loop over the length of those elements
                if (c.length) {
                    for (j = 0; j < c.length; j++) {
						// if this element is a checkbox, do our thing
                        if (c[j].getAttribute("type") == "checkbox" && (c[j].getAttribute("id") == "gn_checkbox1" || c[j].getAttribute("id") == "gn_checkbox2")) {
							// hide the original checkbox
							c[j].style.position = "absolute";
							c[j].style.left = "-9000px";
							// create the replacement image
							n = d.createElement("img");
                            n.setAttribute("class", "chkbox");
							// check if the corresponding checkbox is checked or not. set the
							// status of the image accordingly
                            if (c[j].checked == false) {
                                if (browser == "Internet Explorer") {
                                    n.style.width = "9px";
                                    n.style.height = "9px";
								}					
                                n.setAttribute("src", "$$$image_root$$$/assets/pages/framework/chk_off.gif");
								//n.setAttribute("title","click here to select this option.");
								//n.setAttribute("alt","click here to select this option.");
							} else {
                                n.setAttribute("src", "$$$image_root$$$/assets/pages/framework/chk_on.gif");
								//n.setAttribute("title","click here to deselect this option.");
								//n.setAttribute("alt","click here to deselect this option.");
							}
							// there are several pieces of data we'll need to know later.
							// assign them as attributes of the image we've created
							// first - the name of the corresponding checkbox
							n.xid = c[j].getAttribute("id");
							// next, the index of the FORM element so we'll know which form object to access later
							n.frmIndex = i;
							// assign the onclick event to the image
                            n.onclick = function () { so_toggleCheckBox(this, 0); return false; }
							// insert the image into the DOM
                            c[j].parentNode.insertBefore(n, c[j].nextSibling)
							// this attribute is a bit of a hack - we need to know in the event of a label click (for browsers that support it)
							// which image we need turn on or off. So, we set the image as an attribute!
							c[j].objRef = n;
							// assign the checkbox objects event handlers to its replacement image
							if (events.length) {
                                for (e = 0; e < events.length; e++) if (eval('c[j].' + events[e])) eval('n.' + events[e] + '= c[j].' + events[e]);
							}
							// append our onchange event handler to any existing ones.
							fn = c[j].onchange;
                            if (typeof (fn) == "function") {
                                c[j].onchange = function () { fn(); so_toggleCheckBox(this.objRef, 1); return false; }
							} else {
                                c[j].onchange = function () { so_toggleCheckBox(this.objRef, 1); return false; }
							}
						}
					}
				}
			}
		}
	}
	
};

function so_toggleCheckBox(imgObj, caller) {
	
	// if caller is 1, this method has been called from the onchange event of the checkbox, which means
	// the user has clicked the label element. Dont change the checked status of the checkbox in this instance
	// or we'll set it to the opposite of what the user wants. caller is 0 if coming from the onclick event of the image
	// reference to the form object
	formObj = d.forms[imgObj.frmIndex];
	// the name of the checkbox we're changing
	objName = imgObj.xid;
	// change the checked status of the checkbox if coming from the onclick of the image
    if (!caller) formObj.elements[objName].checked = !formObj.elements[objName].checked ? true : false;
	// finally, update the image to reflect the current state of the checkbox.
    if (imgObj.src.indexOf("$$$image_root$$$/assets/pages/framework/chk_on.gif") > -1) {
        imgObj.setAttribute("src", "$$$image_root$$$/assets/pages/framework/chk_off.gif");
		
		//imgObj.setAttribute("title","click here to select this option.");
		//imgObj.setAttribute("alt","click here to select this option.");
	} else {
        imgObj.setAttribute("src", "$$$image_root$$$/assets/pages/framework/chk_on.gif");
		//imgObj.setAttribute("title","click here to deselect this option.");
		//imgObj.setAttribute("alt","click here to deselect this option.");
	}
};

//sfHovers
function findPos(obj) {
	var curleft = curtop = curwidth = 0;
	if (obj.offsetParent) {
		curleft = obj.offsetLeft
		curtop = obj.offsetTop
		curwidth = obj.offsetWidth
		while (obj = obj.offsetParent) {
			curleft += obj.offsetLeft
			curtop += obj.offsetTop
			curwidth += obj.offsetWidth
		}
	}
	//alert (curleft);
    return [curleft, curtop, curwidth];
};

/*function globalNav(className) {
	var sfEls;
	if (document.getElementById(className)) {
		sfEls = document.getElementById(className).getElementsByTagName("li");
	}
	//Detect IE6.0<
    version = 0;
    if (navigator.appVersion.indexOf("MSIE") != -1) {
        temp = navigator.appVersion.split("MSIE");
        version = parseFloat(temp[1]);
	}
	if (sfEls.length) {
        for (var i = 0; i < sfEls.length; i++) {
            sfEls[i].onmouseover = function () {
				var compareTo;
                if (this.name == 'products') {
					compareTo = "hidethis1";
				}
                if (this.name == 'products-h') {
					compareTo = "hidethis1 hidethis2";
				}
                if (this.name == 'products-s') {
					compareTo = "hidethis1 hidethis2";
				}
                if (this.name == 'services') {
					compareTo = "hidethis2";
				}
                if (this.name == 'solcenter') {
					compareTo = "hidethis3";
				}
                if (this.name == 'whatcdwoffers') {
					compareTo = "hidethis4";
				}
                if (this.name == 'order-center') {
					compareTo = "hidethis1";
				}
                if (this.name == 'quotes-fav') {
					compareTo = "hidethis2";
				}
                if (this.name == 'acct-manage') {
					compareTo = "hidethis3";
				}
                if (this.name == 'tools') {
					compareTo = "hidethis4";
				}
                if (this.name == 'acct-support') {
					compareTo = "hidethis4";
				}
                this.className += " sfhover";
                if (navigator.appVersion.indexOf("MSIE") != -1) {
                    temp = navigator.appVersion.split("MSIE");
                    version = parseFloat(temp[1]);
				}
                if (version <= 6.0 && navigator.appName == "Microsoft Internet Explorer") { //NON IE browser will return 0
					var sfSelects = document.getElementsByName('hidethis');
                    if (sfSelects.length) {
                        for (var s = 0; s < sfSelects.length; s++) {
							//alert(sfSelects[s].className);
							var classnames = sfSelects[s].className.split(" ");
							var compareToArr = compareTo.split(" ");
							//alert(classnames.length);
							if (compareToArr.length) {
                                for (var ct = 0; ct < compareToArr.length; ct++) {
                                    if (classnames.length) {
                                        for (var j = 0; j < classnames.length; j++) {
                                            if (sfSelects[s].id == "hideme" && classnames[j] == compareToArr[ct])
												sfSelects[s].style.visibility = "hidden";
										}
									}
								}
							}
						}
					}
				}
			}
            sfEls[i].onmouseout = function () {
                this.className = this.className.replace(new RegExp("sfhover\\b"), "");
				var sfSelects = document.getElementsByName('hidethis');
				if (sfSelects.length) {
                    for (var s = 0; s < sfSelects.length; s++) {
						sfSelects[s].style.visibility = "visible";
					}
				}
			}
		}
	}
};

function footerNav() {
	//if ((OS == "Windows") && (browser == "Internet Explorer")) {	
    if (document.all && document.getElementById) {
			var navRoot = document.getElementById("footernav");						
			if (navRoot != null) {								
            for (i = 0; i < navRoot.childNodes.length; i++) {
					node = navRoot.childNodes[i];					
                if (node.nodeName == "LI") {
                    node.onmouseover = function () {
                        this.className += " over";
						}
                    node.onmouseout = function () {
                        this.className = this.className.replace(" over", "");
						}
					}
				}
			}
		}
	//}
};*/

var detect = navigator.userAgent.toLowerCase();
var OS,browser,total,thestring;
var version = 0;

if (checkIt('konqueror'))
{
	browser = "Konqueror";
	OS = "Linux";
}
else if (checkIt('safari')) browser = "Safari";
else if (checkIt('omniweb')) browser = "OmniWeb";
else if (checkIt('opera')) browser = "Opera";
else if (checkIt('webtv')) browser = "WebTV";
else if (checkIt('icab')) browser = "iCab";
else if (checkIt('msie')) browser = "Internet Explorer";
else if (!checkIt('compatible'))
{
	browser = "Netscape Navigator";
	version = detect.charAt(8);
}
else browser = "An unknown browser";

if (!version) version = detect.charAt(place + thestring.length);

if (!OS)
{
	if (checkIt('linux')) OS = "Linux";
	else if (checkIt('x11')) OS = "Unix";
	else if (checkIt('mac')) OS = "Mac";
	else if (checkIt('win')) OS = "Windows"
	else OS = "an unknown operating system";
}

function checkIt(string) {
	place = detect.indexOf(string) + 1;
	thestring = string;
	return place;
};

function rememberParent(childId) {
	var elem;
	if (document.getElementById(childId)) {
		elem = document.getElementById(childId);
	}
    if (document.getElementById(childId)) {
		elem.style.color = "#fff";
	} else {
		alert("none found");
	}
};

function forgetParent(childId) {
    if (document.getElementById(childId)) {
		var elem = document.getElementById(childId);
		elem.style.color = "";
	} else {
		alert("none found");
	}
};

/*if javascript disabled

function javascriptIsEnabled() {
	replaceClassValue("shopcart-sec", "grey-toplink-arrow");
	replaceClassValue("offThisNSLink", "header-top-link-txt margin-left8 header-nav-sec-items");
	replaceClassValue("offThisLink", "header-top-link-txt margin-left8");
	replaceHrefValue("logontext", "#");
	
	replaceClassValue("products-tab", "tab offLink");
	replaceClassValue("services-tab", "tab offLink");
	replaceClassValue("solcenter-tab", "tab offLink");
	replaceClassValue("whatcdwoffers-tab", "tab-last offLink");
	
	replaceClassValue("ordercenter-tab", "order-menu-tab offLink");
	replaceClassValue("quotesandfaves-tab", "order-menu-tab offLink");
	replaceClassValue("acctmgmt-tab", "order-menu-tab offLink");
	replaceClassValue("tools-tab", "order-menu-tab offLink");
	replaceClassValue("acctsupport-tab", "order-menu-tab-last offLink");
};*/

function replaceClassValue(idval, replaceWith) {
    if (document.getElementById(idval)) {
		var valueId = document.getElementById(idval);
		valueId.className = replaceWith;
	}
};

function replaceHrefValue(idval, replaceWith) {
    if (document.getElementById(idval)) {
		var valueId = document.getElementById(idval);
		valueId.href = replaceWith;
	}
};
/************************ IE Dropdown iframe fix ******************************/

// Created a long, long time ago by Tanny O'Haley
// Update history.
// 24 Oct 2006 - Added to http://tanny.ica.com
// 14 Jul 2006 - Modified the way the iframe is added to the DOM. I no longer use 
//			innerHTML but use DOM methods.
// 15 Sep 2006 - Added a check to see if the target element onmouseout is contained
//			in the onmouseout element. If it is then I dont' remove the
//			sfhover class. I made use of the Microsoft proprietary
//			obj.contains() method.
//		 Added check to make sure that the sfhover class is not already in
//			the li element.
//
/*sfHover = function () {
	// Support the standard nav without a class of nav.
	var el = null;
    if (document.getElementById("header-menu-tab")) {
		el = document.getElementById("header-menu-tab");
	}
	
    if (el == null) {
		if (document.getElementById("order-menu-tab")) {
	    	el = document.getElementById("order-menu-tab");	
		}
	}	
	
    if (el != null) {
        if (!/\bnav\b/.test(el.className) && el.tagName == "ul") {
	        setHover(el);
	    }
	    // Find all unordered lists.
	    var ieNavs = document.getElementsByTagName('ul');
	    if (ieNavs.length) {
            for (i = 0; i < ieNavs.length; i++) {
				var ul = ieNavs[i];		    
				// If they have a class of nav add the menu hover.
                if (/\bnav\b/.test(ul.className)) {
					setHover(ul);
				}
			}
		}
	}
};*/

function pauseFor(millis) {
	var date = new Date();
	var curDate = null;
	
	do { curDate = new Date(); }
    while (curDate - date < millis);
};

/*function setHover(nav) {
	var ieULs = nav.getElementsByTagName('ul');		
    if (navigator.appVersion.substr(22, 3) != "5.0") {
		// IE script to cover <select> elements with <iframe>s
		if (ieULs.length) {
            for (j = 0; j < ieULs.length; j++) {
                var ieMat = document.createElement('iframe');
                if (document.location.protocol == "https:")
                    ieMat.src = "javascript:false";
                else if (window.opera != "undefined")
                    ieMat.src = "";
				else
                    ieMat.src = "javascript:false";
                ieMat.scrolling = "no";
                ieMat.frameBorder = "0";
                ieMat.style.width = ieULs[j].offsetWidth + "px";
                ieMat.style.height = ieULs[j].offsetHeight + 2 + "px";
                ieMat.style.zIndex = "-1";
				ieULs[j].insertBefore(ieMat, ieULs[j].childNodes[0]);
                ieULs[j].style.zIndex = "101";
				
			}
		}
		// IE script to change class on mouseover
		var ieLIs = nav.getElementsByTagName('li');
        if (ieLIs.length) {
            for (var i = 0; i < ieLIs.length; i++) if (ieLIs[i]) {
				// Add a sfhover class to the li.
                ieLIs[i].onmouseover = function () {
                    if (!/\bsfhover\b/.test(this.className))
					
                        this.className += " sfhover";
				}
                ieLIs[i].onmouseout = function () {
                    if (!this.contains(event.toElement))
                        this.className = this.className.replace(' sfhover', '');
				}
			}
		}
	} else {
		// IE 5.0 doesn't support iframes so hide the select statements on hover and show on mouse out.
		// IE script to change class on mouseover
		var ieLIs;
		if (document.getElementById('nav').getElementsByTagName('li')) {
			ieLIs = document.getElementById('nav').getElementsByTagName('li');	
		}
        if (ieLIs.length) {
            for (var i = 0; i < ieLIs.length; i++) if (ieLIs[i]) {
                ieLIs[i].onmouseover = function () { this.className += " sfhover"; hideSelects(); }
                ieLIs[i].onmouseout = function () { this.className = this.className.replace(' sfhover', ''); showSelects() }
		}
	}
}
};

// If IE 5.0 hide and show the select statements.
function hideSelects() {
    var oSelects = document.getElementsByTagName("select");
    if (oSelects.length) {
        for (var i = 0; i < oSelects.length; i++)
            oSelects[i].className += " hide";
}
};

function showSelects() {
    var oSelects = document.getElementsByTagName("select");
    if (oSelects.length) {
        for (var i = 0; i < oSelects.length; i++)
            oSelects[i].className = oSelects[i].className.replace(" hide", "");
}
};*/



/*--------- Check Box Select ---------------*/
function getChecked(thisObj) {
	var objId = thisObj.id;
	var txtObjId = objId + "value";
	var objSrc = thisObj.src;
	var lastSlash = objSrc.lastIndexOf("/");
    var imgName = objSrc.slice(lastSlash + 1);
    if (imgName == "chk_off.gif") {
        thisObj.setAttribute("src", "$$$image_root$$$/assets/pages/framework/chk_on.gif");
		if (document.getElementById(txtObjId)) {
			document.getElementById(txtObjId).value = "on";
		}
    } else if (imgName == "chk_on.gif") {
        thisObj.setAttribute("src", "$$$image_root$$$/assets/pages/framework/chk_off.gif");
		if (document.getElementById(txtObjId)) {
			document.getElementById(txtObjId).value = "off";
		}
	}
};

/*function setGlobalNav() {
    var el = null;
	if (document.getElementById("header-menu-tab")) {
		el = document.getElementById("header-menu-tab");
	}
    if (el == null) {
		if (document.getElementById("order-menu-tab")) {
	    	el = document.getElementById("order-menu-tab");	
		}    
        if (el != null) {
	        globalNav("order-menu-tab");
	    }
	} else {
	    globalNav("header-menu-tab");
	}
};

// Run this only for IE.
if (window.attachEvent) window.attachEvent('onload', sfHover);
// end
window.onload=function(){setGlobalNav();javascriptIsEnabled();footerNav();}*/

// JQuery routines


    function BlockUI(controlLabel) {   
		 $.blockUI.defaults.css = {};
         $.blockUI({ 
                message: $(controlLabel),  
        css: {
            padding: 0,
            margin: 0,
            width: 'auto',
            top: '40%',
            left: '35%',
            color: '#333',
            border: '1px solid #806E56',
            backgroundColor: '#f1f2ec',
            cursor: 'default'
                },
                overlayCSS: {        
            backgroundColor: '#fff',
            opacity: '0.5'
                }    
          }); 
};
// Override BlockUI default CSS
	function MsgPopoverBlockUI(controlLabel) {   
		 $.blockUI.defaults.css = {};
    var w = Math.round((100 - Math.round((520 / $(window).width()) * 100)) / 2) + '%';
		 //alert(w);
         $.blockUI({ 
                message: $(controlLabel),  
        css: {
            padding: 0,
            margin: 0,
            width: 'auto',
            top: '20%',
            left: w,
            color: '#333',
            border: '1px solid #806E56',
            backgroundColor: '#f1f2ec',
            cursor: 'default'
                },
                overlayCSS: {        
            backgroundColor: '#fff',
            opacity: '0.5'
                }    
          }); 
};

    // BEGIN -- routines related to Information Module and PopUp Module
    var isIE = (window.navigator.userAgent.indexOf("MSIE") > -1);
	function gFindPos(obj) {
		var offsets = jQuery(obj).offset();
		return [offsets.left, offsets.top, true];
    };

    function gShowInfoModulePopup(parent, posX, posY) {
        gShowPopupMessage(
            parent,
            findInfoModulePopupId(parent),
            posX,
            posY);
    };

    function gHideInfoModulePopup(parent) {
        gHidePopupMessage(findInfoModulePopupId(parent));
    };

    function findInfoModulePopupId(parent) {
        return parent.id.slice(0, parent.id.indexOf('lblInfoMsg')) + 'popupInfo_pnlOverlay';
    };

    function gShowPopupMessage(parent, elementID, posX, posY) {
		var element = document.getElementById(elementID);	    
		var placement = gFindPos(parent);     
		//Get source top,left corner, then add the width of the source
		element.style.left = parseInt(placement[0] + posX) + "px";
		element.style.top = parseInt(placement[1] + posY) + "px";
		if (placement[2]) { 
			moveToBody(element);
		}
        element.style.visibility = "visible";
    };
    function gHidePopupMessage(elementID) {
		var elem = document.getElementById(elementID); 
		var elemStyle = (elem.style || elem).visibility = "hidden";
	    
    };
    function gShowPopupSimple(parent, elementID, posX, posY) {
		var elem = document.getElementById(elementID); 
		var elemStyle = elem.style || elem; // for NS4
        elemStyle.visibility = "visible";
		// If the correct position has not already been set, then set it now
        if (!elemStyle.left) elemStyle.left = parseInt(elem.offsetLeft + posX) + "px";
        if (!elemStyle.top) elemStyle.top = parseInt(elem.offsetTop + posY) + "px";
    };
	    
    function isOverflowHidden(elem) {
        if (elem) {
			var elemStyle = elem.currentStyle;
            if (elemStyle) {
                return ((elemStyle.overflow == 'hidden') ||
						 (elemStyle.overflowX == 'hidden') ||
						 (elemStyle.overflowY == 'hidden'));
			}
		}
		return false;
    };

	function moveToBody(el) {
		var containerID = 'div' + el.id + 'cntr';
		var popupContainer = document.getElementById(containerID);
        if (popupContainer) {
			// If its already moved then just get out of dodge
			if (el.parentNode && el.parentNode.id == containerID)
				return;
	            
			// Async postback must have been done after displaying
			//  a popup so we now have multiple copies of the element
			//  so wipe out the old one which is now obsolete
			popupContainer.innerHTML = '';
		}
        else {
			popupContainer = createPopupContainerDiv(containerID);
			setParent(popupContainer, document.body);
		}
		setParent(el, popupContainer);
    };

    function createPopupContainerDiv(popupContainterID) {
		var dv = document.createElement('div'); 
		dv.setAttribute('id', popupContainterID);
		return dv;
    };

	function setParent(el, newParent) {
		newParent.appendChild(el);
    };
// END - Information Module and PopUp Module user control js routines

	///*--------- Used for ratings and reviews metrics ---------------*/
	//var baseline = "-_--_--_--_--_-testing-_-some-_-other-_-values-_--_-";
	//var fields = ["", "5", "10.3", "", "", "overwrite"];
	//alert(CoreMetricsSetAttibuteFields(baseline, fields));
	function CoreMetricsSetAttibuteFields(baseline, fields) {
		var populated = "";

		//has any existing values
		var splitBase = baseline.split("-_-");

		//keep any existing values other than those we want to overwrite based on passed in position
		for (var i = 0; i < splitBase.length; i++) {
			if (i < fields.length && fields[i] != "") {
				populated += fields[i];
			}
			else {
				populated += splitBase[i];
			}
			if (i < splitBase.length - 1) {
				populated += "-_-";
			}
		}
		return populated;
    };

function OneLink(sHostname) {
    document.location.href = document.location.protocol + "//" + sHostname + document.location.pathname;
    };

//JS functions for Product Review Rating 


    function ReviewRedirect(url, cmtagval) {
    window.location.href = url;
    CdwTagMan.createElementPageTag("Read All Reviews", cmtagval);    
    return false;
    };

    function ReviewMouseHover(lblreviewlink) {
document.getElementById(lblreviewlink).className = 'lblreviewhoverin';
    return false;
    };

    function ReviewMouseOut(lblreviewlink) {
document.getElementById(lblreviewlink).className = 'lblreviewhoverout';
    return false;
    };


function enablePopups() {
	$('.paneLock,.hasPopup').hover(function () {
		var pos = $(this).position();
		var $bubble = $('.' + $(this).attr('id') + 'Bubble');

		if ($bubble.size() == 0)
		    $bubble = $(this).prev('.infoPopup');
		if ($.trim($($bubble).text()) != "") {
		    // Code here
		$bubble.css({ position: 'absolute', top: pos.top - 75 + 'px', left: pos.left - 19 + 'px' }).stop().slideDown(100);
        }
	}, function () {
		$('.infoPopup').hide();
	});
    };

//if formId is not undefined it will attempt to 
function enableTitles(formId) {
        $("input").each(function (i, v) {
	    if (v.value == '') {
	        v.value = v.title;
	        $(v).addClass('inputLabel');
	    }

	    $(v).addClass('toolTip')
                .focusin(function (eo) {
			    if (v.value == v.title) {
			        v.value = '';
			        $(v).removeClass('inputLabel');
			    }
                }).focusout(function (eo) {
			    if (v.value == '') {
			        v.value = v.title;
			        $(v).addClass('inputLabel');
			    }
			});
	});
	
	if (formId)
		$('form[id=' + formId + ']').submit(clearInputsBeforeSubmit);
    };

    function clearInputsBeforeSubmit() {
        $("input:text:not([value=])").each(function (i, v) { if (v.value == v.title) v.value = ""; });
    };

function TagManValidationErrorEventWriter(errormsg) {
    if (window.ensCustomEvent && window.ensCustomEvent.create) {
        window.ensCustomEvent.create('FORM_SUBMIT_VALIDATIONERROR', { validation_error: errormsg });
    } else {
        if (window.TAGMAN && window.TAGMAN.fireEvent) {
            window.TAGMAN.fireEvent('FORM_SUBMIT_VALIDATIONERROR', { validation_error: errormsg });
        }
    }
}

function TagManVIRBannerEventWriter(promoteid, sourceString) {
    if (window.ensCustomEvent && window.ensCustomEvent.create) {
        window.ensCustomEvent.create('FORM_CLICK_VIR_BANNER', { promote_type: promoteid, source_type: sourceString });
    } else {
        if (window.TAGMAN && window.TAGMAN.fireEvent) {
            window.TAGMAN.fireEvent('FORM_CLICK_VIR_BANNER', { promote_type: promoteid, source_type: sourceString });
        }
    }
}