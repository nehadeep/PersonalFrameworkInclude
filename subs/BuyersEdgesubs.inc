<% 
'******************************************************************************************************************************
'	DESCRIPTION:	Functions for the Buyer' Edge application
'	ASSUMPTIONS:	/global/subs/sqlconn.inc

'***************************************************************
'	DESCRIPTION:
'		Adds a Channel.
'		Returns ChannelID/Null

Function BuyersEdge_AddChannel(ChannelKey,Name,Description)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(ChannelKey) and Not IsNull(ChannelKey) and Len(ChannelKey) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_AddChannel")
		Cmd.Parameters.Append Cmd.CreateParameter("@ChannelID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@ChannelKey", adChar, adParamInput, 32, ChannelKey)
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 50, Name)
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 255, Description)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(1).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_AddChannel = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds a Channel.
'		Returns ChannelID/Null

Function BuyersEdge_UpdateChannel(ChannelID,Name,Description)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(ChannelID) and Not IsNull(ChannelID) and IsNumeric(ChannelID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_UpdateChannel")
		Cmd.Parameters.Append Cmd.CreateParameter("@ChannelID", adInteger, adParamInput, , ChannelID)
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 50, Name)
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 255, Description)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(1).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_UpdateChannel = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Removes a Channel.
'		Returns true/false

Function BuyersEdge_RemoveChannel(ChannelID)
	Dim returnValue, Cmd
	returnValue = false
	If Not IsEmpty(ChannelID) and Not IsNull(ChannelID) and IsNumeric(ChannelID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_RemoveChannel")
		Cmd.Parameters.Append Cmd.CreateParameter("@ChannelID", adInteger, adParamInput, , ChannelID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = true
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_RemoveChannel = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Restores a Channel.
'		Returns true/false

Function BuyersEdge_RestoreChannel(ChannelID)
	Dim returnValue, Cmd
	returnValue = false
	If Not IsEmpty(ChannelID) and Not IsNull(ChannelID) and IsNumeric(ChannelID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_RestoreChannel")
		Cmd.Parameters.Append Cmd.CreateParameter("@ChannelID", adInteger, adParamInput, , ChannelID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = true
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_RestoreChannel = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get all the Active Channels
'		Array/Null
'		ach = Active Channels

const achChannelID			= 0
const achChannelKey			= 1
const achName				= 2
const achDescription		= 3
const achDateCreated		= 4
const achDateModified		= 5
const achDateRemoved		= 6

Function BuyersEdge_GetActiveChannels(ViewActive)
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetActiveChannels")
	Cmd.Parameters.Append Cmd.CreateParameter("@ViewActive", adInteger, adParamInput, , ViewActive)
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	BuyersEdge_GetActiveChannels = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a Channel
'		Array/Null
'		ch = Channel

Const chChannelID			= 0
Const chChannelKey			= 1
Const chName				= 2
Const chDescription			= 3
Const chDateCreated			= 4
Const chDateModified		= 5
Const chDateRemoved			= 6

Function BuyersEdge_GetChannel(ChannelID)
	Dim returnValue
	returnValue = Null
	If (Not IsEmpty(ChannelID)) and (Not IsNull(ChannelID)) and IsNumeric(ChannelID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetChannel")
		Cmd.Parameters.Append Cmd.CreateParameter("@ChannelID", adInteger, adParamInput, , ChannelID)
		Cmd.Parameters.Append Cmd.CreateParameter("@ChannelKey", adChar, adParamOutput, 32)
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 50)
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 255)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateRemoved", adDBTimeStamp, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(6)
			ary(chChannelID)				= ChannelID
			ary(chChannelKey)				= Cmd.Parameters(2).Value
			ary(chName)						= Cmd.Parameters(3).Value
			ary(chDescription)				= Cmd.Parameters(4).Value
			ary(chDateCreated)				= Cmd.Parameters(5).Value
			ary(chDateModified)				= Cmd.Parameters(6).Value
			ary(chDateRemoved)				= Cmd.Parameters(7).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetChannel = returnValue
End Function

Function BuyersEdge_GetChannelByChannelKey(ChannelKey)
	Dim returnValue
	returnValue = Null
	If (Not IsEmpty(ChannelKey)) and (Not IsNull(ChannelKey)) and Len(Trim(ChannelKey)) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetChannelByChannelKey")
		Cmd.Parameters.Append Cmd.CreateParameter("@ChannelID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@ChannelKey", adChar, adParamInput, 32, ChannelKey)
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 50)
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 255)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateRemoved", adDBTimeStamp, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(6)
			ary(chChannelID)				= Cmd.Parameters(1).Value
			ary(chChannelKey)				= ChannelKey
			ary(chName)						= Cmd.Parameters(3).Value
			ary(chDescription)				= Cmd.Parameters(4).Value
			ary(chDateCreated)				= Cmd.Parameters(5).Value
			ary(chDateModified)				= Cmd.Parameters(6).Value
			ary(chDateRemoved)				= Cmd.Parameters(7).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetChannelByChannelKey = returnValue
End Function








'***************************************************************
'	DESCRIPTION:
'		Adds a Newsletter.
'		Returns NewsletterID/Null

Function BuyersEdge_AddNewsletter(NewsletterKey,ChannelID,FromName,FromEmailAddress,Subject,BodyFormat)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(NewsletterKey) and Not IsNull(NewsletterKey) and Len(NewsletterKey) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_AddNewsletter")
		Cmd.Parameters.Append Cmd.CreateParameter("@NewsletterID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@NewsletterKey", adChar, adParamInput, 32, NewsletterKey)
		Cmd.Parameters.Append Cmd.CreateParameter("@ChannelID", adInteger, adParamInput, , ChannelID)
		Cmd.Parameters.Append Cmd.CreateParameter("@FromName", adVarChar, adParamInput, 50, FromName)
		Cmd.Parameters.Append Cmd.CreateParameter("@FromEmailAddress", adVarChar, adParamInput, 50, FromEmailAddress)
		Cmd.Parameters.Append Cmd.CreateParameter("@Subject", adVarChar, adParamInput, 50, Subject)
		Cmd.Parameters.Append Cmd.CreateParameter("@BodyFormat", adInteger, adParamInput, , BodyFormat)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(1).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_AddNewsletter = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds a Newsletter.
'		Returns NewsletterID/Null

Function BuyersEdge_UpdateNewsletter(NewsletterID,ChannelID,FromName,FromEmailAddress,Subject,BodyFormat)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(NewsletterID) and Not IsNull(NewsletterID) and IsNumeric(NewsletterID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_UpdateNewsletter")
		Cmd.Parameters.Append Cmd.CreateParameter("@NewsletterID", adInteger, adParamInput, , NewsletterID)
		Cmd.Parameters.Append Cmd.CreateParameter("@ChannelID", adInteger, adParamInput, , ChannelID)
		Cmd.Parameters.Append Cmd.CreateParameter("@FromName", adVarChar, adParamInput, 50, FromName)
		Cmd.Parameters.Append Cmd.CreateParameter("@FromEmailAddress", adVarChar, adParamInput, 50, FromEmailAddress)
		Cmd.Parameters.Append Cmd.CreateParameter("@Subject", adVarChar, adParamInput, 50, Subject)
		Cmd.Parameters.Append Cmd.CreateParameter("@BodyFormat", adInteger, adParamInput, , BodyFormat)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(1).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_UpdateNewsletter = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds Text Body to a Newsletter.
'		Returns NewsletterID/Null

Function BuyersEdge_UpdateNewsletterTextBody(NewsletterID,TextBody)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(NewsletterID) and Not IsNull(NewsletterID) and IsNumeric(NewsletterID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_UpdateNewsletterTextBody")
		Cmd.Parameters.Append Cmd.CreateParameter("@NewsletterID", adInteger, adParamInput, , NewsletterID)
		Cmd.Parameters.Append Cmd.CreateParameter("@TextBody", adLongVarChar, adParamInput, 1000000, TextBody)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(1).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_UpdateNewsletterTextBody = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds Html Body to a Newsletter.
'		Returns NewsletterID/Null

Function BuyersEdge_UpdateNewsletterHtmlBody(NewsletterID,HtmlBody)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(NewsletterID) and Not IsNull(NewsletterID) and IsNumeric(NewsletterID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_UpdateNewsletterHtmlBody")
		Cmd.Parameters.Append Cmd.CreateParameter("@NewsletterID", adInteger, adParamInput, , NewsletterID)
		Cmd.Parameters.Append Cmd.CreateParameter("@HtmlBody", adLongVarChar, adParamInput, 1000000, HtmlBody)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(1).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_UpdateNewsletterHtmlBody = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a Newsletter
'		Array/Null
'		nw = Newsletter

Const nwNewsletterID		= 0
Const nwNewsletterKey		= 1
Const nwChannelID			= 2
Const nwFromName			= 3
Const nwFromEmailAddress	= 4
Const nwSubject				= 5
Const nwBodyFormat			= 6
Const nwDateCreated			= 7
Const nwDateModified		= 8

Function BuyersEdge_GetNewsletter(NewsletterID)
	Dim returnValue
	returnValue = Null
	If (Not IsEmpty(NewsletterID)) and (Not IsNull(NewsletterID)) and IsNumeric(NewsletterID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetNewsletter")
		Cmd.Parameters.Append Cmd.CreateParameter("@NewsletterID", adInteger, adParamInput, , NewsletterID)
		Cmd.Parameters.Append Cmd.CreateParameter("@NewsletterKey", adChar, adParamOutput, 32)
		Cmd.Parameters.Append Cmd.CreateParameter("@ChannelID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@FromName", adVarChar, adParamOutput, 50)
		Cmd.Parameters.Append Cmd.CreateParameter("@FromEmailAddress", adVarChar, adParamOutput, 50)
		Cmd.Parameters.Append Cmd.CreateParameter("@Subject", adVarChar, adParamOutput, 50)
		Cmd.Parameters.Append Cmd.CreateParameter("@BodyFormat", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(8)
			ary(nwNewsletterID)				= NewsletterID
			ary(nwNewsletterKey)			= Cmd.Parameters(2).Value
			ary(nwChannelID)				= Cmd.Parameters(3).Value
			ary(nwFromName)					= Cmd.Parameters(4).Value
			ary(nwFromEmailAddress)			= Cmd.Parameters(5).Value
			ary(nwSubject)					= Cmd.Parameters(6).Value
			ary(nwBodyFormat)				= Cmd.Parameters(7).Value
			ary(nwDateCreated)				= Cmd.Parameters(8).Value
			ary(nwDateModified)				= Cmd.Parameters(9).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetNewsletter = returnValue
End Function

Function BuyersEdge_GetNewsletterByNewsletterKey(NewsletterKey)
	Dim returnValue
	returnValue = Null
	If (Not IsEmpty(NewsletterKey)) and (Not IsNull(NewsletterKey)) and Len(Trim(NewsletterKey)) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetNewsletterByNewsletterKey")
		Cmd.Parameters.Append Cmd.CreateParameter("@NewsletterID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@NewsletterKey", adChar, adParamInput, 32, NewsletterKey)
		Cmd.Parameters.Append Cmd.CreateParameter("@ChannelID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@FromName", adVarChar, adParamOutput, 50)
		Cmd.Parameters.Append Cmd.CreateParameter("@FromEmailAddress", adVarChar, adParamOutput, 50)
		Cmd.Parameters.Append Cmd.CreateParameter("@Subject", adVarChar, adParamOutput, 50)
		Cmd.Parameters.Append Cmd.CreateParameter("@BodyFormat", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(8)
			ary(nwNewsletterID)				= Cmd.Parameters(1).Value
			ary(nwNewsletterKey)			= NewsletterKey
			ary(nwChannelID)				= Cmd.Parameters(3).Value
			ary(nwFromName)					= Cmd.Parameters(4).Value
			ary(nwFromEmailAddress)			= Cmd.Parameters(5).Value
			ary(nwSubject)					= Cmd.Parameters(6).Value
			ary(nwBodyFormat)				= Cmd.Parameters(7).Value
			ary(nwDateCreated)				= Cmd.Parameters(8).Value
			ary(nwDateModified)				= Cmd.Parameters(9).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetNewsletterByNewsletterKey = returnValue
End Function


'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Newsletters for a Channel
'		Array/Null
'		nwc = Newletter from Channel

const nwcNewsletterID		= 0
const nwcNewsletterKey		= 1
const nwcFromName			= 2
const nwcFromEmailAddress	= 3
const nwcSubject			= 4
const nwcBodyFormat			= 5
const nwcDateCreated		= 6
const nwcDateModified		= 7

Function BuyersEdge_GetNewslettersFromChannelID(ChannelID)
	Dim returnValue, resultSet
	returnValue = Null
	If (Not IsEmpty(ChannelID)) and (Not IsNull(ChannelID)) and IsNumeric(ChannelID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetNewslettersFromChannelID")
		Cmd.Parameters.Append Cmd.CreateParameter("@ChannelID", adInteger, adParamInput, , ChannelID)
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetNewslettersFromChannelID = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Newsletter's Body
'		String/Null

Function BuyersEdge_GetNewsletterTextBody(NewsletterID)
	Dim returnValue, resultSet
	returnValue = Null
	If (Not IsEmpty(NewsletterID)) and (Not IsNull(NewsletterID)) and IsNumeric(NewsletterID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetNewsletterTextBody")
		Cmd.Parameters.Append Cmd.CreateParameter("@NewsletterID", adInteger, adParamInput, , NewsletterID)
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If resultSet.State = adstateOpen Then
			If Not resultSet.EOF Then
				Dim returnArray
				returnArray = resultSet.GetRows
				If IsArray(returnArray) Then
					returnValue = returnArray(0,0)
				End If
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetNewsletterTextBody = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Newsletter's Html Body
'		String/Null

Function BuyersEdge_GetNewsletterHtmlBody(NewsletterID)
	Dim returnValue, resultSet
	returnValue = Null
	If (Not IsEmpty(NewsletterID)) and (Not IsNull(NewsletterID)) and IsNumeric(NewsletterID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetNewsletterHtmlBody")
		Cmd.Parameters.Append Cmd.CreateParameter("@NewsletterID", adInteger, adParamInput, , NewsletterID)
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If resultSet.State = adstateOpen Then
			If Not resultSet.EOF Then
				Dim returnArray
				returnArray = resultSet.GetRows
				If IsArray(returnArray) Then
					returnValue = returnArray(0,0)
				End If
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetNewsletterHtmlBody = returnValue
End Function


Function BuyersEdge_GetNewsletterBodyFormatValue(BodyFormat)
	Dim returnValue
	If BodyFormat = 0 then
		returnValue = "TEXT"
	ElseIf BodyFormat = 1 then
		returnValue = "HTML"
	ElseIf BodyFormat = 2 then
		returnValue = "TEXT or HTML (MultiPart/Alternative)"
	End If
	BuyersEdge_GetNewsletterBodyFormatValue = returnValue
End Function






'***************************************************************
'	DESCRIPTION:
'		Adds a Mailing List.
'		Returns MailingListID/Null

Function BuyersEdge_AddMailingList(MailingListKey,Name,Description,InsertSQL,ListType)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(MailingListKey) and Not IsNull(MailingListKey) and Len(MailingListKey) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_AddMailingList")
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListKey", adChar, adParamInput, 32, MailingListKey)
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 50, Name)
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 255, Description)
		Cmd.Parameters.Append Cmd.CreateParameter("@InsertSQL", adVarChar, adParamInput, 7000, InsertSQL)
		Cmd.Parameters.Append Cmd.CreateParameter("@Type", adInteger, adParamInput, , ListType)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(1).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_AddMailingList = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a Mailing List.
'		Returns MailingListID/Null

Function BuyersEdge_UpdateMailingList(MailingListID,Name,Description,InsertSQL,ListType)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(MailingListID) and Not IsNull(MailingListID) and IsNumeric(MailingListID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_UpdateMailingList")
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListID", adInteger, adParamInput, , MailingListID)
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 50, Name)
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 255, Description)
		Cmd.Parameters.Append Cmd.CreateParameter("@InsertSQL", adVarChar, adParamInput, 7000, InsertSQL)
		Cmd.Parameters.Append Cmd.CreateParameter("@Type", adInteger, adParamInput, , ListType)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(1).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_UpdateMailingList = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Removes a Mailing List.
'		Returns true/false

Function BuyersEdge_RemoveMailingList(MailingListID)
	Dim returnValue, Cmd
	returnValue = false
	If Not IsEmpty(MailingListID) and Not IsNull(MailingListID) and IsNumeric(MailingListID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_RemoveMailingList")
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListID", adInteger, adParamInput, , MailingListID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = true
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_RemoveMailingList = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Restores a Mailing List.
'		Returns true/false

Function BuyersEdge_RestoreMailingList(MailingListID)
	Dim returnValue, Cmd
	returnValue = false
	If Not IsEmpty(MailingListID) and Not IsNull(MailingListID) and IsNumeric(MailingListID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_RestoreMailingList")
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListID", adInteger, adParamInput, , MailingListID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = true
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_RestoreMailingList = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a MailingList
'		Array/Null
'		nw = MailingList

Const mlMailingListID		= 0
Const mlMailingListKey		= 1
Const mlName				= 2
Const mlDescription			= 3
Const mlInsertSQL			= 4
Const mlType				= 5
Const mlDateCreated			= 6
Const mlDateModified		= 7
Const mlDateRemoved			= 8

Function BuyersEdge_GetMailingList(MailingListID)
	Dim returnValue
	returnValue = Null
	If (Not IsEmpty(MailingListID)) and (Not IsNull(MailingListID)) and IsNumeric(MailingListID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetMailingList")
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListID", adInteger, adParamInput, , MailingListID)
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListKey", adChar, adParamOutput, 32)
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 50)
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 255)
		Cmd.Parameters.Append Cmd.CreateParameter("@InsertSQL", adVarChar, adParamOutput, 7000)
		Cmd.Parameters.Append Cmd.CreateParameter("@Type", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateRemoved", adDBTimeStamp, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(8)
			ary(mlMailingListID)			= MailingListID
			ary(mlMailingListKey)			= Cmd.Parameters(2).Value
			ary(mlName)						= Cmd.Parameters(3).Value
			ary(mlDescription)				= Cmd.Parameters(4).Value
			ary(mlInsertSQL)				= Cmd.Parameters(5).Value
			ary(mlType)						= Cmd.Parameters(6).Value
			ary(mlDateCreated)				= Cmd.Parameters(7).Value
			ary(mlDateModified)				= Cmd.Parameters(8).Value
			ary(mlDateRemoved)				= Cmd.Parameters(9).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetMailingList = returnValue
End Function

Function BuyersEdge_GetMailingListByMailingListKey(MailingListKey)
	Dim returnValue
	returnValue = Null
	If (Not IsEmpty(MailingListKey)) and (Not IsNull(MailingListKey)) and Len(Trim(MailingListKey)) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetMailingListByMailingListKey")
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListKey", adChar, adParamInput, 32, MailingListKey)
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 50)
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 255)
		Cmd.Parameters.Append Cmd.CreateParameter("@InsertSQL", adVarChar, adParamOutput, 7000)
		Cmd.Parameters.Append Cmd.CreateParameter("@Type", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateRemoved", adDBTimeStamp, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(8)
			ary(mlMailingListID)			= Cmd.Parameters(1).Value
			ary(mlMailingListKey)			= MailingListKey
			ary(mlName)						= Cmd.Parameters(3).Value
			ary(mlDescription)				= Cmd.Parameters(4).Value
			ary(mlInsertSQL)				= Cmd.Parameters(5).Value
			ary(mlType)						= Cmd.Parameters(6).Value
			ary(mlDateCreated)				= Cmd.Parameters(7).Value
			ary(mlDateModified)				= Cmd.Parameters(8).Value
			ary(mlDateRemoved)				= Cmd.Parameters(9).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetMailingListByMailingListKey = returnValue
End Function


'******************************************************************************************************************************
'	DESCRIPTION:
'		Get all the Active Mailing Lists
'		Array/Null
'		ach = Active Mailing Lists

const amlMailingListID		= 0
const amlMailingListKey		= 1
const amlName				= 2
const amlDescription		= 3
const amlInsertSQL			= 4
const amlType				= 5
const amlDateCreated		= 6
const amlDateModified		= 7
const amlDateRemoved		= 8

Function BuyersEdge_GetActiveMailingLists(ViewActive)
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetActiveMailingLists")
	Cmd.Parameters.Append Cmd.CreateParameter("@ViewActive", adInteger, adParamInput, , ViewActive)
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	BuyersEdge_GetActiveMailingLists = returnValue
End Function


Function BuyersEdge_GetMailingListTypeValue(ListType)
	Dim returnValue
	If ListType = 0 then
		returnValue = "Manual"
	Else
		returnValue = "SQL"
	End If
	BuyersEdge_GetMailingListTypeValue = returnValue
End Function






'***************************************************************
'	DESCRIPTION:
'		Adds an Email Address.
'		Returns EmailAddressID/Null

Function BuyersEdge_AddEmailAddress(EmailAddressKey,MailingListID,EmailAddress,TempFlag)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(EmailAddressKey) and Not IsNull(EmailAddressKey) and Len(EmailAddressKey) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_AddEmailAddress")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddressID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddressKey", adChar, adParamInput, 32, EmailAddressKey)
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListID", adInteger, adParamInput, , MailingListID)
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamInput, 80, EmailAddress)
		Cmd.Parameters.Append Cmd.CreateParameter("@TempFlag", adInteger, adParamInput, , TempFlag)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(1).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_AddEmailAddress = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes an Email Address.
'		Returns true/false

Function BuyersEdge_DeleteEmailAddress(EmailAddressID)
	Dim returnValue, Cmd
	returnValue = false
	If Not IsEmpty(EmailAddressID) and Not IsNull(EmailAddressID) and IsNumeric(EmailAddressID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_DeleteEmailAddress")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddressID", adInteger, adParamInput, , EmailAddressID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = true
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_DeleteEmailAddress = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes all Email Addresses for a Mailing List.
'		Returns true/false

Function BuyersEdge_DeleteEmailAddressByMailingListID(MailingListID)
	Dim returnValue, Cmd
	returnValue = false
	If Not IsEmpty(MailingListID) and Not IsNull(MailingListID) and IsNumeric(MailingListID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_DeleteEmailAddressByMailingListID")
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListID", adInteger, adParamInput, , MailingListID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = true
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_DeleteEmailAddressByMailingListID = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns an Email Address
'		Array/Null
'		em = Email Address

Const emEmailAddressID		= 0
Const emEmailAddressKey		= 1
Const emMailingListID		= 2
Const emEmailAddress		= 3
Const emTempFlag			= 4

Function BuyersEdge_GetEmailAddress(EmailAddressID)
	Dim returnValue
	returnValue = Null
	If (Not IsEmpty(EmailAddressID)) and (Not IsNull(EmailAddressID)) and IsNumeric(EmailAddressID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetEmailAddress")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddressID", adInteger, adParamInput, , EmailAddressID)
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddressKey", adChar, adParamOutput, 32)
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@TempFlag", adInteger, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(4)
			ary(emEmailAddressID)			= EmailAddressID
			ary(emEmailAddressKey)			= Cmd.Parameters(2).Value
			ary(emMailingListID)			= Cmd.Parameters(3).Value
			ary(emEmailAddress)				= Cmd.Parameters(4).Value
			ary(emTempFlag)					= Cmd.Parameters(5).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetEmailAddress = returnValue
End Function

Function BuyersEdge_GetEmailAddressByEmailAddressKey(EmailAddressID)
	Dim returnValue
	returnValue = Null
	If (Not IsEmpty(EmailAddressKey)) and (Not IsNull(EmailAddressKey)) and Len(Trim(EmailAddressKey)) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetEmailAddressByEmailAddressKey")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddressKey", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddressKey", adChar, adParamInput, 32, EmailAddressKey)
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@TempFlag", adInteger, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(4)
			ary(emEmailAddressID)			= Cmd.Parameters(1).Value
			ary(emEmailAddressKey)			= EmailAddressKey
			ary(emMailingListID)			= Cmd.Parameters(3).Value
			ary(emEmailAddress)				= Cmd.Parameters(4).Value
			ary(emTempFlag)					= Cmd.Parameters(5).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetEmailAddressByEmailAddressKey = returnValue
End Function


'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Email Addresses for a Mailing List
'		Array/Null
'		eam = Email Addresses from Mailing List

const eamEmailAddressID		= 0
const eamEmailAddressKey	= 1
const eamMailingListID		= 2
const eamEmailAddress		= 3
const eamTempFlag			= 4

Function BuyersEdge_GetEmailAddressesFromMailingListID(MailingListID)
	Dim returnValue, resultSet
	returnValue = Null
	If (Not IsEmpty(MailingListID)) and (Not IsNull(MailingListID)) and IsNumeric(MailingListID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetEmailAddressesFromMailingListID")
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListID", adInteger, adParamInput, , MailingListID)
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetEmailAddressesFromMailingListID = returnValue
End Function








'***************************************************************
'	DESCRIPTION:
'		Adds a Schedule.
'		Returns ScheduleID/Null

Function BuyersEdge_AddSchedule(ScheduleKey,NewsletterID,MailingListID,DateScheduled)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(ScheduleKey) and Not IsNull(ScheduleKey) and Len(ScheduleKey) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_AddSchedule")
		Cmd.Parameters.Append Cmd.CreateParameter("@ScheduleID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@ScheduleKey", adChar, adParamInput, 32, ScheduleKey)
		Cmd.Parameters.Append Cmd.CreateParameter("@NewsletterID", adInteger, adParamInput, , NewsletterID)
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListID", adInteger, adParamInput, , MailingListID)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateScheduled", adDBTimeStamp, adParamInput, , DateScheduled)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(1).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_AddSchedule = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a Schedule.
'		Returns true/false

Function BuyersEdge_UpdateScheduleStatus(ScheduleID,DeliveryStatus)
	Dim returnValue, Cmd
	returnValue = false
	If Not IsEmpty(ScheduleID) and Not IsNull(ScheduleID) and IsNumeric(ScheduleID) Then
		If Not IsEmpty(DeliveryStatus) and Not IsNull(DeliveryStatus) and IsNumeric(DeliveryStatus) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_UpdateScheduleStatus")
			Cmd.Parameters.Append Cmd.CreateParameter("@ScheduleID", adInteger, adParamInput, , ScheduleID)
			Cmd.Parameters.Append Cmd.CreateParameter("@DeliveryStatus", adInteger, adParamInput, , DeliveryStatus)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = true
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	BuyersEdge_UpdateScheduleStatus = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns an Schedule
'		Array/Null
'		sc = Schedule

Const scScheduleID			= 0
Const scScheduleKey			= 1
Const scNewsletterID		= 2
Const scMailingListID		= 3
Const scDeliveryStatus		= 4
Const scNumberDelivered		= 5
Const scDateScheduled		= 6

Function BuyersEdge_GetSchedule(ScheduleID)
	Dim returnValue
	returnValue = Null
	If (Not IsEmpty(ScheduleID)) and (Not IsNull(ScheduleID)) and IsNumeric(ScheduleID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetSchedule")
		Cmd.Parameters.Append Cmd.CreateParameter("@ScheduleID", adInteger, adParamInput, , ScheduleID)
		Cmd.Parameters.Append Cmd.CreateParameter("@ScheduleKey", adChar, adParamOutput, 32)
		Cmd.Parameters.Append Cmd.CreateParameter("@NewsletterID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DeliveryStatus", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@NumberDelivered", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateScheduled", adDBTimeStamp, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(6)
			ary(scScheduleID)				= ScheduleID
			ary(scScheduleKey)				= Cmd.Parameters(2).Value
			ary(scNewsletterID)				= Cmd.Parameters(3).Value
			ary(scMailingListID)			= Cmd.Parameters(4).Value
			ary(scDeliveryStatus)			= Cmd.Parameters(5).Value
			ary(scNumberDelivered)			= Cmd.Parameters(6).Value
			ary(scDateScheduled)			= Cmd.Parameters(7).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetSchedule = returnValue
End Function

Function BuyersEdge_GetScheduleByScheduleKey(ScheduleKey)
	Dim returnValue
	returnValue = Null
	If (Not IsEmpty(ScheduleKey)) and (Not IsNull(ScheduleKey)) and Len(Trim(ScheduleKey)) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetScheduleByScheduleKey")
		Cmd.Parameters.Append Cmd.CreateParameter("@ScheduleID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@ScheduleKey", adChar, adParamInput, 32, ScheduleKey)
		Cmd.Parameters.Append Cmd.CreateParameter("@NewsletterID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DeliveryStatus", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@NumberDelivered", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@DateScheduled", adDBTimeStamp, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(6)
			ary(scScheduleID)				= Cmd.Parameters(1).Value
			ary(scScheduleKey)				= ScheduleKey
			ary(scNewsletterID)				= Cmd.Parameters(3).Value
			ary(scMailingListID)			= Cmd.Parameters(4).Value
			ary(scDeliveryStatus)			= Cmd.Parameters(5).Value
			ary(scNumberDelivered)			= Cmd.Parameters(6).Value
			ary(scDateScheduled)			= Cmd.Parameters(7).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetScheduleByScheduleKey = returnValue
End Function


'******************************************************************************************************************************
'	DESCRIPTION:
'		Get Schedules by Delivery Status
'		Array/Null
'		scd = Schedules by Delivery Status

const scdScheduleID			= 0
const scdScheduleKey		= 1
const scdNewsletterID		= 2
const scdNewsletterSubject	= 3
const scdMailingListID		= 4
const scdMailingListName	= 5
const scdDeliveryStatus		= 6
const scdNumberDelivered	= 7
const scdDateScheduled		= 8

Function BuyersEdge_GetSchedulesByDeliveryStatus(DeliveryStatus)
	Dim returnValue, resultSet
	returnValue = Null
	If (Not IsEmpty(DeliveryStatus)) and (Not IsNull(DeliveryStatus)) and IsNumeric(DeliveryStatus) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetSchedulesByDeliveryStatus")
		Cmd.Parameters.Append Cmd.CreateParameter("@DeliveryStatus", adInteger, adParamInput, , DeliveryStatus)
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetSchedulesByDeliveryStatus = returnValue
End Function


'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Schedules for a Newsletter
'		Array/Null
'		scn = Schedules from Newsletter

const scnScheduleID			= 0
const scnScheduleKey		= 1
const scnMailingListID		= 2
const scnMailingListName	= 3
const scnDeliveryStatus		= 4
const scnNumberDelivered	= 5
const scnDateScheduled		= 6

Function BuyersEdge_GetSchedulesFromNewsletterID(NewsletterID)
	Dim returnValue, resultSet
	returnValue = Null
	If (Not IsEmpty(NewsletterID)) and (Not IsNull(NewsletterID)) and IsNumeric(NewsletterID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetSchedulesFromNewsletterID")
		Cmd.Parameters.Append Cmd.CreateParameter("@NewsletterID", adInteger, adParamInput, , NewsletterID)
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetSchedulesFromNewsletterID = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Schedules for a Mailing List
'		Array/Null
'		scm = Schedules from Mailing List

const scmScheduleID			= 0
const scmScheduleKey		= 1
const scmNewsletterID		= 2
const scmNewsletterSubject	= 3
const scmDeliveryStatus		= 4
const scmNumberDelivered	= 5
const scmDateScheduled		= 6

Function BuyersEdge_GetSchedulesFromMailingListID(MailingListID)
	Dim returnValue, resultSet
	returnValue = Null
	If (Not IsEmpty(MailingListID)) and (Not IsNull(MailingListID)) and IsNumeric(MailingListID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_GetSchedulesFromMailingListID")
		Cmd.Parameters.Append Cmd.CreateParameter("@MailingListID", adInteger, adParamInput, , MailingListID)
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetSchedulesFromMailingListID = returnValue
End Function

Function BuyersEdge_GetScheduleDeliveryStatusValue(DeliveryStatus)
	Dim returnValue
	If DeliveryStatus = 0 then
		returnValue = "Scheduled"
	ElseIf DeliveryStatus = 1 then
		returnValue = "In Progress"
	ElseIf DeliveryStatus = 2 then
		returnValue = "Delivered"
	ElseIf DeliveryStatus = 3 then
		returnValue = "Cancelled"
	ElseIf DeliveryStatus = 4 then
		returnValue = "Paused"
	End If
	BuyersEdge_GetScheduleDeliveryStatusValue = returnValue
End Function



'***************************************************************
'	DESCRIPTION:
'		Deletes all Temporary Email Addresses based on ScheduleID.
'		Returns true/false

Function BuyersEdge_DeleteTempEmailAddressesFromSchedule(ScheduleID)
	Dim returnValue, Cmd
	returnValue = false
	If Not IsEmpty(ScheduleID) and Not IsNull(ScheduleID) and IsNumeric(ScheduleID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newBuyersEdge_DeleteTempEmailAddressesFromSchedule")
		Cmd.Parameters.Append Cmd.CreateParameter("@ScheduleID", adInteger, adParamInput, , ScheduleID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = true
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_DeleteTempEmailAddressesFromSchedule = returnValue
End Function








'***************************************************************
'	DESCRIPTION:
'		Adds a Global Remove Email Address.
'		Returns GlobalRemoveID/Null

Function Email_AddGlobalRemove(GlobalRemoveKey,EmailAddress)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(GlobalRemoveKey) and Not IsNull(GlobalRemoveKey) and Len(GlobalRemoveKey) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Email_AddGlobalRemove")
		Cmd.Parameters.Append Cmd.CreateParameter("@GlobalRemoveID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@GlobalRemoveKey", adChar, adParamInput, 32, GlobalRemoveKey)
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamInput, 80, EmailAddress)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(1).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Email_AddGlobalRemove = returnValue
End Function


'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Global Remove Email Addresses
'		Array/Null
'		grea = Global Remove Email Addresses

const greaGlobalRemoveID	= 0
const greaGlobalRemoveKey	= 1
const greaEmailAddress		= 2
const greaDateCreated		= 3

Function Email_GetGlobalRemoveEmailAddresses()
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Email_GetGlobalRemoveEmailAddresses")
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Email_GetGlobalRemoveEmailAddresses = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes all Email Addresses from the Global Remove List.
'		Returns true/false

Function Email_DeleteGlobalRemoveEmailAddresses()
	Dim returnValue, Cmd
	returnValue = false
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Email_DeleteGlobalRemoveEmailAddresses")
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	If Cmd.Parameters(0).Value = 0 then
		returnValue = true
	End If
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Email_DeleteGlobalRemoveEmailAddresses = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a Promotion Type to the Buyers Edge table.
'		Returns ChannelID/Null

Function BuyersEdge_AddPromotionType(Name, Description, CompanyID, IsSpecialPromotion)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(ChannelKey) and Not IsNull(ChannelKey) and Len(ChannelKey) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..BuyersEdge_AddPromotionType")
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 50, Name)								'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 255, Description)				'2
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)						'3
		Cmd.Parameters.Append Cmd.CreateParameter("@IsSpecialPromotion", adBoolean, adParamInput, 255, IsSpecialPromotion)	'4
		Cmd.Parameters.Append Cmd.CreateParameter("@PromotionTypeID", adInteger, adParamOutput)								'5
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(5).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_AddPromotionType = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the buyers edge promotion types
'		Array/Null
'		bept
const beptPromotionTypeID		= 0
const beptPromotionKey			= 1
const beptName					= 2
const beptDescription			= 3
const beptCompanyID				= 4
const beptIsSpecialPromotion	= 5

Function BuyersEdge_GetBuyersEdgePromotionTypes()
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..BuyersEdge_GetBuyersEdgePromotionTypes")
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	BuyersEdge_GetBuyersEdgePromotionTypes = returnValue

End Function

'***************************************************************
'	DESCRIPTION:
'		Adds a Buyers Edge Promotion Subscription to the Buyers Edge table.
'		Returns PromoSubscriptionID/Null

Function BuyersEdge_AddPromotionSubscription(PromotionTypeID, UserID, EmailAddress, EmailFormat)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(PromotionTypeID) and Not IsNull(PromotionTypeID) and IsNumeric(PromotionTypeID) Then
		If ( Not(IsNull(UserID)) OR len(UserID) > 0 ) AND IsNumeric(UserID)  Then
			If Not IsEmpty(EmailAddress) and Not IsNull(EmailAddress) and validEmailAddress(EmailAddress) Then
				If Not IsEmpty(EmailFormat) and Not IsNull(EmailFormat) and IsNumeric(EmailFormat) Then
					Call OpenSQLConnection(ctReadWrite)
					Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..BuyersEdge_AddPromotionSubscription")
					Cmd.Parameters.Append Cmd.CreateParameter("@PromotionTypeID", adInteger, adParamInput,, PromotionTypeID)	'1
					Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)						'2
					Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamInput,100, EmailAddress)		'3
					Cmd.Parameters.Append Cmd.CreateParameter("@EmailFormat", adInteger, adParamInput,, EmailFormat)			'4
					Cmd.Parameters.Append Cmd.CreateParameter("@PromoSubscriptionID", adInteger, adParamOutput)					'5
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
					If Cmd.Parameters(0).Value = 0 then
						returnValue = Cmd.Parameters(5).Value
					End If
					Set Cmd = Nothing
					Call CloseSQLConnection(ctReadWrite)
				End If
			End If
		End If
	End If
	BuyersEdge_AddPromotionSubscription = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Removes a promotion subscription.
'		Returns true/false

Function BuyersEdge_DeletePromotionSubscription(PromoSubscriptionID)
	Dim returnValue, Cmd
	returnValue = false
	If Not IsEmpty(PromoSubscriptionID) and Not IsNull(PromoSubscriptionID) and IsNumeric(PromoSubscriptionID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..BuyersEdge_DeletePromotionSubscription")
		Cmd.Parameters.Append Cmd.CreateParameter("@PromoSubscriptionID", adInteger, adParamInput, , PromoSubscriptionID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = true
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_DeletePromotionSubscription = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the buyers edge promotion subscriptions by UserID or EmailAddress
'		Array/Null
'		bept

	const bepsPromoSubscriptionID	= 0
	const bepsPromotionTypeID		= 1
	const bepsPromotionKey			= 2
	const bepsPromotionName			= 3
	const bepsPromotionDescription	= 4
	const bepsUserID				= 5
	const bepsEmailAddress			= 6
	const bepsEmailFormat			= 7
	const bepsCompanyID				= 8
	const bepsIsSpecialPromotion	= 9
	const bepsDateCreated			= 10
	const bepsDateModified			= 11

Function BuyersEdge_GetPromotionSubscriptions(UserID, EmailAddress)
	Dim returnValue, resultSet
	returnValue = Null
	If ( (Not(IsNull(UserID)) OR len(UserID) > 0 ) AND IsNumeric(UserID) OR ( (Not(IsNull(EmailAddress)) OR len(EmailAddress) > 0) ) and validEmailAddress(EmailAddress) ) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..BuyersEdge_GetPromotionSubscriptions")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
			Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarchar, adParamInput, 100, EmailAddress)
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	BuyersEdge_GetPromotionSubscriptions = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Update a promotion subscription.
'		Returns ChannelID/Null

Function BuyersEdge_UpdatePromotionSubscription(PromoSubscriptionID, PromotionTypeID, UserID, EmailAddress, EmailFormat)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(PromoSubscriptionID) and Not IsNull(PromoSubscriptionID) and IsNumeric(PromoSubscriptionID) Then
'		If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
			If Not IsEmpty(PromotionTypeID) and Not IsNull(PromotionTypeID) and IsNumeric(PromotionTypeID) Then
				If Not IsEmpty(EmailAddress) and Not IsNull(EmailAddress) and validEmailAddress(EmailAddress) Then
					If Not IsEmpty(EmailFormat) and Not IsNull(EmailFormat) and IsNumeric(EmailFormat) Then
						Call OpenSQLConnection(ctReadWrite)
						Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..BuyersEdge_UpdatePromotionSubscription")
						Cmd.Parameters.Append Cmd.CreateParameter("@PromoSubscriptionID", adInteger, adParamInput, , PromoSubscriptionID)
						Cmd.Parameters.Append Cmd.CreateParameter("@PromotionTypeID", adInteger, adParamInput, , PromotionTypeID)
						Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput, , UserID)
						Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamInput, 100, EmailAddress)
						Cmd.Parameters.Append Cmd.CreateParameter("@EmailFormat", adInteger, adParamInput, , EmailFormat)
						Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
						If Cmd.Parameters(0).Value = 0 then
							returnValue = Cmd.Parameters(1).Value
						End If
						Set Cmd = Nothing
						Call CloseSQLConnection(ctReadWrite)
					End If
				End If
			End If
'		End If
	End If
	BuyersEdge_UpdatePromotionSubscription = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Update a promotion subscription.
'		Returns ChannelID/Null

Function BuyersEdge_AddBEOptLog(UserID, PromotionTypeID, OptAction, IP)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(PromotionTypeID) and Not IsNull(OptAction) and Not IsNull(IP) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "LoggingDB..BuyersEdge_AddOptLog")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput, , UserID)
		Cmd.Parameters.Append Cmd.CreateParameter("@PromotionTypeID", adInteger, adParamInput, , PromotionTypeID)
		Cmd.Parameters.Append Cmd.CreateParameter("@OptAction", adBoolean, adParamInput, , OptAction)
		Cmd.Parameters.Append Cmd.CreateParameter("@IP", adVarChar, adParamInput, 15, IP)
		Cmd.Parameters.Append Cmd.CreateParameter("@BEOptLogID", adInteger, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(1).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)			
	End If
	BuyersEdge_AddBEOptLog = returnValue
End Function

%>