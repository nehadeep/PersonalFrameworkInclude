<%

Const ptInteger			= 0 ' <i4> or <int>
Const ptBoolean			= 1 ' <boolean>
Const ptString			= 3	' <string>
Const ptDouble			= 4 ' <double>
Const ptDateTime		= 5 ' <dateTime.iso8601>
Const ptBinary			= 6 ' <base64>
Const ptArray			= 7 ' <array>
Const ptStruct			= 8 ' <struct>
'Const ptStructMember	= 99 ' <member>

Class XMLRPC_Client
	Private m_ServerUrl, m_ProcedureName, m_ErrorOccured
	Private m_InputParameters, m_OutputParameter

'	Public Function Parameters(index)
'		Dim returnValue
'		returnValue = null
'		If IsArray(m_ReturnParameters) Then
'			If index <= ubound(m_ReturnParameters) Then
'				If isobject(m_ReturnParameters(index)) Then
'					Set returnValue = m_ReturnParameters(index)
'				Else
'					returnValue = m_ReturnParameters(index)
'				End If
'			End If
'		End If
'		Parameters = returnValue
'	End Function

'	Public Property Get ParameterCount()
'		If IsArray(m_ReturnParameters) Then
'			ParameterCount = ubound(m_ReturnParameters) + 1
'		else
'			ParameterCount = 0
'		End If
'	End Property

	Public Property Get ErrorOccured()
		ErrorOccured = m_ErrorOccured
	End Property

	Public Property Get OutputParameter()
'		If Not IsEmpty(m_OutputParameter) Then
			Set OutputParameter = m_OutputParameter
'		Else
'			OutputParameter = null
'		End If
	End Property

	Public Function CreateParameter(ParameterName,ParameterValue,ParameterType)
		Dim Param
		Set Param = New XMLRPC_Parameter
		Call Param.CreateParameter(ParameterName,ParameterValue,ParameterType)
		Set CreateParameter = Param
	End Function

	Public Sub AddParameter(Parameter)
		Dim i
		If IsArray(m_InputParameters) Then
			i = ubound(m_InputParameters) + 1
			redim Preserve m_InputParameters(i)
		Else
			i = 0
			redim m_InputParameters(0)
		End If
		Set m_InputParameters(i) = Parameter
	End Sub

	Public Sub CreateProcedure(ServerUrl,ProcedureName)
		m_ServerUrl = ServerUrl
		m_ProcedureName = ProcedureName
		m_ErrorOccured = false
		set m_InputParameters = nothing
		set m_OutputParameter = nothing
	End Sub
	
	Public Sub ExecuteProcedure()
'		On Error Resume Next
		Dim objXML, objLst
		Dim ProcedureXML, ResponseXML

		m_ErrorOccured = false
		ProcedureXML = GetProcedureXML()
'		response.write ProcedureXML

		On Error Resume Next		' Turn on Error Checking
		Set objXML = Server.CreateObject("MSXML2.ServerXMLHTTP")
		Call objXML.open("POST", m_ServerUrl, false)
		Call objXML.setRequestHeader("Content-Type", "text/xml")
		Call objXML.send(ProcedureXML)
		If Err.Number <> 0 Then
			Call Fire_OnXMLRPC_ClientError("Err [" & Err.Number & "]",Err.Description,ResponseXML)
			Exit Sub
		End If
		On Error Goto 0				' Turn off Error Checking

		responseXML = objXML.responseText

		If Not objXML.status = 200 Then
			Call Fire_OnXMLRPC_ClientError("Status Error [" & objXML.status & "]","The remote machine returned a status error",ResponseXML)
			Set objXML = Nothing
			Exit Sub
		End If

		If objXML.responseXML.parseError.errorCode <> 0 Then
			Call Fire_OnXMLRPC_ClientError("Parse Error [" & objXML.responseXML.parseError.errorCode & "]","There was an error parsing the response",ResponseXML)
			Set objXML = Nothing
			Exit Sub
		End If

		Set objLst = Server.CreateObject("MSXML2.DOMDocument")
		Set objLst = objXML.responseXML.getElementsByTagName("fault")
		If objLst.length <> 0 Then
			Dim FaultCode, FaultString
			FaultCode = objLst.item(0).childNodes(0).childNodes(0).childNodes(0).childNodes(1).childNodes(0).text
			FaultString = objLst.item(0).childNodes(0).childNodes(0).childNodes(1).childNodes(1).childNodes(0).text
			Call Fire_OnXMLRPC_ClientError("Fault Error [" & FaultCode & "]",FaultString,ResponseXML)
			Set objXML = Nothing
			Set objLst = Nothing
			Exit Sub
		Else
			Set objLst = objXML.responseXML.getElementsByTagName("param")
			If objLst.length = 0 Then
				Call Fire_OnXMLRPC_ClientError("Param Error","The remote machine did not return any params",ResponseXML)
				Set objXML = Nothing
				Set objLst = Nothing
				Exit Sub
			Else
				Set m_OutputParameter = New XMLRPC_Parameter
				Call m_OutputParameter.SetXML(objLst.item(0).childNodes(0).childNodes(0))
			End If
		End If

'		If Err.Number <> 0 Then
'			Call Fire_OnXMLRPC_ClientError("Err [" & Err.Number & "]",Err.Description,ResponseXML)
'			Err.Clear
'		End If

		Set objXML = Nothing
		Set objLst = Nothing
	End Sub

	Private Function GetProcedureXML()
		Dim returnValue, XMLParameters, i
		returnValue = null
		XMLParameters = null
		If IsArray(m_InputParameters) Then
			For i = 0 to ubound(m_InputParameters)
				XMLParameters = XMLParameters & "<param>" & m_InputParameters(i).GetXML() & "</param>"
			Next
		End If
		returnValue = returnValue & (_
			"<?xml version=""1.0""?>" &_
			"<methodCall>" &_
				"<methodName>" & m_ProcedureName & "</methodName>" &_
				"<params>" & XMLParameters & "</params>" &_
			"</methodCall>" )
		GetProcedureXML = returnValue
	End Function

	' Fires OnXMLRPC_ClientError if it exists
	Private Sub Fire_OnXMLRPC_ClientError(Title,Description,ResponseXML)
		m_ErrorOccured = true
		If IsFunction("OnXMLRPC_ClientError") Then
			Call OnXMLRPC_ClientError(Title,Description,ResponseXML)
		End If
	End Sub

End Class

Class XMLRPC_Server
	Private m_InputParameters, m_ReturnParameter
	Private m_Functions

	Private Function GetResponseXML()
		Dim returnValue
		returnValue = (_
			"<?xml version=""1.0""?>" &_
			"<methodResponse>" &_
				"<params>")
		If TypeName(m_ReturnParameter) <> "Nothing" Then
			returnValue = returnValue & "<param>" & m_ReturnParameter.GetXML() & "</param>"
		Else
			returnValue = returnValue & "<param><value><int>-1</int></value></param>"
		End If
		returnValue = returnValue & (_
				"</params>" &_
			"</methodResponse>")
		GetResponseXML = returnValue
	End Function

	Public Sub SendXMLFault(errNum, errDesc, from)
		Dim xmlText
		xmlText = (_
			"<?xml version=""1.0""?>" &_
	       	"<methodResponse>" &_
				"<fault>" &_
					"<value>" &_
						"<struct>" &_
							"<member>" &_
								"<name>faultCode</name>" &_
								"<value><int>" & errNum & "</int></value>" &_
							"</member>" &_
							"<member>" &_
								"<name>faultString</name>" &_
								"<value><string>" & Server.HTMLEncode(errDesc) & " : " & from & "</string></value>" &_
							"</member>" &_
						"</struct>" &_
					"</value>" &_
				"</fault>" &_
			"</methodResponse>")
		Response.Write(xmlText)
		Response.End
	End Sub

	Public Function CreateParameter(ParameterName,ParameterValue,ParameterType)
		Dim Param
		Set Param = New XMLRPC_Parameter
		Call Param.CreateParameter(ParameterName,ParameterValue,ParameterType)
		Set CreateParameter = Param
	End Function

	Public Sub SetReturnParameter(Parameter)
		Set m_ReturnParameter = Parameter
	End Sub
	
	Public Function Parameters(index)
		Dim returnValue
		returnValue = null
		If IsArray(m_InputParameters) Then
			If index <= ubound(m_InputParameters) Then
				Set returnValue = m_InputParameters(index)
			End If
		End If
		Set Parameters = returnValue
	End Function

	Public Property Get ParameterCount()
		If IsArray(m_InputParameters) Then
			ParameterCount = ubound(m_InputParameters) + 1
		Else
			ParameterCount = 0
		End If
	End Property

	Public Sub AddFunction(FunctionName, ExposedName)
		If Not IsObject(m_Functions) Then
			Set m_Functions = Server.CreateObject("Scripting.Dictionary")
		End If
		Call m_Functions.Add(FunctionName,ExposedName)
	End Sub

	' Called by server (listener) piece.
	Public Sub StartServer()
'		On Error Resume Next
'		On Error goto cleanup
		Dim objXML, objLst
		Dim FunctionName, i

		Set m_InputParameters = nothing
		Set m_ReturnParameter = nothing

		Response.ContentType = "text/XML"
		
		' REMOVED - Recall you'll need IE 5.x DLLs here
		' Set objserveXML = Server.CreateObject("Microsoft.XMLDOM")
		' Set objserveLst = Server.CreateObject("Microsoft.XMLDOM")

		' Now use the MS redistributable parser alone
'		Set objXML = Server.CreateObject("MSXML2.DOMDocument")
		Set objXML = Server.CreateObject("Microsoft.XMLDOM")
		objXML.async = false
		Call objXML.load(Request)

		'Extract parameters and function from XML
		If objXML.parseError.errorCode <> 0 Then
			Call SendXMLFault(objXML.parseError.errorCode, "error parsing the xml passed to the server", "rpcserver(1)" )
		Else
			' Function to to call
			FunctionName = objXML.childNodes(1).childNodes(0).text
			If m_Functions.exists(FunctionName) Then
				FunctionName = m_Functions.item(FunctionName)
				Set m_Functions = nothing
			Else
				Set m_Functions = nothing
				Call SendXMLFault("1.2", "No such function", "This is not a valid function call for this server" )
			End If

			' Build the Parameter Array
'			Set objLst = Server.CreateObject("MSXML2.DOMDocument")
'			Set objLst = Server.CreateObject("Microsoft.XMLDOM")
			Set objLst = objXML.getElementsByTagName("param")
			If (objLst.length > 0 ) Then
				ReDim m_InputParameters(objLst.length - 1)
				For i = 0 to objLst.length - 1
					Set m_InputParameters(i) = New XMLRPC_Parameter
					Call m_InputParameters(i).SetXML(objLst.item(i).childNodes(0).childNodes(0))
'					Call catchError ("rpcserver(1.5): args to XML [value was " & typename(argList(i)) & "]")
				Next
			End if
			set objLst = nothing
		End If
		' "free" objects
		set objXML = nothing
'		Call catchError("rpcserver(2): freeing objects ")

		If FunctionName <> "" Then
			Execute(FunctionName & "()")

'			Call catchError("rpcserver(3)(return from eval) :[in function " & FunctionName & " {evaled string: " & server.htmlencode(EvalString) & "}" &_
'				"{returnArr(1)(0) was " & typename(returnArr(1)(0)) & "} (TypeName was: " & TypeName(eval_ret) & ")]")

'			Call catchError("rpcserver(4) :[in function " & stringToEval & "] {arg 1: " & TypeName(returnArr(1)(0)) & "}" )
			Response.Write GetResponseXML()
		End If
'		Call catchError("rpcserver(5) :[in function " & EvalString & "] {arg 1: " & TypeName(returnArr(1)(0)) & "}" )
'	cleanup:
		If Err.Number <> 0 Then
			Call SendXMLFault(Err.Number,Err.Description,EvalString)
			Err.Clear
		End If
	End sub
	
'	sub catchError(from)
'		if err.number=0 then
'			exit sub
'		end if
'		Call writeFaultXML(err.number, err.description, from )
'	end sub

End Class

Class XMLRPC_Parameter
	Dim m_Name, m_Value, m_Type
	Dim m_Elements

	Public Property Get Value()
		Value = m_Value
	End Property

	Public Property Get ParameterType()
		ParameterType = m_Type
	End Property

	Public Property Get Name()
		Name = m_Name
	End Property

	Public Property Let Name(NewName)
		m_Name = NewName
	End Property

	Public Sub CreateParameter(Name,Value,ParameterType)
		m_Type = ParameterType
		m_Value = Value
		m_Name = Name
		set m_Elements = nothing
	End Sub

	Public Sub AddParameter(Parameter)
		Dim i
		If IsArray(m_Elements) Then
			i = ubound(m_Elements) + 1
			redim Preserve m_Elements(i)
		Else
			i = 0
			redim m_Elements(0)
		End If
		Set m_Elements(i) = Parameter
	End Sub

	Public Function GetXML()
		Dim returnValue, i
		returnValue = null
		returnValue = returnValue & "<value>"
		Select Case m_Type
			Case ptInteger
				returnValue = returnValue & "<i4>" & m_Value & "</i4>"
			Case ptBoolean
				returnValue = returnValue & "<boolean>" & -1 * cInt(m_Value) & "</boolean>"
			Case ptString
				returnValue = returnValue & "<string>" & String_Encode(m_Value) & "</string>"
			Case ptDouble
				returnValue = returnValue & "<double>" & m_Value & "</double>"
			Case ptDateTime
				returnValue = returnValue & "<dateTime.iso8601>" & DateTime_Encode(m_Value) & "</dateTime.iso8601>"
			Case ptBinary
				returnValue = returnValue & "<base64>" & Base64_Encode(m_Value) & "</base64>"
			Case ptArray
				If IsArray(m_Elements) Then
					returnValue = returnValue & "<array><data>"
					For i = 0 to ubound(m_Elements)
						returnValue = returnValue & m_Elements(i).GetXML()
					Next
					returnValue = returnValue & "</data></array>"
				End If
			Case ptStruct
				If IsArray(m_Elements) Then
					returnValue = returnValue & "<struct>"
					For i = 0 to ubound(m_Elements)
						returnValue = returnValue & "<member><name>" & m_Elements(i).Name & "</name>" & m_Elements(i).GetXML() & "</member>"
					Next
					returnValue = returnValue & "</struct>"
				End If
		End Select
		returnValue = returnValue & "</value>"
		GetXML = returnValue
	End Function

	Public Sub SetXML(ValueXML)
		Dim NumElements, i
		Select Case ValueXML.tagName
			Case "int"
				m_Type = ptInteger
				m_Value = CLng(ValueXML.text)
			Case "i4"
				m_Type = ptInteger
				m_Value = CLng(ValueXML.text)
			Case "boolean"
				m_Type = ptBoolean
				m_Value = CBool(ValueXML.text)
			Case "string"
				m_Type = ptString
				m_Value = String_Decode(ValueXML.text)
			Case "double"
				m_Type = ptDouble
				m_Value = CDbl(ValueXML.text)
			Case "dateTime.iso8601"
				m_Type = ptDateTime
				m_Value = DateTime_Decode(ValueXML.text)
			Case "base64"
				m_Type = ptBinary
				m_Value = Base64_Decode(ValueXML.text)
			Case "array"
				m_Type = ptArray
				NumElements = ValueXML.childNodes(0).childNodes.length
				redim m_Elements(NumElements - 1)
				For i = 0 to NumElements - 1
					Set m_Elements(i) = New XMLRPC_Parameter
					Call m_Elements(i).SetXML(ValueXML.childNodes(0).childNodes(i).childNodes(0))
				Next
			Case "struct"
				m_Type = ptStruct
				NumElements = ValueXML.childNodes.length
				redim m_Elements(NumElements - 1)
				For i = 0 to NumElements - 1
					Set m_Elements(i) = New XMLRPC_Parameter
					m_Elements(i).Name = ValueXML.childNodes(i).childNodes(0).text
					Call m_Elements(i).SetXML(ValueXML.childNodes(i).childNodes(1).childNodes(0))
				Next
		End Select
	End Sub

End Class

' ************************************************************************************************
' *                          String Encoding and Decoding functions                              *
' ************************************************************************************************
Function String_Encode(Data)
	Dim returnValue
	returnValue = Data
	If Not IsNull(Data) Then
		returnValue = Replace(returnValue, "&", "&amp;", 1, -1, 1)
		returnValue = Replace(returnValue, "<", "&lt;", 1, -1, 1)
		returnValue = Replace(returnValue, ">", "&gt;", 1, -1, 1)
		returnValue = Replace(returnValue, "'", "&apos;", 1, -1, 1)
		returnValue = Replace(returnValue, """", "&quot;", 1, -1, 1)
	End If
	String_Encode = returnValue
End Function

Function String_Decode(Data)
	Dim returnValue
	returnValue = Data
	If Not IsNull(Data) Then
		returnValue = Replace(returnValue, "&quot;", """", 1, -1, 1)
		returnValue = Replace(returnValue, "&apos;", "'", 1, -1, 1)
		returnValue = Replace(returnValue, "&gt;", ">", 1, -1, 1)
		returnValue = Replace(returnValue, "&lt;", "<", 1, -1, 1)
		returnValue = Replace(returnValue, "&amp;", "&", 1, -1, 1)
	End If
	String_Decode = returnValue
End Function

' ************************************************************************************************
' *                          DateTime Encoding and Decoding functions                            *
' ************************************************************************************************
Function DateTime_Encode(Data)
	Dim returnValue
	returnValue = (_
		Year(Data) &_
		TwoChars(Month(Data)) &_
		TwoChars(Day(Data)) &_
		"T" &_
		TwoChars(Hour(Data)) &_
		":" &_
		TwoChars(Minute(Data)) &_
		":" &_
		TwoChars(Second(Data)) )
	DateTime_Encode = returnValue
End Function

Function DateTime_Decode(Data)
	Dim returnValue
	returnValue = null
	returnValue = CDate(mid(Data, 1, 4) & "/" & mid(Data, 5, 2) & "/" & mid(Data, 7, 2))
	returnValue = dateadd("h", CInt(mid(Data, 10, 2)), returnValue)
	returnValue = dateadd("n", CInt(mid(Data, 13, 2)), returnValue)
	returnValue = dateadd("s", CInt(mid(Data, 16, 2)), returnValue)
	DateTime_Decode = returnValue
End Function

Function TwoChars(Data)
	Dim returnValue
	returnValue = CStr(Data)
	If len(returnValue) = 1 Then
		returnValue = "0" & returnValue
	End If
	TwoChars = returnValue
End Function



%>