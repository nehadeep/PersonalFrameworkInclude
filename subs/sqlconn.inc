<!-- METADATA TYPE="typelib" UUID="00000200-0000-0010-8000-00AA006D2EA4" -->
<%
	'DESCRIPTION :
	'	Contains global function related to getting sql connections and
	'	creating stored procs.
	'ASSUMPTIONS:
	'	"/global/subs/mathsubs.inc"

	Const ctReadOnly				= 0		'Web read only sql server
	Const ctReadWrite				= 1		'Web read write sql server
	Const ctSearch					= 2		'Web search sql server
	Const ctLegacy					= 3		'Internal sql server
	Const ctReport					= 4		'Web report sql server		Discontinued 12/15/2011
	Const ctAS400					= 5		'As/400
	Const ctKB						= 6		'KnowledgeBase sql server	Discontinued 12/15/2011
	Const ctAEX						= 7		'ApplicationXtender sql server
	Const ctClarify					= 8		'Clarify sql server			Discontinued 12/15/2011
	Const ctAdminSQL				= 9 	'Admin SQL Server
	Const ctCSJobsSalesSQL			= 10	'CS SQL Server
	Const ctReadOnlyOrderHistory	= 11	'Point to WebDB3 and WebDB4
	Const ctRemedy					= 12	' Remedy SQL Server
	Const CallidusReadWrite			= 13

	Dim SQLConnections(13, 5)
	Dim SQLConnectionStrings(13, 5)

	'*********************************************************************
	'this allows for running locally but still using dbs from other modes

	'e.g. running code locally against production db for defect fixes

	'BUT YOU SHOULD ALWAYS REMEMBER TO TURN THE VALUE BACK TO glDevelopmentMode AFTER YOU ARE DONE !!!!
	'*********************************************************************
	Dim DbConnMode	'glDevelopmentMode
	DbConnMode = glDevelopmentMode	'glLocalMode, glDevelopmentMode, glBetaMode, glPreviewMode, glQAMode, glStagingMode, glProductionMode
	'*********************************************************************
	
	' Make sure we clear out our Connection list

	SQLConnections( ctReadOnly, glLocalMode )			= Null
	SQLConnections( ctReadOnly, glBetaMode )			= Null
	SQLConnections( ctReadOnly, glPreviewMode )			= Null
	SQLConnections( ctReadOnly, glQAMode )			    = Null
	SQLConnections( ctReadOnly, glStagingMode )			= Null
	SQLConnections( ctReadOnly, glProductionMode )		= Null

	SQLConnections( ctReadWrite, glLocalMode )			= Null
	SQLConnections( ctReadWrite, glBetaMode )			= Null
	SQLConnections( ctReadWrite, glPreviewMode )		= Null
	SQLConnections( ctReadWrite, glQAMode )		        = Null
	SQLConnections( ctReadWrite, glStagingMode )		= Null
	SQLConnections( ctReadWrite, glProductionMode )		= Null

	SQLConnections( ctSearch, glLocalMode )				= Null
	SQLConnections( ctSearch, glBetaMode )				= Null
	SQLConnections( ctSearch, glPreviewMode )			= Null
	SQLConnections( ctSearch, glQAMode )			    = Null
	SQLConnections( ctSearch, glStagingMode )			= Null
	SQLConnections( ctSearch, glProductionMode )		= Null

	SQLConnections( ctLegacy, glLocalMode )				= Null
	SQLConnections( ctLegacy, glBetaMode )				= Null
	SQLConnections( ctLegacy, glPreviewMode )			= Null
	SQLConnections( ctLegacy, glQAMode )			    = Null
	SQLConnections( ctLegacy, glStagingMode )			= Null
	SQLConnections( ctLegacy, glProductionMode )		= Null

	SQLConnections( ctReport, glLocalMode )				= Null
	SQLConnections( ctReport, glBetaMode )				= Null
	SQLConnections( ctReport, glPreviewMode )			= Null
	SQLConnections( ctReport, glQAMode )			    = Null
	SQLConnections( ctReport, glStagingMode )			= Null
	SQLConnections( ctReport, glProductionMode )		= Null

	SQLConnections( ctAS400, glLocalMode )				= Null
	SQLConnections( ctAS400, glBetaMode )				= Null
	SQLConnections( ctAS400, glPreviewMode )			= Null
	SQLConnections( ctAS400, glQAMode )			        = Null
	SQLConnections( ctAS400, glStagingMode )			= Null
	SQLConnections( ctAS400, glProductionMode )			= Null

	SQLConnections( ctKB, glLocalMode )					= Null
	SQLConnections( ctKB, glBetaMode )					= Null
	SQLConnections( ctKB, glPreviewMode )				= Null
	SQLConnections( ctKB, glQAMode )				    = Null
	SQLConnections( ctKB, glStagingMode )				= Null
	SQLConnections( ctKB, glProductionMode )			= Null

	SQLConnections( ctAEX, glLocalMode )				= Null
	SQLConnections( ctAEX, glBetaMode )					= Null
	SQLConnections( ctAEX, glPreviewMode )				= Null
	SQLConnections( ctAEX, glQAMode )				    = Null
	SQLConnections( ctAEX, glStagingMode )				= Null
	SQLConnections( ctAEX, glProductionMode )			= Null

	SQLConnections( ctClarify, glLocalMode )			= Null
	SQLConnections( ctClarify, glBetaMode )				= Null
	SQLConnections( ctClarify, glPreviewMode )			= Null
	SQLConnections( ctClarify, glQAMode )			    = Null
	SQLConnections( ctClarify, glStagingMode )			= Null
	SQLConnections( ctClarify, glProductionMode )		= Null

	SQLConnections( ctAdminSQL, glLocalMode )			= Null
	SQLConnections( ctAdminSQL, glBetaMode )			= Null
	SQLConnections( ctAdminSQL, glPreviewMode )			= Null
	SQLConnections( ctAdminSQL, glQAMode )			    = Null
	SQLConnections( ctAdminSQL, glStagingMode )			= Null
	SQLConnections( ctAdminSQL, glProductionMode )		= Null
	
	SQLConnections( ctCSJobsSalesSQL, glLocalMode )		    = Null
	SQLConnections( ctCSJobsSalesSQL, glBetaMode )		    = Null
	SQLConnections( ctCSJobsSalesSQL, glPreviewMode )	    = Null
	SQLConnections( ctCSJobsSalesSQL, glQAMode )		    = Null
	SQLConnections( ctCSJobsSalesSQL, glStagingMode )		= Null
	SQLConnections( ctCSJobsSalesSQL, glProductionMode )	= Null

	'SQLConnections( ctReadWriteBeaconTracking, glLocalMode )		= Null
	'SQLConnections( ctReadWriteBeaconTracking, glBetaMode )		= Null
	'SQLConnections( ctReadWriteBeaconTracking, glPreviewMode )		= Null
	'SQLConnections( ctReadWriteBeaconTracking, glQAMode )			= Null
	'SQLConnections( ctReadWriteBeaconTracking, glStagingMode )		= Null
	'SQLConnections( ctReadWriteBeaconTracking, glProductionMode )	= Null
	
	SQLConnections( ctReadOnlyOrderHistory, glLocalMode )		= Null
	SQLConnections( ctReadOnlyOrderHistory, glBetaMode )		= Null
	SQLConnections( ctReadOnlyOrderHistory, glPreviewMode )		= Null
	SQLConnections( ctReadOnlyOrderHistory, glQAMode )			= Null
	SQLConnections( ctReadOnlyOrderHistory, glStagingMode )		= Null
	SQLConnections( ctReadOnlyOrderHistory, glProductionMode )	= Null
	
	SQLConnections( ctRemedy, glLocalMode )			= Null
	SQLConnections( ctRemedy, glBetaMode )			= Null
	SQLConnections( ctRemedy, glPreviewMode )		= Null
	SQLConnections( ctRemedy, glQAMode )			= Null
	SQLConnections( ctRemedy, glStagingMode )		= Null
	SQLConnections( ctRemedy, glProductionMode )	= Null
	
	' Set up Connection Strings
'Include removed

	Function GetSQLConnectionString(ConnectionType)
		Dim returnValue
		
		returnValue = Null
		If (Not IsEmpty(ConnectionType)) and (Not IsNull(ConnectionType)) and Len(ConnectionType) > 0 Then
			returnValue = SQLConnectionStrings( ConnectionType, DbConnMode )
		End If
		GetSQLConnectionString = returnValue
	End Function

	Function GetSQLConnection(ConnectionType)
		Dim returnValue, conn
		
		returnValue = Null
		If Not IsNull(SQLConnections( ConnectionType, DbConnMode )) Then
			Set conn = SQLConnections( ConnectionType, DbConnMode )
			If conn.State = adStateClosed Then
				Set SQLConnections( ConnectionType, DbConnMode ) = Nothing
				SQLConnections( ConnectionType, DbConnMode ) = Null
				Set conn = Nothing
			End If
		End If
		If IsNull(SQLConnections( ConnectionType, DbConnMode )) Then
			Set conn = CreateObject("ADODB.Connection")		

			Response.write("ConnectionType:  " & ConnectionType & "<br />")
'			Response.Write("conn.ConnectionString = " & GetSQLConnectionString(ConnectionType) & "<hr />")
'			Response.End
			
			conn.ConnectionString = GetSQLConnectionString( ConnectionType )
			conn.ConnectionTimeout = 5
			conn.CommandTimeout = 20
			conn.Open
			Set SQLConnections( ConnectionType, DbConnMode ) = conn
			Set conn = Nothing
		End If
		Set returnValue = SQLConnections( ConnectionType, DbConnMode )
		Set GetSQLConnection = returnValue
	End Function

	Function OpenSQLConnection(ConnectionType)
		Dim returnValue
		returnValue = Null
		Set returnValue = GetSQLConnection(ConnectionType)
		Set OpenSQLConnection = returnValue
	End Function

	Function CloseSQLConnection(ConnectionType)
		Dim returnValue, conn
		
		returnValue = Null
		If Not IsNull(SQLConnections( ConnectionType, DbConnMode )) Then
			Set conn = SQLConnections( ConnectionType, DbConnMode )
			Set SQLConnections( ConnectionType, DbConnMode ) = Nothing
			SQLConnections( ConnectionType, DbConnMode ) = Null
			conn.Close
			Set conn = Nothing
		End If
		CloseSQLConnection = returnValue
	End Function

	Function CreateSQLProcedure(ConnectionType, ProcedureName)
		Dim returnValue
		returnValue = Null
		Set returnValue = CreateObject("ADODB.Command")
		Set returnValue.ActiveConnection = GetSQLConnection(ConnectionType)
		returnValue.CommandText = ProcedureName
		returnValue.CommandType = adCmdStoredProc
		returnValue.Parameters.Append returnValue.CreateParameter("RETURN_VALUE", adInteger, adParamReturnValue)
		Set CreateSQLProcedure = returnValue
	End Function

	Function CreateSQLStatement(ConnectionType, SQLStatement)
		Dim returnValue
		returnValue = Null
		Set returnValue = CreateObject("ADODB.Command")
		Set returnValue.ActiveConnection = GetSQLConnection(ConnectionType)
		returnValue.CommandText = SQLStatement
		returnValue.CommandType = adCmdText
		Set CreateSQLStatement = returnValue
	End Function

	Function CreateSOAPEnvelope()
		Dim Envelope
		Envelope = Null
		Set Envelope = CreateObject("pocketSOAP.Envelope.2")
		Envelope.serializerFactory.setConfig(1)
		Envelope.EncodingStyle = ""
		Set CreateSOAPEnvelope = Envelope
	End Function

	Function SendSOAPEnvelope(Envelope, URL)
		Dim http, returnValue
		returnValue = Null
		Set http = CreateObject("pocketSOAP.HTTPTransport.2")
		http.SOAPAction = Envelope.URI & Envelope.MethodName
		http.Send "http://" & GetSiteName(webservice_cdw_com) & URL, Envelope.serialize
		Envelope.parse http
		Set http = Nothing
		Set returnValue = Envelope.Parameters.Item(0).Nodes
		Set Envelope = Nothing
		Set SendSOAPEnvelope = returnValue
	End Function

	'****	 Procedures for Execution

	Function ExecuteSQLProcedure(ConnectionType, SQLProcedure, ReturnsRowSet)
		'Parms - (ConnectionType - int, SQLProcedure - ADODB.Cmd, ReturnsRowSet - bool)
		Dim returnValue
		Dim ErrorInfo

		'ajl - provider error  
		Dim Number, Source, Description, ProviderDescription
		'ajl - End

		returnValue = Null
		If(ReturnsRowSet) Then
			Set returnValue = CreateObject("ADODB.Recordset")
			returnValue.CursorType = adOpenForwardOnly
			returnValue.CursorLocation = adUseClient
			returnValue.LockType = adLockReadOnly
			returnValue.CacheSize = 50
			Set returnValue.Source = SQLProcedure

			'ajl - provider error handling added
			on error resume next 
			returnValue.Open
			If Err.Number <> 0 Then

				Number = Err.Number
				Source = Err.Source
				Description = Err.Description  
				Description = Description &  "<br><br>" & "<u>General Information:</u>"
				Description = Description &  "<br>" & "Connection Type: " & GetCDWConnTypeDesc(ConnectionType)
				Description = Description &  "<br>" & "Stored Procedure: " & SQLProcedure.CommandText
				Description = Description &  "<br><br>" & "<u>Stored Procedure Parameter Dump:</u>" & "<br>" & GetParameterDump(SQLProcedure)
				Description = Description &  "<br>" & "<u>Provider Information:</u>" & "<br>" & GetProviderErrorInfo(SQLProcedure.ActiveConnection)

				Err.Clear
				On Error Goto 0
				Call Err.Raise( Number, Source, Description )
			End If
			On Error Goto 0
			'ajl - End
							
			If(ReturnValue.State = adStateOpen) then
				Set returnValue.ActiveConnection = Nothing
			End If
			Set ExecuteSQLProcedure = returnValue
		Else
			'ajl - provider error handling added
			on error resume next 
			SQLProcedure.Execute null, null, adExecuteNoRecords
			If Err.Number <> 0 Then
				Number = Err.Number
				Source = Err.Source
				Description = Err.Description  
				Description = Description &  "<br><br>" & "<u>General Information:</u>"
				Description = Description &  "<br>" & "Connection Type: " & GetCDWConnTypeDesc(ConnectionType)
				Description = Description &  "<br>" & "Stored Procedure: " & SQLProcedure.CommandText
				Description = Description &  "<br><br>" & "<u>Stored Procedure Parameter Dump:</u>" & "<br>" & GetParameterDump(SQLProcedure)
				Description = Description &  "<br>" & "<u>Provider Information:</u>" & "<br>" & GetProviderErrorInfo(SQLProcedure.ActiveConnection)
				Err.Clear
				On Error Goto 0
				Call Err.Raise( Number, Source, Description )
			End If
			On Error Goto 0 
			'ajl - End
			
			ExecuteSQLProcedure = returnValue
		End If
		CloseSQLConnection(ConnectionType)
	End Function



	Function ExecuteSQLStatement(ConnectionType, SQLStatement, ReturnsRowSet)
		'Parms - (ConnectionType - int, SQLStatement - ADODB.Cmd, ReturnsRowSet - bool)
		Dim returnValue
		returnValue = Null
		If(ReturnsRowSet) Then
			Set returnValue = CreateObject("ADODB.Recordset")
			returnValue.CursorType = adOpenForwardOnly
			returnValue.CursorLocation = adUseClient
			returnValue.LockType = adLockReadOnly
			returnValue.CacheSize = 50
			Set returnValue.Source = SQLStatement

			'ajl - provider error handling added
			on error resume next 
			returnValue.Open
			If Err.Number <> 0 Then
				Dim Number, Source, Description
				Number = Err.Number
				Source = Err.Source

				Description = Err.Description  
				Description = Description &  "<br><br>" & "<u>General Information:</u>"
				Description = Description &  "<br>" & "Connection Type: " & GetCDWConnTypeDesc(ConnectionType)
				Description = Description &  "<br>" & "Stored Procedure: " & SQLStatement.CommandText
				Description = Description &  "<br><br>" & "<u>Stored Procedure Parameter Dump:</u>" & "<br>" & GetParameterDump(SQLStatement)
				Description = Description &  "<br>" & "<u>Provider Information:</u>" & "<br>" & GetProviderErrorInfo(returnValue.ActiveConnection)

				Err.Clear
				On Error Goto 0
				Call Err.Raise( Number, Source, Description )
			End If
			On Error Goto 0
			
			If(returnValue.State = adStateOpen) then
				Set returnValue.ActiveConnection = Nothing
			End If
			Set ExecuteSQLStatement = returnValue
		Else

			'ajl - provider error handling added
			on error resume next 
			SQLStatement.Execute Null, Null, adExecuteNoRecords
			If Err.Number <> 0 Then
				Number = Err.Number
				Source = Err.Source

				Description = Err.Description  
				Description = Description &  "<br><br>" & "<u>General Information:</u>"
				Description = Description &  "<br>" & "Connection Type: " & GetCDWConnTypeDesc(ConnectionType)
				Description = Description &  "<br>" & "Stored Procedure: " & SQLStatement.CommandText
				Description = Description &  "<br><br>" & "<u>Stored Procedure Parameter Dump:</u>" & "<br>" & GetParameterDump(SQLStatement)
				Description = Description &  "<br>" & "<u>Provider Information:</u>" & "<br>" & GetProviderErrorInfo(SQLStatement.ActiveConnection)
				
				Err.Clear
				On Error Goto 0
				Call Err.Raise( Number, Source, Description )
			End If
			On Error Goto 0 
			'ajl - End
			
			ExecuteSQLStatement = returnValue
		End If
		CloseSQLConnection(ConnectionType)
	End Function




	'ajl - provider error handling added
	Function GetProviderErrorInfo(objConnection)

		Dim ErrorInfo,x
		ErrorInfo = ""
		x = 0
		
		do while (x < objConnection.Errors.Count)

		   ErrorInfo = ErrorInfo & "Provider Error Description; " & objConnection.Errors(x).Description & "<br>" &_
			  "Provider Native Error: " & objConnection.Errors(x).NativeError & "<br>" &_
			  "Provider Error Number: " & objConnection.Errors(x).Number & "<br>" &_
			  "Provider Error Source: " & objConnection.Errors(x).Source & "<br>" &_
			  "Provider Error SQL State: " & objConnection.Errors(x).SQLState & "<br>"
			
			x = x + 1
		loop 
		
		GetProviderErrorInfo = ErrorInfo
	End Function
	'ajl - End


	'ajl - parameter dump added
	Function GetParameterDump(objCmd)

		Dim ParmInfo,y
		ParmInfo = ""
		y = 0
		
		do while (y < objCmd.Parameters.Count)

		   ParmInfo = ParmInfo &_
			  "Parameter " & cstr(y) & " Name:"  & objCmd.Parameters(y).Name & "<br>" &_
			  "Parameter " & cstr(y) & " Size:"  & objCmd.Parameters(y).Size & "<br>" &_
			  "Parameter " & cstr(y) & " Type:"  & GetADOParmType(objCmd.Parameters(y).Type) & "<br>" &_
			  "Parameter " & cstr(y) & " Direction:"  & GetADOParmDirection(objCmd.Parameters(y).Direction) & "<br>" &_
			  "Parameter " & cstr(y) & " Value:<strong>"  & objCmd.Parameters(y).Value & "</strong><br><br>"
			
			y = y + 1
		loop 
		
		GetParameterDump = ParmInfo
	End Function
	'ajl - End

	function GetADOParmType(ParmType)
		dim TypeString
		select case ParmType
			case 3
				TypeString = "adInteger"
			case 200
				TypeString = "adVarChar"
			case 129
				TypeString = "adChar"
			case 5
				TypeString = "adDouble"
			case 16	
				TypeString = "adTinyInt"
			case 11
				TypeString = "adBoolean"
			case else
				TypeString = ParmType
		end select		

		GetADOParmType = TypeString
	end Function


	function GetADOParmDirection(ParmDirect)
		dim DirectString
		select case ParmDirect
			case 2
				DirectString = "adParamOutput"
			case 1
				DirectString = "adParamInput"
			case 3
				DirectString = "adParamInputOutput"
			case 4
				DirectString = "adParamReturnValue"
			case 0	
				DirectString = "adParamUnknown"
			case else
				DirectString = ParmDirect
		end select		

		GetADOParmDirection = DirectString
	end Function




function GetCDWConnTypeDesc (ConnType)
	Dim ConnDesc
	select case ConnType
		case ctReadOnly
			ConnDesc = "ctReadOnly"
		case ctReadWrite
			ConnDesc = "ctReadWrite"
		case ctSearch
			ConnDesc = "ctSearch"
		case ctLegacy
			ConnDesc = "ctLegacy"
		case ctAS400
			ConnDesc = "ctAS400"
		case ctAEX
			ConnDesc = "ctAEX"
		case ctClarify
			ConnDesc = "ctClarify"
		case else
			ConnDesc = ConnType & " unknown connection type"
	end select	
	select case GetSiteDevelopmentMode()
		case glLocalMode
			ConnDesc = ConnDesc & " - glLocalMode"
		case glBetaMode
			ConnDesc = ConnDesc & " - glBetaMode"
		case glPreviewMode
			ConnDesc = ConnDesc & " - glPreviewMode"
		case glQAMode
			ConnDesc = ConnDesc & " - glQAMode"
		case glStagingMode
			ConnDesc = ConnDesc & " - glStagingMode"
		case glProductionMode
			ConnDesc = ConnDesc & " - glProductionMode"
		case else
			ConnDesc = ConnDesc & " - unknown mode"
	end select	
	GetCDWConnTypeDesc = ConnDesc
end function



	
	'****	END Procedures for Execution




'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'Legacy Wrappers below
	' ****
	Sub OpenConnection()
		OpenSQLConnection(ctReadWrite)
	End Sub

	Sub CloseConnection()
		CloseSQLConnection(ctReadWrite)
	End Sub

	Function GetConnection()
		Set GetConnection = GetSQLConnection(ctReadWrite)
	End Function

	Function CreateStoredProcCommand(storedProcName)
		Set CreateStoredProcCommand = CreateSQLProcedure(ctReadWrite, storedProcName)
	End Function

	' ****
	Sub OpenFTConnection()
		OpenSQLConnection(ctSearch)
	End Sub

	Sub CloseFTConnection
		CloseSQLConnection(ctSearch)
	End Sub

	Function GetFTConnection
		set GetFTConnection = GetSQLConnection(ctSearch)
	End Function

	Function CreateFTStoredProcCommand(storedProcName)
		Set CreateFTStoredProcCommand = CreateSQLProcedure(ctSearch, storedProcName)
	End Function

	' ****
	Sub OpenSalesSQLConnection()
		OpenSQLConnection(ctLegacy)
	End Sub

	Sub CloseSalesSQLConnection()
		CloseSQLConnection(ctLegacy)
	End Sub

	Function GetSalesSQLConnection()
		Set GetSalesSQLConnection = GetSQLConnection(ctLegacy)
	End Function

	Function CreateSalesSQLStoredProcCommand(storedProcName)
		Set CreateSalesSQLStoredProcCommand = CreateSQLProcedure(ctLegacy, storedProcName)
	End Function

	' ****
	Sub OpenROConnection()
		OpenSQLConnection(ctReadOnly)
	End Sub

	Sub CloseROConnection()
		CloseSQLConnection(ctReadOnly)
	End Sub

	Function GetROConnection
		set GetROConnection = GetSQLConnection(ctReadOnly)
	End Function

	Function CreateROStoredProcCommand(storedProcName)
		Set CreateROStoredProcCommand = CreateSQLProcedure(ctReadOnly, storedProcName)
	End Function
	
	' ****
	Sub OpenAdminSQLConnection()
		OpenSQLConnection(ctAdminSQL)
	End Sub

	Sub CloseAdminSQLConnection()
		CloseSQLConnection(ctAdminSQL)
	End Sub

	Function GetAdminSQLConnection
		set GetAdminSQLConnection = GetSQLConnection(ctAdminSQL)
	End Function

	Function CreateAdminSQLStoredProcCommand(storedProcName)
		Set CreateAdminSQLStoredProcCommand = CreateSQLProcedure(ctAdminSQL, storedProcName)
	End Function
%>
