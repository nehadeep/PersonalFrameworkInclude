var cacheJSObjects = {};
var errorMessageCollection = '';

function LoadCache() {
    cacheJSObjects.txtUserName = $("#txtUserName");
    cacheJSObjects.txtUserContactEmailAddress = $("#txtUserContactEmailAddress");
    cacheJSObjects.txtUserContactConfirmEmailAddress = $("#txtUserContactConfirmEmailAddress");
    cacheJSObjects.ddlCategories = $("#ddlCategories");
    cacheJSObjects.txtvNewUserPassword = $("#txtvNewUserPassword");
    cacheJSObjects.txtNewUserPassword = $("#txtNewUserPassword");

    cacheJSObjects.txtvNewUserPasswordLabel = $("#txtvNewUserPasswordLabel");
    cacheJSObjects.txtNewUserPasswordLabel = $("#txtNewUserPasswordLabel");

    cacheJSObjects.divCaptcha = $("#divCaptcha");

    cacheJSObjects.divReenterPasswordErrorMessage = $("#divReenterPasswordErrorMessage");
    cacheJSObjects.divEmailErrorMessage = $("#divEmailErrorMessage");
    cacheJSObjects.divEmailReEnterErrorMessage = $("#divEmailReEnterErrorMessage");
    cacheJSObjects.divUserNameErrorMessage = $("#divUserNameErrorMessage");
    cacheJSObjects.ddlCategoriesErrorMessage = $("#ddlCategoriesErrorMessage");

    cacheJSObjects.divPwdBubble = $("#divPwdBubble");
}

function displayRedirectMessage() {
    var selValue = $("#ddlCategories option:selected").val();
    var $redirMessageDivG = $("#divRedirectMsgG");
    var $redirMessageDivC = $("#divRedirectMsgC");
    $redirMessageDivG.hide();
    $redirMessageDivC.hide();
    if ($('#hidIsCorpSite').val() == 'True' && (selValue == '8372B33175A04E43967EFD073D821CC6' || selValue == 'ECEB7BAFB5914799A5F1282492B5A3B6' || selValue == '0715B00C9B2845758B88B32F51CF2344' || selValue == 'FDB6E9835F13434B9582BB328F9CE242' || selValue == 'ABF4B60A754B411881EECBFCF806BABE')) {
        $redirMessageDivG.show();
    }
    if ($('#hidIsGovSite').val() == 'True' && (selValue == '72E970C55F474DFEB4AA54AB8CD5DBF6' || selValue == '020CA28285C84668AD03EBBA66B39D77' || selValue == '3226FF13CE524457BF10674CCEB4017B' || selValue == '7043525B990D4520841612BB213F455A' || selValue == '1B238F32F66F4DA6B6A8DE49B508CCB9' || selValue == 'C7B1AB155F88461B82F31190D570EE97')) {

        $redirMessageDivC.show();
    }
}
function SetValidationEvents() {
    // Redirect Message;   
    cacheJSObjects.ddlCategories.change(function () {
        validateUserType();
        displayRedirectMessage();
    });
    cacheJSObjects.txtUserContactConfirmEmailAddress.blur(function () {
        validateReenterEmail();
    });
    cacheJSObjects.txtvNewUserPassword.blur(function () {
        if ($(this).val().length == 0) {
            cacheJSObjects.txtvNewUserPasswordLabel.show();
            $(this).hide();
        }
        validateReenterPwd();
    });

    cacheJSObjects.txtUserName.blur(function () {
        validateUserName();
    });

    // we need to validate reenter fields when original field is changed /  on leaving the field (blur)
    cacheJSObjects.txtUserContactEmailAddress.blur(function () {
        validateEmail();
    });

    setCaptcha();
    //ValidateOnSubmit();

    // set watermark labels    
    cacheJSObjects.txtNewUserPasswordLabel.focus(function () {
        cacheJSObjects.txtNewUserPasswordLabel.hide();
        cacheJSObjects.txtNewUserPassword.show();
        cacheJSObjects.txtNewUserPassword.focus();
        cacheJSObjects.divPwdBubble.show();
    });
    cacheJSObjects.txtNewUserPassword.focus(function () {
        cacheJSObjects.divPwdBubble.show();
    });
    cacheJSObjects.txtvNewUserPasswordLabel.focus(function () {
        cacheJSObjects.txtvNewUserPassword.show();
        cacheJSObjects.txtvNewUserPassword.focus();
        $(this).hide();
    });
}

function setInitialPlaceholder() {
    //logon section of registration screen  
    $('#UserPasswordLabel').focus(function () {
        $('#UserPassword').show();
        $('#UserPassword').focus();
        $(this).hide();
    });
    $("#UserPassword").blur(function () {
        if ($(this).val().length == 0) {
            $("#UserPasswordLabel").show();
            $(this).hide();
        }
    });
}

function ValidateOnSubmit() {

    $("#LogonFormServer").submit(function () {
        if (createAccountHasInValidValuesOnSubmit()) {
            placeholdersForIE8onSubmit();
            return false;
        } else {
            return true;
        }
    });
}
function placeholdersForIE8onSubmit() {
    if (cacheJSObjects.txtNewUserPassword.val().length == 0) {
        cacheJSObjects.txtNewUserPasswordLabel.show();
        cacheJSObjects.txtNewUserPassword.hide();
    }
    if (cacheJSObjects.txtvNewUserPassword.val().length == 0) {
        cacheJSObjects.txtvNewUserPasswordLabel.show();
        cacheJSObjects.txtvNewUserPassword.hide();
    }
}
function validateReenterEmail() {
    var emailValue = cacheJSObjects.txtUserContactEmailAddress.val();
    // if Email is either empty or does not have an error. do not validate re-enter email
    if (cacheJSObjects.txtUserContactEmailAddress.hasClass("error") || emailValue == "" || emailValue == cacheJSObjects.txtUserContactEmailAddress.attr('placeholder')) {
        cacheJSObjects.txtUserContactConfirmEmailAddress.removeClass("error");
        cacheJSObjects.divEmailReEnterErrorMessage.hide();
    }
    else if (emailValue != cacheJSObjects.txtUserContactConfirmEmailAddress.val()) {
        cacheJSObjects.txtUserContactConfirmEmailAddress.addClass("error");
        cacheJSObjects.divEmailReEnterErrorMessage.show();
    } else {
        cacheJSObjects.txtUserContactConfirmEmailAddress.removeClass("error");
        cacheJSObjects.divEmailReEnterErrorMessage.hide();
    }
}

function validateReenterPwd() {
    // if password is either empty or does not have an error. do not validate re-enter password
    if ((cacheJSObjects.txtNewUserPassword.val() != "" && cacheJSObjects.txtNewUserPassword.hasClass("passwordErrorCreateAccount")) || cacheJSObjects.txtNewUserPassword.val() == "") {
        cacheJSObjects.divReenterPasswordErrorMessage.hide();
    }
    else if (cacheJSObjects.txtvNewUserPassword.val() != cacheJSObjects.txtNewUserPassword.val()) {
        cacheJSObjects.txtvNewUserPassword.addClass("error");
        cacheJSObjects.txtvNewUserPasswordLabel.addClass("error");
        cacheJSObjects.divReenterPasswordErrorMessage.show();
    } else {
        cacheJSObjects.txtvNewUserPassword.removeClass("error");
        cacheJSObjects.txtvNewUserPasswordLabel.removeClass("error");
        cacheJSObjects.divReenterPasswordErrorMessage.hide();
    }
}

function validateEmail() {
    var emailValue = cacheJSObjects.txtUserContactEmailAddress.val();

    var emailReg = /^(([^<>()[\]\\.,;:\s@\"]+(\.[^<>()[\]\\.,;:\s@\"]+)*)|(\".+\"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/igm;
    if (emailValue == "" || emailValue == cacheJSObjects.txtUserContactEmailAddress.attr('placeholder')) {
        cacheJSObjects.txtUserContactEmailAddress.removeClass("error");
        cacheJSObjects.divEmailErrorMessage.hide();
        return;
    } else if (emailValue.match(emailReg)) {
        cacheJSObjects.txtUserContactEmailAddress.removeClass("error");
        cacheJSObjects.divEmailErrorMessage.hide();
        if (emailValue != "" && cacheJSObjects.txtUserContactConfirmEmailAddress.val() != "" && cacheJSObjects.txtUserContactConfirmEmailAddress.val() != cacheJSObjects.txtUserContactConfirmEmailAddress.attr("placeholder") && emailValue != cacheJSObjects.txtUserContactConfirmEmailAddress.val()) {
            cacheJSObjects.divEmailReEnterErrorMessage.show();
            errorMessageCollection += cacheJSObjects.divEmailReEnterErrorMessage.text().trim() + '|';
        } else {
            cacheJSObjects.divEmailReEnterErrorMessage.hide();
        }
    } else {
        cacheJSObjects.txtUserContactEmailAddress.addClass("error");
        cacheJSObjects.divEmailErrorMessage.show();
        errorMessageCollection += cacheJSObjects.divEmailErrorMessage.text().trim() + '|';
    }
}

function validateUserType() {
    if (cacheJSObjects.ddlCategories.prop("selectedIndex") > 0) {
        cacheJSObjects.ddlCategories.removeClass("error");
        cacheJSObjects.ddlCategoriesErrorMessage.hide();
    } else {
        cacheJSObjects.ddlCategories.addClass("error");
        cacheJSObjects.ddlCategoriesErrorMessage.show();
    }
}

function validateUserName() {
    var userNameValue = cacheJSObjects.txtUserName.val();
    if (userNameValue == "" || userNameValue == cacheJSObjects.txtUserName.attr('placeholder')) {
        cacheJSObjects.txtUserName.removeClass("error");
        cacheJSObjects.divUserNameErrorMessage.hide();
        return;
    }
    if ((userNameValue.length >= 6 && userNameValue.length <= 50) && $.trim(userNameValue).indexOf(' ') < 0) {
        cacheJSObjects.txtUserName.removeClass("error");
        cacheJSObjects.divUserNameErrorMessage.hide();
    } else {
        cacheJSObjects.txtUserName.addClass("error");
        cacheJSObjects.divUserNameErrorMessage.show();
        errorMessageCollection += cacheJSObjects.divUserNameErrorMessage.text().trim() + '|';
    }
}

function validateVisualCaptcha() {
    var $vcapIndex = $("#hidVisualCaptchaSelectedXAxis");

    if ($vcapIndex.size() === 0) {
        return false;
    }

    if ($vcapIndex.val() === '') {
        $("#VisualCaptchaContainer").addClass("error");
        return true;
    }

    return false;
}

function setCaptcha() {
    $("#captchaTooltip").hide(); // hide initialy
    cacheJSObjects.divCaptcha.focusin(function () {
        $("#captchaTooltip").show();
    });
    cacheJSObjects.divCaptcha.focusout(function () {
        $("#captchaTooltip").hide();
    });

    /*$("#txtNewUserPassword , #txtUserName, #txtvNewUserPassword,#txtUserContactEmailAddress , #txtUserContactConfirmEmailAddress ").blur(function () {*/
    $("#txtvNewUserPassword").blur(function () {
        if (!createAccountHasInValidValues()) {
            if (!cacheJSObjects.divCaptcha.is(":visible")) {
                cacheJSObjects.divCaptcha.show();
                $("#recaptcha_response_field").focus();
            }
        }
    });
}

function createAccountHasInValidValues() {
    if (cacheJSObjects.ddlCategories.prop("selectedIndex") == 0) {
        return true;
    }
    var hasError = false;
    var elToTest = ["#txtNewUserPassword", "#txtUserName", "#txtvNewUserPassword", "#txtUserContactEmailAddress", "#txtUserContactConfirmEmailAddress"];

    for (var i = 0, l = elToTest.length; i < l; i++) {
        if (hasError == true) {
            break;
        }
        if ($(elToTest[i]).val() == "" || $(elToTest[i]).hasClass("error") || $(elToTest[i]).parents("div").hasClass("error") || $(elToTest[i]).hasClass("passwordErrorCreateAccount")) {
            hasError = true;
        }
        else {

            if (Page_IsValid) {
                hasError = false;
            } else {
                hasError = true;
            }
        }
    }
    return hasError;
}

function createAccountHasInValidValuesOnSubmit() {
    var hasError = false;
    errorMessageCollection = '';
    if (cacheJSObjects.ddlCategories.prop("selectedIndex") == 0) {
        cacheJSObjects.ddlCategories.addClass("error");
        cacheJSObjects.ddlCategoriesErrorMessage.show();
        hasError = true;
        errorMessageCollection += cacheJSObjects.ddlCategoriesErrorMessage.text().trim() + '|';
    }
    validateEmail();
    if (cacheJSObjects.txtUserContactEmailAddress.val() == "") {
        cacheJSObjects.txtUserContactEmailAddress.addClass("error");
        cacheJSObjects.divEmailErrorMessage.show();
        hasError = true;
        errorMessageCollection += cacheJSObjects.divEmailErrorMessage.text().trim() + '|';
    }
    if (cacheJSObjects.txtUserContactConfirmEmailAddress.val() == "") {
        cacheJSObjects.txtUserContactConfirmEmailAddress.addClass("error");
        cacheJSObjects.divEmailReEnterErrorMessage.show();
        hasError = true;
        errorMessageCollection += cacheJSObjects.divEmailReEnterErrorMessage.text().trim() + '|';
    }
    validateUserName();
    if (cacheJSObjects.txtUserName.val() == "") {
        cacheJSObjects.txtUserName.addClass("error");
        cacheJSObjects.divUserNameErrorMessage.show();
        hasError = true;
        errorMessageCollection += cacheJSObjects.divUserNameErrorMessage.text().trim() + '|';
    }

    if (cacheJSObjects.txtNewUserPassword.val() == "") {
        cacheJSObjects.txtNewUserPasswordLabel.addClass("error");
        cacheJSObjects.divPwdBubble.show();
        hasError = true;
    }

    if (cacheJSObjects.txtvNewUserPassword.val() == "") {
        cacheJSObjects.txtvNewUserPasswordLabel.addClass("error");
        cacheJSObjects.divReenterPasswordErrorMessage.show();
        hasError = true;
    }

    if ((cacheJSObjects.txtUserContactEmailAddress.val() != "" && cacheJSObjects.txtUserContactConfirmEmailAddress.val() == "") || cacheJSObjects.txtUserContactEmailAddress.val() != cacheJSObjects.txtUserContactConfirmEmailAddress.val()) {
        cacheJSObjects.txtUserContactConfirmEmailAddress.addClass("error");
        cacheJSObjects.divEmailReEnterErrorMessage.show();
        hasError = true;
    }

    if ((!cacheJSObjects.txtNewUserPassword.hasClass("passwordErrorCreateAccount") && cacheJSObjects.txtvNewUserPassword.val() == "") || cacheJSObjects.txtNewUserPassword.val() != cacheJSObjects.txtvNewUserPassword.val()) {
        cacheJSObjects.txtvNewUserPassword.addClass("error");
        cacheJSObjects.divReenterPasswordErrorMessage.show();
        hasError = true;
    }
    if (cacheJSObjects.txtNewUserPassword.val() !== cacheJSObjects.txtvNewUserPassword.val()) {
        errorMessageCollection += cacheJSObjects.divReenterPasswordErrorMessage.text().trim() + '|';
        cacheJSObjects.divReenterPasswordErrorMessage.show()
    }

    if (hasError == false && $('#hidCaptchaEnabled').val() == 'True') {

        if (!cacheJSObjects.divCaptcha.is(":visible") && $("#VisualCaptchaContainer").size() === 0) {
            cacheJSObjects.divCaptcha.show();
            hasError = true;
            errorMessageCollection += cacheJSObjects.divCaptcha.text().trim() + '|';
        } else {
            hasError = validateVisualCaptcha();
        }
    }
    errorMessageCollection = errorMessageCollection.substring(0, errorMessageCollection.length - 1);
    TagManValidationErrorEventWriter(errorMessageCollection);
    return hasError;
}


function setPwdValidationsetup() {

    setupPwdControl({
        "PwdId": "txtNewUserPassword",
        "UserNameId": "txtUserName",
        "ErrorClass": "error",
        "SpaceMsg": "pwdCritSpc",
        "CharMsg": "pwdCritChar",
        "LenMsg": "pwdCritLen",
        "NameMsg": "pwdCritName",
        "TextErrorClass": "passwordErrorCreateAccount",
        "ExecuteOnSubmit": true,
        "HasError": false
    });
}

function displayPWdError() {
    cacheJSObjects.divPwdBubble.hide();
    cacheJSObjects.txtNewUserPassword.blur(function () {
        if ($(this).hasClass("passwordErrorCreateAccount")) {
            cacheJSObjects.txtvNewUserPassword.removeClass("error");
            cacheJSObjects.txtvNewUserPasswordLabel.removeClass("error");
            cacheJSObjects.divPwdBubble.show();
        } else {
            cacheJSObjects.divPwdBubble.hide();
        }
        // if blank, placeholder    
        if ($(this).val().length == 0) {
            cacheJSObjects.txtNewUserPasswordLabel.show();
            $(this).hide();
        }
    });
}

//DOCUMENT READY
$(function () {
    //Set cursor on page load
    $("#LogonForm #UserName").focus();

    $('#LogonFormServer :input:not(#txtNewUserPassword,#txtUserName,#txtvNewUserPassword,#txtNewUserPasswordLabel,#txtvNewUserPasswordLabel,#recaptcha_response_field)').tooltip({
        // place tooltip on the right edge
        relative: true,
        position: "top right",

        // a little tweaking of the position
        offset: [40, 0],

        // custom opacity setting
        opacity: 1,

        events: {
            input: "focus,blur",
            widget: "focus,blur"
        }

    });
    $('#LogonFormServer :input:#txtUserName').tooltip({
        // place tooltip on the right edge
        relative: true,
        position: "top right",

        // a little tweaking of the position
        offset: [30, 0],

        // custom opacity setting
        opacity: 1,

        events: {
            input: "focus,blur",
            widget: "focus,blur"
        }

    });
});
// password
function setupPwdControl(pwdControl) {
    $('#' + pwdControl.PwdId).blur(function () {
        ValidatePWDField(pwdControl);
        // reenter pwd must be validated after validating the password / changed to match with reenter password field.
        if (cacheJSObjects.txtvNewUserPassword.val() != "") {
            validateReenterPwd();
        }
    });
    var pwdElem = '#' + pwdControl.PwdId;
    var frm = $(pwdElem).closest("form");
    if (pwdControl.ExecuteOnSubmit) {

        $(frm).submit(function () {
            var submitHasError = false;
            if ($(pwdElem).val().length == 0) {
                // pwd field cannot be empty on submit
                $(pwdElem).addClass(pwdControl.TextErrorClass);
                $('#' + pwdControl.LenMsg).addClass(pwdControl.ErrorClass);
                submitHasError = true; //  password has error then return false to cancel the submit action
            }

            if (createAccountHasInValidValuesOnSubmit()) {
                submitHasError = true;
            }
            if (submitHasError == true) {
                placeholdersForIE8onSubmit();
                return false;
            }
            // If Pwd is not empty on submit validate for pwd requirements
            ValidatePWDField(pwdControl);

            if (pwdControl.HasError == true) {
                placeholdersForIE8onSubmit();
                return false; // if password has error then return false to cancel the submit action
            } else {
                return true;
            }
        });
    }
}

function ValidatePWDField(pwdControl) {
    var $thisElement = $('#' + pwdControl.PwdId);
    var pwdVal = $.trim($thisElement.val());
    var pwdLower = /^(?=.*[a-z])/g;
    var pwdUpper = /^(?=.*[A-Z])/g;
    var pwdNum = /^(?=.*\d)/g;
    var pwdNoSpc = /^[\S]*$/g;
    var spaceMsg = $('#' + pwdControl.SpaceMsg);
    var charMsg = $('#' + pwdControl.CharMsg);
    var lenMsg = $('#' + pwdControl.LenMsg);
    var nameMsg = $('#' + pwdControl.NameMsg);
    var usrName = $('#' + pwdControl.UserNameId);
    var displayError = false;
    pwdControl.HasError = false;
    if ($thisElement.val().length > 0) {
        if (!pwdNoSpc.test(pwdVal)) {
            spaceMsg.addClass(pwdControl.ErrorClass);
            displayError = true;
        } else spaceMsg.removeClass(pwdControl.ErrorClass);

        if (!pwdLower.test(pwdVal) || !pwdUpper.test(pwdVal) || !pwdNum.test(pwdVal)) {
            charMsg.addClass(pwdControl.ErrorClass);
            displayError = true;
        } else charMsg.removeClass(pwdControl.ErrorClass);

        if (pwdVal.length < 8 || pwdVal.length > 20) {
            lenMsg.addClass(pwdControl.ErrorClass);
            displayError = true;
        } else lenMsg.removeClass(pwdControl.ErrorClass);

        if (usrName.length > 0) {
            if ((usrName.val().length > 0 && usrName.val().toLowerCase() == pwdVal.toLowerCase())
                || (usrName.text().length > 0 && usrName.text().toLowerCase() == pwdVal.toLowerCase())) {
                nameMsg.addClass(pwdControl.ErrorClass);
                displayError = true;
            } else nameMsg.removeClass(pwdControl.ErrorClass);
        }

        if (displayError) {
            $thisElement.addClass(pwdControl.TextErrorClass);
            $thisElement.addClass(pwdControl.ErrorClass);
            pwdControl.HasError = true;
        } else {
            $thisElement.removeClass(pwdControl.TextErrorClass);
            $thisElement.removeClass(pwdControl.ErrorClass);
            pwdControl.HasError = false;
        }

    } else {
        spaceMsg.removeClass(pwdControl.ErrorClass);
        charMsg.removeClass(pwdControl.ErrorClass);
        lenMsg.removeClass(pwdControl.ErrorClass);
        nameMsg.removeClass(pwdControl.ErrorClass);
        $thisElement.removeClass(pwdControl.TextErrorClass);
    }
}