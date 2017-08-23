var saveChangesText = "Do you want to save your changes?";
var removeText = "Do you want to remove this record?";

$(function() {
	Sys.WebForms.PageRequestManager.getInstance().add_endRequest(jAccordionEndRequestHandler);
	Sys.WebForms.PageRequestManager.getInstance().add_beginRequest(jAccordionBeginRequestHandler);
	Sys.WebForms.PageRequestManager.getInstance().add_pageLoading(function(sender, args) { eval(args.get_dataItems()["LoadingScript"]); });

	var divPopup = '<div id="popup" class="confirmationPopUp">';
	divPopup += '<span id="popupText" /><br/>';
	divPopup += '<a href="javascript:;" id="aPopupYes" class="blueButton">Yes</a>';
	divPopup += '<a href="javascript:;" id="aPopupNo" class="whiteButton">No</a>';
	divPopup += '</div>';
	$('#popupWrapper').append(divPopup);

	// this is needed for update panels to maintain the '#' anchor tags on partial post-backs.
	Sys._Application.prototype._setState = function() { };
	
	// this is for supporting keyboard accessibility when Yes/No dialog is displayed
	$(document).keydown(function(e) {
		if (popupStatus && e.keyCode == 13) {
			if (document.activeElement.id == 'aPopupYes') {
				$('#aPopupYes').trigger('click');
			}
			if (document.activeElement.id == 'aPopupNo') {
				$('#aPopupNo').trigger('click');
			}
		}
	});
});

var jAccordionInputChanged = false;
var jAccordionToDoRequest = "";
var jAccordionSaveChanges = false;
var jAccordionKeepInputChanged = false;

function setUpPopup(htmlString, yesMethod, noMethod){
	$("#popupText").html(htmlString);
	$("#aPopupYes").off("click");
	$("#aPopupYes").click(yesMethod);
	$("#aPopupNo").off("click");
	$("#aPopupNo").click(noMethod);
}

function showDeletePopup(anchor){
	showPopup(anchor, removeText);
}

function showPopup(anchor, message){
	centerPopup();
	setUpPopup(message, function() {eval($(anchor).attr("href"));disablePopup(null);}, 
		function() {disablePopup(null);});
	loadPopup();
}

function jAccordionEndRequestHandler(sender, args) {
	// raise event to page async end request handler if one is defined and if it evaluates to false, stop processing     	
	if (typeof (pageEndRequestHandler) != 'undefined') {
		if (!pageEndRequestHandler(sender, args)) {
			return false;
		}
	}
    
	$("#accordion :input").on("click", function (event) {	    
	    jAccordionInputChanged = true;
	    $('.actionSuccess').hide();
	});

	if (!jAccordionKeepInputChanged) jAccordionInputChanged = false;
	else jAccordionKeepInputChanged = false;
	eval(args.get_dataItems()["EndRequestScript"]);
	
	$(".pnpWrapper a:not(#popup a):not(.deleteLink)").unbind("click", jAccordionConfirmSave).bind("click", jAccordionConfirmSave);
	
	enableTitles();
	
	if (uiMethods != null) uiMethods();
}

function jAccordionConfirmSave(e) {
	clearInputsBeforeSubmit();
	if (jAccordionInputChanged && !($(e.target).hasClass("UpdateButton") || $(e.target).hasClass("CancelButton") || $(e.target).hasClass("globalContentModal") || $(e.target).hasClass("excludeClick"))) {
		var target = $(e.target).is("img")? $(e.target).closest("a") : $(e.target);
		e.preventDefault();
		jAccordionToDoRequest = target.attr("href");
		centerPopup();
		setUpPopup(saveChangesText, jAccordionYesResponse, jAccordionNoResponse);
		loadPopup();
	}
	
	if ($(e.target).hasClass("excludeClick")) jAccordionKeepInputChanged = true;
}

function jAccordionYesResponse() {
	jAccordionSaveChanges = true;
	disablePopup(function() { eval(jAccordionToDoRequest); });
}

function jAccordionNoResponse() {
	disablePopup(function() { eval(jAccordionToDoRequest); });
}

function jAccordionBeginRequestHandler(sender, args) {
	args.get_request().set_url(args.get_request().get_url().replace("?doSave=true", ""));
	if (jAccordionSaveChanges)
		args.get_request().set_url(args.get_request().get_url() + (args.get_request().get_url().indexOf("?") > -1? "&" : "?") + "doSave=true");

	jAccordionSaveChanges = false;
	jAccordionToDoRequest = "";
}
var popupStatus = 0;
function loadPopup() {
	//loads popup only if it is disabled  
	if (popupStatus == 0) {
		$("#backgroundPopup").css({
			"opacity": "0.5"
		});
		$("#backgroundPopup").fadeIn();
		$("#popup").fadeIn("slow");
		popupStatus = 1;
		$('#aPopupYes').focus();
	}
	
}
function disablePopup(afterFunc) {
	//disables popup only if it is enabled
	if (popupStatus == 1) {
		$("#backgroundPopup").fadeOut();
		$("#popup").fadeOut("fast", afterFunc);
		popupStatus = 0;
	}
}
function centerPopup() {
	//request data for centering  
	var windowWidth = document.documentElement.clientWidth;
	var windowHeight = document.documentElement.clientHeight;
	var popupHeight = $("#popupContact").height();
	var popupWidth = $("#popupContact").width();


	//only need force for IE6  

	$("#backgroundPopup").css({
		"height": windowHeight
	});
}

function alertState() {alert('KeepChanges: ' + jAccordionKeepInputChanged + ', Changed: ' + jAccordionInputChanged);}