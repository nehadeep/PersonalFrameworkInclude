<%
	'DESCRIPTION:
	'	Contains all functionality related to Captcha.
	'ASSUMPTIONS:
	'	/global/subs/Global.inc

Const EnableCaptchaOnEppSetupAddEmail = "8C3AE702848E459E8639DC1FC99A25C0"	' Enable Captcha - EAccount/ComposeEmail/SendEmployee.aspx

Const ReCaptchaPrivateKey_C = "6LcqLNYSAAAAALXXutslOh_ucev11u-0TSa0O8UH"
Const ReCaptchaPrivateKey_G = "6LcrLNYSAAAAAHnSRPwqSJJ9Yev33fT8x4tKYT9E"
Const ReCaptchaPrivateKey_CA = "6LcsLNYSAAAAABh-UT7rveNoj0zYrlYYuHqualBF"

Const ReCaptchaPublicKey_C = "6LcqLNYSAAAAAPch75LRpzD46QvtWJJ_dz9z0C1q"
Const ReCaptchaPublicKey_G = "6LcrLNYSAAAAAC5pdQlU1ACxzBPdXxRaWWfMI7IP"
Const ReCaptchaPublicKey_CA = "6LcsLNYSAAAAACi3MOaRP_UXYsG93QQDNWTkSi6B"

Sub RenderReCaptchaMarkup()
	Call Response.Write("<script type='text/javascript'>")
	Call Response.Write("var RecaptchaOptions = { theme : 'white' };")
	Call Response.Write("</script>")
	Call Response.Write("")
	Call Response.Write("<script type='text/javascript' src='" & GetChallengeUrl() & "'></script>")
	Call Response.Write("<noscript>")
	Call Response.Write("")
	Call Response.Write("<iframe src='" & GetNoScriptChallengeUrl() & "' height='300' width='500' frameborder='0'></iframe><br />")
	Call Response.Write("<textarea name='recaptcha_challenge_field' rows='3' cols='40'></textarea>")
	Call Response.Write("<input type='hidden' name='recaptcha_response_field' value='manual_challenge' />")
	Call Response.Write("</noscript>")
End Sub


Function IsCaptchaEnabledOnPage(configSettingKeyForPage)
	Dim configRecords
	Dim returnValue

	returnValue = False
	configRecords = GetConfigurationSettings()
	
	For counter = Lbound(configRecords, 2) To Ubound(configRecords, 2)
		If configRecords(1, counter) = configSettingKeyForPage And configRecords(2, counter) = "True" Then
			returnValue = True
			Exit For
		End If
	Next
	
	IsCaptchaEnabledOnPage = returnValue
End Function


Function GetConfigurationSettings()
	Dim settingRecords
	Dim resultSet
	Dim Cmd
	
	settingRecords = Null

	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "GlobalDB.dbo.Configuration_ListSettings")
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	
	If (resultSet.State = adstateOpen) Then
		If (Not(resultSet.EOF)) Then
			settingRecords = resultSet.GetRows
		End If
		Call resultSet.Close()
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)

	GetConfigurationSettings = settingRecords
End Function


Function GetChallengeUrl()
	GetChallengeUrl = "https://www.google.com/recaptcha/api/challenge?k=" & GetReCaptchaPublicKey()
End Function


Function GetNoScriptChallengeUrl()
	GetNoScriptChallengeUrl = "https://www.google.com/recaptcha/api/noscript?k=" & GetReCaptchaPublicKey()
End Function

Dim CaptchaValidationStatus : CaptchaValidationStatus = False
Dim CaptchaValidationMessage : CaptchaValidationMessage = ""


Function GetFriendlyReCaptchaErrorMessage()
	If LCase(CaptchaValidationMessage) = "incorrect-captcha-sol" Then
		GetFriendlyReCaptchaErrorMessage = "Invalid Validation Code"
	Else
		GetFriendlyReCaptchaErrorMessage = "An Error was Encountered While Validating Captcha Response"
	End If
End Function


Function IsUserInputValid(challenge, userResponse)
	Dim ipAddress
	ipAddress = GetClientServerVariable(glGetRemoteHost)
	Call EvaluateReCaptchaResponse(ipAddress, challenge, userResponse)
	
	If CaptchaValidationStatus <> True Then
		ipAddress = GetClientServerVariable(glGetRemoteAddr)
		Call EvaluateReCaptchaResponse(ipAddress, challenge, userResponse)
	End If
	
	IsUserInputValid = CaptchaValidationStatus
End Function


Sub SetReCaptchaOutcome(wsResponseText)
	wsResponseText = Replace(wsResponseText, "[", "")
	wsResponseText = Replace(wsResponseText, "]", "")
	wsResponseText = Replace(wsResponseText, """", "")
	
	Dim responseArray
	responseArray = Split(wsResponseText, ",")
	
	CaptchaValidationStatus = CBool(responseArray(0))
	CaptchaValidationMessage = responseArray(1)
End Sub


Sub EvaluateReCaptchaResponse(ipAddress, challenge, userResponse)
	Dim requestBody
	requestBody = "PrivateKey=" & GetReCaptchaPrivateKey() & "&IpAddress=" & ipAddress & "&Challenge=" & challenge & "&Response=" & userResponse
	Set http = Server.CreateObject("MSXML2.ServerXMLHTTP")
	Call http.open("POST", glCaptchaValidationServiceUrl, false)
	Call http.setRequestHeader("Content-type", "application/x-www-form-urlencoded")
	Call http.Send(requestBody)
	SetReCaptchaOutcome(http.responseText)
End Sub


Function GetReCaptchaPrivateKey()
	Dim returnValue
	returnValue = ReCaptchaPrivateKey_C
	
	Select Case GetThisSite()
		Case www_cdwg_com
			returnValue = ReCaptchaPrivateKey_G
		Case www_cdw_ca
			returnValue = ReCaptchaPrivateKey_CA
	End Select
	
	GetReCaptchaPrivateKey = returnValue
End Function


Function GetReCaptchaPublicKey()
	Dim returnValue

	returnValue = ReCaptchaPublicKey_C
	
	Select Case GetThisSite()
		Case www_cdwg_com
			returnValue = ReCaptchaPublicKey_G
		Case www_cdw_ca
			returnValue = ReCaptchaPublicKey_CA
	End Select
	
	GetReCaptchaPublicKey = returnValue
End Function
%>